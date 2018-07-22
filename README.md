zwave-switch
============

A tool for controlling Z-Wave switches.


Usage
-----

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


Examples
--------

Add a new node to the network:

    $ zwave-switch /dev/ttyACM0
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
    Listening...
    Switch 2 [ZW096 Smart Switch 6] is ON
    Switch 2 [ZW096 Smart Switch 6] is OFF

Send `SIGUSR1`/`SIGUSR2` to turn the switch on/off:

    $ pkill -SIGUSR1 zwave-switch
    $ pkill -SIGUSR2 zwave-switch

Get or set a configuration value (in this example the default state of the
switch after a power cycle):

    $ zwave-switch /dev/ttyACM0 config get 2 20
    Last status (Default)
    $ zwave-switch /dev/ttyACM0 config set 2 20 2
    Always off

Consult the `zwcfg_*.xml` file for possible configuration values. The file
is created automatically by OpenZWave. By default it is created in the
current directory. This can be controlled using the `--ozw-user-path` option.


Installation
------------

    git clone git@github.com:neuroid/zwave-switch.git
    cd zwave-switch
    git submodule init
    git submodule update
    mkdir build
    cd build
    cmake ..
    make

The tool relies heavily on [OpenZWave][1]. Make sure the library is installed
before trying to compile the tool. If OpenZWave is installed in a non-standard
location the `OPENZWAVE_PREFIX` variable might be useful:

    cmake -DOPENZWAVE_PREFIX=/opt/openzwave ..

[1]: https://github.com/OpenZWave/open-zwave


Testing
-------

Only two devices were used to test the tool: [Aeotec Z-Stick Gen5][2] controller
and [Aeotec Smart Switch 6][3]. In theory it should work with other devices, as
long as they are supported by OpenZWave. YMMV.

[2]: https://aeotec.com/z-wave-usb-stick
[3]: https://aeotec.com/z-wave-plug-in-switch
