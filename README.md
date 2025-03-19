<h1 align="center" style="font-weight: bold;">SpinSat - EiT TTK4852</h1>

<p align="center">
 <a href="#intro">Introduction</a> •
 <a href="#started">Getting Started</a> •
 <a href="#third-party">Third Party Libraries/Drivers</a>
</p>

<p align="center">
    <b>The Spinning Cubesat</b>
    <p align="center">A small satellite that emulates reaction wheel functionality in space using bluetooth control and an internal state machine.</p>
</p>
---

<h2 id="intro">Introduction</h2>

We've designed a demonstration satellite that suspends from the ceiling and rotates on its axis, simulating key aspects of satellite operation. The system features:
	•	Bluetooth command reception and telemetry transmission
	•	Environmental awareness through onboard sensors
	•	A sophisticated state machine with three operational modes:
	▪	Standby: Awaiting commands
	▪	Locked: Fixed orientation
	▪	Spinning: Active rotation
The satellite's architecture is built on Zephyr RTOS, utilizing the West build system for efficient development and deployment.
Detailed documentation on the state machine implementation and system architecture is available in our project documentation.

This repository is versioned together with the [Zephyr main tree][zephyr]. This
means that every time that Zephyr is tagged, this repository is tagged as well
with the same version number, and the [manifest](west.yml) entry for `zephyr`
will point to the corresponding Zephyr tag. For example, the `example-application`
v2.6.0 will point to Zephyr v2.6.0. Note that the `main` branch always
points to the development branch of Zephyr, also `main`.


<h2 id="started">Getting Started</h2>

Before getting started, make sure you have a proper Zephyr development
environment. Follow the official
[Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html).

<h3 id="pre">Prerequisites</h3>

- Python 3 <
- Zephyr SDK
- West Build Tool
- Linux/Mac (please)

<h3 id="pre">Setup</h3>

To get started with this project, you will need to follow the Zephyr getting started guide, as you will need to have west installed.
You can read more about the west manifest [here](https://docs.zephyrproject.org/latest/develop/west/manifest.html)

```shell
west init -m https://github.com/sandviklee/spinsat --mr main spinsat
cd spinsat
west update
```

<h3>Running</h3>

```shell
cd example-application
west build -b $BOARD app
```

where `$BOARD` is the target board.

You can use *qemu_x86* as a board to run the application (in emulation) in a QEMU environment.

A sample debug configuration is also provided. To apply it, run the following
command:

```shell
west build -b $BOARD app -- -DEXTRA_CONF_FILE=debug.conf
```

Once you have built the application, run the following command to flash it:

```shell
west flash
```

### Testing

To execute Twister integration tests, run the following command:

```shell
west twister -T tests --integration
```

Remember to source the zephyr environment script before running this!

### Documentation

A minimal documentation setup is provided for Doxygen and Sphinx. To build the
documentation first change to the ``doc`` folder:

```shell
cd doc
```

Before continuing, check if you have Doxygen installed. It is recommended to
use the same Doxygen version used in [CI](.github/workflows/docs.yml). To
install Sphinx, make sure you have a Python installation in place and run:

```shell
pip install -r requirements.txt
```

API documentation (Doxygen) can be built using the following command:

```shell
doxygen
```

The output will be stored in the ``_build_doxygen`` folder. Similarly, the
Sphinx documentation (HTML) can be built using the following command:

```shell
make html
```

The output will be stored in the ``_build_sphinx`` folder. You may check for
other output formats other than HTML by running ``make help``.

<h2 id="third-party">Third party Libraries/Drivers</h2>

Adding third party libraries or drivers to the project is quite easy.

<h3 id="lib">Adding a library</h3>

To add a library, you need to add a directory into the 'lib' folder.
Then you add the configuration files, Kconfig and CMakeLists.txt and the source files to the library folder.

- The Kconfig file shall include everything that is needed to configure the library. For example the enable flag and other configuration options.
- The CMakeLists.txt file shall include the library source files and the configuration file.
```txt
# CMakeLists.txt

zephyr_library()
zephyr_library_sources(<lib_name>.c)
```

Then you need to add it to the main CMakeLists.txt file in the 'lib' folder.

```txt
# CMakeLists.txt

...

add_subdirectory_ifdef(CONFIG_<FLAG> <lib_name>)
```

At last you configure the header file in the /include/app/lib/<lib_name>.h file.

To enable this library in the project, you need to add the configuration flag to the prj.conf file in the app folder.

```txt
CONFIG_<FLAG>=y
```

<h3 id="driv">Adding a Driver</h3>

The same as adding a library, you need to add a directory into the 'drivers' folder.
