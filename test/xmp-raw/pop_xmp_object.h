#include "popc_group_interface.h"

// This file is a sample generated interface

// Generated by the compiler

const unsigned CLASSUID_POPXMPObject = unsigned(3001);

class POPXMPObject : public POPC_GroupInterface {
public:
    POPXMPObject();
    ~POPXMPObject();

    //
    POPXMPObject& operator[](const int index);

    // Declared in .ph as sync collective void execute_xmp_1();
    void execute_xmp_1();

    // Declared in .ph as sync seq void set_value(int val);
    void set_value(int val);

    // Generated generated by the compiler
    virtual char* get_class_name() {
        return (char*)"POPXMPObject";
    }

    // Generated by the compiler
    virtual void construct_remote_object();
    void _popc_constructor();
};
