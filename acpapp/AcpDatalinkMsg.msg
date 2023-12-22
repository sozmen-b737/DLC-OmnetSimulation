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

import inet.common.INETDefs;
import inet.common.Units;
import inet.common.packet.chunk.Chunk;

namespace inet;

//
// Message class for generic request-reply style applications.
// The client sends a ~GenericAppMsg which contains the number of
// bytes the client wants the server to send back as reply.
//
// This way intelligence (behaviour specific to the modelled application,
// e.g. HTTP, SMB, database protocol) needs only to be present in
// the client, and the server model can be kept simple and dumb.
//
// @see ~TcpGenericServerApp, TCPGenericCliAppBase (C++ only)

enum AeroCommAppType
{
    DLCM = 0;
    CPDLC = 1;
}

enum CommandType
{
    REGISTER = 1;
    DEREGISTER = 2;
    LOGON = 3;
    LOGOUT = 4;
    CONTACT = 5;
    UPDATE = 6;
    GRDFWD = 7;
    CPDLC_CONNECT = 8;
    CPDLC_MSG = 9;
    CPDLC_CONTACT = 10;
    CPDLC_DISCONNECT = 11;
}

enum NodeType 
{
    SERVER = 0;
    AGENT = 1;
    CLIENT = 2;
}

enum NodeRole 
{
    ATM = 0;
    ATC = 1;
    FIXED = 2;
    MOBILE = 3;
}

struct datalinkMsgHeader {
    
    unsigned short version;
    bool request;
    bool reTranmission;
    unsigned short priority;
    int atmNodeType;
    
    AeroCommAppType appId;
    CommandType commandCode;
    unsigned short requestIdentifier; 
    
}
struct avpHeader {
    
    int origNodeType;
    string origNodeName;
   	string origAddress; 
   	 
   	int destNodeType;
    string destNodeName; 
    string destAddress;
    
    string nextDataAuthorityName;
     
    string nextAgentName; 
    string nextAgentAddress;
    
    int contextId;
    
}

struct datalinkMsgBody {
    
   avpHeader  appHeader;
   string payload;    
}



//
class AcpDatalinkMsg extends FieldsChunk
{
    
    datalinkMsgHeader hdr;
    datalinkMsgBody body;

}
   
