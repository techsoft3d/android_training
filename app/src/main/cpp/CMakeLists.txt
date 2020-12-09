cmake_minimum_required(VERSION 3.4.1)

project(Visualize_Training)

set(HPS_PATH $ENV{HPS_VISUALIZE_INSTALL_DIR})
set(SHARED_SOURCES_PATH ${PROJECT_SOURCE_DIR})
set(JNI_SOURCES_PATH ${PROJECT_SOURCE_DIR}/JNI)

if (USING_DEBUG_HPS_LIBS)
    set(HPS_BIN_PATH ${HPS_PATH}/bin/android_${ANDROID_ABI}d)
else()
    set(HPS_BIN_PATH ${HPS_PATH}/bin/android_${ANDROID_ABI})
endif()

set(USING_EXCHANGE FALSE)

# add hps shared object lib
add_library(hps_core SHARED IMPORTED )
set_target_properties(hps_core PROPERTIES IMPORTED_LOCATION ${HPS_BIN_PATH}/libhps_core.so )

add_library(hps_sprk SHARED IMPORTED )
set_target_properties(hps_sprk PROPERTIES IMPORTED_LOCATION ${HPS_BIN_PATH}/libhps_sprk.so )

add_library(hps_sprk_ops SHARED IMPORTED )
set_target_properties(hps_sprk_ops PROPERTIES IMPORTED_LOCATION ${HPS_BIN_PATH}/libhps_sprk_ops.so )

if (USING_EXCHANGE)
    message("Using Exchange")
    if(DEFINED ENV{HEXCHANGE_INSTALL_DIR})
        file(TO_CMAKE_PATH "$ENV{HEXCHANGE_INSTALL_DIR}" EXCHANGE_PATH)
        message(STATUS "Using HEXCHANGE_INSTALL_DIR from the environment: ${EXCHANGE_PATH}")
        set(EXCHANGE_BIN_PATH ${EXCHANGE_PATH}/bin/android/${ANDROID_ABI})
    else()
        message(ERROR "The environment variable HEXCHANGE_INSTALL_DIR is not set.")
    endif()

    add_library(A3DLIBS SHARED IMPORTED )
    set_target_properties(A3DLIBS PROPERTIES IMPORTED_LOCATION ${EXCHANGE_BIN_PATH}/libA3DLIBS.so )

    add_library(hps_sprk_exchange SHARED IMPORTED )
    set_target_properties(hps_sprk_exchange PROPERTIES IMPORTED_LOCATION ${HPS_BIN_PATH}/libhps_sprk_exchange.so )
endif()

set(SOURCES
    ${JNI_SOURCES_PATH}/AndroidMobileSurfaceViewJNI.cpp
    ${JNI_SOURCES_PATH}/AndroidUserMobileSurfaceViewJNI.cpp
    ${JNI_SOURCES_PATH}/MobileAppJNI.cpp
    ${JNI_SOURCES_PATH}/OnLoadJNI.cpp
    ${SHARED_SOURCES_PATH}/MobileApp.cpp
    ${SHARED_SOURCES_PATH}/MobileSurface.cpp
    ${SHARED_SOURCES_PATH}/UserMobileSurface.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set(DEFINES TARGET_OS_ANDROID=1)

set(INCLUDES
    ${HPS_PATH}/include
    ${PROJECT_SOURCE_DIR}
    ${SHARED_SOURCES_PATH}
)

if (USING_EXCHANGE)
    list(APPEND DEFINES USING_EXCHANGE=1)
    list(APPEND INCLUDES ${EXCHANGE_PATH}/include)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC ${DEFINES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})

set(DYNAMIC_LIBRARY_DEPENDENCIES
    hps_core
    hps_sprk
    hps_sprk_ops
)

if (USING_EXCHANGE)
    list(APPEND DYNAMIC_LIBRARY_DEPENDENCIES
        hps_sprk_exchange
    )
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC
    android
    log
    GLESv2
    EGL
    ${DYNAMIC_LIBRARY_DEPENDENCIES}
)

# -- sip --
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND python ${PROJECT_SOURCE_DIR}/../../../../sip/sip.py
    COMMENT "SIP: Updating JNI wrappers"
)
# ---

foreach(link_item ${DYNAMIC_LIBRARY_DEPENDENCIES})
    list(APPEND LIBRARIES_TO_COPY $<TARGET_FILE:${link_item}>)
endforeach()

#  A3DLIBS must not be loaded directly but should be packaged
if (USING_EXCHANGE)
    list(APPEND LIBRARIES_TO_COPY $<TARGET_FILE:A3DLIBS>)
endif()

# make sure our libs are in the jniLibs directory so they get packaged
set(JNILIBS_DIR ${PROJECT_SOURCE_DIR}/../jniLibs)
file(MAKE_DIRECTORY ${JNILIBS_DIR})
set(JNILIBS_ABI_DIR ${JNILIBS_DIR}/${ANDROID_ABI})
file(MAKE_DIRECTORY ${JNILIBS_ABI_DIR})
file(GLOB PAST_LIBS  ${JNILIBS_ABI_DIR}/*)

if (PAST_LIBS)
    file (REMOVE ${PAST_LIBS})
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${LIBRARIES_TO_COPY}
    ${JNILIBS_ABI_DIR}
    COMMENT "Copying dynamic libraries"
)