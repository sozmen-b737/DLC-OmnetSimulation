//
// Generated file, do not edit! Created by opp_msgtool 6.0 from inet/applications/acpapp/AcpDatalinkMsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "AcpDatalinkMsg_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace inet {

Register_Enum(inet::AeroCommAppType, (inet::AeroCommAppType::DLCM, inet::AeroCommAppType::CPDLC));

Register_Enum(inet::CommandType, (inet::CommandType::REGISTER, inet::CommandType::DEREGISTER, inet::CommandType::LOGON, inet::CommandType::LOGOUT, inet::CommandType::CONTACT, inet::CommandType::UPDATE, inet::CommandType::GRDFWD, inet::CommandType::CPDLC_CONNECT, inet::CommandType::CPDLC_MSG, inet::CommandType::CPDLC_CONTACT, inet::CommandType::CPDLC_DISCONNECT));

Register_Enum(inet::NodeType, (inet::NodeType::SERVER, inet::NodeType::AGENT, inet::NodeType::CLIENT));

Register_Enum(inet::NodeRole, (inet::NodeRole::ATM, inet::NodeRole::ATC, inet::NodeRole::FIXED, inet::NodeRole::MOBILE));

datalinkMsgHeader::datalinkMsgHeader()
{
}

void __doPacking(omnetpp::cCommBuffer *b, const datalinkMsgHeader& a)
{
    doParsimPacking(b,a.version);
    doParsimPacking(b,a.request);
    doParsimPacking(b,a.reTranmission);
    doParsimPacking(b,a.priority);
    doParsimPacking(b,a.atmNodeType);
    doParsimPacking(b,a.appId);
    doParsimPacking(b,a.commandCode);
    doParsimPacking(b,a.requestIdentifier);
}

void __doUnpacking(omnetpp::cCommBuffer *b, datalinkMsgHeader& a)
{
    doParsimUnpacking(b,a.version);
    doParsimUnpacking(b,a.request);
    doParsimUnpacking(b,a.reTranmission);
    doParsimUnpacking(b,a.priority);
    doParsimUnpacking(b,a.atmNodeType);
    doParsimUnpacking(b,a.appId);
    doParsimUnpacking(b,a.commandCode);
    doParsimUnpacking(b,a.requestIdentifier);
}

class datalinkMsgHeaderDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_version,
        FIELD_request,
        FIELD_reTranmission,
        FIELD_priority,
        FIELD_atmNodeType,
        FIELD_appId,
        FIELD_commandCode,
        FIELD_requestIdentifier,
    };
  public:
    datalinkMsgHeaderDescriptor();
    virtual ~datalinkMsgHeaderDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(datalinkMsgHeaderDescriptor)

datalinkMsgHeaderDescriptor::datalinkMsgHeaderDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::datalinkMsgHeader)), "")
{
    propertyNames = nullptr;
}

datalinkMsgHeaderDescriptor::~datalinkMsgHeaderDescriptor()
{
    delete[] propertyNames;
}

bool datalinkMsgHeaderDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<datalinkMsgHeader *>(obj)!=nullptr;
}

const char **datalinkMsgHeaderDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *datalinkMsgHeaderDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int datalinkMsgHeaderDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 8+base->getFieldCount() : 8;
}

unsigned int datalinkMsgHeaderDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_version
        FD_ISEDITABLE,    // FIELD_request
        FD_ISEDITABLE,    // FIELD_reTranmission
        FD_ISEDITABLE,    // FIELD_priority
        FD_ISEDITABLE,    // FIELD_atmNodeType
        FD_ISEDITABLE,    // FIELD_appId
        FD_ISEDITABLE,    // FIELD_commandCode
        FD_ISEDITABLE,    // FIELD_requestIdentifier
    };
    return (field >= 0 && field < 8) ? fieldTypeFlags[field] : 0;
}

