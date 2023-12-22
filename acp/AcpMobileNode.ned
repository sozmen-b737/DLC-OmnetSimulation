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



package inet.node.acp;
import inet.node.inet.WirelessHost;
import inet.applications.acpapp.AcpClientApp;

//
// TODO auto-generated type
//
module AcpMobileNode extends WirelessHost
{
       
        //numWlanInterfaces = default(1);
        @display("i=device/wifilaptop");
   
   		int nodeType = default(0);
   		int nodeRole = default(0);
              	
   		bool dlcmSupport	= default(true);
   		bool cpdlcSupport = default(false);
   
   		int dlcmListenPort = default(1001);    
   		int cpdlcListenPort = default(1003); // port number to listen on
         
   		int dlcmConnectPort = default(1000); // port number to connect to
   		int cpdlcConnectPort = default(1002); // port number to connect to
   
  		xml adjNodeList = nullptr; // for now
     
}
