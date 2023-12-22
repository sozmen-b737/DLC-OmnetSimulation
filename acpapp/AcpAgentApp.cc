//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "AcpAgentApp.h"
#include "AcpDatalinkMsg_m.h"

#include "inet/common/ModuleAccess.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/Simsignals.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "inet/common/socket/SocketTag_m.h"
#include "inet/transportlayer/contract/tcp/TcpCommand_m.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/networklayer/common/FragmentationTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/common/TransportPseudoHeader_m.h"
#include "inet/transportlayer/contract/tcp/TcpCommand_m.h"
#include "inet/transportlayer/tcp/Tcp.h"
#include "inet/transportlayer/tcp/TcpConnection.h"
#include "inet/transportlayer/tcp/TcpReceiveQueue.h"
#include "inet/transportlayer/tcp/TcpSendQueue.h"
#include "inet/transportlayer/tcp_common/TcpHeader.h"

#include <stdio.h>
#include <string.h>


namespace inet {

Define_Module(AcpAgentApp);
Define_Module(AcpAgentAppThread);

AcpAgentApp::AcpAgentApp()
{
}

AcpAgentApp::~AcpAgentApp()
{
}

void AcpAgentApp::initialize(int stage)
{
    TcpServerHostApp::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {

        host = getContainingNode(this);
        thisNodeName = host->getFullName();// par("nodeName");
        thisNodeAddr = L3AddressResolver().resolve( par("localAddress"));

        int myNodeType = par("nodeType");
        int myNodeRole = par("nodeRole");

        if (myNodeType == AGENT && myNodeRole == ATC) {
            thisNodeType = NODE_ATC_AGENT;

            numRequestSend = numRequestReceived = numResponseSend = numResponseReceived = 0;

            WATCH(numRequestSend);
            WATCH(numRequestReceived);
            WATCH(numResponseSend);
            WATCH(numResponseReceived);
        }
        else {
            EV_INFO << " the Node Named as  " << thisNodeName << " is not an AGENT " <<  " Node Type: " << myNodeType  << " Node Role  " << myNodeRole << "\n";
            thisNodeType = NODE_INVALID;
        }
    }
}


void AcpAgentApp::sendDown(Packet *msg)
{
    if (msg->isPacket()) {
        Packet *pk = static_cast<Packet *>(msg);
        emit(packetSentSignal, pk);
    }

    msg->addTagIfAbsent<DispatchProtocolReq>()->setProtocol(&Protocol::tcp);
    msg->getTag<SocketReq>();
    send(msg, "socketOut");
}

void AcpAgentApp::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();

    char buf[160];
    sprintf(buf, "threads: %d\numRequestReceived: %d \numResponseSend: %d numRequestSend: %d \numResponseReceived: %d", socketMap.size(), numRequestReceived, numResponseSend, numRequestSend,numResponseReceived);
    getDisplayString().setTagArg("t", 0, buf);
}

void AcpAgentApp::finish()
{
    TcpServerHostApp::finish();

    recordScalar("numRequestSend", numRequestSend);
    recordScalar("numRequestReceived", numRequestReceived);
    recordScalar("numResponseSend", numResponseSend);
    recordScalar("numResponseReceived", numRequestReceived);
}

datalinkMsgHeader * AcpAgentApp::agent_create_datalinkHdr(AeroCommAppType applicationId,CommandType commandCode, bool request, int reqIdent, int thisNodeType )
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


