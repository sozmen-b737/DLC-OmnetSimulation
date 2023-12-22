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
import inet.examples.mobileipv6.ethernetline;
import inet.linklayer.ieee8021as.GptpEndstation;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.Eth100M;
import inet.node.ethernet.Eth10M;
import inet.node.ethernet.Eth1G;
import inet.node.ethernet.EthernetLink;
import inet.node.ethernet.EthernetSwitch;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import inet.node.inet.WirelessHost;
import inet.node.acp.AcpFixedNode;
import inet.node.acp.AcpMobileNode;
import inet.node.internetcloud.InternetCloud;
import inet.node.wireless.AccessPoint;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.tutorials.configurator.Eth100MLong;
import inet.visualizer.canvas.integrated.IntegratedCanvasVisualizer;
import inet.visualizer.common.IntegratedVisualizer;
import inet.physicallayer.wireless.common.contract.packetlevel.IRadioMedium;
import inet.physicallayer.wireless.unitdisk.UnitDiskRadioMedium;

network AirportNetwork
{
    parameters:

        @display("bgb=152724.58,82273.18");
    submodules:

        configurator: Ipv4NetworkConfigurator {
            @display("p=5470.875,21226.994");
        }

        visualizer: IntegratedCanvasVisualizer {
            //visualizer: IntegratedVisualizer {
            @display("p=5470.875,28776.803");
        }

        //radioMedium: <default("UnitDiskRadioMedium")> like IRadioMedium {
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=3939.03,36217.19");
        }

        EuropeBackboneRouter: Router {
            @display("p=52848.652,5033.205;i=abstract/router2");
        }

        istAreaRouter: Router {
            @display("p=33481.754,12583.013");
            hasTcp = true;
        }

        ankAreaRouter: Router {
            @display("p=74294.484,14552.527");
            hasTcp = true;
        }

        istAreaAgent: AcpFixedNode {
            @display("p=47049.523,22211.752");
            nodeType = 1; // AGET
            nodeRole = 1; // ATC
        }

        ankAreaAgent: AcpFixedNode {
            @display("p=59085.45,21226.994");
            nodeType = 1; // AGET
            nodeRole = 1; // ATC
        }

        atmEsbRouter: Router {
            @display("p=101320.6,28776.803");
            hasTcp = true;
        }

        atmSawRouter: Router {
            @display("p=19257.48,28776.803");
            hasTcp = true;
        }

        atmSawSwitch: EthernetSwitch {
            @display("p=19366.896,42453.99");
        }

        atmEsbSwitch: EthernetSwitch {
            @display("p=98256.914,48471.953");
        }

        atmSawAP: AccessPoint {
            @display("p=47377.777,39609.137");

        }

        atmEsbAP: AccessPoint {
            @display("p=74294.484,39609.137");
        }

        sawDelivery: AcpFixedNode {
            @display("p=7549.8076,63243.316");
            nodeType = 2; // CLIENT
            nodeRole = 2; // FIXED
        }

        sawGround: AcpFixedNode {
            @display("p=20679.908,63243.316");
            nodeType = 2; // CLIENT
            nodeRole = 2; // FIXED
        }

        sawTower: AcpFixedNode {
            @display("p=33481.754,63243.316");
            nodeType = 2; // CLIENT
            nodeRole = 2; // FIXED
        }

        esbTower: AcpFixedNode {
            @display("p=85126.81,63243.316");
            nodeType = 2; // CLIENT
            nodeRole = 2; // FIXED
        }

        esbGround: AcpFixedNode {
            @display("p=98256.914,61492.633");
            nodeType = 2; // CLIENT
            nodeRole = 2; // FIXED
        }

        esbDelivery: AcpFixedNode {
            @display("p=110730.51,62477.39");
            nodeType = 2; // CLIENT
            nodeRole = 2; // FIXED
        }


        thyFlight: AcpMobileNode {
            @display("p=46967.6,39618.93");
            nodeType = 2; // CLIENT
            nodeRole = 3; // FIXED
        }

    connections:

        istAreaRouter.ethg++ <--> Eth1G <--> EuropeBackboneRouter.ethg++;

        istAreaAgent.ethg++ <--> Eth100MLong <--> istAreaRouter.ethg++;
        atmSawRouter.ethg++ <--> Eth100MLong <--> istAreaRouter.ethg++;

        atmSawSwitch.ethg++ <--> Eth100MLong <--> atmSawRouter.ethg++;

        sawDelivery.ethg++ <--> Eth10M <--> atmSawSwitch.ethg++;
        sawGround.ethg++ <--> Eth10M <--> atmSawSwitch.ethg++;
        sawTower.ethg++ <--> Eth10M <--> atmSawSwitch.ethg++;

        atmSawAP.ethg++ <--> Eth100M <--> atmSawRouter.ethg++;


        EuropeBackboneRouter.ethg++ <--> Eth1G <--> ankAreaRouter.ethg++;

        ankAreaAgent.ethg++ <--> Eth100MLong <--> ankAreaRouter.ethg++;
        ankAreaRouter.ethg++ <--> Eth100MLong <--> atmEsbRouter.ethg++;

        atmEsbSwitch.ethg++ <--> Eth100MLong <--> atmEsbRouter.ethg++;

        esbTower.ethg++ <--> Eth10M <--> atmEsbSwitch.ethg++;
        atmEsbSwitch.ethg++ <--> Eth10M <--> esbDelivery.ethg++;
        atmEsbSwitch.ethg++ <--> Eth10M <--> esbGround.ethg++;

        atmEsbAP.ethg++ <--> Eth100M <--> atmEsbRouter.ethg++;
}
