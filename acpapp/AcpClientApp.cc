/*
 * AcpClientApp.cc
 *
 *  Created on: May 28, 2023
 *      Author: sozmen
 */

#include "AcpClientApp.h"
#include "AcpDatalinkMsg_m.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/Simsignals.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/networklayer/common/FragmentationTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/linklayer/ieee80211/mgmt/Ieee80211MgmtAp.h"
#include "inet/linklayer/ieee80211/mgmt/Ieee80211MgmtSta.h"


#include <string.h>

using namespace std;
namespace inet {

#define MSGKIND_CONNECT             1
#define MSGKIND_LOGON               2
#define MSGKIND_LOGOUT              3
#define MSGKIND_STARTSERVICE        4
#define MSGKIND_ENDSERVICE          5
#define MSGKIND_UPDATE              6
#define MSGKIND_CLOSE               7


Define_Module(AcpClientApp);

AcpClientApp::~AcpClientApp()
{
    cancelAndDelete(timeoutMsg);
}

void AcpClientApp::initialize(int stage)
{

    TcpAppBase::initialize(stage);


    if (stage == INITSTAGE_LOCAL) {

        host = getContainingNode(this);
        int myNodeType = par("nodeType");

        airport = host->getParentModule();
        area = airport->getParentModule();
        fir = area->getParentModule();
        continent = fir->getParentModule();

        if (myNodeType == CLIENT)  {

            std::string nodeFullPath = host->getFullPath();
            thisNodeName = nodeFullPath.c_str();

            int myNodeRole = par("nodeRole");
            if (myNodeRole == FIXED) {
               thisNodeType = NODE_FIXED_CLIENT;

               string adjAtc = par("adjAtcName");
               string Name = host->getName();
               if (!strcmp(Name.c_str(),"controller")) {

                   //setNextAdjAtc ();
                   // setPrevAdjAtc ();
               }
               else  {
                   string Name = host->getName();
                   if (!strcmp(Name.c_str(),"sector")) {
                          thisNodeName = nodeFullPath.c_str();
                   }
               }

               atcClient *nda = retrieveNextDataAuthority(create_destairport());

            }
            else if (myNodeRole == MOBILE) {
               thisNodeType = NODE_MOBILE_CLIENT;

               setCDA ();
               setDestandDeptAirport();
               createRoute(deptAirport, destAirport);

               par("interface").setStringValue("wlan0");

               host->subscribe(l2AssociatedSignal, this);
               host->subscribe(l2DisassociatedSignal,this);
               host->subscribe(l2AssociatedNewApSignal,this);
               host->subscribe(l2AssociatedOldApSignal,this);
               host->subscribe(l2BeaconLostSignal, this);
               host->subscribe(interfaceIpv4ConfigChangedSignal, this);

            }


            string AgentAddr = area->getFullPath() + ".atcAgent";
            par("atcAgentConnAddr").setStringValue(AgentAddr.c_str());
            par("connectAddress").setStringValue(AgentAddr.c_str());

            numResponseSend = numRequestReceived = numRequestSend = numResponseReceived = numHandoverRequest = interAgentHandover = intraAgentHandover = 0;
            numCpdlcRequestSend = numCpdlcRequestReceived = numCpdlcResponseSend = numCpdlcResponseReceived = 0;


            WATCH(numRequestSend);
            WATCH(numResponseReceived);
            WATCH(numResponseSend);
            WATCH(numRequestReceived);
            WATCH(handoverDelay);
            WATCH(numHandoverRequest);
            WATCH(interAgentHandover);
            WATCH(intraAgentHandover);
            WATCH(numCpdlcRequestSend);
            WATCH(numCpdlcResponseReceived);
            WATCH(numCpdlcRequestReceived);
            WATCH(numCpdlcResponseSend);

            startTime = par("startTime");
            stopTime = par("stopTime");

            serviceDurationPar = &par("serviceDuration");

            if (stopTime >= SIMTIME_ZERO && stopTime < startTime)
                throw cRuntimeError("Invalid startTime/stopTime parameters");
            timeoutMsg = new cMessage("timer");
        }
        else {
           EV_INFO << " the Node Named as  " << thisNodeName << " is not an CLIENT " <<  " Node Type: " << myNodeType << "\n";
           thisNodeType = NODE_INVALID;
        }
    }
    else if (stage == INITSTAGE_APPLICATION_LAYER) {

        }
}

void AcpClientApp::handleStartOperation(LifecycleOperation *operation)
{
    simtime_t now = simTime();
    simtime_t start = std::max(startTime, now);
    if (timeoutMsg && ((stopTime < SIMTIME_ZERO) || (start < stopTime) || (start == stopTime && startTime == stopTime))) {
        ie = chooseInterface();
        macAddress = ie->getMacAddress();
        thisNodeAddr = ie->getProtocolData<Ipv4InterfaceData>()->getIPAddress();
        rescheduleAtOrDeleteTimer(simTime(),MSGKIND_CONNECT);
    }
}
void AcpClientApp::handleStopOperation(LifecycleOperation *operation)
{
    cancelEvent(timeoutMsg);
   if (socket.getState() == TcpSocket::CONNECTED || socket.getState() == TcpSocket::CONNECTING || socket.getState() == TcpSocket::PEER_CLOSED)
       close();
}
void AcpClientApp::handleCrashOperation(LifecycleOperation *operation)
{
    cancelEvent(timeoutMsg);
    if (operation->getRootModule() != getContainingNode(this))
        socket.destroy();
}

void AcpClientApp::setCDA ()
{

    cModule *thisAirpotController = airport->getSubmodule("controller", 0);
    string FirsContactController = thisAirpotController->getFullPath();

    CurrentDataAuthority.nodeName = FirsContactController.c_str();
    par("currentDataAuthorityName").setStringValue(FirsContactController.c_str());

    string ConnectedAgentName = area->getFullPath() + ".atcAgent";
    CurrentDataAuthority.connectedAgent.nodeName = ConnectedAgentName.c_str();
    CurrentDataAuthority.connectedAgent.connectAddress = ConnectedAgentName.c_str();

}

void AcpClientApp::createRoute(routeType deptAirport, routeType destAirport)
{
    bool reachEndSector = false;

    int numofController = airport->getVectorSize();

    for (int cindex = 0; cindex < (numofController -1 ); cindex++)
    {
        routeType route;
        route.push_back(deptAirport[0]); // continent
        route.push_back(deptAirport[1]); // FIR
        route.push_back(deptAirport[2]); // Area
        route.push_back(deptAirport[3]); // Airport
        route.push_back(cindex); // Airport

        routeList.push_back(route);
    }

    for (int sindex = 0; sindex < numofController; sindex++)
    {
        routeType route;
        route.push_back(deptAirport[0]); // continent
        route.push_back(deptAirport[1]); // FIR
        route.push_back(deptAirport[2]); // Area
        route.push_back(sindex); // Airport

        routeList.push_back(route);
    }

    for (int sindex = 0; sindex < numofController; sindex++)
    {
        if (deptAirport[2] != destAirport[2]){

            routeType route;
            route.push_back(deptAirport[0]); // continent
            route.push_back(deptAirport[1]); // FIR
            route.push_back(destAirport[2]); // Area
            route.push_back(sindex); // Airport

            routeList.push_back(route);

        }
        else if (deptAirport[1] != destAirport[1]){

            routeType route;
            route.push_back(deptAirport[0]); // continent
            route.push_back(destAirport[1]); // FIR
            route.push_back(destAirport[2]); // Area
            route.push_back(sindex); // Airport

            routeList.push_back(route);
        }
        else if (deptAirport[0] != destAirport[0]){

            routeType route;
            route.push_back(destAirport[0]); // continent
            route.push_back(destAirport[1]); // FIR
            route.push_back(destAirport[2]); // Area
            route.push_back(sindex); // Airport

            routeList.push_back(route);

            reachEndSector = true;
        }
    }

    if (!reachEndSector)
    {
        for (int sindex = 0; sindex < numofController; sindex++)
        {

            if (deptAirport[1] != destAirport[1]){

                routeType route;
                route.push_back(deptAirport[0]); // continent
                route.push_back(destAirport[1]); // FIR
                route.push_back(destAirport[2]); // Area
                route.push_back(sindex); // Airport

                routeList.push_back(route);
            }
            else if (deptAirport[0] != destAirport[0]){

                routeType route;
                route.push_back(destAirport[0]); // continent
                route.push_back(destAirport[1]); // FIR
                route.push_back(destAirport[2]); // Area
                route.push_back(sindex); // Airport

                routeList.push_back(route);

                reachEndSector = true;
            }
        }
    }

    if (!reachEndSector)
    {
          for (int sindex = 0; sindex < numofController; sindex++)
          {

             if (deptAirport[0] != destAirport[0]){

                  routeType route;
                  route.push_back(destAirport[0]); // continent
                  route.push_back(destAirport[1]); // FIR
                  route.push_back(destAirport[2]); // Area
                  route.push_back(sindex); // Airport

                  routeList.push_back(route);

                  reachEndSector = true;
              }
          }
     }

     if (reachEndSector)
     {
        int cindex = numofController-1;
         while ((cindex > 0) or (cindex == 0))
        {

             routeType route;
             route.push_back(destAirport[0]); // continent
             route.push_back(destAirport[1]); // FIR
             route.push_back(destAirport[2]); // Area
             route.push_back(destAirport[3]); // Airport
             route.push_back(cindex); // Airport

             routeList.push_back(route);
             cindex --;
         }

     }
}

void AcpClientApp::setDestandDeptAirport()
{

    string Name = continent->getName();
    auto it = std::find(continentList.begin(), continentList.end(), Name);
    if (it == continentList.end())
    {
        // name not in vector
    } else
    {
        auto Index = std::distance(continentList.begin(), it);
        deptAirport.push_back(Index);
        deptAirport.push_back(fir->getIndex());
        deptAirport.push_back(area->getIndex());
        deptAirport.push_back(airport->getIndex());

    }

    destAirport.push_back(intrand(6)); // continent index
    destAirport.push_back(intrand(fir->getVectorSize()));
    destAirport.push_back(intrand(area->getVectorSize()));
    destAirport.push_back(intrand(airport->getVectorSize()));


}

AcpClientApp::routeType AcpClientApp::create_destairport()
{
    routeType dest;

    dest.push_back(intrand(6)); // continent index
    dest.push_back(intrand(fir->getVectorSize()));
    dest.push_back(intrand(area->getVectorSize()));
    dest.push_back(intrand(airport->getVectorSize()));

    return destAirport;

}

void AcpClientApp::setNDA () {
    // No Need to do someting

}



AcpClientApp::atcClient * AcpClientApp::retrieveNextDataAuthority (routeType destAirport)
{


    string name = host->getName();
    cModule *nextmodule = nullptr;

    if (!strcmp(name.c_str(),"controller"))
    {
        int cindex = host->getIndex();
        int numofController = host->getVectorSize();

        if (cindex != (numofController -1 )) {
            cindex ++;
            nextmodule = airport->getSubmodule("controller", cindex);

        }
        else  {
            string ar = area->getFullPath();
            nextmodule = area->getSubmodule("sector",0);


        }

    }
    else if (!strcmp(name.c_str(),"sector")) {
        int sindex = host->getIndex();
        int numofSector = host->getVectorSize();
        if (sindex != (numofSector -1 )) {
            sindex ++;
            nextmodule = area->getSubmodule("sector", sindex);

        }
        else {
            int aindex = area->getIndex();
            if (aindex != destAirport[2]) {
                nextmodule = fir->getSubmodule("AtcArea", destAirport[2]);
                nextmodule = nextmodule->getSubmodule("sector", 0);

            }
            else {
                int findex = fir->getIndex();
                if (findex != destAirport[1]) {
                    nextmodule = continent->getSubmodule("AtcFir", destAirport[1]);
                    nextmodule = nextmodule->getSubmodule("AtcArea", destAirport[2]);
                    nextmodule = nextmodule->getSubmodule("sector", 0);
                }
                else {
                    string Name = continent->getName();
                    auto it = std::find(continentList.begin(), continentList.end(), Name);
                    if (it == continentList.end())
                    {
                        // name not in vector
                    } else
                    {
                        auto cIndex = std::distance(continentList.begin(), it);
                        int numofController = area->getVectorSize();
                        if (cIndex != destAirport[0])
                        {

                            cModule *parentmodule = continent->getParentModule();
                            nextmodule = parentmodule->getSubmodule(continentList[destAirport[0]].c_str());
                            nextmodule = nextmodule->getSubmodule("AtcFir", destAirport[1]);
                            nextmodule = nextmodule->getSubmodule("AtcArea", destAirport[2]);
                            nextmodule = nextmodule->getSubmodule("sector", 0);
                        }
                        else {
                            cModule *parentmodule = continent->getParentModule();
                            cModule * nextmodule = parentmodule->getSubmodule(Name.c_str());
                            nextmodule = nextmodule->getSubmodule("AtcFir", destAirport[1]);
                            nextmodule = nextmodule->getSubmodule("AtcArea", destAirport[2]);
                            nextmodule = nextmodule->getSubmodule("airport", destAirport[3]);
                            nextmodule = nextmodule->getSubmodule("controller",numofController-1 );
                        }

                }
            }

        }

    }
    }


    string adjAtcName = nextmodule->getFullPath().c_str();
    string AdjAtcAgentName = area->getFullPath() + ".atcAgent";
    atcClient * NextAdjAtc = new atcClient ();
    NextAdjAtc->nodeName = adjAtcName.c_str();
    NextAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
    NextAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();

    return NextAdjAtc;
}



const char *  AcpClientApp::retrieveMobileNode(const char * nodeName)
{

   for (vector<string>::iterator t = mobileNodeList.begin(); t != mobileNodeList.end(); ++t)
   {
       if (!strcmp(t->c_str(),nodeName))
           return t->c_str();
   }

   return nullptr;

}

bool AcpClientApp::addMobileNode(const char * nodeName)
{
    if ( std::find(mobileNodeList.begin(), mobileNodeList.end(), nodeName) != mobileNodeList.end() )
        return false;
    else {
        mobileNodeList.push_back(nodeName);
        return true;
    }
}

bool AcpClientApp::removeMobileNode(const char * nodeName)
{

    for (vector<string>::iterator t = mobileNodeList.begin(); t != mobileNodeList.end(); ++t)
      {
          if (!strcmp(t->c_str(),nodeName))
          {
              mobileNodeList.erase(t);
              return true;
          }

      }
    return false;
}

void AcpClientApp::updateCDA(datalinkMsgBody * MsgBodyIn)
{
    par("currentDataAuthorityName").setStringValue(MsgBodyIn->appHeader.nextDataAuthorityName.c_str());
}

void AcpClientApp::updateNDA(datalinkMsgBody * MsgBodyIn)
{

    par("nextDataAuthorityName").setStringValue (MsgBodyIn->appHeader.nextDataAuthorityName.c_str());
    par("ndaAtcAgentName").setStringValue(MsgBodyIn->appHeader.nextAgentName.c_str());
    par("ndaAtcAgentAddr").setStringValue(MsgBodyIn->appHeader.nextAgentAddress.c_str());
}

const char * AcpClientApp::retrieveCDA()
{
    return par("currentDataAuthorityName");
}


void AcpClientApp::establishAgentConn()
{
    connect();
}

void AcpClientApp::updateAgentConn()
{
    const char * NDAname = par("ndaAtcAgentName");
    const char * NDAconnaddr = par("ndaAtcAgentAddr");

    par("atcAgentName").setStringValue(NDAname);
    par("atcAgentConnAddr").setStringValue(NDAconnaddr);
    par("connectAddress").setStringValue(NDAconnaddr);
}

void AcpClientApp::terminateAgentConn()
{
   close();
}

const char * AcpClientApp::retrieveAgentConn()
{
   return par("connectAddress");
}

const char * AcpClientApp::retrieveAgentName()
{
   return par("atcAgentName");
}



datalinkMsgHeader * create_datalinkHdr(AeroCommAppType applicationId,CommandType commandCode, bool request, int reqIdent, int thisNodeType )
{
    datalinkMsgHeader * hdr = new datalinkMsgHeader();
    hdr->version = 0;
    hdr->request = request;
    hdr->reTranmission = true;
    hdr->priority = 1;
    hdr->requestIdentifier = reqIdent;
    hdr->commandCode = commandCode;
    hdr->appId = applicationId;
    hdr->atmNodeType = thisNodeType;

    return hdr;
}


datalinkMsgBody *  AcpClientApp::create_datalinkBody(AeroCommAppType applicationId, CommandType commandCode, int destNodeType,const char * destNodeName, datalinkMsgBody * MsgBodyIn)
{
    datalinkMsgBody * body = new datalinkMsgBody();

    body->appHeader.origNodeType = thisNodeType;
    body->appHeader.origNodeName = thisNodeName;

    if (applicationId == DLCM)
        {
            body->appHeader.destNodeType = NODE_ATC_AGENT;
            body->appHeader.destNodeName = retrieveAgentName();
        }

    if (applicationId == CPDLC)
    {
        if (MsgBodyIn != nullptr)
            {
             body->appHeader.destNodeType = MsgBodyIn->appHeader.origNodeType;
             body->appHeader.destNodeName = MsgBodyIn->appHeader.origNodeName;
             body->appHeader.contextId =  MsgBodyIn->appHeader.contextId;

            }
        else{

            body->appHeader.destNodeType = destNodeType;
            body->appHeader.destNodeName = destNodeName;
        }

        if (((thisNodeType & 0xFF ) & 0xFF ) == NODE_FIXED_CLIENT)
        {
           if ((commandCode == CPDLC_CONNECT) || (commandCode == CPDLC_DISCONNECT))
             {
                  // atcClient * adjAtc = retrieveAdjAtc(FORWARD);

                  atcClient *nda = retrieveNextDataAuthority(create_destairport());
                  body->appHeader.nextDataAuthorityName = nda->nodeName;
                  body->appHeader.nextAgentName =  nda->connectedAgent.nodeName;
                  body->appHeader.nextAgentAddress =  nda->connectedAgent.connectAddress;
              }
        }
    }
    return body;
}


void AcpClientApp::initiateDatalinkApplication(AeroCommAppType applicationId, CommandType commandCode)
{
    long msgLength = par("atmMsgLength");
    datalinkMsgBody * body = new datalinkMsgBody();
    datalinkMsgHeader *hdr = create_datalinkHdr(applicationId,commandCode, true, 0, thisNodeType);
    numRequestSend ++;

    if (applicationId == DLCM)
        body = create_datalinkBody(applicationId,commandCode, NODE_ATC_AGENT,retrieveAgentName(), nullptr);
    else  if (applicationId == CPDLC) {
        body = create_datalinkBody(applicationId,commandCode, NODE_FIXED_CLIENT,retrieveCDA(), nullptr);
        numCpdlcRequestSend++;
    }

    const auto& payload = makeShared<AcpDatalinkMsg>();
        Packet *packet = new Packet("data");
        payload->setChunkLength(B(msgLength));
        payload->setHdr(*hdr);
        payload->setBody(*body);

        payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
        packet->insertAtBack(payload);

        sendPacket(packet);
}

void AcpClientApp::updateDatalinkConnection(AeroCommAppType applicationId, CommandType commandCode)
{
    long msgLength = par("atmMsgLength");
    datalinkMsgBody * body = new datalinkMsgBody();
    datalinkMsgHeader *hdr = create_datalinkHdr(applicationId,commandCode, true, 0, thisNodeType);
    numRequestSend ++;

    if (applicationId == DLCM)
        body = create_datalinkBody(applicationId,commandCode, NODE_ATC_AGENT,retrieveCDA(), nullptr);

    const auto& payload = makeShared<AcpDatalinkMsg>();
        Packet *packet = new Packet("data");
        payload->setChunkLength(B(msgLength));
        payload->setHdr(*hdr);
        payload->setBody(*body);

        payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
        packet->insertAtBack(payload);

        sendPacket(packet);
}


void AcpClientApp::terminateDatalinkApplication(AeroCommAppType applicationId, CommandType commandCode)
{
    long msgLength = par("atmMsgLength");
    datalinkMsgBody * body = new datalinkMsgBody();
    datalinkMsgHeader *hdr = create_datalinkHdr(applicationId,commandCode, true, 0, thisNodeType);
    numRequestSend ++;

    if (applicationId == DLCM)
        body = create_datalinkBody(applicationId,commandCode, NODE_ATC_AGENT,retrieveAgentName(), nullptr);
    else  if (applicationId == CPDLC) {
        body = create_datalinkBody(applicationId,commandCode, NODE_FIXED_CLIENT,retrieveCDA(), nullptr);
        numCpdlcRequestSend++;
    }


    const auto& payload = makeShared<AcpDatalinkMsg>();
        Packet *packet = new Packet("data");
        payload->setChunkLength(B(msgLength));
        payload->setHdr(*hdr);
        payload->setBody(*body);

        payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
        packet->insertAtBack(payload);

        sendPacket(packet);
}



void AcpClientApp::sendAcmMessage(AeroCommAppType applicationId, CommandType commandCode, datalinkMsgBody * MsgBodyIn, bool request)
{
    long msgLength = par("atmMsgLength");
    datalinkMsgHeader *hdr = create_datalinkHdr(applicationId,commandCode, request, 0, thisNodeType);
    // context id is 0 for now
    datalinkMsgBody * body = create_datalinkBody(applicationId,commandCode,0, nullptr,MsgBodyIn);


        if (request) {

                EV_INFO << "sending request with " << applicationId << " application " << commandCode << " command \n";
                numRequestSend ++;
                if (applicationId == CPDLC) {
                    numCpdlcRequestSend++;
                }
        }
        else {

            EV_INFO << "sending response with " << applicationId << " application " << commandCode << " command \n";
            numResponseSend ++;
            if (applicationId == CPDLC) {
               numCpdlcResponseSend++;
            }
        }
        const auto& payload = makeShared<AcpDatalinkMsg>();
            Packet *packet = new Packet("data");
            payload->setChunkLength(B(msgLength));
            payload->setHdr(*hdr);
            payload->setBody(*body);

            payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
            packet->insertAtBack(payload);


    sendPacket(packet);
}



void AcpClientApp::receiveSignal(cComponent *source, simsignal_t signal, cObject *object, cObject *details)
{
    Enter_Method("%s", cComponent::getSignalName(signal));

   if (signal == l2AssociatedSignal) {
        prevSSID = currentSSID;
        currentSSID = check_and_cast<inet::ieee80211::Ieee80211MgmtSta::ApInfo *>(details)->ssid;
        if (prevSSID.empty())
                prevSSID = currentSSID;
        else  if (strcmp(prevSSID.c_str(),currentSSID.c_str()))
                ssidChanged = true;
              else ssidChanged = false;
   }
   else if (signal == l2AssociatedNewApSignal) {
            if  (ssidChanged){
                    L2handoverCompleted = simTime();
                    L2handoverDelay = SIMTIME_DBL(L2handoverCompleted) - SIMTIME_DBL(L2handoverStarted);
                    numL2Handover++;
                    beaconGainTime = simTime();
            }
            else {
                beaconGainTime = simTime();
                beaconLostDuration = SIMTIME_DBL(beaconLostTime) - SIMTIME_DBL(beaconGainTime);
                L2handoverCompleted = simTime();
            }
   }
   else if (signal == l2AssociatedOldApSignal) {
       ssidChanged = false;
       L2handoverCompleted = simTime();
       L2handoverDelay = SIMTIME_DBL(L2handoverCompleted) - SIMTIME_DBL(L2handoverStarted);
       numL2Handover++;
   }
   else if (signal == l2DisassociatedSignal ) {
        L2handoverStarted = simTime();
   }
   else if (signal == l2BeaconLostSignal ) {
        numBeaconLost++;
        beaconLostTime = simTime();
   }
   else if ( signal == interfaceIpv4ConfigChangedSignal) {
        if (datalinkConnected) {
           if (isAppAddrChanged()) {
               abort ();
               datalinkConnected = false;
               sendUpdate = true;
               rescheduleAtOrDeleteTimer(simTime() + par("datalinkAddrCheckInterval"), MSGKIND_CONNECT);
            }
        }
   }
   else throw cRuntimeError("Unknown signal");
}

NetworkInterface *AcpClientApp::chooseInterface()
{
    IInterfaceTable *ift = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);
    const char *interfaceName = par("interface");
    NetworkInterface *ie = nullptr;

