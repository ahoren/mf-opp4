/***************************************************************************
 * file:        BasicMacLayer.cc
 *
 * author:      Daniel Willkomm
 *
 * copyright:   (C) 2004 Telecommunication Networks Group (TKN) at
 *              Technische Universitaet Berlin, Germany.
 *
 *              This program is free software; you can redistribute it
 *              and/or modify it under the terms of the GNU General Public
 *              License as published by the Free Software Foundation; either
 *              version 2 of the License, or (at your option) any later
 *              version.
 *              For further information see file COPYING
 *              in the top level directory
 ***************************************************************************
 * part of:     framework implementation developed by tkn
 * description: basic MAC layer class
 *              subclass to create your own MAC layer
 **************************************************************************/


#include "BasicMacLayer.h"
#include "MacControlInfo.h"
#include "SimpleAddress.h"
#include <cassert>
#include "CoreDebug.h"

/**
 * First we have to initialize the module from which we derived ours,
 * in this case BasicLayer.
 *
 **/
void BasicMacLayer::initialize(int stage)
{
    BasicLayer::initialize(stage);
    if(stage==0) {
        headerLength= par("headerLength");

	// The nic id is used as MAC address
        myMacAddr = getParentModule()->getId();

        hasPar("coreDebug") ? coreDebug = par("coreDebug").boolValue() :
	    coreDebug = false;
    }
}

/**
 * Decapsulates the network packet from the received MacPkt
 **/
cMessage* BasicMacLayer::decapsMsg(MacPkt* msg)
{
    cPacket *m = msg->decapsulate();
    m->setControlInfo(new MacControlInfo(msg->getSrcAddr()));
    // delete the macPkt
    delete msg;
    coreEV << " message decapsulated " << endl;
    return m;
}


/**
 * Encapsulates the received NetwPkt into a MacPkt and set all needed
 * header fields.
 **/
MacPkt* BasicMacLayer::encapsMsg(cPacket *msg)
{
    MacPkt *pkt = new MacPkt(msg->getName(), msg->getKind());
    pkt->setBitLength(headerLength);

    // copy dest address from the Control Info attached to the network
    // mesage by the network layer
    MacControlInfo* cInfo = static_cast<MacControlInfo*>(msg->removeControlInfo());

    coreEV <<"CInfo removed, mac addr="<< cInfo->getNextHopMac()<<endl;
    pkt->setDestAddr(cInfo->getNextHopMac());

    //delete the control info
    delete cInfo;

    //set the src address to own mac address (nic module getId())
    pkt->setSrcAddr(myMacAddr);

    //encapsulate the network packet
    pkt->encapsulate(msg);
    coreEV <<"pkt encapsulated\n";

    return pkt;
}

/**
 * This basic implementation just forwards all message that are
 * broadcast (destAddr = L2BROADCAST) or destined for this node
 * (destAddr = nic module getId()) to the network layer
 *
 * @sa sendUp
 **/

void BasicMacLayer::handleLowerMsg(cMessage *msg)
{
    MacPkt *mac = static_cast<MacPkt *>(msg);
    int dest = mac->getDestAddr();
    int src = mac->getSrcAddr();

    //only foward to upper layer if message is for me or broadcast
    if((dest == myMacAddr) || (dest == L2BROADCAST)) {
	coreEV <<"msg from "<< src
	       <<" broadcast or for me -> forward packet to upper layer\n";
	sendUp(decapsMsg(mac));
    }
    else{
	coreEV << "message with mac addr " << src
	       << " not for me (dest=" << dest
	       << ") -> delete (my MAC="<<myMacAddr<<")\n";
	delete mac;
    }
}

void BasicMacLayer::handleUpperMsg(cMessage *mac)
{
        assert(dynamic_cast<cPacket*>(mac));
    sendDown(encapsMsg(static_cast<cPacket*>(mac)));
}