const char *datalinkMsgHeaderDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "version",
        "request",
        "reTranmission",
        "priority",
        "atmNodeType",
        "appId",
        "commandCode",
        "requestIdentifier",
    };
    return (field >= 0 && field < 8) ? fieldNames[field] : nullptr;
}

int datalinkMsgHeaderDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "version") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "request") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "reTranmission") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "priority") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "atmNodeType") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "appId") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "commandCode") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "requestIdentifier") == 0) return baseIndex + 7;
    return base ? base->findField(fieldName) : -1;
}

const char *datalinkMsgHeaderDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "unsigned short",    // FIELD_version
        "bool",    // FIELD_request
        "bool",    // FIELD_reTranmission
        "unsigned short",    // FIELD_priority
        "int",    // FIELD_atmNodeType
        "inet::AeroCommAppType",    // FIELD_appId
        "inet::CommandType",    // FIELD_commandCode
        "unsigned short",    // FIELD_requestIdentifier
    };
    return (field >= 0 && field < 8) ? fieldTypeStrings[field] : nullptr;
}

const char **datalinkMsgHeaderDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_appId: {
            static const char *names[] = { "enum",  nullptr };
            return names;
        }
        case FIELD_commandCode: {
            static const char *names[] = { "enum",  nullptr };
            return names;
        }
        default: return nullptr;
    }
}

const char *datalinkMsgHeaderDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_appId:
            if (!strcmp(propertyName, "enum")) return "inet::AeroCommAppType";
            return nullptr;
        case FIELD_commandCode:
            if (!strcmp(propertyName, "enum")) return "inet::CommandType";
            return nullptr;
        default: return nullptr;
    }
}

int datalinkMsgHeaderDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    datalinkMsgHeader *pp = omnetpp::fromAnyPtr<datalinkMsgHeader>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void datalinkMsgHeaderDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    datalinkMsgHeader *pp = omnetpp::fromAnyPtr<datalinkMsgHeader>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'datalinkMsgHeader'", field);
    }
}

const char *datalinkMsgHeaderDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    datalinkMsgHeader *pp = omnetpp::fromAnyPtr<datalinkMsgHeader>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string datalinkMsgHeaderDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    datalinkMsgHeader *pp = omnetpp::fromAnyPtr<datalinkMsgHeader>(object); (void)pp;
    switch (field) {
        case FIELD_version: return ulong2string(pp->version);
        case FIELD_request: return bool2string(pp->request);
        case FIELD_reTranmission: return bool2string(pp->reTranmission);
        case FIELD_priority: return ulong2string(pp->priority);
        case FIELD_atmNodeType: return long2string(pp->atmNodeType);
        case FIELD_appId: return enum2string(pp->appId, "inet::AeroCommAppType");
        case FIELD_commandCode: return enum2string(pp->commandCode, "inet::CommandType");
        case FIELD_requestIdentifier: return ulong2string(pp->requestIdentifier);
        default: return "";
    }
}

void datalinkMsgHeaderDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    datalinkMsgHeader *pp = omnetpp::fromAnyPtr<datalinkMsgHeader>(object); (void)pp;
    switch (field) {
        case FIELD_version: pp->version = string2ulong(value); break;
        case FIELD_request: pp->request = string2bool(value); break;
        case FIELD_reTranmission: pp->reTranmission = string2bool(value); break;
        case FIELD_priority: pp->priority = string2ulong(value); break;
        case FIELD_atmNodeType: pp->atmNodeType = string2long(value); break;
        case FIELD_appId: pp->appId = (inet::AeroCommAppType)string2enum(value, "inet::AeroCommAppType"); break;
        case FIELD_commandCode: pp->commandCode = (inet::CommandType)string2enum(value, "inet::CommandType"); break;
        case FIELD_requestIdentifier: pp->requestIdentifier = string2ulong(value); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'datalinkMsgHeader'", field);
    }
}