    if (strlen(interfaceName) > 0) {
        ie = ift->findInterfaceByName(interfaceName);
        if (ie == nullptr)
            throw cRuntimeError("Interface \"%s\" does not exist", interfaceName);
    }
    else {
        // there should be exactly one non-loopback interface that we want to configure
        for (int i = 0; i < ift->getNumInterfaces(); i++) {
            NetworkInterface *current = ift->getInterface(i);
            if (!current->isLoopback()) {
                if (ie)
                    throw cRuntimeError("Multiple non-loopback interfaces found, please select explicitly which one you want to configure via DHCP");
                ie = current;
            }
        }
        if (!ie)
            throw cRuntimeError("No non-loopback interface found to be configured via DHCP");
    }

  // if (!ie->getProtocolData<Ipv4InterfaceData>()->getIPAddress().isUnspecified())
      // EV_INFO << " the Node Named as \n";
    //   throw cRuntimeError("Client \"%s\" has not an IP address", ie->getInterfaceName());
    return ie;
}


bool AcpClientApp::isAppAddrAssignedtoInterface()
{
    if (auto ipv4Data = ie->findProtocolData<Ipv4InterfaceData>()) {
            if (ipv4Data->getIPAddress().isUnspecified())
            { return false; }
            else {
                thisNodeAddr = ipv4Data->getIPAddress();
                par("localAddress").setStringValue(thisNodeAddr.str());
                return true;
            }
    }
    else return false;
}
bool AcpClientApp::isAppAddrAssignedtoModule()
{
    if (thisNodeAddr.isUnspecified()) { return false; }
    else return true;
}

