/**
 *
 * Copyright (c) 2005-2012 POP-C++ project - GRID & Cloud Computing group, University of Applied Sciences of western
 *Switzerland.
 * http://gridgroup.hefr.ch/popc
 *
 * @author Tuan Anh Nguyen
 * @date 2005/01/01
 * @brief Buffer abstract class used to pack/unpack data.
 *
 */

#ifndef _POP_DATAPACK_MARSHAL_H
#define _POP_DATAPACK_MARSHAL_H

#include "pop_combox.h"
#include <string>
#include <vector>
#include <cassert>

#define EXCEPTION_INT 1
#define EXCEPTION_UINT 2

#define EXCEPTION_LONG 3
#define EXCEPTION_ULONG 4

#define EXCEPTION_SHORT 5
#define EXCEPTION_USHORT 6

#define EXCEPTION_BOOL 7

#define EXCEPTION_CHAR 8
#define EXCEPTION_UCHAR 9

#define EXCEPTION_STRING 10

#define EXCEPTION_FLOAT 11
#define EXCEPTION_DOUBLE 12

#define EXCEPTION_OBJECT 13
#define EXCEPTION_POPC_STD 14

// bit FLAGS values customized marshalling/demarshalling procedure
#define FLAG_MARSHAL 2
#define FLAG_INPUT 1

#define TYPE_REQUEST 0
#define TYPE_RESPONSE 1
#define TYPE_EXCEPTION 2

#define HEADER_SIZE 24

class pop_interface;
class pop_exception;
class pop_combox;
class pop_connection;
/**
 * @class pop_message_header
 * @author Tuan Anh Nguyen
 */
class pop_message_header {
public:
    pop_message_header(int requestid, int classid, int methodid, int semantics, const char* methodname);
    pop_message_header(const char* methodname);
    pop_message_header(int exceptioncode, const char* methodname);
    pop_message_header();

    void operator=(const pop_message_header& dat);

    inline int GetType() const {
        return type;
    }
    inline int GetRequestID() const {
        return id[0];
    }
    inline int GetClassID() const {
        return id[1];
    }
    inline int GetMethodID() const {
        return id[2];
    }
    inline int GetSemantics() const {
        return id[3];
    }
    inline int GetExceptionCode() const {
        return exception;
    }
    inline const char* GetMethodName() const {
        return methodname;
    }
    inline void SetType(int msgtype) {
        type = msgtype;
    }
    inline void SetRequestID(int requestid) {
        id[0] = requestid;
    }
    inline void SetClassID(int classid) {
        id[1] = classid;
    }
    inline void SetMethodID(int methodid) {
        id[2] = methodid;
    }
    inline void SetSemantics(int semantics) {
        id[3] = semantics;
    }
    inline void SetExceptionCode(int code) {
        exception = code;
    }
    inline void SetMethodName(const char* name) {
        methodname = name;
    }

private:
    int type;
    int id[4];
    const char* methodname;
    int exception;
};

/**
 * @class pop_buffer
 * @brief Buffer abstract class used to pack/unpack data, used by POP-C++ runtime.
 * @author Tuan Anh Nguyen
 */
class pop_buffer {
public:
    pop_buffer();
    virtual ~pop_buffer();

public:
    virtual void Reset() = 0;

    virtual void SetHeader(const pop_message_header& data);
    virtual const pop_message_header& GetHeader() const;

    virtual void Push(const char* paramname, const char* paramtype, int nelem);
    virtual void Pop();

    virtual void Pack(const int* data, int n) = 0;
    virtual void UnPack(int* data, int n) = 0;

    virtual void Pack(const unsigned* data, int n) = 0;
    virtual void UnPack(unsigned* data, int n) = 0;

    virtual void Pack(const long* data, int n) = 0;
    virtual void UnPack(long* data, int n) = 0;

    virtual void Pack(const unsigned long* data, int n) = 0;
    virtual void UnPack(unsigned long* data, int n) = 0;

    virtual void Pack(const short* data, int n) = 0;
    virtual void UnPack(short* data, int n) = 0;

    virtual void Pack(const unsigned short* data, int n) = 0;
    virtual void UnPack(unsigned short* data, int n) = 0;

    virtual void Pack(const bool* data, int n) = 0;
    virtual void UnPack(bool* data, int n) = 0;

    virtual void Pack(const char* data, int n) = 0;
    virtual void UnPack(char* data, int n) = 0;

    virtual void Pack(const unsigned char* data, int n) = 0;
    virtual void UnPack(unsigned char* data, int n) = 0;

    virtual void Pack(const float* data, int n) = 0;
    virtual void UnPack(float* data, int n) = 0;

    virtual void Pack(const double* data, int n) = 0;
    virtual void UnPack(double* data, int n) = 0;

    virtual void Pack(const std::string* list, int n);
    virtual void UnPack(std::string* list, int n);

    virtual void Pack(const signed char* data, int n) = 0;
    virtual void UnPack(signed char* data, int n) = 0;

    virtual bool Send(pop_combox& s, pop_connection* conn = 0) = 0;
    virtual bool Send(pop_connection* conn);

    virtual bool Recv(pop_combox& s, pop_connection* conn = 0) = 0;
    virtual bool Recv(pop_connection* conn);

    virtual int get_size() = 0;
    virtual char* get_load() = 0;
    virtual void load(char* data, int length) = 0;

#ifdef OD_DISCONNECT
    virtual bool RecvCtrl(pop_combox& s, pop_connection* conn = 0) = 0;
#endif
    // Exception stubs...
    static bool SendException(pop_buffer& except, pop_connection* s, int code);
    static bool SendException(pop_buffer& except, pop_connection* s, unsigned code);

    static bool SendException(pop_buffer& except, pop_connection* s, long code);
    static bool SendException(pop_buffer& except, pop_connection* s, unsigned long code);

    static bool SendException(pop_buffer& except, pop_connection* s, short code);
    static bool SendException(pop_buffer& except, pop_connection* s, unsigned short code);

    static bool SendException(pop_buffer& except, pop_connection* s, bool code);

    static bool SendException(pop_buffer& except, pop_connection* s, char code);
    static bool SendException(pop_buffer& except, pop_connection* s, unsigned char code);

    static bool SendException(pop_buffer& except, pop_connection* s, char* code);

    static bool SendException(pop_buffer& except, pop_connection* s, float code);
    static bool SendException(pop_buffer& except, pop_connection* s, double code);

    static bool SendException(pop_buffer& except, pop_connection* s, pop_exception& code);
    static bool SendException(pop_buffer& except, pop_connection* s, pop_interface& code);

    static void CheckAndThrow(pop_buffer& except);

    template <typename T>
    void Pack(const std::vector<T>* vect, int n) {
        (void)n;
        assert(n == 1);
        int s = vect->size();
        Pack(&s, 1);
        if (s > 0) {
            Pack((T*)&(*vect)[0], s);
        }
    }

    template <typename T>
    void UnPack(std::vector<T>* vect, int n) {
        (void)n;
        assert(n == 1);
        int s = 0;
        UnPack(&s, 1);
        vect->resize(s);
        if (s > 0) {
            UnPack((T*)&(*vect)[0], s);
        }
    }

protected:
    pop_message_header header;
};

typedef pop_buffer POPBuffer;
#endif