omnetpp::cValue datalinkMsgHeaderDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    datalinkMsgHeader *pp = omnetpp::fromAnyPtr<datalinkMsgHeader>(object); (void)pp;
    switch (field) {
        case FIELD_version: return (omnetpp::intval_t)(pp->version);
        case FIELD_request: return pp->request;
        case FIELD_reTranmission: return pp->reTranmission;
        case FIELD_priority: return (omnetpp::intval_t)(pp->priority);
        case FIELD_atmNodeType: return pp->atmNodeType;
        case FIELD_appId: return static_cast<int>(pp->appId);
        case FIELD_commandCode: return static_cast<int>(pp->commandCode);
        case FIELD_requestIdentifier: return (omnetpp::intval_t)(pp->requestIdentifier);
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'datalinkMsgHeader' as cValue -- field index out of range?", field);
    }
}

void datalinkMsgHeaderDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    datalinkMsgHeader *pp = omnetpp::fromAnyPtr<datalinkMsgHeader>(object); (void)pp;
    switch (field) {
        case FIELD_version: pp->version = omnetpp::checked_int_cast<unsigned short>(value.intValue()); break;
        case FIELD_request: pp->request = value.boolValue(); break;
        case FIELD_reTranmission: pp->reTranmission = value.boolValue(); break;
        case FIELD_priority: pp->priority = omnetpp::checked_int_cast<unsigned short>(value.intValue()); break;
        case FIELD_atmNodeType: pp->atmNodeType = omnetpp::checked_int_cast<int>(value.intValue()); break;
        case FIELD_appId: pp->appId = static_cast<inet::AeroCommAppType>(value.intValue()); break;
        case FIELD_commandCode: pp->commandCode = static_cast<inet::CommandType>(value.intValue()); break;
        case FIELD_requestIdentifier: pp->requestIdentifier = omnetpp::checked_int_cast<unsigned short>(value.intValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'datalinkMsgHeader'", field);
    }
}

const char *datalinkMsgHeaderDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr datalinkMsgHeaderDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    datalinkMsgHeader *pp = omnetpp::fromAnyPtr<datalinkMsgHeader>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void datalinkMsgHeaderDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    datalinkMsgHeader *pp = omnetpp::fromAnyPtr<datalinkMsgHeader>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'datalinkMsgHeader'", field);
    }
}

avpHeader::avpHeader()
{
}

void __doPacking(omnetpp::cCommBuffer *b, const avpHeader& a)
{
    doParsimPacking(b,a.origNodeType);
    doParsimPacking(b,a.origNodeName);
    doParsimPacking(b,a.origAddress);
    doParsimPacking(b,a.destNodeType);
    doParsimPacking(b,a.destNodeName);
    doParsimPacking(b,a.destAddress);
    doParsimPacking(b,a.nextDataAuthorityName);
    doParsimPacking(b,a.nextAgentName);
    doParsimPacking(b,a.nextAgentAddress);
    doParsimPacking(b,a.contextId);
}

void __doUnpacking(omnetpp::cCommBuffer *b, avpHeader& a)
{
    doParsimUnpacking(b,a.origNodeType);
    doParsimUnpacking(b,a.origNodeName);
    doParsimUnpacking(b,a.origAddress);
    doParsimUnpacking(b,a.destNodeType);
    doParsimUnpacking(b,a.destNodeName);
    doParsimUnpacking(b,a.destAddress);
    doParsimUnpacking(b,a.nextDataAuthorityName);
    doParsimUnpacking(b,a.nextAgentName);
    doParsimUnpacking(b,a.nextAgentAddress);
    doParsimUnpacking(b,a.contextId);
}

class avpHeaderDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_origNodeType,
        FIELD_origNodeName,
        FIELD_origAddress,
        FIELD_destNodeType,
        FIELD_destNodeName,
        FIELD_destAddress,
        FIELD_nextDataAuthorityName,
        FIELD_nextAgentName,
        FIELD_nextAgentAddress,
        FIELD_contextId,
    };
  public:
    avpHeaderDescriptor();
    virtual ~avpHeaderDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(avpHeaderDescriptor)

