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

package atmaca.simulations;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.Eth100M;
import inet.node.ethernet.Eth10M;
import inet.node.ethernet.EthernetLink;
import inet.node.ethernet.EthernetSwitch;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import inet.node.inet.WirelessHost;
import inet.node.acp.AcpMobileNode;
import inet.node.acp.AcpFixedNode;
import inet.node.internetcloud.InternetCloud;
import inet.node.wireless.AccessPoint;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.canvas.integrated.IntegratedCanvasVisualizer;
import inet.visualizer.canvas.physicallayer.MediumCanvasVisualizer;
import inet.physicallayer.wireless.common.contract.packetlevel.IRadioMedium;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.Eth100M;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import inet.node.internetcloud.InternetCloud;


network AirportCloud
{
    parameters:

        int numFixedClients;
        int numMobileClients;

        @display("i=aviation/airport4;bgb=17537.893,11531.258");
    gates:
        inout pppg[];

    submodules:

        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=3151.63,9195.344");
        }

        controller[numFixedClients]: AcpFixedNode {
            @display("p=4004.4238,1112.34");
            nodeType = 2; // CLIENT
            nodeRole = 2; // FIXED
        }

        aircraft[numMobileClients]: AcpMobileNode {
            @display("p=12421.13,1112.34");
            nodeType = 2; // CLIENT
            nodeRole = 3; // FIXED
        }

        airportRouter: Router {
            @display("p=7675.1455,6525.7275");
            hasTcp = true;
        }

        atmEthSwitch: EthernetSwitch {
            @display("p=4486.438,4115.6577");
        }

        atmAccessPoint: AccessPoint {
            @display("p=12421.13,4115.6577");
        }


    connections:

        for i=0..numFixedClients-1 {
            controller[i].ethg++ <--> Eth100M <--> atmEthSwitch.ethg++;
        }

        for i=0..numMobileClients-1 {
            aircraft[i].ethg++ <--> Eth100M <--> atmAccessPoint.ethg++;
        }


        for i=0..sizeof(pppg)-1 {
            pppg[i] <--> airportRouter.pppg++;
        }

        airportRouter.ethg++ <--> Eth100M <--> atmEthSwitch.ethg++;
        airportRouter.ethg++ <--> Eth100M <--> atmAccessPoint.ethg++;
}


network AtcAreaCloud
{
    parameters:

        int numOfAirports;
        int numOfSectors;
        @display("i=device/lan-ring;bgb=517,255");

    gates:
        inout pppg[];

    submodules:

        airport[numOfAirports]: AirportCloud {
            @display("p=216,38");
        }

        sector[numOfSectors]: AcpFixedNode {
            @display("p=421,144");
        }

        atcAgent: AcpFixedNode {
            @display("p=80,183");
            nodeType = 1; // AGENT
            nodeRole = 1; // ATC
        }

        areaRouter: Router {
            @display("p=216,183");
            hasTcp = true;
        }


    connections:

        for i=0..numOfAirports-1 {
            areaRouter.pppg++ <--> Eth100M <--> airport[i].pppg++;
        }

        for i=0..numOfSectors-1 {
            areaRouter.ethg++ <--> Eth100M <--> sector[i].pppg++;
        }

        atcAgent.ethg++ <--> Eth100M <--> areaRouter.ethg++;

        for i=0..sizeof(pppg)-1 {
            pppg[i] <--> areaRouter.pppg++;
        }
}

network AtcFirCloud
{
    parameters:
        int numOfAtcArea;
        @display("i=device/lan-ring;bgb=235.71,255.15001");
    gates:
        inout pppg[];
    submodules:

        FirRouter: Router {
            @display("i=abstract/router2;p=115.83,157.14");
        }

        AtcArea[numOfAtcArea]: AtcAreaCloud {
            @display("p=109.35,46.98");
        }
    connections:
        for i=0..sizeof(pppg)-1 {
            pppg[i] <--> FirRouter.pppg++;
        }

        for i=0..numOfAtcArea-1 {
            FirRouter.pppg++ <--> Eth100M <--> AtcArea[i].pppg++;
        }
}


network SimpleContinentCloud
{
    parameters:
        int numOfAtcFir;
        @display("i=device/lan-ring;bgb=235.71,255.15001");
    gates:
        inout pppg[];
    submodules:


        backboneRouter: Router {
            @display("i=abstract/router2;p=115.83,157.14");
        }

        AtcFir[numOfAtcFir]: AtcFirCloud {
            @display("p=109.35,46.98");
        }
    connections:
        for i=0..sizeof(pppg)-1 {
            pppg[i] <--> backboneRouter.pppg++;
        }

        for i=0..numOfAtcFir-1 {
            backboneRouter.pppg++ <--> Eth100M <--> AtcFir[i].pppg++;
        }
}


network AeroCommNetwork
{
    @display("bgi=maps/world");
    submodules:

        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=43.95,130.75125;is=s");
        }

        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=43.95,196.67625");
        }

        internet: InternetCloud {
            @display("p=387,185;is=vl");
        }
        africa: SimpleContinentCloud {
            @display("p=465,288");
        }
        asia: SimpleContinentCloud {
            @display("p=662,121");
        }
        australia: SimpleContinentCloud {
            @display("p=759,299;");
        }
        europe: SimpleContinentCloud {
            @display("p=446,77");
        }
        northAmerica: SimpleContinentCloud {
            @display("p=170,120");
        }
        southAmerica: SimpleContinentCloud {
            @display("p=260,270");
        }
    connections:
        africa.pppg++ <--> {  datarate = 100Mbps; delay = 2ms; } <--> internet.pppg++;
        asia.pppg++ <--> {  datarate = 100Mbps; delay = 2ms; } <--> internet.pppg++;
        australia.pppg++ <--> {  datarate = 100Mbps; delay = 2ms; } <--> internet.pppg++;
        europe.pppg++ <--> {  datarate = 100Mbps; delay = 2ms; } <--> internet.pppg++;
        northAmerica.pppg++ <--> {  datarate = 100Mbps; delay = 2ms; } <--> internet.pppg++;
        southAmerica.pppg++ <--> {  datarate = 100Mbps; delay = 2ms; } <--> internet.pppg++;
}