bool AcpClientApp::isAppAddrChanged()
{
    Ipv4Address newIP = ie->getProtocolDataForUpdate<Ipv4InterfaceData>()->getIPAddress();

    string currentIP = thisNodeAddr.str();
    string newIPAddress = newIP.str();
    if (thisNodeAddr.equals(newIP))
            return false;
    else if (!newIP.isUnspecified()) {
            thisNodeAddr = newIP;
            par("localAddress").setStringValue(thisNodeAddr.str());
    }
    return true;
}

bool AcpClientApp::tcpSocketConnected()
{
    cancelEvent(timeoutMsg);
    if (socket.getState() == TcpSocket::CONNECTED || socket.getState() == TcpSocket::CONNECTING || socket.getState() == TcpSocket::PEER_CLOSED)
            return true;
    else return false;
}
void AcpClientApp::handleTimer(cMessage *msg)
{
    switch (msg->getKind()) {

    case MSGKIND_CONNECT:
            if ((!datalinkConnected)) {
                if (isAppAddrAssignedtoInterface()) {
                    if (!tcpSocketConnected()) {
                        establishAgentConn();
                    }
                    else rescheduleAtOrDeleteTimer(simTime() + par("datalinkAddrCheckInterval"),MSGKIND_CONNECT);
                }
                else rescheduleAtOrDeleteTimer(simTime() + par("datalinkAddrCheckInterval"),MSGKIND_CONNECT);
            }
            else if (!atmConnected) rescheduleAtOrDeleteTimer(simTime(),MSGKIND_LOGON);
                else if (!atmOnline) rescheduleAtOrDeleteTimer(simTime(),MSGKIND_STARTSERVICE);

        break;

        case MSGKIND_LOGON:

            if ((!atmConnected) && (!atmOnline))
                initiateDatalinkApplication(DLCM,LOGON);
            break;

        case MSGKIND_LOGOUT:
             if ((atmConnected) && (!atmOnline))
               terminateDatalinkApplication(DLCM,LOGOUT);
             break;

        case MSGKIND_STARTSERVICE:
            if ((atmConnected) && (!atmOnline))
              initiateDatalinkApplication(CPDLC,CPDLC_CONNECT);
        break;

        case MSGKIND_ENDSERVICE:
             if((atmConnected) && (atmOnline))
                terminateDatalinkApplication(CPDLC,CPDLC_DISCONNECT);
             break;

        case MSGKIND_UPDATE:
             updateDatalinkConnection(DLCM,UPDATE);
             break;

        case MSGKIND_CLOSE:
            if (datalinkConnected)
             terminateAgentConn();
        break;

        default:
            throw cRuntimeError("Invalid timer msg: kind=%d", msg->getKind());
    }
}