avpHeaderDescriptor::avpHeaderDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::avpHeader)), "")
{
    propertyNames = nullptr;
}

avpHeaderDescriptor::~avpHeaderDescriptor()
{
    delete[] propertyNames;
}

bool avpHeaderDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<avpHeader *>(obj)!=nullptr;
}

const char **avpHeaderDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *avpHeaderDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int avpHeaderDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 10+base->getFieldCount() : 10;
}

unsigned int avpHeaderDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_origNodeType
        FD_ISEDITABLE,    // FIELD_origNodeName
        FD_ISEDITABLE,    // FIELD_origAddress
        FD_ISEDITABLE,    // FIELD_destNodeType
        FD_ISEDITABLE,    // FIELD_destNodeName
        FD_ISEDITABLE,    // FIELD_destAddress
        FD_ISEDITABLE,    // FIELD_nextDataAuthorityName
        FD_ISEDITABLE,    // FIELD_nextAgentName
        FD_ISEDITABLE,    // FIELD_nextAgentAddress
        FD_ISEDITABLE,    // FIELD_contextId
    };
    return (field >= 0 && field < 10) ? fieldTypeFlags[field] : 0;
}

const char *avpHeaderDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "origNodeType",
        "origNodeName",
        "origAddress",
        "destNodeType",
        "destNodeName",
        "destAddress",
        "nextDataAuthorityName",
        "nextAgentName",
        "nextAgentAddress",
        "contextId",
    };
    return (field >= 0 && field < 10) ? fieldNames[field] : nullptr;
}

int avpHeaderDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "origNodeType") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "origNodeName") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "origAddress") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "destNodeType") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "destNodeName") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "destAddress") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "nextDataAuthorityName") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "nextAgentName") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "nextAgentAddress") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "contextId") == 0) return baseIndex + 9;
    return base ? base->findField(fieldName) : -1;
}

const char *avpHeaderDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_origNodeType
        "string",    // FIELD_origNodeName
        "string",    // FIELD_origAddress
        "int",    // FIELD_destNodeType
        "string",    // FIELD_destNodeName
        "string",    // FIELD_destAddress
        "string",    // FIELD_nextDataAuthorityName
        "string",    // FIELD_nextAgentName
        "string",    // FIELD_nextAgentAddress
        "int",    // FIELD_contextId
    };
    return (field >= 0 && field < 10) ? fieldTypeStrings[field] : nullptr;
}

const char **avpHeaderDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *avpHeaderDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int avpHeaderDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    avpHeader *pp = omnetpp::fromAnyPtr<avpHeader>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void avpHeaderDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    avpHeader *pp = omnetpp::fromAnyPtr<avpHeader>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'avpHeader'", field);
    }
}

const char *avpHeaderDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    avpHeader *pp = omnetpp::fromAnyPtr<avpHeader>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string avpHeaderDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    avpHeader *pp = omnetpp::fromAnyPtr<avpHeader>(object); (void)pp;
    switch (field) {
        case FIELD_origNodeType: return long2string(pp->origNodeType);
        case FIELD_origNodeName: return oppstring2string(pp->origNodeName);
        case FIELD_origAddress: return oppstring2string(pp->origAddress);
        case FIELD_destNodeType: return long2string(pp->destNodeType);
        case FIELD_destNodeName: return oppstring2string(pp->destNodeName);
        case FIELD_destAddress: return oppstring2string(pp->destAddress);
        case FIELD_nextDataAuthorityName: return oppstring2string(pp->nextDataAuthorityName);
        case FIELD_nextAgentName: return oppstring2string(pp->nextAgentName);
        case FIELD_nextAgentAddress: return oppstring2string(pp->nextAgentAddress);
        case FIELD_contextId: return long2string(pp->contextId);
        default: return "";
    }
}

void avpHeaderDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    avpHeader *pp = omnetpp::fromAnyPtr<avpHeader>(object); (void)pp;
    switch (field) {
        case FIELD_origNodeType: pp->origNodeType = string2long(value); break;
        case FIELD_origNodeName: pp->origNodeName = (value); break;
        case FIELD_origAddress: pp->origAddress = (value); break;
        case FIELD_destNodeType: pp->destNodeType = string2long(value); break;
        case FIELD_destNodeName: pp->destNodeName = (value); break;
        case FIELD_destAddress: pp->destAddress = (value); break;
        case FIELD_nextDataAuthorityName: pp->nextDataAuthorityName = (value); break;
        case FIELD_nextAgentName: pp->nextAgentName = (value); break;
        case FIELD_nextAgentAddress: pp->nextAgentAddress = (value); break;
        case FIELD_contextId: pp->contextId = string2long(value); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'avpHeader'", field);
    }
}

omnetpp::cValue avpHeaderDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    avpHeader *pp = omnetpp::fromAnyPtr<avpHeader>(object); (void)pp;
    switch (field) {
        case FIELD_origNodeType: return pp->origNodeType;
        case FIELD_origNodeName: return pp->origNodeName;
        case FIELD_origAddress: return pp->origAddress;
        case FIELD_destNodeType: return pp->destNodeType;
        case FIELD_destNodeName: return pp->destNodeName;
        case FIELD_destAddress: return pp->destAddress;
        case FIELD_nextDataAuthorityName: return pp->nextDataAuthorityName;
        case FIELD_nextAgentName: return pp->nextAgentName;
        case FIELD_nextAgentAddress: return pp->nextAgentAddress;
        case FIELD_contextId: return pp->contextId;
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'avpHeader' as cValue -- field index out of range?", field);
    }
}

void avpHeaderDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    avpHeader *pp = omnetpp::fromAnyPtr<avpHeader>(object); (void)pp;
    switch (field) {
        case FIELD_origNodeType: pp->origNodeType = omnetpp::checked_int_cast<int>(value.intValue()); break;
        case FIELD_origNodeName: pp->origNodeName = value.stringValue(); break;
        case FIELD_origAddress: pp->origAddress = value.stringValue(); break;
        case FIELD_destNodeType: pp->destNodeType = omnetpp::checked_int_cast<int>(value.intValue()); break;
        case FIELD_destNodeName: pp->destNodeName = value.stringValue(); break;
        case FIELD_destAddress: pp->destAddress = value.stringValue(); break;
        case FIELD_nextDataAuthorityName: pp->nextDataAuthorityName = value.stringValue(); break;
        case FIELD_nextAgentName: pp->nextAgentName = value.stringValue(); break;
        case FIELD_nextAgentAddress: pp->nextAgentAddress = value.stringValue(); break;
        case FIELD_contextId: pp->contextId = omnetpp::checked_int_cast<int>(value.intValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'avpHeader'", field);
    }
}

const char *avpHeaderDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr avpHeaderDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    avpHeader *pp = omnetpp::fromAnyPtr<avpHeader>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void avpHeaderDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    avpHeader *pp = omnetpp::fromAnyPtr<avpHeader>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'avpHeader'", field);
    }
}

datalinkMsgBody::datalinkMsgBody()
{
}

void __doPacking(omnetpp::cCommBuffer *b, const datalinkMsgBody& a)
{
    doParsimPacking(b,a.appHeader);
    doParsimPacking(b,a.payload);
}

void __doUnpacking(omnetpp::cCommBuffer *b, datalinkMsgBody& a)
{
    doParsimUnpacking(b,a.appHeader);
    doParsimUnpacking(b,a.payload);
}

class datalinkMsgBodyDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_appHeader,
        FIELD_payload,
    };
  public:
    datalinkMsgBodyDescriptor();
    virtual ~datalinkMsgBodyDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(datalinkMsgBodyDescriptor)

datalinkMsgBodyDescriptor::datalinkMsgBodyDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::datalinkMsgBody)), "")
{
    propertyNames = nullptr;
}

datalinkMsgBodyDescriptor::~datalinkMsgBodyDescriptor()
{
    delete[] propertyNames;
}

