#include "POPObject.ph"
#include <unistd.h>

/**
 * @author clementval
 * @date 2012.07.11
 * This program tests passing array with the standard C++ declaration instead of specific POP-C++ one.
 */

POPObject::POPObject() {
    cout << "POPCobject created (by JobMgr) on machine:" << GetAccessPoint().GetAccessString() << popcendl;
}

POPObject::~POPObject() {
    cout << "POPCobject: on machine:" << POPSystem::GetHost() << " is being destroyed" << popcendl;
}

void POPObject::voidMethod(void) {
    cout << "Method with void param is called" << popcendl;
}

@pack(POPObject);
