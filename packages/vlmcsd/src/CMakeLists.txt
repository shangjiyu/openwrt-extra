cmake_minimum_required(VERSION 2.6)

PROJECT(vlmcsd)

SET(CMAKE_SYSTEM_NAME Linux)
#SET(CMAKE_C_COMPILER "mipsel-openwrt-linux-gcc")
#SET(CMAKE_CXX_COMPILER "mipsel-openwrt-linux-g++")

SET(CMAKE_C_FLAGS "-Os -std=gnu99 -pthread -fpic ")
SET(CMAKE_CXX_FLAGS "-Os -std=gnu++0x -pthread -fpic")

ADD_DEFINITIONS("-DBUILD_TIME")
SET(SOURCES
	vlmcsd.c
	kms.c
	crypto.c
	rpc.c
	network.c
	output.c
	crypto_internal.c
	shared_globals.c
	helpers.c
	dns_srv.c
	ns_name.c
	ns_parser.c
)

ADD_EXECUTABLE(vlmcsd ${SOURCES})
TARGET_LINK_LIBRARIES(vlmcsd pthread dl)



INSTALL(TARGETS vlmcsd
	RUNTIME DESTINATION /usr/sbin
)