bool AcpClientApp::startCpdlcExchange()
{
    simtime_t now = simTime();
    double exchangeDuration = *serviceDurationPar;
    if (exchangeDuration < 0.0)
        return false;
    else {
        endService = now + exchangeDuration;
        return true;
    }
}

bool AcpClientApp::stopCpdlcEchange()
{
    simtime_t now = simTime();
    if (endService > now)
       return false;
    return true;
}

void AcpClientApp::handleDatalinkMsg(const Ptr<const AcpDatalinkMsg>& msg)
{
    datalinkMsgHeader hdr = msg->getHdr();
    datalinkMsgBody body = msg->getBody();


    if (hdr.request) {

        numRequestReceived ++;
        switch (hdr.appId) {

               case  DLCM:
                   switch  (hdr.commandCode) {

                       case CONTACT:
                             break;

                       default:
                          EV_INFO << " Unknown Command ID: " << hdr.commandCode <<  " for application " << hdr.appId << "\n";
                          break;
                   }
                   break;

                case CPDLC:

                    numCpdlcRequestReceived++;

                    switch  (hdr.commandCode) {

                        case CPDLC_CONNECT:
                            if (((thisNodeType & 0xFF ) & 0xFF ) == NODE_FIXED_CLIENT) {

                              sendAcmMessage(CPDLC,CPDLC_CONNECT,&body,false);
                              addMobileNode(body.appHeader.origNodeName.c_str());
                            }
                            else if (((thisNodeType & 0xFF ) & 0xFF ) == NODE_MOBILE_CLIENT) {
                                sendAcmMessage(CPDLC,CPDLC_CONNECT,&body,false);
                                updateNDA(&body);
                            }
                            intraAgentHandover = false;
                            atmOnline = true;
                            break;

                        case CPDLC_MSG:
                            if (((thisNodeType & 0xFF ) & 0xFF ) == NODE_FIXED_CLIENT)
                                sendAcmMessage(CPDLC,CPDLC_MSG,&body,false);
                            else  if (((thisNodeType & 0xFF ) & 0xFF ) == NODE_MOBILE_CLIENT)
                                sendAcmMessage(CPDLC,CPDLC_MSG,&body,false);
                            break;

                          case CPDLC_DISCONNECT:
                              if (((thisNodeType & 0xFF ) & 0xFF ) == NODE_FIXED_CLIENT) {
                                 sendAcmMessage(CPDLC,CPDLC_DISCONNECT,&body,false);
                                 removeMobileNode(body.appHeader.origNodeName.c_str());
                              }
                              else if (((thisNodeType & 0xFF ) & 0xFF ) == NODE_MOBILE_CLIENT) {
                                  sendAcmMessage(CPDLC,CPDLC_DISCONNECT,&body,false);
                                  updateCDA(&body);
                                  if (!strcmp(body.appHeader.nextAgentAddress.c_str(),par("connectAddress")))
                                     terminateDatalinkApplication(DLCM,LOGOUT);
                                  else
                                     initiateDatalinkApplication(CPDLC,CPDLC_CONNECT);
                              }
                              atmOnline = false;
                              break;

                           default:
                               EV_INFO << " Unknown Command ID: " << hdr.commandCode <<  " for application " << hdr.appId << "\n";
                               break;
                    }
                   break;

                default:
                    break;
                 }

    }
    else {

        numResponseReceived ++;
        switch (hdr.appId) {
               case  DLCM:
                   switch  (hdr.commandCode) {
                       case LOGON:
                           if ((thisNodeType & 0xFF ) == NODE_MOBILE_CLIENT) {
                               if (AuthorityTransferInProgress){
                                   handoverCompleted = simTime();
                                   handoverDelay = SIMTIME_DBL(handoverCompleted) - SIMTIME_DBL(handoverStarted);
                                   AuthorityTransferInProgress = false;
                                   interAgentHandover ++;
                               }
                               if (!atmOnline) {
                                   rescheduleAtOrDeleteTimer(simTime(),MSGKIND_STARTSERVICE);
                                   // initiateDatalinkApplication(CPDLC,CPDLC_CONNECT);
                               }

                           }
                           else if ((thisNodeType & 0xFF ) == NODE_FIXED_CLIENT){} // do nothing
                           atmConnected = true;
                           break;

                       case LOGOUT:
                           if ((thisNodeType & 0xFF ) == NODE_MOBILE_CLIENT) {
                               if (!AuthorityTransferInProgress) {
                                  handoverStarted = simTime();
                                  AuthorityTransferInProgress = true;
                               }
                               updateAgentConn();
                               rescheduleAtOrDeleteTimer(simTime(),MSGKIND_CLOSE);
                           }
                           else if ((thisNodeType & 0xFF ) == NODE_FIXED_CLIENT) {
                               rescheduleAtOrDeleteTimer(simTime(),MSGKIND_CLOSE);
                           }
                           atmConnected = false;
                           break;

                       case UPDATE:
                             sendUpdate = false;
                        break;

                       default:
                           EV_INFO << " Unknown Command ID: " << hdr.commandCode <<  " for application " << hdr.appId << "\n";
                          break;
                   } // close inner switch
                   break;

                case CPDLC:

                    numCpdlcResponseReceived++;

                    switch  (hdr.commandCode) {

                        case CPDLC_CONNECT:
                            if (((thisNodeType & 0xFF ) & 0xFF ) == NODE_MOBILE_CLIENT) {
                                if (AuthorityTransferInProgress )
                                {
                                    handoverCompleted = simTime();
                                    handoverDelay = SIMTIME_DBL(handoverCompleted) - SIMTIME_DBL(handoverStarted);
                                    AuthorityTransferInProgress = false;
                                    intraAgentHandover ++;
                                }
                                updateNDA(&body);
                                if (startCpdlcExchange())
                                    sendAcmMessage(CPDLC,CPDLC_MSG,&body,true);
                                else throw cRuntimeError("CPDLC service not started ");

                            }
                            if (((thisNodeType & 0xFF ) & 0xFF ) == NODE_FIXED_CLIENT)
                            {
                                addMobileNode(body.appHeader.origNodeName.c_str());
                                sendAcmMessage(CPDLC,CPDLC_MSG,&body,true);

                            }
                            atmOnline = true;
                            break;

                        case CPDLC_MSG:
                            if (((thisNodeType & 0xFF ) & 0xFF ) == NODE_MOBILE_CLIENT) {

                                if (stopCpdlcEchange())
                                    rescheduleAtOrDeleteTimer(simTime(),MSGKIND_ENDSERVICE);
                                else
                                   sendAcmMessage(CPDLC,CPDLC_MSG,&body,true);

                            }
                            else  if (((thisNodeType & 0xFF ) & 0xFF ) == NODE_FIXED_CLIENT)
                            {
                                int maxNumberofCpdlMessages = par("maxNumberofMessages");
                                if ( maxNumberofCpdlMessages == 0 )
                                    sendAcmMessage(CPDLC,CPDLC_DISCONNECT,&body,true); // this is from FIXED NODE
                                else sendAcmMessage(CPDLC,CPDLC_MSG,&body,true);
                            }

                            break;

                        case CPDLC_DISCONNECT:
                            if (((thisNodeType & 0xFF ) & 0xFF ) == NODE_MOBILE_CLIENT)
                            {
                                updateCDA(&body);

                                if (!AuthorityTransferInProgress) {
                                     handoverStarted = simTime();
                                     AuthorityTransferInProgress = true;
                                 }
                                if (!strcmp(body.appHeader.nextAgentAddress.c_str(),retrieveAgentConn()))
                                    initiateDatalinkApplication(CPDLC,CPDLC_CONNECT);
                                else
                                    terminateDatalinkApplication(DLCM,LOGOUT);

                            }
                            else if (((thisNodeType & 0xFF ) & 0xFF ) == NODE_FIXED_CLIENT)
                                removeMobileNode(body.appHeader.origNodeName.c_str());
                            atmOnline = false;
                            break;

                        default:
                            break;
                    }

                   break;
                default:
                    EV_INFO << " Unknown Application ID: " << hdr.appId << "\n";
                     break;
           } // close outer switch
    }

}