datalinkMsgBody * AcpAgentApp::agent_create_datalinkBody(AeroCommAppType applicationId,CommandType commandCode,  datalinkMsgBody * MsgBodyIn)
{
    datalinkMsgBody * body = new datalinkMsgBody();

    if (applicationId == DLCM) {

        body->appHeader.origNodeType = thisNodeType;
        body->appHeader.origNodeName = thisNodeName;
        body->appHeader.destNodeType = MsgBodyIn->appHeader.origNodeType;
        body->appHeader.destNodeName = MsgBodyIn->appHeader.origNodeName;

       }
       else if (applicationId == CPDLC) {  // forwarding msg

           body->appHeader.origNodeType = MsgBodyIn->appHeader.origNodeType;
           body->appHeader.origNodeName = MsgBodyIn->appHeader.origNodeName;
           body->appHeader.destNodeType = MsgBodyIn->appHeader.destNodeType;
           body->appHeader.destNodeName = MsgBodyIn->appHeader.destNodeName;


           if (((MsgBodyIn->appHeader.origNodeType & 0xFF ) & 0xFF ) == NODE_FIXED_CLIENT)
           {

               if ((commandCode == CPDLC_CONNECT) || (commandCode == CPDLC_DISCONNECT))
               {
                   body->appHeader.nextDataAuthorityName = MsgBodyIn->appHeader.nextDataAuthorityName;
                   body->appHeader.nextAgentName= MsgBodyIn->appHeader.nextAgentName;
                   body->appHeader.nextAgentAddress = MsgBodyIn->appHeader.nextAgentAddress;
               }
           }
       }
    return body;
}


int AcpAgentApp::retrieveConnId(opp_string nodeName)
{

    auto itr = atmNodeConnMap.find(nodeName);
      if (itr != atmNodeConnMap.end()) {
          return  (*itr).second;
      }
      return 0;
}

bool AcpAgentApp::deleteSocketfromMap(opp_string nodeName)
{

    int sockid = isSocketExist(nodeName);
    ISocket * thisSocket = TcpServerHostApp::socketMap.getSocketById(sockid);
    //if  (thisSocket != nullptr)
        //return true;
    //else return false;
    return true;
}

int AcpAgentApp::isSocketExist(opp_string nodeName)
{

    auto itr = atmNodeConnMap.find(nodeName);
    if (itr != atmNodeConnMap.end()) {
         return (*itr).second;
    }
    return 0;
}

AcpDatalinkMsg * AcpAgentApp::createAcmMessage(AeroCommAppType applicationId, CommandType commandCode, datalinkMsgBody * MsgBodyIn, bool request)
{
    AcpDatalinkMsg * payload = new AcpDatalinkMsg();
    long msgLength = par("atmMsgLength");

    datalinkMsgHeader * hdr = agent_create_datalinkHdr(applicationId,commandCode, request, 0, thisNodeType);
    datalinkMsgBody * body = agent_create_datalinkBody(applicationId, commandCode, MsgBodyIn);

    payload->setHdr(*hdr);
    payload->setBody(*body);

    payload->setChunkLength(B(msgLength));
    payload->addTag<CreationTimeTag>()->setCreationTime(simTime());

    if (request)
        EV_INFO << "sending request Agent Name " << thisNodeName << " application ID: " << applicationId <<  " command ID: " << commandCode << "\n";
    else
        EV_INFO << "sending response Agent Name " << thisNodeName << " application ID: " << applicationId <<  " command ID: " << commandCode << "\n";

   return payload;
}

