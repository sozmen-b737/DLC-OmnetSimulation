/*
 * AcpClientApp.h
 *
 *  Created on: May 28, 2023
 *      Author: sozmen
 */

#ifndef INET_APPLICATIONS_TCPAPP_ACPCLIENTAPP_H_
#define INET_APPLICATIONS_TCPAPP_ACPCLIENTAPP_H_

#include "inet/applications/tcpapp/TcpAppBase.h"

#include "inet/common/lifecycle/ILifecycle.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "AcpDatalinkMsg_m.h"
#include "inet/common/Simsignals.h"
#ifdef INET_WITH_IPv4
#include "inet/networklayer/ipv4/Ipv4InterfaceData.h"
#endif // INET_WITH_IPv4
#ifdef INET_WITH_IPv6
#include "inet/networklayer/ipv6/Ipv6InterfaceData.h"
#endif // INET_WITH_IPv6
#include "inet/networklayer/common/NetworkInterface.h"
#include "inet/networklayer/contract/IInterfaceTable.h"

using namespace std;

namespace inet {

#define FORWARD  0
#define BACKWARD 1

#define NODE_INVALID (0)
#define NODE_MOBILE_CLIENT  (1 << 0)
#define NODE_FIXED_CLIENT   (1 << 1)
#define NODE_ATC_AGENT      (1 << 2)
#define NODE_ATM_SERVER     (1 << 3)

/**
 * An example request-reply based client application.
 */

class INET_API AcpClientApp : public TcpAppBase, public cListener
{
  protected:

    cModule *host = nullptr; // containing host module (@networkNode)
    cModule *airport = nullptr;
    cModule *area = nullptr;
    cModule *fir = nullptr;
    cModule *continent = nullptr;
    cModule *sector = nullptr;

    struct atcAgent
    {
        const char * nodeName;
        const char *connectAddress;
    };


    struct atcClient
    {
        const char * nodeName;
        atcAgent connectedAgent;
    };

    cMessage *timeoutMsg = nullptr;

    const char * thisNodeName;
    int thisNodeType;
    Ipv4Address thisNodeAddr;


    vector <int > destAirport;
    vector <int > deptAirport;
    vector <string > continentList = { "africa", "asia", "australia", "europe", "northAmerica", "southAmerica"};

    typedef vector <int > routeType;

    vector <routeType > routeList;

    vector <string > mobileNodeList;
    vector <atcClient> adjAtcUnitList;

    datalinkMsgHeader acpMsgHeader;

    atcClient CurrentDataAuthority;

    const char * ndaAtcAgentName;
    const char * ndaAtcAgentAddr;

    int dlcmPortConnect;

    int numRequestSend = 0; // requests to send in this session
    int numResponseReceived = 0;
    int numResponseSend = 0; // requests to send in this session
    int numRequestReceived = 0;

    int numCpdlcRequestSend = 0;
    int numCpdlcResponseReceived = 0;
    int numCpdlcRequestReceived = 0;
    int numCpdlcResponseSend = 0;

    int numHandoverRequest = 0;
    double handoverDelay = 0;
    bool AuthorityTransferInProgress = false;

    simtime_t handoverStarted;
    simtime_t handoverCompleted;

    int numL2Handover = 0;
    int numBeaconLost = 0;
    double L2handoverDelay;
    simtime_t L2handoverStarted;
    simtime_t L2handoverCompleted;
    double beaconLostDuration;
    simtime_t beaconLostTime;
    simtime_t beaconGainTime;

    bool ssidChanged = false;
    string currentSSID;
    string  prevSSID;

    simtime_t startTime;
    simtime_t stopTime;

    cPar *serviceDurationPar = nullptr;
    simtime_t endService;


    bool datalinkConnected = false;
    bool atmConnected = false;
    bool atmOnline = false;
    bool sendUpdate = false;
    int intraAgentHandover = 0;
    int interAgentHandover = 0;

    MacAddress macAddress; // client's MAC address
    // cModule *host = nullptr; // containing host module (@networkNode)
    NetworkInterface *ie = nullptr; // interface to configure

    void sendAcmMessage(AeroCommAppType applicationId, CommandType commandCode, datalinkMsgBody * MsgBodyIn,bool request);
    void initiateDatalinkApplication(AeroCommAppType applicationId, CommandType commandCode);
    void terminateDatalinkApplication(AeroCommAppType applicationId, CommandType commandCode);
    void updateDatalinkConnection(AeroCommAppType applicationId, CommandType commandCode);
    datalinkMsgBody * create_datalinkBody(AeroCommAppType applicationId, CommandType commandCode, int destNodeType,const char * destNodeName, datalinkMsgBody * MsgBodyIn);
    void handleDatalinkMsg(const Ptr<const AcpDatalinkMsg>& msg);
    virtual void scheduleAtOrDeleteTimer(simtime_t d, short int msgKind);
    virtual void rescheduleAtOrDeleteTimer(simtime_t d, short int msgKind);

    //void setNextAdjAtc ();
    // void setPrevAdjAtc ();
    // void setNextAdjGrdAtc ();
    // void setPrevAdjGrdAtc ();
    // void setNextAdjRdrAtc ();
    // void setPrevAdjRdrAtc ();

    routeType create_destairport();
    atcClient * retrieveNextDataAuthority (routeType destAirport);

    const char * retrieveMobileNode(const char * nodeName);
    bool addMobileNode(const char *nodeName);
    bool removeMobileNode(const char * nodeName);

    // atcClient * retrieveAdjAtc(int nth);
    // bool addAdjAtc(atcClient * adjAtc);
    // bool removeAdjAtc(const char * nodeName);

    void setCDA();
    void setNDA();

    void setDestandDeptAirport ();
    void createRoute(routeType deptAirport, routeType destAirport);

    void updateCDA(datalinkMsgBody * MsgBodyIn);
    void updateNDA(datalinkMsgBody * MsgBodyIn);
    const char *retrieveCDA();
    const char * retrieveAgentName();
    const char * retrieveAgentConn();
    void establishAgentConn();
    void terminateAgentConn();
    void updateAgentConn();
    bool startCpdlcExchange();
    bool stopCpdlcEchange();

    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleTimer(cMessage *msg) override;

    virtual void socketEstablished(TcpSocket *socket) override;
    virtual void socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent) override;
    virtual void socketClosed(TcpSocket *socket) override;
    virtual void socketFailure(TcpSocket *socket, int code) override;

    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;
    virtual void close() override;

    bool isAppAddrAssignedtoModule();
    NetworkInterface * chooseInterface();
    bool isAppAddrAssignedtoInterface();
    bool isAppAddrChanged();
    bool tcpSocketConnected();

    virtual void receiveSignal(cComponent *source, simsignal_t signal, cObject *object, cObject *details) override;

  public:
    AcpClientApp() {}
    virtual ~AcpClientApp();
};

} // namespace inet



#endif /* INET_APPLICATIONS_TCPAPP_ACPCLIENTAPP_H_ */