void AcpClientApp::scheduleAtOrDeleteTimer(simtime_t d, short int msgKind)
{
    if (stopTime < SIMTIME_ZERO || d < stopTime) {
         timeoutMsg->setKind(msgKind);
         scheduleAt(d, timeoutMsg);
     }
     else {
         cancelAndDelete(timeoutMsg);
         timeoutMsg = nullptr;
      }
}
void AcpClientApp::rescheduleAtOrDeleteTimer(simtime_t d, short int msgKind)
{
    if (stopTime < SIMTIME_ZERO || d < stopTime) {
         timeoutMsg->setKind(msgKind);
         rescheduleAt(d, timeoutMsg);
    }
    else {
        cancelAndDelete(timeoutMsg);
        timeoutMsg = nullptr;
    }
}

void AcpClientApp::socketEstablished(TcpSocket *socket)
{
    TcpAppBase::socketEstablished(socket);
    datalinkConnected = true;
    if ((sendUpdate) && (atmConnected)) {
        if (timeoutMsg) rescheduleAtOrDeleteTimer(simTime(), MSGKIND_UPDATE);
    }
    else
        if (timeoutMsg) rescheduleAtOrDeleteTimer(simTime(), MSGKIND_LOGON);
}


void AcpClientApp::socketDataArrived(TcpSocket *socket, Packet *rcvdPkt, bool urgent)
{
    TcpAppBase::socketDataArrived(socket, rcvdPkt, urgent);
    const auto& msg = rcvdPkt->peekData<AcpDatalinkMsg>();
    handleDatalinkMsg(msg);
}



