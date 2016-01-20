/**
 *
 * Copyright (c) 2005-2012 POP-C++ project - GRID & Cloud Computing group, University of Applied Sciences of western
 *Switzerland.
 * http://gridgroup.hefr.ch/popc
 *
 * @author Tuan Anh Nguyen
 * @date 2005/01/01
 * @brief Implementation of raw message buffer.
 *
 *
 */

/*
  Deeply need refactoring:
    POPC_BufferRAW instead of pop_buffer_raw
 */

#include <stdio.h>
#include <string.h>
#include "pop_interface.h"
#include "pop_buffer_raw.h"
#include "pop_exception.h"
#include "pop_logger.h"

pop_buffer_raw::pop_buffer_raw() {
    Reset();
}

pop_buffer_raw::~pop_buffer_raw() {
}

void pop_buffer_raw::Reset() {
    unpackpos = HEADER_SIZE;
    // packeddata.RemoveAll();
    packeddata.resize(unpackpos);
}

void pop_buffer_raw::Pack(const char* data, int n) {
    if (n <= 0) {
        return;
    }
    int t = packeddata.size();
    packeddata.resize(t + ((n - 1) / 4 + 1) * 4);
    memcpy(packeddata.data() + t, data, n);
}

void pop_buffer_raw::UnPack(char* data, int n) {
    if (n <= 0) {
        return;
    }
    CheckUnPack(n);  // Error with this check in 64 bits
    packeddata.size();
    memcpy(data, packeddata.data() + unpackpos, n);
    unpackpos += ((n - 1) / 4 + 1) * 4;
}

void pop_buffer_raw::Pack(const int* data, int n) {
    Pack(reinterpret_cast<const char*>(data), n * sizeof(int));
}

void pop_buffer_raw::UnPack(int* data, int n) {
    UnPack(reinterpret_cast<char*>(data), n * sizeof(int));
}

void pop_buffer_raw::Pack(const unsigned* data, int n) {
    Pack(reinterpret_cast<const char*>(data), n * sizeof(unsigned));
}
void pop_buffer_raw::UnPack(unsigned* data, int n) {
    UnPack(reinterpret_cast<char*>(data), n * sizeof(unsigned));
}

void pop_buffer_raw::Pack(const long* data, int n) {
    Pack(reinterpret_cast<const char*>(data), n * sizeof(long));
}

void pop_buffer_raw::UnPack(long* data, int n) {
    UnPack(reinterpret_cast<char*>(data), n * sizeof(long));
}

void pop_buffer_raw::Pack(const unsigned long* data, int n) {
    Pack(reinterpret_cast<const char*>(data), n * sizeof(unsigned long));
}

void pop_buffer_raw::UnPack(unsigned long* data, int n) {
    UnPack(reinterpret_cast<char*>(data), n * sizeof(unsigned long));
}

void pop_buffer_raw::Pack(const short* data, int n) {
    Pack(reinterpret_cast<const char*>(data), n * sizeof(short));
}

void pop_buffer_raw::UnPack(short* data, int n) {
    UnPack(reinterpret_cast<char*>(data), n * sizeof(short));
}

void pop_buffer_raw::Pack(const unsigned short* data, int n) {
    Pack(reinterpret_cast<const char*>(data), n * sizeof(unsigned short));
}

void pop_buffer_raw::UnPack(unsigned short* data, int n) {
    UnPack(reinterpret_cast<char*>(data), n * sizeof(unsigned short));
}

void pop_buffer_raw::Pack(const bool* data, int n) {
    Pack(reinterpret_cast<const char*>(data), n * sizeof(bool));
}

void pop_buffer_raw::UnPack(bool* data, int n) {
    UnPack(reinterpret_cast<char*>(data), n * sizeof(bool));
}

void pop_buffer_raw::Pack(const unsigned char* data, int n) {
    Pack(reinterpret_cast<const char*>(data), n);
}

void pop_buffer_raw::UnPack(unsigned char* data, int n) {
    UnPack(reinterpret_cast<char*>(data), n);
}

void pop_buffer_raw::Pack(const float* data, int n) {
    Pack(reinterpret_cast<const char*>(data), n * sizeof(float));
}

void pop_buffer_raw::UnPack(float* data, int n) {
    UnPack(reinterpret_cast<char*>(data), n * sizeof(float));
}

void pop_buffer_raw::Pack(const double* data, int n) {
    Pack(reinterpret_cast<const char*>(data), n * sizeof(double));
}

void pop_buffer_raw::UnPack(double* data, int n) {
    UnPack(reinterpret_cast<char*>(data), n * sizeof(double));
}

// added by lwk
void pop_buffer_raw::Pack(const signed char* data, int n) {
    Pack(reinterpret_cast<const char*>(data), n);
}

void pop_buffer_raw::UnPack(signed char* data, int n) {
    UnPack(reinterpret_cast<char*>(data), n);
}

