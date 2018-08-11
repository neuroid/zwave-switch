set(OPENWRT_PREFIX $ENV{HOME}/src/openwrt/staging_dir)
set(OPENWRT_TOOLCHAIN_PREFIX ${OPENWRT_PREFIX}/toolchain-arm_cortex-a9+vfpv3_gcc-7.3.0_musl_eabi)
set(OPENWRT_TARGET_PREFIX ${OPENWRT_PREFIX}/target-arm_cortex-a9+vfpv3_musl_eabi)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_C_COMPILER ${OPENWRT_TOOLCHAIN_PREFIX}/bin/arm-openwrt-linux-gcc)
set(CMAKE_CXX_FLAGS "-Wno-ignored-qualifiers -Wno-unknown-pragmas -Wno-unused-parameter" CACHE STRING "" FORCE)
set(CMAKE_FIND_ROOT_PATH ${OPENWRT_TARGET_PREFIX})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

message(STATUS "NOTE: build with `STAGING_DIR=${OPENWRT_TOOLCHAIN_PREFIX} make`")
