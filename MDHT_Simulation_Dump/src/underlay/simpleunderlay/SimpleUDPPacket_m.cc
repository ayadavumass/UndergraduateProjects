//
// Generated file, do not edit! Created by opp_msgc 4.1 from underlay/simpleunderlay/SimpleUDPPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "SimpleUDPPacket_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(SimpleUDPPacket);

SimpleUDPPacket::SimpleUDPPacket(const char *name, int kind) : cPacket(name,kind)
{
    this->sourcePort_var = -1;
    this->destinationPort_var = -1;
}

SimpleUDPPacket::SimpleUDPPacket(const SimpleUDPPacket& other) : cPacket()
{
    setName(other.getName());
    operator=(other);
}

SimpleUDPPacket::~SimpleUDPPacket()
{
}

SimpleUDPPacket& SimpleUDPPacket::operator=(const SimpleUDPPacket& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    this->sourcePort_var = other.sourcePort_var;
    this->destinationPort_var = other.destinationPort_var;
    return *this;
}

void SimpleUDPPacket::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->sourcePort_var);
    doPacking(b,this->destinationPort_var);
}

void SimpleUDPPacket::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->sourcePort_var);
    doUnpacking(b,this->destinationPort_var);
}

int SimpleUDPPacket::getSourcePort() const
{
    return sourcePort_var;
}

void SimpleUDPPacket::setSourcePort(int sourcePort_var)
{
    this->sourcePort_var = sourcePort_var;
}

int SimpleUDPPacket::getDestinationPort() const
{
    return destinationPort_var;
}

void SimpleUDPPacket::setDestinationPort(int destinationPort_var)
{
    this->destinationPort_var = destinationPort_var;
}

class SimpleUDPPacketDescriptor : public cClassDescriptor
{
  public:
    SimpleUDPPacketDescriptor();
    virtual ~SimpleUDPPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(SimpleUDPPacketDescriptor);

SimpleUDPPacketDescriptor::SimpleUDPPacketDescriptor() : cClassDescriptor("SimpleUDPPacket", "cPacket")
{
}

SimpleUDPPacketDescriptor::~SimpleUDPPacketDescriptor()
{
}

bool SimpleUDPPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SimpleUDPPacket *>(obj)!=NULL;
}

const char *SimpleUDPPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SimpleUDPPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int SimpleUDPPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *SimpleUDPPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "sourcePort",
        "destinationPort",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int SimpleUDPPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourcePort")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationPort")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SimpleUDPPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *SimpleUDPPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int SimpleUDPPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SimpleUDPPacket *pp = (SimpleUDPPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SimpleUDPPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SimpleUDPPacket *pp = (SimpleUDPPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSourcePort());
        case 1: return long2string(pp->getDestinationPort());
        default: return "";
    }
}

bool SimpleUDPPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SimpleUDPPacket *pp = (SimpleUDPPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setSourcePort(string2long(value)); return true;
        case 1: pp->setDestinationPort(string2long(value)); return true;
        default: return false;
    }
}

const char *SimpleUDPPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *SimpleUDPPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SimpleUDPPacket *pp = (SimpleUDPPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


