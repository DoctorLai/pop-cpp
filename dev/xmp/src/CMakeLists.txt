#
# POP-C++
# Advanced library build process
#
# AUTHOR: Valentin Clement
# DATE: 2012/11/14
#

# Refers to the dynamic version of POP-C++
include_directories(${CMAKE_SOURCE_DIR}/include/dynamic)

# Add more warnings
add_definitions(-Wall -Wextra)

# Build libpopc_advanced
add_library(popc_advanced
  popc_group_interface.cpp
  popc_group_broker.cpp
  popc_group_broker_factory.cpp
  popc_object.cpp
  popc_connection_mpi.cpp
)

# Install headers (only for dynamic communication model)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/popc_group_exception.h DESTINATION include/dynamic)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/popc_connection_mpi.h DESTINATION include/dynamic)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/popc_advanced_header.h DESTINATION include/dynamic)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/popc_group_interface.h DESTINATION include/dynamic)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/popc_group_broker.h DESTINATION include/dynamic)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/popc_group_broker_factory.h DESTINATION include/dynamic)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/popc_object.h DESTINATION include/dynamic)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/popc-xmp.h DESTINATION include/dynamic)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/libpopc_advanced.a DESTINATION lib/dynamic)
