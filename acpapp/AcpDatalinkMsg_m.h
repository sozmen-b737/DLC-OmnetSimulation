//
// Generated file, do not edit! Created by opp_msgtool 6.0 from inet/applications/acpapp/AcpDatalinkMsg.msg.
//

#ifndef __INET_ACPDATALINKMSG_M_H
#define __INET_ACPDATALINKMSG_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// opp_msgtool version check
#define MSGC_VERSION 0x0600
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgtool: 'make clean' should help.
#endif

// dll export symbol
#ifndef INET_API
#  if defined(INET_EXPORT)
#    define INET_API  OPP_DLLEXPORT
#  elif defined(INET_IMPORT)
#    define INET_API  OPP_DLLIMPORT
#  else
#    define INET_API
#  endif
#endif


namespace inet {

struct datalinkMsgHeader;
struct avpHeader;
struct datalinkMsgBody;
class AcpDatalinkMsg;

}  // namespace inet

#include "inet/common/INETDefs_m.h" // import inet.common.INETDefs

#include "inet/common/Units_m.h" // import inet.common.Units

#include "inet/common/packet/chunk/Chunk_m.h" // import inet.common.packet.chunk.Chunk


namespace inet {

/**
 * Enum generated from <tt>inet/applications/acpapp/AcpDatalinkMsg.msg:34</tt> by opp_msgtool.
 * <pre>
 * enum AeroCommAppType
 * {
 *     DLCM = 0;
 *     CPDLC = 1;
 * }
 * </pre>
 */
enum AeroCommAppType {
    DLCM = 0,
    CPDLC = 1
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const AeroCommAppType& e) { b->pack(static_cast<int>(e)); }
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, AeroCommAppType& e) { int n; b->unpack(n); e = static_cast<AeroCommAppType>(n); }

/**
 * Enum generated from <tt>inet/applications/acpapp/AcpDatalinkMsg.msg:40</tt> by opp_msgtool.
 * <pre>
 * enum CommandType
 * {
 *     REGISTER = 1;
 *     DEREGISTER = 2;
 *     LOGON = 3;
 *     LOGOUT = 4;
 *     CONTACT = 5;
 *     UPDATE = 6;
 *     GRDFWD = 7;
 *     CPDLC_CONNECT = 8;
 *     CPDLC_MSG = 9;
 *     CPDLC_CONTACT = 10;
 *     CPDLC_DISCONNECT = 11;
 * }
 * </pre>
 */
enum CommandType {
    REGISTER = 1,
    DEREGISTER = 2,
    LOGON = 3,
    LOGOUT = 4,
    CONTACT = 5,
    UPDATE = 6,
    GRDFWD = 7,
    CPDLC_CONNECT = 8,
    CPDLC_MSG = 9,
    CPDLC_CONTACT = 10,
    CPDLC_DISCONNECT = 11
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const CommandType& e) { b->pack(static_cast<int>(e)); }
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, CommandType& e) { int n; b->unpack(n); e = static_cast<CommandType>(n); }

/**
 * Enum generated from <tt>inet/applications/acpapp/AcpDatalinkMsg.msg:55</tt> by opp_msgtool.
 * <pre>
 * enum NodeType
 * {
 *     SERVER = 0;
 *     AGENT = 1;
 *     CLIENT = 2;
 * }
 * </pre>
 */
enum NodeType {
    SERVER = 0,
    AGENT = 1,
    CLIENT = 2
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const NodeType& e) { b->pack(static_cast<int>(e)); }
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, NodeType& e) { int n; b->unpack(n); e = static_cast<NodeType>(n); }

/**
 * Enum generated from <tt>inet/applications/acpapp/AcpDatalinkMsg.msg:62</tt> by opp_msgtool.
 * <pre>
 * enum NodeRole
 * {
 *     ATM = 0;
 *     ATC = 1;
 *     FIXED = 2;
 *     MOBILE = 3;
 * }
 * </pre>
 */
enum NodeRole {
    ATM = 0,
    ATC = 1,
    FIXED = 2,
    MOBILE = 3
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const NodeRole& e) { b->pack(static_cast<int>(e)); }
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, NodeRole& e) { int n; b->unpack(n); e = static_cast<NodeRole>(n); }

/**
 * Struct generated from inet/applications/acpapp/AcpDatalinkMsg.msg:69 by opp_msgtool.
 */
struct INET_API datalinkMsgHeader
{
    datalinkMsgHeader();
    unsigned short version = 0;
    bool request = false;
    bool reTranmission = false;
    unsigned short priority = 0;
    int atmNodeType = 0;
    AeroCommAppType appId = static_cast<inet::AeroCommAppType>(-1);
    CommandType commandCode = static_cast<inet::CommandType>(-1);
    unsigned short requestIdentifier = 0;
};

// helpers for local use
void INET_API __doPacking(omnetpp::cCommBuffer *b, const datalinkMsgHeader& a);
void INET_API __doUnpacking(omnetpp::cCommBuffer *b, datalinkMsgHeader& a);

