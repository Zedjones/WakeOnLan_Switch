# WoL_Switch ![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-blue.svg)

A very basic [Wake-on-LAN](https://en.wikipedia.org/wiki/Wake-on-LAN) client for the Nintendo Switch written using [libnx](https://github.com/switchbrew/libnx). 

## Current Features

* Sending Wake-on-LAN magic packets with a hardcoded MAC address

## Planned Features

* Configurable broadcast address and MAC address through a file 
* Wake-on-WAN functionality (wake across networks)
* An icon for the application

## Potential Features 

* Input directly in the application
* A GUI 

## Getting Started

Make sure you have [DevKitA64](https://devkitpro.org/) installed. Installations can be found on their website.
For Unix-like platforms, it is recommended to use [Pacman](https://github.com/devkitPro/pacman) for installing the
required tools.

In addition, make sure you have [libnx](https://github.com/switchbrew/libnx) installed. 

### Running the app

Run `make` inside this folder. Put the `.nro` file you just created on your SD card and launch it.

## Author

Nicholas Jones
