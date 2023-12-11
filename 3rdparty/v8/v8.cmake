cmake_minimum_required(VERSION 3.17)

set(V8_DIR ${CMAKE_CURRENT_LIST_DIR})

add_library(v8 SHARED IMPORTED)
set_target_properties(v8 PROPERTIES IMPORTED_LOCATION ${V8_DIR}/libs/libv8.dylib)

# must be defined
target_compile_definitions(v8 INTERFACE V8_COMPRESS_POINTERS)

# include v8 header file
target_include_directories(v8 INTERFACE ${V8_DIR}/include)

add_library(v8::base SHARED IMPORTED)
set_target_properties(v8::base PROPERTIES IMPORTED_LOCATION ${V8_DIR}/libs/libv8_libbase.dylib)

add_library(v8::platform SHARED IMPORTED)
set_target_properties(v8::platform PROPERTIES IMPORTED_LOCATION ${V8_DIR}/libs/libv8_libplatform.dylib)

add_library(icuuc SHARED IMPORTED)
set_target_properties(icuuc PROPERTIES IMPORTED_LOCATION ${V8_DIR}/libs/libicuuc.dylib)

add_library(icui18n SHARED IMPORTED)
set_target_properties(icui18n PROPERTIES IMPORTED_LOCATION ${V8_DIR}/libs/libicui18n.dylib)

add_library(chrome_zlib SHARED IMPORTED)
set_target_properties(chrome_zlib PROPERTIES IMPORTED_LOCATION ${V8_DIR}/libs/libchrome_zlib.dylib)

add_library(v8Dll INTERFACE IMPORTED)
set_property(TARGET v8Dll PROPERTY
        INTERFACE_LINK_LIBRARIES v8 v8::base v8::platform icuuc icui18n chrome_zlib)