void AcpClientApp::close()
{
    TcpAppBase::close();
    cancelEvent(timeoutMsg);
    datalinkConnected = false;
}


void AcpClientApp::socketClosed(TcpSocket *socket)
{
    TcpAppBase::socketClosed(socket);
    // start another session after a delay
   if (operationalState == State::OPERATING) {
       // start another session after a delay
       if (!datalinkConnected)
           rescheduleAtOrDeleteTimer(simTime(), MSGKIND_CONNECT);
   }
   else if (operationalState == State::STOPPING_OPERATION) {
        if (!this->socket.isOpen())
             startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
    }
}

void AcpClientApp::socketFailure(TcpSocket *socket, int code)
{
    TcpAppBase::socketFailure(socket, code);
    datalinkConnected = false;
    if (timeoutMsg) rescheduleAtOrDeleteTimer(simTime() + par("reconnectInterval"), MSGKIND_CONNECT);
}

} // namespace inet

/***************************************************************************************

void AcpClientApp::setPrevAdjRdrAtc ()
{

    int cindex = host->getIndex();
    int numofController = host->getVectorSize();


    if (cindex != 0 ) {

        cModule *prevmodule = area->getSubmodule("sector", cindex - 1);
        string adjAtcName = prevmodule->getFullPath().c_str();
        string AdjAtcAgentName = area->getFullPath() + ".atcAgent";
        atcClient * PrevAdjAtc = new atcClient ();
        PrevAdjAtc->nodeName = adjAtcName.c_str();
        PrevAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
        PrevAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
        addAdjAtc(PrevAdjAtc);
    }
}

void AcpClientApp::setPrevAdjGrdAtc ()
{

    int cindex = host->getIndex();

    if (cindex != 0 ) {

        cModule *prevmodule = airport->getSubmodule("controller", cindex - 1);
        string adjAtcName = prevmodule->getFullPath().c_str();
        string AdjAtcAgentName = area->getFullPath() + ".atcAgent";
        atcClient * PrevAdjAtc = new atcClient ();
        PrevAdjAtc->nodeName = adjAtcName.c_str();
        PrevAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
        PrevAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
        addAdjAtc(PrevAdjAtc);

    }
}


void AcpClientApp::setNextAdjAtc ()
{

    int cindex = host->getIndex();
    int airportIndex = airport->getIndex();
    int areaIndex = area->getIndex();
    int firIndex = fir->getIndex();


    int numofController = host->getVectorSize();
    int numofAirport = airport->getVectorSize();
    int numofArea = area->getVectorSize();
    int numofFir = fir->getVectorSize();

    if (cindex != (numofController -1 )) {
        cindex ++;
        cModule *nextmodule = airport->getSubmodule("controller", cindex);
        string adjAtcName = nextmodule->getFullPath().c_str();
        string AdjAtcAgentName = area->getFullPath() + ".atcAgent";
        atcClient * NextAdjAtc = new atcClient ();
        NextAdjAtc->nodeName = adjAtcName.c_str();
        NextAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
        NextAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
        addAdjAtc(NextAdjAtc);
    }
    else {

        if (airportIndex != (numofAirport-1)) {
            airportIndex ++ ;
            cModule *nextmodule = area->getSubmodule("airport", airportIndex);
            nextmodule = nextmodule->getSubmodule("controller", 0);
            string adjAtcName = nextmodule->getFullPath();
            string AdjAtcAgentName = area->getFullPath() + ".atcAgent";
            atcClient * NextAdjAtc = new atcClient ();
            NextAdjAtc->nodeName = adjAtcName.c_str();
            NextAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
            NextAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
            addAdjAtc(NextAdjAtc);

        }
        else {

            if (areaIndex != (numofArea-1)) {
                areaIndex ++;
                cModule *nextmodule = fir->getSubmodule("AtcArea", areaIndex);
                string AdjAtcAgentName = nextmodule->getFullPath() + ".atcAgent";
                nextmodule = nextmodule->getSubmodule("airport", 0);
                nextmodule = nextmodule->getSubmodule("controller", 0);
                string adjAtcName = nextmodule->getFullPath();
                atcClient * NextAdjAtc = new atcClient ();
                NextAdjAtc->nodeName = adjAtcName.c_str();
                NextAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
                NextAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
                addAdjAtc(NextAdjAtc);
            }
            else {

                if (firIndex != (numofFir-1)) {
                    firIndex ++;
                    cModule *nextmodule = continent->getSubmodule("AtcFir", firIndex);
                    nextmodule = nextmodule->getSubmodule("AtcArea", 0);
                    string AdjAtcAgentName = nextmodule->getFullPath() + ".atcAgent";
                    nextmodule = nextmodule->getSubmodule("airport", 0);
                    nextmodule = nextmodule->getSubmodule("controller", 0);
                    string adjAtcName = nextmodule->getFullPath();
                    atcClient * NextAdjAtc = new atcClient ();
                    NextAdjAtc->nodeName = adjAtcName.c_str();
                    NextAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
                    NextAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
                    addAdjAtc(NextAdjAtc);
                }
                else {

                   string Name = continent->getName();
                   if (strcmp(Name.c_str(),"asia")) {
                       cModule *parentmodule = continent->getParentModule();
                       cModule * continentModule = parentmodule->getSubmodule("asia");
                       string Name = continentModule->getName();
                       cModule *nextmodule = continentModule->getSubmodule("AtcFir", 0);
                       nextmodule = nextmodule->getSubmodule("AtcArea", 0);
                       string AdjAtcAgentName = nextmodule->getFullPath() + ".atcAgent";
                       nextmodule = nextmodule->getSubmodule("airport", 0);
                       nextmodule = nextmodule->getSubmodule("controller", 0);
                       string adjAtcName = nextmodule->getFullPath();
                       atcClient * NextAdjAtc = new atcClient ();
                       NextAdjAtc->nodeName = adjAtcName.c_str();
                       NextAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
                       NextAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
                       addAdjAtc(NextAdjAtc);
                   }

                   if (strcmp(Name.c_str(),"africa")) {
                       cModule *parentmodule = continent->getParentModule();
                       cModule * continentModule = parentmodule->getSubmodule("asia");
                       string Name = continentModule->getName();
                       cModule *nextmodule = continentModule->getSubmodule("AtcFir", 0);
                       nextmodule = nextmodule->getSubmodule("AtcArea", 0);
                       string AdjAtcAgentName = nextmodule->getFullPath() + ".atcAgent";
                       nextmodule = nextmodule->getSubmodule("airport", 0);
                       nextmodule = nextmodule->getSubmodule("controller", 0);
                       string adjAtcName = nextmodule->getFullPath();
                       atcClient * NextAdjAtc = new atcClient ();
                       NextAdjAtc->nodeName = adjAtcName.c_str();
                       NextAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
                       NextAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
                       addAdjAtc(NextAdjAtc);
                   }

                   if (strcmp(Name.c_str(),"africa")) {
                       cModule *parentmodule = continent->getParentModule();
                       cModule * continentModule = parentmodule->getSubmodule("africa");
                       string Name = continentModule->getName();
                       cModule *nextmodule = continentModule->getSubmodule("AtcFir", 0);
                       nextmodule = nextmodule->getSubmodule("AtcArea", 0);
                       string AdjAtcAgentName = nextmodule->getFullPath() + ".atcAgent";
                       nextmodule = nextmodule->getSubmodule("airport", 0);
                       nextmodule = nextmodule->getSubmodule("controller", 0);
                       string adjAtcName = nextmodule->getFullPath();
                       atcClient * NextAdjAtc = new atcClient ();
                       NextAdjAtc->nodeName = adjAtcName.c_str();
                       NextAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
                       NextAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
                       addAdjAtc(NextAdjAtc);
                   }
                   if (strcmp(Name.c_str(),"australia")) {
                       cModule *parentmodule = continent->getParentModule();
                       cModule * continentModule = parentmodule->getSubmodule("australia");
                       string Name = continentModule->getName();
                       cModule *nextmodule = continentModule->getSubmodule("AtcFir", 0);
                       nextmodule = nextmodule->getSubmodule("AtcArea", 0);
                       string AdjAtcAgentName = nextmodule->getFullPath() + ".atcAgent";
                       nextmodule = nextmodule->getSubmodule("airport", 0);
                       nextmodule = nextmodule->getSubmodule("controller", 0);
                       string adjAtcName = nextmodule->getFullPath();
                       atcClient * NextAdjAtc = new atcClient ();
                       NextAdjAtc->nodeName = adjAtcName.c_str();
                       NextAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
                       NextAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
                       addAdjAtc(NextAdjAtc);
                   }
                   if (strcmp(Name.c_str(),"europe")) {
                       cModule *parentmodule = continent->getParentModule();
                       cModule * continentModule = parentmodule->getSubmodule("europe");
                       string Name = continentModule->getName();
                       cModule *nextmodule = continentModule->getSubmodule("AtcFir", 0);
                       nextmodule = nextmodule->getSubmodule("AtcArea", 0);
                       string AdjAtcAgentName = nextmodule->getFullPath() + ".atcAgent";
                       nextmodule = nextmodule->getSubmodule("airport", 0);
                       nextmodule = nextmodule->getSubmodule("controller", 0);
                       string adjAtcName = nextmodule->getFullPath();
                       atcClient * NextAdjAtc = new atcClient ();
                       NextAdjAtc->nodeName = adjAtcName.c_str();
                       NextAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
                       NextAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
                       addAdjAtc(NextAdjAtc);
                   }
                   if (strcmp(Name.c_str(),"northAmerica")) {
                       cModule *parentmodule = continent->getParentModule();
                       cModule * continentModule = parentmodule->getSubmodule("northAmerica");
                       string Name = continentModule->getName();
                       cModule *nextmodule = continentModule->getSubmodule("AtcFir", 0);
                       nextmodule = nextmodule->getSubmodule("AtcArea", 0);
                       string AdjAtcAgentName = nextmodule->getFullPath() + ".atcAgent";
                       nextmodule = nextmodule->getSubmodule("airport", 0);
                       nextmodule = nextmodule->getSubmodule("controller", 0);
                       string adjAtcName = nextmodule->getFullPath();
                       atcClient * NextAdjAtc = new atcClient ();
                       NextAdjAtc->nodeName = adjAtcName.c_str();
                       NextAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
                       NextAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
                       addAdjAtc(NextAdjAtc);
                   }
                   if (strcmp(Name.c_str(),"southAmerica")) {
                       cModule *parentmodule = continent->getParentModule();
                       cModule * continentModule = parentmodule->getSubmodule("southAmerica");
                       string Name = continentModule->getName();
                       cModule *nextmodule = continentModule->getSubmodule("AtcFir", 0);
                       nextmodule = nextmodule->getSubmodule("AtcArea", 0);
                       string AdjAtcAgentName = nextmodule->getFullPath() + ".atcAgent";
                       nextmodule = nextmodule->getSubmodule("airport", 0);
                       nextmodule = nextmodule->getSubmodule("controller", 0);
                       string adjAtcName = nextmodule->getFullPath();
                       atcClient * NextAdjAtc = new atcClient ();
                       NextAdjAtc->nodeName = adjAtcName.c_str();
                       NextAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
                       NextAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
                       addAdjAtc(NextAdjAtc);
                   }
                }


            }


        }
    }
}


void AcpClientApp::setPrevAdjAtc ()
{

    int cindex = host->getIndex();
    int airportIndex = airport->getIndex();
    int areaIndex = area->getIndex();
    int firIndex = fir->getIndex();

    int numofController = host->getVectorSize();
    int numofAirport = airport->getVectorSize();
    int numofArea = area->getVectorSize();
    int numofFir = fir->getVectorSize();

    if (cindex != 0 ) {

        cModule *prevmodule = airport->getSubmodule("controller", cindex - 1);
        string adjAtcName = prevmodule->getFullPath().c_str();
        string AdjAtcAgentName = area->getFullPath() + ".atcAgent";
        atcClient * PrevAdjAtc = new atcClient ();
        PrevAdjAtc->nodeName = adjAtcName.c_str();
        PrevAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
        PrevAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
        addAdjAtc(PrevAdjAtc);

    }
    else  if (airportIndex != 0) {

        cModule *prevmodule = area->getSubmodule("airport", airportIndex -1);
        prevmodule = prevmodule->getSubmodule("controller", numofController -1);
        string adjAtcName = prevmodule->getFullPath().c_str();
        string AdjAtcAgentName = area->getFullPath() + ".atcAgent";
        atcClient * PrevAdjAtc = new atcClient ();
        PrevAdjAtc->nodeName = adjAtcName.c_str();
        PrevAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
        PrevAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
        addAdjAtc(PrevAdjAtc);
    }
    else if (areaIndex != 0)  {

         cModule *prevmodule = fir->getSubmodule("AtcArea", areaIndex -1);
         string AdjAtcAgentName = prevmodule->getFullPath() + ".atcAgent";
         prevmodule = prevmodule->getSubmodule("airport", numofAirport-1);
         prevmodule = prevmodule->getSubmodule("controller", numofController -1);
         string adjAtcName = prevmodule->getFullPath().c_str();

         atcClient * PrevAdjAtc = new atcClient ();
         PrevAdjAtc->nodeName = adjAtcName.c_str();
         PrevAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
         PrevAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
         addAdjAtc(PrevAdjAtc);
     }
    else if (firIndex != 0) {

        cModule *prevmodule = continent->getSubmodule("AtcFir", firIndex - 1);
        prevmodule = prevmodule->getSubmodule("AtcArea", numofArea -1);
        string AdjAtcAgentName = prevmodule->getFullPath() + ".atcAgent";
        prevmodule = prevmodule->getSubmodule("airport", numofAirport-1);
        prevmodule = prevmodule->getSubmodule("controller", numofController -1);
        string adjAtcName = prevmodule->getFullPath().c_str();

        atcClient * PrevAdjAtc = new atcClient ();
        PrevAdjAtc->nodeName = adjAtcName.c_str();
        PrevAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
        PrevAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
        addAdjAtc(PrevAdjAtc);
    }
}

void AcpClientApp::setNextAdjRdrAtc ()
{

    int cindex = host->getIndex();
    int numofController = host->getVectorSize();

    if (cindex != (numofController -1 )) {
        cindex ++;
        cModule *nextmodule = sector->getSubmodule("rdrController", cindex);
        string adjAtcName = nextmodule->getFullPath().c_str();
        string AdjAtcAgentName = area->getFullPath() + ".atcAgent";
        atcClient * NextAdjAtc = new atcClient ();
        NextAdjAtc->nodeName = adjAtcName.c_str();
        NextAdjAtc->connectedAgent.nodeName = AdjAtcAgentName.c_str();
        NextAdjAtc->connectedAgent.connectAddress = AdjAtcAgentName.c_str();
        addAdjAtc(NextAdjAtc);
    }
}




AcpClientApp::atcClient * AcpClientApp::retrieveAdjAtc(int nth)
{
   int i = 0;
   for (vector<atcClient>::iterator t = adjAtcUnitList.begin(); t != adjAtcUnitList.end(); ++t, i++)
   {
        if (i==nth)
            return &adjAtcUnitList.at(nth);
   }

   return nullptr;

}

bool AcpClientApp::addAdjAtc(atcClient * adjAtc)
{
    //if ( std::find(adjAtcUnitList.begin(), adjAtcUnitList.end(), adjAtc->nodeName) != adjAtcUnitList.end() )

    for (vector<atcClient>::iterator t = adjAtcUnitList.begin(); t != adjAtcUnitList.end(); ++t)
    {
        if (!strcmp(t->nodeName,adjAtc->nodeName))
            return true;
    }

    adjAtcUnitList.push_back(*adjAtc);
    return false;
}

bool AcpClientApp::removeAdjAtc(const char * nodeName)
{

    for (vector<atcClient>::iterator t = adjAtcUnitList.begin(); t != adjAtcUnitList.end(); ++t)
      {
          if (!strcmp(t->nodeName,nodeName))
          {
              adjAtcUnitList.erase(t);
              return true;
          }

      }
    return false;
}

******************************************************************************************************/


