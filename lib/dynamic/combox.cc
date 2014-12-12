/**
 *
 * Copyright (c) 2005-2012 POP-C++ project - GRID & Cloud Computing group, University of Applied Sciences of western Switzerland.
 * http://gridgroup.hefr.ch/popc
 *
 * @author Tuan Anh Nguyen
 * @date 2005/01/01
 * @brief Implementation of the communication box abstraction.
 *
 *
 */

/*
  Deeply need refactoring:
    POPC_Connection instead of paroc_connector
    POPC_Combox instead of paroc_combox
   Need to separate connection and combox implementation in two separate file.
 */

#include <stdio.h>

#include "paroc_combox.h"
#include "paroc_exception.h"
#include "debug.h"

#include <string.h>


paroc_connection::paroc_connection(paroc_combox *com) : _is_initial_connection(false) {
    if(com != NULL) {
        fact = com->GetBufferFactory();
        combox = com;
    }
}

paroc_connection::paroc_connection(paroc_combox* com, bool init) {
    if(com != NULL) {
        fact = com->GetBufferFactory();
        combox = com;
    }
    _is_initial_connection = init;
}

paroc_connection::paroc_connection(paroc_combox *com, paroc_buffer_factory *f) : _is_initial_connection(false) {
    fact=f;
    combox=com;
}
paroc_connection::~paroc_connection() {
}

bool paroc_connection::is_initial_connection() {
    return _is_initial_connection;
}

void paroc_connection::SetBufferFactory(paroc_buffer_factory *f) {
    fact=f;
}

paroc_buffer_factory *paroc_connection::GetBufferFactory() {
    return fact;
}

paroc_combox *paroc_connection::GetCombox() {
    return combox;
}



/**
 * COMBOX Implementation
 */

const char* paroc_combox::PROTOCOL_SEPARATOR = "://";



paroc_combox::paroc_combox() {
    defaultFact = paroc_buffer_factory_finder::GetInstance()->FindFactory("xdr");
    if(defaultFact == NULL) {
        printf("POP-C++ Error: can not find the xdr buffer factory!\n");
    }

    timeout = -1;
    for(int i = 0; i < 2; i++) {
        cblist[i] = NULL;
        cbdata[i] = NULL;
    }
}

paroc_combox::~paroc_combox() {
}

bool paroc_combox::SendAck(paroc_connection *conn) {
    char buf[4]="ACK";
    Send(buf,3,conn);
    return true;
}

bool paroc_combox::RecvAck(paroc_connection *conn) {
    paroc_connection * connex= Wait();
    if(connex==NULL) {
        paroc_exception::paroc_throw(ACK_NOT_RECEIVED,"[paroc_combox_socket.cc]");
    }
    char buf[4];
    int n = Recv(buf,3, connex);
    if(n!=3||strcmp(buf,"ACK")) {
        paroc_exception::paroc_throw(ACK_NOT_RECEIVED,"[paroc_combox_socket.cc]");
    }

    return true;
}

void paroc_combox::SetTimeout(int millisec) {
    timeout=millisec;
}

int paroc_combox::GetTimeout() {
    return timeout;
}

void paroc_combox::Destroy() {
    delete this;
}


bool paroc_combox::SetCallback(COMBOX_EVENTS ev, COMBOX_CALLBACK cb, void *arg) {
    int idx=(int)ev;
    if(idx<0 || idx>=2) {
        return false;
    }
    cblist[idx]=cb;
    cbdata[idx]=arg;
    return true;
}

void paroc_combox::SetBufferFactory(paroc_buffer_factory *fact) {
    defaultFact=fact;
}

paroc_buffer_factory *paroc_combox::GetBufferFactory() {
    return defaultFact;
}

bool paroc_combox::OnNewConnection(paroc_connection *conn) {
    COMBOX_CALLBACK cb=cblist[COMBOX_NEW];
    if(cb!=NULL) {
        return cb(cbdata[COMBOX_NEW], conn);
    }
    return true;
}

bool paroc_combox::OnCloseConnection(paroc_connection *conn) {
    COMBOX_CALLBACK cb=cblist[COMBOX_CLOSE];
    if(cb!=NULL) {
        return cb(cbdata[COMBOX_CLOSE], conn);
    }
    return true;
}
