## SpinSat - The spinning cubesat

This repository contains the code for the spinsat project. The spinsat project is a satellite made to emulate how a reaction wheel works in space. This will be done with a bluetooth controller and a state machine inside the satellite.

### Introduction
The spinsat project is made in collaboration with the course EiT - TTK4852 Small satellites at NTNU. The main goal of this project is educating others on CubeSats.

The objective of this project is to create a satellite that hovers in the air (by a string connected to the ceiling) and spins around its own axis. The satellite will receive commands from a bluetooth controller and output telemetry data to a computer. The satellite will have different states that we can change between:

- Standby
- Locked
- Spinning

These states are described more in the state machine sections in our documents.

The satellite will be able to "understand" its own environments by different sensors.

The satellite will run Zephyr RTOS as the main operating system. The build system used is West.

### Requirements
- Python 3
- Zephyr RTOS
- Zephyr SDK
- Bluetooth controller
- Linux (please)

### Installation
Install Python 3
```
sudo apt-get install python3 (Ubuntu)

brew install python3 (MacOS)
```

Clone the repository
```
git clone --recurse-submodules -j8 https://github.com/sandviklee/spinsat.git
cd spinsat
```

Check that the zephyr sub-module is initialized (there should be content in the zephyr folder),
if not
```
git submodule update --recursive --init
```

Create and Source a Python environment
```
python3 -m venv .venv
source .venv/bin/activate
```

Install West
```
pip install west
```

Install python packages for Zephyr
```
pip install -r zephyr/scripts/requirements.txt
```

Update West
```
west update
```

### Installing the Zephyr SDK
Download the Zephyr SDK from entering the zephyr directory and running the following command
```
cd zephyr
west sdk install
```

### Building the project (MacOS)
```
rm -rf build
west build -t -s clean
west build -b qemu_x86 app/
```

### Building the project (Linux)
```
rm -rf build
west build -t -s clean
west build -b native_posix app/
```

### Building the project (Dev kit)
```
rm -rf build
west ...
```

### Running/Flashing the project
