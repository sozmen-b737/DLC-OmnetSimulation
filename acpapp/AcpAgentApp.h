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

#ifndef __INET4_4_ACPAGENTAPP_H_
#define __INET4_4_ACPAGENTAPP_H_

#include "inet/applications/tcpapp/TcpServerHostApp.h"
#include "inet/common/INETMath.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"
#include "AcpDatalinkMsg_m.h"

#include <omnetpp.h>

using namespace omnetpp;
using namespace std;

namespace inet {

#define NODE_INVALID (0)
#define NODE_MOBILE_CLIENT  (1 << 0)
#define NODE_FIXED_CLIENT   (1 << 1)
#define NODE_ATC_AGENT      (1 << 2)
#define NODE_ATM_SERVER     (1 << 3)

/**
 * An example request-reply based client application.
 */

 struct atmNodeConnData
 {
     const char * nodeName;
     int connId;
 };

 typedef atmNodeConnData atmNodeConnDataType;

/**
 * TODO - Generated class
 */

class AcpAgentApp : public TcpServerHostApp
{

    protected:

     cModule *host = nullptr; // containing host module (@networkNode)
     const char * thisNodeName;
     int thisNodeType;
     L3Address thisNodeAddr;

     vector <atmNodeConnDataType *> atmNodeConnList;
     map<opp_string,int> atmNodeConnMap;

     datalinkMsgHeader acpMsgHeader;

     int dlcmPortConnect;

     int numRequestSend = 0; // requests to send in this session
     int numRequestReceived = 0;

     int numResponseSend = 0; // requests to send in this session
     int numResponseReceived = 0;


  protected:
    virtual void sendDown(Packet *packet);

    virtual void initialize(int stage) override;

    // const Ptr<const AcpDatalinkMsg>& incommingMsg
    datalinkMsgHeader * agent_create_datalinkHdr(AeroCommAppType applicationId,CommandType commandCode, bool request, int reqIdent, int thisNodeType );
    datalinkMsgBody * agent_create_datalinkBody(AeroCommAppType applicationId, CommandType commandCode, datalinkMsgBody * MsgBodyIn);
    virtual AcpDatalinkMsg *createAcmMessage(AeroCommAppType applicationId, CommandType commandCode, datalinkMsgBody * MsgBodyIn , bool request);
    virtual void  handleDatalinkMsg(Packet *rcvdPkt);

    virtual void finish() override;
    virtual void refreshDisplay() const override;

    int retrieveConnId(opp_string nodeName);
    bool deleteSocketfromMap(opp_string nodeName);
    int isSocketExist(opp_string nodeName);

    void forwardMsg(AcpDatalinkMsg *payload, Packet *rcvdPkt, int socketId);
    void sendResponse(Packet *rcvdPkt, AcpDatalinkMsg *payload);

  public:
    AcpAgentApp();
    ~AcpAgentApp();

    friend class AcpAgentAppThread;

};


class INET_API AcpAgentAppThread : public TcpServerThreadBase
{
  protected:
    AcpAgentApp *acpAppModule = nullptr;

  public:
    /**
     * Called when connection is established.
     */
    virtual void established() override;

    /*
     * Called when a data packet arrives. To be redefined.
     */
   virtual void dataArrived(Packet *msg, bool urgent) override;

    /*
     * Called when a timer (scheduled via scheduleAt()) expires. To be redefined.
     */
   virtual void timerExpired(cMessage *timer) override;

   virtual void init(TcpServerHostApp *hostmodule, TcpSocket *socket) override { TcpServerThreadBase::init(hostmodule, socket); acpAppModule = check_and_cast<AcpAgentApp *>(hostmod); }
};

} //namespace

#endif
