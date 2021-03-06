/**
 *
 * Copyright (c) 2005-2012 POP-C++ project - GRID & Cloud Computing group, University of Applied Sciences of western
 *Switzerland.
 * http://gridgroup.hefr.ch/popc
 *
 * @author Valentin Clement
 * @date 2012/12/04
 * @brief Declaration of the base class pop_allocatorFactory. The allocator factory allows to provide the right
 *allocator for
 *        parallel object allocation depending the lower layer (SSH, MPI, POP-C++ MPI Interconnector ...).
 *
 *
 */

#include "pop_allocator_mpi_pseudo.h"

#include "pop_system.h"
#include "pop_exception.h"
#include "pop_combox.h"
#include "pop_combox_factory.h"
#include "pop_broker.h"

/**
 * Allocate single object with a pseudo-dynamic allocation mechanism
 * @param objectname  Name of the class to instantiate the object
 * @param od          Object description for the allocation process
 * @return A std::string object representing the access-point of the allocated parallel object.
 */
std::string mpi_allocator_pseudo::allocate(const std::string& /*objectname*/, const pop_od& /*od*/) {
    std::string objectaddress;

    /* TODO Allocation process here */

    return objectaddress;
}

/**
 * Allocate parallel object groupwith a pseudo-dynamic allocation mechanism
 * @param objectname  Name of the class to instantiate the object
 * @param od          Object description for the allocation process
 * @param nb          Number of objects to allocate
 * @return A pointer to a combox or an array of combox connected with the group
 */
pop_combox* mpi_allocator_pseudo::allocate_group(const std::string& /*objectname*/, const pop_od& /*od*/, int /*nb*/) {
    /* TODO Allocation process here */

    return nullptr;
}
