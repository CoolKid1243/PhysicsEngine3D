########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(glew_FIND_QUIETLY)
    set(glew_MESSAGE_MODE VERBOSE)
else()
    set(glew_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/glewTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${glew_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(glew_VERSION_STRING "2.2.0")
set(glew_INCLUDE_DIRS ${glew_INCLUDE_DIRS_DEBUG} )
set(glew_INCLUDE_DIR ${glew_INCLUDE_DIRS_DEBUG} )
set(glew_LIBRARIES ${glew_LIBRARIES_DEBUG} )
set(glew_DEFINITIONS ${glew_DEFINITIONS_DEBUG} )


# Only the last installed configuration BUILD_MODULES are included to avoid the collision
foreach(_BUILD_MODULE ${glew_BUILD_MODULES_PATHS_DEBUG} )
    message(${glew_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()