bool datalinkMsgBodyDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<datalinkMsgBody *>(obj)!=nullptr;
}

const char **datalinkMsgBodyDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *datalinkMsgBodyDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int datalinkMsgBodyDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int datalinkMsgBodyDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,    // FIELD_appHeader
        FD_ISEDITABLE,    // FIELD_payload
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *datalinkMsgBodyDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "appHeader",
        "payload",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int datalinkMsgBodyDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "appHeader") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "payload") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *datalinkMsgBodyDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "inet::avpHeader",    // FIELD_appHeader
        "string",    // FIELD_payload
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **datalinkMsgBodyDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *datalinkMsgBodyDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int datalinkMsgBodyDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    datalinkMsgBody *pp = omnetpp::fromAnyPtr<datalinkMsgBody>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void datalinkMsgBodyDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    datalinkMsgBody *pp = omnetpp::fromAnyPtr<datalinkMsgBody>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'datalinkMsgBody'", field);
    }
}

const char *datalinkMsgBodyDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    datalinkMsgBody *pp = omnetpp::fromAnyPtr<datalinkMsgBody>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string datalinkMsgBodyDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    datalinkMsgBody *pp = omnetpp::fromAnyPtr<datalinkMsgBody>(object); (void)pp;
    switch (field) {
        case FIELD_appHeader: return "";
        case FIELD_payload: return oppstring2string(pp->payload);
        default: return "";
    }
}

void datalinkMsgBodyDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    datalinkMsgBody *pp = omnetpp::fromAnyPtr<datalinkMsgBody>(object); (void)pp;
    switch (field) {
        case FIELD_payload: pp->payload = (value); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'datalinkMsgBody'", field);
    }
}

omnetpp::cValue datalinkMsgBodyDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    datalinkMsgBody *pp = omnetpp::fromAnyPtr<datalinkMsgBody>(object); (void)pp;
    switch (field) {
        case FIELD_appHeader: return omnetpp::toAnyPtr(&pp->appHeader); break;
        case FIELD_payload: return pp->payload;
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'datalinkMsgBody' as cValue -- field index out of range?", field);
    }
}

void datalinkMsgBodyDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    datalinkMsgBody *pp = omnetpp::fromAnyPtr<datalinkMsgBody>(object); (void)pp;
    switch (field) {
        case FIELD_payload: pp->payload = value.stringValue(); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'datalinkMsgBody'", field);
    }
}

const char *datalinkMsgBodyDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_appHeader: return omnetpp::opp_typename(typeid(avpHeader));
        default: return nullptr;
    };
}

omnetpp::any_ptr datalinkMsgBodyDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    datalinkMsgBody *pp = omnetpp::fromAnyPtr<datalinkMsgBody>(object); (void)pp;
    switch (field) {
        case FIELD_appHeader: return omnetpp::toAnyPtr(&pp->appHeader); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void datalinkMsgBodyDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    datalinkMsgBody *pp = omnetpp::fromAnyPtr<datalinkMsgBody>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'datalinkMsgBody'", field);
    }
}

Register_Class(AcpDatalinkMsg)

AcpDatalinkMsg::AcpDatalinkMsg() : ::inet::FieldsChunk()
{
}

AcpDatalinkMsg::AcpDatalinkMsg(const AcpDatalinkMsg& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

AcpDatalinkMsg::~AcpDatalinkMsg()
{
}

AcpDatalinkMsg& AcpDatalinkMsg::operator=(const AcpDatalinkMsg& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void AcpDatalinkMsg::copy(const AcpDatalinkMsg& other)
{
    this->hdr = other.hdr;
    this->body = other.body;
}

void AcpDatalinkMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->hdr);
    doParsimPacking(b,this->body);
}

void AcpDatalinkMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->hdr);
    doParsimUnpacking(b,this->body);
}

const datalinkMsgHeader& AcpDatalinkMsg::getHdr() const
{
    return this->hdr;
}