void AcpAgentApp::handleDatalinkMsg(Packet *rcvdPkt)
{
    AcpDatalinkMsg *msgreply  = new AcpDatalinkMsg();
    if ((thisNodeType & 0xFF ) == NODE_ATC_AGENT) {

       const auto& msg = rcvdPkt->peekData<AcpDatalinkMsg>();
       datalinkMsgHeader hdr = msg->getHdr();
       datalinkMsgBody body = msg->getBody();

        int socketId = rcvdPkt->getTag<SocketInd>()->getSocketId();
        //TcpSocket *socket = check_and_cast_nullable<TcpSocket *>(TcpServerHostApp::socketMap.getSocketById(socketId));

        if (hdr.request) {

            switch (hdr.appId) {

            case  DLCM:

                switch  (hdr.commandCode) {

                    case LOGON:
                        atmNodeConnMap.insert(make_pair(body.appHeader.origNodeName.c_str(),socketId));
                        msgreply = createAcmMessage(DLCM,LOGON,&body,false);
                        sendResponse(rcvdPkt,msgreply);
                        break;

                    case LOGOUT:
                        atmNodeConnMap.erase(body.appHeader.origNodeName.c_str());
                        msgreply = createAcmMessage(DLCM,LOGOUT,&body,false);
                        sendResponse(rcvdPkt,msgreply);
                        break;

                    case UPDATE:

                        atmNodeConnMap.erase(body.appHeader.origNodeName.c_str());
                        atmNodeConnMap.insert(make_pair(body.appHeader.origNodeName.c_str(),socketId));
                        msgreply = createAcmMessage(DLCM,UPDATE,&body,false);
                        sendResponse(rcvdPkt, msgreply);
                        break;

                    default:
                        EV_INFO << " Unknown Command ID: " << hdr.commandCode <<  " for application " << hdr.appId << "\n";
                        break;
                }

                break;

            case CPDLC:
            {
                int connId = retrieveConnId(body.appHeader.destNodeName);
                if (connId != 0 )
                    forwardMsg(createAcmMessage(CPDLC,hdr.commandCode,&body,true),rcvdPkt,connId);
                else EV_INFO << "Connection ID not found " << body.appHeader.destNodeName.c_str() << "\n";
            }
            break;

            default:
                EV_INFO << " Unknown Application ID: " << hdr.appId << "\n";
               break;
        }
    }
    else { // response
            switch (hdr.appId) {

                case  DLCM:

                      switch  (hdr.commandCode) {

                      case CONTACT:
                      break;

                      default:
                          EV_INFO << " Unknown Command ID: " << hdr.commandCode <<  " for application " << hdr.appId  << "\n";
                          break;
                      }
                      break;

                case CPDLC:
                {

                      int connId = retrieveConnId(body.appHeader.destNodeName);
                      if (connId != 0 )
                          forwardMsg(createAcmMessage(CPDLC,hdr.commandCode,&body,false),rcvdPkt,connId);
                }
                break;

                default:
                      EV_INFO << " Unknown Application ID: " << hdr.appId << "\n";
                     break;
               }
    }
    }// main if
}


void AcpAgentApp::forwardMsg(AcpDatalinkMsg *payload, Packet *rcvdPkt, int socketId)
{
    const auto& payloadData = makeShared<AcpDatalinkMsg>(*payload);
    Packet *outPkt = new Packet(rcvdPkt->getName(), TCP_C_SEND);
    outPkt->addTag<SocketReq>()->setSocketId(socketId);
    outPkt->insertAtBack(payloadData);
    sendDown(outPkt);
}

void AcpAgentApp::sendResponse(Packet *rcvdPkt, AcpDatalinkMsg *payload)
{
    const auto& payloadData = makeShared<AcpDatalinkMsg>(*payload);
    Packet *outPkt = new Packet(rcvdPkt->getName(), TCP_C_SEND);
    int socketId = rcvdPkt->getTag<SocketInd>()->getSocketId();
    outPkt->addTag<SocketReq>()->setSocketId(socketId);
    outPkt->insertAtBack(payloadData);
    sendDown(outPkt);
}


void AcpAgentAppThread::established()
{
}


void AcpAgentAppThread::dataArrived(Packet *rcvdPkt, bool urgent)
{
     acpAppModule->emit(packetReceivedSignal, rcvdPkt);

     if (sock->getState() == TcpSocket::CONNECTED)
         acpAppModule->handleDatalinkMsg(rcvdPkt);

     delete rcvdPkt;
}

/*
 * Called when a timer (scheduled via scheduleAt()) expires. To be redefined.
 */

void AcpAgentAppThread::timerExpired(cMessage *timer) // Not Used Yet
{
    Packet *pkt = check_and_cast<Packet *>(timer);
    pkt->setContextPointer(nullptr);
    acpAppModule->sendDown(pkt);
}

} // namespace inet

