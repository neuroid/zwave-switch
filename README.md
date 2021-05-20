# zwave-switch

A tool for controlling Z-Wave switches.


## Usage

    Usage: zwave-switch CONTROLLER add [OPTION]...
           zwave-switch CONTROLLER on NODE_ID [OPTION]...
           zwave-switch CONTROLLER off NODE_ID [OPTION]...
           zwave-switch CONTROLLER listen NODE_ID [OPTION]...
           zwave-switch CONTROLLER config get NODE_ID INDEX [OPTION]...
           zwave-switch CONTROLLER config set NODE_ID INDEX VALUE [OPTION]...
           zwave-switch CONTROLLER remove [OPTION]...
           zwave-switch CONTROLLER remove failed [OPTION]...

    Options:
      -c, --ozw-config-path PATH    Path to OpenZWave device database
      -u, --ozw-user-path PATH      Path to OpenZWave user data


## Examples

Add a new node to the network:

    $ zwave-switch /dev/ttyACM0 add
    Added node 1 [ZW090 Z-Stick Gen5 EU]
    Press a button on your switch to add it to the network...
    Added node 2 [ZW096 Smart Switch 6]
    ^C

Turn the switch on/off:

    $ zwave-switch /dev/ttyACM0 on 2
    Switch 2 [ZW096 Smart Switch 6] is ON
    $ zwave-switch /dev/ttyACM0 off 2
    Switch 2 [ZW096 Smart Switch 6] is OFF

Run continuously and listen for signals to turn the switch on/off:

    $ zwave-switch /dev/ttyACM0 listen 2
    Listening [pid: 10400]...
    Switch 2 [ZW096 Smart Switch 6] is ON
    Switch 2 [ZW096 Smart Switch 6] is OFF

Send `SIGUSR1`/`SIGUSR2` to turn the switch on/off:

    $ kill -SIGUSR1 10400
    $ kill -SIGUSR2 10400

Get or set a configuration value (in this example the default state of the
switch after a power cycle):

    $ zwave-switch /dev/ttyACM0 config get 2 20
    Last status (Default)
    $ zwave-switch /dev/ttyACM0 config set 2 20 2
    Always off

Consult the `zwcfg_*.xml` file for possible configuration values. The file
is created automatically by OpenZWave. By default it is created in the
current directory. This can be controlled using the `--ozw-user-path` option.


## Building

    git clone git@github.com:neuroid/zwave-switch.git
    cd zwave-switch
    git submodule update --init
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make
    ./zwave-switch -h

The tool relies heavily on [OpenZWave][1]. Make sure the library is installed
before trying to compile the tool. If OpenZWave is installed in a non-standard
location the `OPENZWAVE_PREFIX` variable might be useful:

    cmake -DOPENZWAVE_PREFIX=/opt/openzwave -DCMAKE_BUILD_TYPE=Release ..

[1]: https://github.com/OpenZWave/open-zwave


### Cross compiling for OpenWrt

This requires a functioning [OpenWrt build system][2]. While preparing the
build system make sure to select the `libopenzwave` library. Depending on the
target, your `diffconfig` file might look like this:

    CONFIG_TARGET_mvebu=y
    CONFIG_TARGET_mvebu_cortexa9=y
    CONFIG_TARGET_mvebu_cortexa9_Default=y
    CONFIG_PACKAGE_libopenzwave=m
    CONFIG_PACKAGE_libstdcpp=m
    CONFIG_PACKAGE_openzwave=m
    CONFIG_PACKAGE_openzwave-config=m

The `openwrt-toolchain.cmake` file expects to find the build system in
`$HOME/src/openwrt`. The location can be changed by modifying the
`OPENWRT_PREFIX` variable. The file also makes an assumption about the
target system. You will need to modify the `OPENWRT_TOOLCHAIN_PREFIX` and
`OPENWRT_TARGET_PREFIX` variables to match the platform you are building for.
If everything is set, the cross compilation is a simple as:

    $ mkdir build-openwrt
    $ cd build-openwrt
    $ cmake -DCMAKE_TOOLCHAIN_FILE=../openwrt-toolchain.cmake -DCMAKE_BUILD_TYPE=Release ..
    -- NOTE: build with `STAGING_DIR=/home/user/src/openwrt/staging_dir/toolchain-arm_cortex-a9+vfpv3_gcc-7.3.0_musl_eabi make`
    -- NOTE: build with `STAGING_DIR=/home/user/src/openwrt/staging_dir/toolchain-arm_cortex-a9+vfpv3_gcc-7.3.0_musl_eabi make`
    -- The C compiler identification is GNU 7.3.0
    -- The CXX compiler identification is GNU 7.3.0
    -- Check for working C compiler: /home/user/src/openwrt/staging_dir/toolchain-arm_cortex-a9+vfpv3_gcc-7.3.0_musl_eabi/bin/arm-openwrt-linux-gcc
    -- Check for working C compiler: /home/user/src/openwrt/staging_dir/toolchain-arm_cortex-a9+vfpv3_gcc-7.3.0_musl_eabi/bin/arm-openwrt-linux-gcc -- works
    -- Detecting C compiler ABI info
    -- Detecting C compiler ABI info - done
    -- Detecting C compile features
    -- Detecting C compile features - done
    -- Check for working CXX compiler: /home/user/src/openwrt/staging_dir/toolchain-arm_cortex-a9+vfpv3_gcc-7.3.0_musl_eabi/bin/arm-openwrt-linux-c++
    -- Check for working CXX compiler: /home/user/src/openwrt/staging_dir/toolchain-arm_cortex-a9+vfpv3_gcc-7.3.0_musl_eabi/bin/arm-openwrt-linux-c++ -- works
    -- Detecting CXX compiler ABI info
    -- Detecting CXX compiler ABI info - done
    -- Detecting CXX compile features
    -- Detecting CXX compile features - done
    -- Configuring done
    -- Generating done
    -- Build files have been written to: /home/user/src/zwave-switch/build-openwrt
    $ STAGING_DIR=/home/user/src/openwrt/staging_dir/toolchain-arm_cortex-a9+vfpv3_gcc-7.3.0_musl_eabi make
    Scanning dependencies of target zwave-switch
    ...
    [100%] Built target zwave-switch

You can use `file` as a sanity check before transferring the binary to your
target system:

    $ file zwave-switch
    zwave-switch: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-musl-armhf.so.1, not stripped

On the target system you might need to install the `cdc_acm` module for
`/dev/ttyACM?` to become available:

    opkg install kmod-usb-acm

[2]: https://openwrt.org/docs/guide-developer/build-system/start


## Testing

Only two devices were used to test the tool: [Aeotec Z-Stick Gen5][3] controller
and [Aeotec Smart Switch 6][4]. In theory it should work with other devices, as
long as they are supported by OpenZWave. YMMV.

[3]: https://aeotec.com/z-wave-usb-stick
[4]: https://aeotec.com/z-wave-plug-in-switch
