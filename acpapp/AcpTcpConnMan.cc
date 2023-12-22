//
// Copyright (C) 2004 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#include "inet/applications/tcpapp/TcpAppBase.h"
#include "inet/applications/acpapp/AcpTcpConnMan.h"

#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"

namespace inet {

simsignal_t AcpTcpConnMan::connectSignal = registerSignal("connect");

void AcpTcpConnMan::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        numSessions = numBroken = packetsSent = packetsRcvd = bytesSent = bytesRcvd = 0;

        WATCH(numSessions);
        WATCH(numBroken);
        WATCH(packetsSent);
        WATCH(packetsRcvd);
        WATCH(bytesSent);
        WATCH(bytesRcvd);
    }
    else if (stage == INITSTAGE_APPLICATION_LAYER) {
        // parameters
    }
}

void AcpTcpConnMan::handleMessageWhenUp(cMessage *msg)
{
    if (msg->isSelfMessage())
        handleTimer(msg);
    else {
            auto socket = check_and_cast_nullable<TcpSocket *>(socketMap.findSocketFor(msg));
            //if (socket)
                socket->processMessage(msg);
           // else
               //throw cRuntimeError("Unaccepted message: %s(%s)", msg->getName(), msg->getClassName());
        }
        if (operationalState == State::STOPPING_OPERATION && socketMap.size() == 0)
            startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}


void AcpTcpConnMan::open()
{

    socket = new TcpSocket();
    const char *localAddress = par("localAddress");
    int localPort = par("localPort");
    socket->bind(*localAddress ? L3AddressResolver().resolve(localAddress) : L3Address(), localPort);

    socket->setCallback(this);
    socket->setOutputGate(gate("socketOut"));
    socketMap.addSocket(socket);
}

void AcpTcpConnMan::connect()
{
    // we need a new connId if this is not the first connection
    socket->renewSocket();

    const char *localAddress = par("localAddress");
    int localPort = par("localPort");
    socket->bind(*localAddress ? L3AddressResolver().resolve(localAddress) : L3Address(), localPort);

    int timeToLive = par("timeToLive");
    if (timeToLive != -1)
        socket->setTimeToLive(timeToLive);

    int dscp = par("dscp");
    if (dscp != -1)
        socket->setDscp(dscp);

    int tos = par("tos");
    if (tos != -1)
        socket->setTos(tos);

    // connect
    const char *connectAddress = par("connectAddress");
    int connectPort = par("connectPort");

    L3Address destination;
    L3AddressResolver().tryResolve(connectAddress, destination);
    if (destination.isUnspecified()) {
        EV_ERROR << "Connecting to " << connectAddress << " port=" << connectPort << ": cannot resolve destination address\n";
    }
    else {
        EV_INFO << "Connecting to " << connectAddress << "(" << destination << ") port=" << connectPort << endl;

        socket->connect(destination, connectPort);

        numSessions++;
        emit(connectSignal, 1L);
    }
}

void AcpTcpConnMan::close()
{
    EV_INFO << "issuing CLOSE command\n";
    socket->close();
    emit(connectSignal, -1L);
}

void AcpTcpConnMan::sendPacket(Packet *msg)
{
    int numBytes = msg->getByteLength();
    emit(packetSentSignal, msg);
    socket->send(msg);

    packetsSent++;
    bytesSent += numBytes;
}

void AcpTcpConnMan::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();
    getDisplayString().setTagArg("t", 0, TcpSocket::stateName(socket->getState()));
}

void AcpTcpConnMan::socketEstablished(TcpSocket *)
{
    // *redefine* to perform or schedule first sending
    EV_INFO << "connected\n";
}

void AcpTcpConnMan::socketDataArrived(TcpSocket *, Packet *msg, bool)
{
    // *redefine* to perform or schedule next sending
    packetsRcvd++;
    bytesRcvd += msg->getByteLength();
    emit(packetReceivedSignal, msg);
    delete msg;
}

void AcpTcpConnMan::socketPeerClosed(TcpSocket *socket_)
{
    ASSERT(socket_ == socket);
    // close the connection (if not already closed)
    if (socket->getState() == TcpSocket::PEER_CLOSED) {
        EV_INFO << "remote TCP closed, closing here as well\n";
        close();
    }
}

void AcpTcpConnMan::socketClosed(TcpSocket *)
{
    // *redefine* to start another session etc.
    EV_INFO << "connection closed\n";
}

void AcpTcpConnMan::socketFailure(TcpSocket *, int code)
{
    // subclasses may override this function, and add code try to reconnect after a delay.
    EV_WARN << "connection broken\n";
    numBroken++;
}

void AcpTcpConnMan::finish()
{
    std::string modulePath = getFullPath();

    EV_INFO << modulePath << ": opened " << numSessions << " sessions\n";
    EV_INFO << modulePath << ": sent " << bytesSent << " bytes in " << packetsSent << " packets\n";
    EV_INFO << modulePath << ": received " << bytesRcvd << " bytes in " << packetsRcvd << " packets\n";
}

} // namespace inet

