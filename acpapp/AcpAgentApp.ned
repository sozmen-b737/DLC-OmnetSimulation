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
import inet.applications.tcpapp.TcpServerHostApp;


//
// TODO auto-generated module
//
simple AcpAgentApp extends TcpServerHostApp
{
    parameters:
        @class(AcpAgentApp);
        
        volatile int atmMsgLength @unit(B) = default(100B); // length atm datalink message length
         
        string nodeName = default("");
        int nodeType = parent.nodeType;
        int nodeRole = parent.nodeRole;
                 	
   		bool dlcmSupport  = parent.dlcmSupport;
   		bool cpdlcSupport = parent.cpdlcSupport;
        
        int dlcmListenPort = default(-1);    
        int cpdlcListenPort = default(-1); // port number to listen on
          
        serverThreadModuleType = default("inet.applications.acpapp.AcpAgentAppThread"); // module type of "thread" module to launch on incoming connections
        @signal[packetSent](type=inet::Packet);
        @signal[packetReceived](type=inet::Packet);
        @statistic[packetReceived](title="packets received"; source=packetReceived; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[packetSent](title="packets sent"; source=packetSent; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[endToEndDelay](title="end-to-end delay"; source="dataAge(packetReceived)"; unit=s; record=histogram,weightedHistogram,vector; interpolationmode=none);
}

// Thread for ~AcpAgentApp
simple AcpAgentAppThread
{
}