inline void doParsimPacking(omnetpp::cCommBuffer *b, const datalinkMsgHeader& obj) { __doPacking(b, obj); }
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, datalinkMsgHeader& obj) { __doUnpacking(b, obj); }

/**
 * Struct generated from inet/applications/acpapp/AcpDatalinkMsg.msg:82 by opp_msgtool.
 */
struct INET_API avpHeader
{
    avpHeader();
    int origNodeType = 0;
    ::omnetpp::opp_string origNodeName;
    ::omnetpp::opp_string origAddress;
    int destNodeType = 0;
    ::omnetpp::opp_string destNodeName;
    ::omnetpp::opp_string destAddress;
    ::omnetpp::opp_string nextDataAuthorityName;
    ::omnetpp::opp_string nextAgentName;
    ::omnetpp::opp_string nextAgentAddress;
    int contextId = 0;
};

// helpers for local use
void INET_API __doPacking(omnetpp::cCommBuffer *b, const avpHeader& a);
void INET_API __doUnpacking(omnetpp::cCommBuffer *b, avpHeader& a);

inline void doParsimPacking(omnetpp::cCommBuffer *b, const avpHeader& obj) { __doPacking(b, obj); }
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, avpHeader& obj) { __doUnpacking(b, obj); }

/**
 * Struct generated from inet/applications/acpapp/AcpDatalinkMsg.msg:101 by opp_msgtool.
 */
struct INET_API datalinkMsgBody
{
    datalinkMsgBody();
    avpHeader appHeader;
    ::omnetpp::opp_string payload;
};

// helpers for local use
void INET_API __doPacking(omnetpp::cCommBuffer *b, const datalinkMsgBody& a);
void INET_API __doUnpacking(omnetpp::cCommBuffer *b, datalinkMsgBody& a);

inline void doParsimPacking(omnetpp::cCommBuffer *b, const datalinkMsgBody& obj) { __doPacking(b, obj); }
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, datalinkMsgBody& obj) { __doUnpacking(b, obj); }

/**
 * Class generated from <tt>inet/applications/acpapp/AcpDatalinkMsg.msg:111</tt> by opp_msgtool.
 * <pre>
 * //
 * class AcpDatalinkMsg extends FieldsChunk
 * {
 *     datalinkMsgHeader hdr;
 *     datalinkMsgBody body;
 * 
 * }
 * </pre>
 */
class INET_API AcpDatalinkMsg : public ::inet::FieldsChunk
{
  protected:
    datalinkMsgHeader hdr;
    datalinkMsgBody body;

  private:
    void copy(const AcpDatalinkMsg& other);

  protected:
    bool operator==(const AcpDatalinkMsg&) = delete;

  public:
    AcpDatalinkMsg();
    AcpDatalinkMsg(const AcpDatalinkMsg& other);
    virtual ~AcpDatalinkMsg();
    AcpDatalinkMsg& operator=(const AcpDatalinkMsg& other);
    virtual AcpDatalinkMsg *dup() const override {return new AcpDatalinkMsg(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    virtual const datalinkMsgHeader& getHdr() const;
    virtual datalinkMsgHeader& getHdrForUpdate() { handleChange();return const_cast<datalinkMsgHeader&>(const_cast<AcpDatalinkMsg*>(this)->getHdr());}
    virtual void setHdr(const datalinkMsgHeader& hdr);

    virtual const datalinkMsgBody& getBody() const;
    virtual datalinkMsgBody& getBodyForUpdate() { handleChange();return const_cast<datalinkMsgBody&>(const_cast<AcpDatalinkMsg*>(this)->getBody());}
    virtual void setBody(const datalinkMsgBody& body);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const AcpDatalinkMsg& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, AcpDatalinkMsg& obj) {obj.parsimUnpack(b);}


}  // namespace inet


namespace omnetpp {

inline any_ptr toAnyPtr(const inet::datalinkMsgHeader *p) {return any_ptr(p);}
template<> inline inet::datalinkMsgHeader *fromAnyPtr(any_ptr ptr) { return ptr.get<inet::datalinkMsgHeader>(); }
inline any_ptr toAnyPtr(const inet::avpHeader *p) {return any_ptr(p);}
template<> inline inet::avpHeader *fromAnyPtr(any_ptr ptr) { return ptr.get<inet::avpHeader>(); }
inline any_ptr toAnyPtr(const inet::datalinkMsgBody *p) {return any_ptr(p);}
template<> inline inet::datalinkMsgBody *fromAnyPtr(any_ptr ptr) { return ptr.get<inet::datalinkMsgBody>(); }
template<> inline inet::AcpDatalinkMsg *fromAnyPtr(any_ptr ptr) { return check_and_cast<inet::AcpDatalinkMsg*>(ptr.get<cObject>()); }

}  // namespace omnetpp

#endif // ifndef __INET_ACPDATALINKMSG_M_H
