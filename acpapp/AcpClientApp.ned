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

package inet.applications.acpapp;
import inet.applications.contract.IApp;
//
// TODO auto-generated module
//
simple AcpClientApp like IApp
{
    parameters:
        @display("i=block/app");
        
        string interfaceTableModule; // path to the InterfaceTable module
        string routingTableModule; // path to the Ipv4RoutingTable modul
       	volatile string interface @mutable = default(""); // which NIC should be configured (i.e. wlan0, eth1 etc.)
        
       	volatile int atmMsgLength @unit(B) = default(100B); // length atm datalink message length
        
        string nodeName = default("");
        int nodeType = parent.nodeType;
        int nodeRole = parent.nodeRole;
        
        bool dlcmSupport  = parent.dlcmSupport;
   		bool cpdlcSupport = parent.cpdlcSupport;
   		
   		bool DhcpServerInUse = default(true);
        
        volatile int localPort @mutable  = parent.dlcmListenPort;  // default(-1); // port number to listen on
        volatile int connectPort @mutable  =  parent.dlcmConnectPort;  // default(1000); // port number to connect to
        
        volatile string localAddress @mutable = default(""); // may be left empty ("")
        volatile string connectAddress @mutable = default("");  // Agent Address
        
        volatile string atcAgentName @mutable =  default("");
        volatile string atcAgentConnAddr @mutable =  default("");
        
        volatile string currentDataAuthorityName @mutable =  default("");
        volatile string nextDataAuthorityName @mutable =  default("");
        volatile string ndaAtcAgentName @mutable =  default("");
        volatile string ndaAtcAgentAddr @mutable =  default("");
        
        string adjAtcName = default("");
        string adjAtcConnAgentName= default("");
        string adjAtcConnAgentAddr= default("");
        
        @lifecycleSupport;
        double stopOperationExtraTime @unit(s) = default(-1s);    // extra time after lifecycle stop operation finished
        double stopOperationTimeout @unit(s) = default(2s);    // timeout value for lifecycle stop operation
        volatile double idleInterval @unit(s) = default(exponential(300s));
        volatile double reconnectInterval @unit(s) = default(30s);  // if connection breaks, waits this much before trying to reconnect
        int timeToLive = default(-1); // if not -1, set the TTL (IPv4) or Hop Limit (IPv6) field of sent packets to this value
        int dscp = default(-1); // if not -1, set the DSCP (IPv4/IPv6) field of sent packets to this value
        int tos = default(-1); 
        
        
        double startTime @unit(s) = default(1s); // time first session begins
        double stopTime @unit(s) = default(-1s);  // time of finishing sending, negative values mean forever
       
        double datalinkConnOpenDelay @unit(s) = default(3s);	// not used for now 
        double datalinkConnCloseDelay @unit(s) = default(5ms);	
        
        double serviceDuration @unit(s) = 	default(2ms);
       	//double serviceStartDelay @unit(s) = default(1s);// not used for now
       	int maxNumberofMessages = 	default(2);
       	
       	double datalinkAddrCheckInterval @unit(s) = default(30s);
       	
        @signal[packetSent](type=inet::Packet);
        @signal[packetReceived](type=inet::Packet);
        @signal[connect](type=long);  // 1 for open, -1 for close
        @statistic[packetReceived](title="packets received"; source=packetReceived; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[packetSent](title="packets sent"; source=packetSent; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[endToEndDelay](title="end-to-end delay"; source="dataAge(packetReceived)"; unit=s; record=histogram,weightedHistogram,vector; interpolationmode=none);
    
        
      gates:
        input socketIn @labels(TcpCommand/up);
        output socketOut @labels(TcpCommand/down);
        
        
}
