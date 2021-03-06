//***************************************************************************
// * file:        Host.ned
// *
// * author:      Marc Loebbers
// *
// * copyright:   (C) 2004 Telecommunication Networks Group (TKN) at
// *              Technische Universitaet Berlin, Germany.
// *
// *              This program is free software; you can redistribute it 
// *              and/or modify it under the terms of the GNU General Public 
// *              License as published by the Free Software Foundation; either
// *              version 2 of the License, or (at your option) any later 
// *              version.
// *              For further information see file COPYING 
// *              in the top level directory
// ***************************************************************************
// * part of:     framework implementation developed by tkn
// * description: a host in the 802.11 test network
// *              
// ***************************************************************************
// * changelog:   $Revision: 266 $
// *              last modified:   $Date: 2006-04-28 16:14:49 +0200 (Fr, 28 Apr 2006) $
// *              by:              $Author: koepke $
// **************************************************************************/


package org.mf2.networks.csma;

import org.mf2.contrib.applLayer.BurstApplLayer;
import org.mf2.contrib.netwLayer.SimpleArp;
import org.mf2.contrib.netwLayer.SimpleNetwLayer;
import org.mf2.contrib.nic.NicCsma;
import org.mf2.core.basicModules.BasicMobility;
import org.mf2.core.blackboard.Blackboard;


module Host

{
    parameters:
        @display("bgb=250,250,white,,;bgp=10,10");
    gates:
        input radioIn; // gate for sendDirect

    submodules:
        blackboard: Blackboard {
            parameters:
                @display("p=200,75");

        }
        mobility: BasicMobility {
            parameters:
                @display("p=200,139");

        }
        arp: SimpleArp {
            parameters:
                @display("p=200,205");

        }
        appl: BurstApplLayer {
            parameters:
                @display("p=90,50");

        }
        net: SimpleNetwLayer {
            parameters:
                @display("p=90,114");

        }
        nic: NicCsma {
            parameters:
                @display("p=90,176");

        }
    connections:
        nic.uppergateOut --> net.lowergateIn;
        nic.uppergateIn <-- net.lowergateOut;
        nic.upperControlOut --> net.lowerControlIn;

        net.uppergateOut --> appl.lowergateIn;
        net.uppergateIn <-- appl.lowergateOut;
        net.upperControlOut --> appl.lowerControlIn;

        radioIn --> nic.radioIn;

}