void AcpDatalinkMsg::setHdr(const datalinkMsgHeader& hdr)
{
    handleChange();
    this->hdr = hdr;
}

const datalinkMsgBody& AcpDatalinkMsg::getBody() const
{
    return this->body;
}

void AcpDatalinkMsg::setBody(const datalinkMsgBody& body)
{
    handleChange();
    this->body = body;
}

class AcpDatalinkMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_hdr,
        FIELD_body,
    };
  public:
    AcpDatalinkMsgDescriptor();
    virtual ~AcpDatalinkMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(AcpDatalinkMsgDescriptor)

AcpDatalinkMsgDescriptor::AcpDatalinkMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::AcpDatalinkMsg)), "inet::FieldsChunk")
{
    propertyNames = nullptr;
}

AcpDatalinkMsgDescriptor::~AcpDatalinkMsgDescriptor()
{
    delete[] propertyNames;
}

bool AcpDatalinkMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<AcpDatalinkMsg *>(obj)!=nullptr;
}

const char **AcpDatalinkMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *AcpDatalinkMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int AcpDatalinkMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int AcpDatalinkMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,    // FIELD_hdr
        FD_ISCOMPOUND,    // FIELD_body
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *AcpDatalinkMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "hdr",
        "body",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int AcpDatalinkMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "hdr") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "body") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *AcpDatalinkMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "inet::datalinkMsgHeader",    // FIELD_hdr
        "inet::datalinkMsgBody",    // FIELD_body
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **AcpDatalinkMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *AcpDatalinkMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int AcpDatalinkMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    AcpDatalinkMsg *pp = omnetpp::fromAnyPtr<AcpDatalinkMsg>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void AcpDatalinkMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    AcpDatalinkMsg *pp = omnetpp::fromAnyPtr<AcpDatalinkMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'AcpDatalinkMsg'", field);
    }
}

const char *AcpDatalinkMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    AcpDatalinkMsg *pp = omnetpp::fromAnyPtr<AcpDatalinkMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string AcpDatalinkMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    AcpDatalinkMsg *pp = omnetpp::fromAnyPtr<AcpDatalinkMsg>(object); (void)pp;
    switch (field) {
        case FIELD_hdr: return "";
        case FIELD_body: return "";
        default: return "";
    }
}

void AcpDatalinkMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    AcpDatalinkMsg *pp = omnetpp::fromAnyPtr<AcpDatalinkMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'AcpDatalinkMsg'", field);
    }
}

omnetpp::cValue AcpDatalinkMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    AcpDatalinkMsg *pp = omnetpp::fromAnyPtr<AcpDatalinkMsg>(object); (void)pp;
    switch (field) {
        case FIELD_hdr: return omnetpp::toAnyPtr(&pp->getHdr()); break;
        case FIELD_body: return omnetpp::toAnyPtr(&pp->getBody()); break;
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'AcpDatalinkMsg' as cValue -- field index out of range?", field);
    }
}

void AcpDatalinkMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    AcpDatalinkMsg *pp = omnetpp::fromAnyPtr<AcpDatalinkMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'AcpDatalinkMsg'", field);
    }
}

const char *AcpDatalinkMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_hdr: return omnetpp::opp_typename(typeid(datalinkMsgHeader));
        case FIELD_body: return omnetpp::opp_typename(typeid(datalinkMsgBody));
        default: return nullptr;
    };
}

omnetpp::any_ptr AcpDatalinkMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    AcpDatalinkMsg *pp = omnetpp::fromAnyPtr<AcpDatalinkMsg>(object); (void)pp;
    switch (field) {
        case FIELD_hdr: return omnetpp::toAnyPtr(&pp->getHdr()); break;
        case FIELD_body: return omnetpp::toAnyPtr(&pp->getBody()); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void AcpDatalinkMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    AcpDatalinkMsg *pp = omnetpp::fromAnyPtr<AcpDatalinkMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'AcpDatalinkMsg'", field);
    }
}

}  // namespace inet

namespace omnetpp {

}  // namespace omnetpp