void pop_buffer_raw::CheckUnPack(int sz) {
    if (static_cast<std::size_t>(sz + unpackpos) > packeddata.size()) {
        pop_exception::pop_throw(POP_BUFFER_FORMAT, "Wrong buffer format in pop_buffer_raw::CheckUnPack");
    }
}

/**
 * Send the packed data to the matching combox
 * @param s
 * @param conn
 * @return
 */
bool pop_buffer_raw::Send(pop_combox& s, pop_connection* conn) {
    // Pack the header
    char* dat = get_load();

    if (dat == nullptr) {
        LOG_ERROR("fail 1");
        return false;
    }
    
    // MPI mod - beg
    char* data_header = new char[HEADER_SIZE];
    memcpy(data_header, h, HEADER_SIZE);

    if (s.Send(data_header, HEADER_SIZE, conn)) {
        LOG_ERROR("Error while sending header");
        return false;
    }
    // MPI mod - end

    dat += HEADER_SIZE;
    n -= HEADER_SIZE;
    if (n > 0) {
        LOG_INFO("RAW: Send message size is %d: %s", n, packeddata.data());
        if (s.Send(dat, n, conn) < 0) {
            LOG_WARNING("Fail to send a message!");
            return false;
        }
    }

    return true;
}

// Propagation of exceptions back to caller...
bool pop_buffer_raw::Recv(pop_combox& s, pop_connection* conn) {
    int h[6];
    int n;

    // Recv the header
    char* dat = (char*)h;
    s.Recv(dat, HEADER_SIZE, conn);
    LOG_INFO("RAW: header received");

    Reset();
    n = h[0];
    if (n < HEADER_SIZE) {
        LOG_ERROR("[CORE] - Buffer RAW - bad message header (size error:%d)", n);
        return false;
    }

    
    header.setRequestID(h[1];
    
    int type = h[2];
    header.SetType(type);
    switch (type) {
        case TYPE_REQUEST:
            header.SetClassID(h[3]);
            header.SetMethodID(h[4]);
            header.SetSemantics(h[5]);
            break;
        case TYPE_EXCEPTION:
            header.SetExceptionCode(h[3]);
            break;
        case TYPE_RESPONSE:
            header.SetClassID(h[3]);
            header.SetMethodID(h[4]);
            break;
        default:
            LOG_ERROR("Unknown type %d", type);
            return false;
    }

    packeddata.resize(n);
    n -= HEADER_SIZE;

    if (n > 0) {
        dat = packeddata.data() + HEADER_SIZE;
        LOG_INFO("RAW: ready to receive %d", n);
        s.Recv(dat, n, conn);
        LOG_INFO("RAW: received %d", n);
    }

    return true;
}

int pop_buffer_raw::get_size() {
    return packeddata.size();
}

char* pop_buffer_raw::get_load() {
    // Pack the header
    char* dat = packeddata.data();

    if (!dat) {
        return nullptr;
    }

    int n = packeddata.size();
    int h[6];
    memset(h, 0, 6 * sizeof(int));

    int type = header.GetType();

    h[0] = n;
    h[1] = header.getRequestID();
    h[2] = type;

    switch (type) {
        case TYPE_REQUEST:
            h[3] = header.GetClassID();
            h[4] = header.GetMethodID();
            h[5] = header.GetSemantics();
            break;
        case TYPE_EXCEPTION:
            h[3] = header.GetExceptionCode();
            break;
        case TYPE_RESPONSE:
            h[3] = header.GetClassID();
            h[4] = header.GetMethodID();
            break;
        default:
            LOG_ERROR("fail 2");
            return nullptr;
    }

    memcpy(dat, h, HEADER_SIZE);

    return packeddata.data();
}

void pop_buffer_raw::load(char* data, int length) {
    memcpy(packeddata.data(), data, length);
}

#ifdef OD_DISCONNECT
bool pop_buffer_raw::RecvCtrl(pop_combox& s, pop_connection* conn) {
    while (true) {
        pop_connection* t = (pop_connection*)s.Wait();
        if (!t) {
            pop_exception::pop_throw("Remote Object not alive (1)");
        }

        if (!Recv(s, t)) {
            pop_exception::pop_throw(errno);
        }

        if (header.GetType() == TYPE_RESPONSE) {
            if (header.GetClassID() == 0 && header.GetMethodID() == 6) {
                return true;
            } else {
                pop_message_header h = header;
                int unpackposold = unpackpos;

                auto packeddataold = packeddata;
                auto t = (pop_connection*)s.Wait();

                if (!t) {
                    pop_exception::pop_throw("Remote Object not alive (2)");
                }

                if (!Recv(s, t)) {
                    pop_exception::pop_throw(errno);
                }

                Reset();
                header = h;
                unpackpos = unpackposold;
                packeddata = packeddataold;

                return false;
            }
        }
    }
}

#endif
