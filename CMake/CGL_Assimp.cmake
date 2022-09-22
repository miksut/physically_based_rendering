MESSAGE("Local Assimp External ")
if (MSVC)
    if(MSVC_VERSION EQUAL 1800)
        set(ASSIMP_MSVC_VERSION "vc120")
    elseif(MSVC_VERSION EQUAL 1900)  
        set(ASSIMP_MSVC_VERSION "vc140")
    elseif((MSVC_VERSION GREATER_EQUAL 1910) AND (MSVC_VERSION LESS_EQUAL 1919))  
        set(ASSIMP_MSVC_VERSION "vc141")
    elseif((MSVC_VERSION GREATER_EQUAL 1920) AND (MSVC_VERSION LESS_EQUAL 1929))  
        set(ASSIMP_MSVC_VERSION "vc142")
    endif(MSVC_VERSION)
    ExternalProject_Add(assimp_external
        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp"
        BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp"
        CMAKE_ARGS "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}"
                   "-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}"
                   "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
                   # These are only useful if you're cross-compiling.
                   # They, however, will not hurt regardless.
                   "-DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}"
                   "-DCMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}"
                   "-DCMAKE_AR=${CMAKE_AR}"
                   "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
                   "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
                   "-DCMAKE_RC_COMPILER=${CMAKE_RC_COMPILER}"
                   "-DCMAKE_COMPILER_PREFIX=${CMAKE_COMPILER_PREFIX}"
                   "-DCMAKE_FIND_ROOT_PATH=${CMAKE_FIND_ROOT_PATH}"
       INSTALL_COMMAND ${CMAKE_COMMAND} -E copy $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/bin/Release/assimp-${ASSIMP_MSVC_VERSION}-mt.dll>$<$<CONFIG:DEBUG>:${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/bin/Debug/assimp-${ASSIMP_MSVC_VERSION}-mtd.dll> $<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/Release/assimp-${ASSIMP_MSVC_VERSION}-mt.dll>$<$<CONFIG:Debug>:${CMAKE_BINARY_DIR}/Debug/assimp-${ASSIMP_MSVC_VERSION}-mtd.dll>
       )
      set(assimp_lib_name_debug "Debug/assimp-${ASSIMP_MSVC_VERSION}-mtd")
      set(assimp_lib_name_release "Release/assimp-${ASSIMP_MSVC_VERSION}-mt")
      set(assimp_lib_suffix ".lib")
elseif(APPLE)
    ExternalProject_Add(assimp_external
        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp"
        BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp"
        CMAKE_ARGS "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}"
                   "-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}"
                   "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
                   # These are only useful if you're cross-compiling.
                   # They, however, will not hurt regardless.
                   "-DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}"
                   "-DCMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}"
                   "-DCMAKE_AR=${CMAKE_AR}"
                   "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
                   "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
                   "-DCMAKE_RC_COMPILER=${CMAKE_RC_COMPILER}"
                   "-DCMAKE_COMPILER_PREFIX=${CMAKE_COMPILER_PREFIX}"
                   "-DCMAKE_FIND_ROOT_PATH=${CMAKE_FIND_ROOT_PATH}"
       INSTALL_COMMAND ""
       )
      set(assimp_lib_name_debug "Debug/libassimp")
      set(assimp_lib_name_release "Release/libassimp")
      set(assimp_lib_suffix ".dylib")
else()
    ExternalProject_Add(assimp_external
        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp"
        BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp"
        CMAKE_ARGS "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}"
                   "-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}"
                   "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
                   # These are only useful if you're cross-compiling.
                   # They, however, will not hurt regardless.
                   "-DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}"
                   "-DCMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}"
                   "-DCMAKE_AR=${CMAKE_AR}"
                   "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
                   "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
                   "-DCMAKE_RC_COMPILER=${CMAKE_RC_COMPILER}"
                   "-DCMAKE_COMPILER_PREFIX=${CMAKE_COMPILER_PREFIX}"
                   "-DCMAKE_FIND_ROOT_PATH=${CMAKE_FIND_ROOT_PATH}"
		   INSTALL_COMMAND ""
		   #		   INSTALL_COMMAND ${CMAKE_COMMAND} -E copy $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/lib/assimp-${ASSIMP_MSVC_VERSION}-mt.dll>$<$<CONFIG:DEBUG>:${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/lib/libassimpd.so.4> $<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/Release/assimp-${ASSIMP_MSVC_VERSION}-mt.dll>$<$<CONFIG:Debug>:${CMAKE_BINARY_DIR}/libassimpd.so.4>
       )
      set(assimp_lib_name_debug "libassimpd")
      set(assimp_lib_name_release "libassimp")
      set(assimp_lib_suffix ".so")
endif()


add_library(assimp_external_debug UNKNOWN IMPORTED)
add_dependencies(assimp_external_debug assimp_external)
set_target_properties(assimp_external_debug PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/lib/${assimp_lib_name_debug}${assimp_lib_suffix})
set_target_properties(assimp_external_debug PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/include)




#add_library(assimp_external_debug_sh SHARED IMPORTED)
#add_dependencies(assimp_external_debug_sh assimp_external)
#set_target_properties(assimp_external_debug_sh PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/lib/${assimp_lib_name_debug}${assimp_lib_suffix}.4)
#set_target_properties(assimp_external_debug_sh PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/include)



add_library(assimp_external_optimized UNKNOWN IMPORTED)
add_dependencies(assimp_external_optimized assimp_external)
set_target_properties(assimp_external_optimized PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/lib/${assimp_lib_name_release}${assimp_lib_suffix})
set_target_properties(assimp_external_optimized PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/include)
set(assimp_external_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/include)

#set(assimp_external_LIBRARIES debug assimp_external_debug optimized  assimp_external_optimized debug assimp_external_debug_sh)
set(assimp_external_LIBRARIES debug assimp_external_debug optimized  assimp_external_optimized)



