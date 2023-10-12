# BlackBox Component Builder cross-platform

## Supported operating systems

List of supported operating systems:
* Windows
* GNU/Linux (amd64, i386)
* OpenBSD (i386)
* FreeBSD (i386)

Tested on:
* Windows XP, Windows 7, Windows 10, Windows 11
* GNU/Linux:
  * Ubuntu 16.04 LTS, 18.04 LTS, 20.04 LTS
  * Debian 9.3, 9.4 (Xfce/Mate/GNOME/Cinnamon)
  * Mint 20 (Cinnamon)
  * Fedora Core 17
  * Red Hat Enterprise Linux Server 6.3
  * CentOS 8
  * Arch Linux 4.9.6, Manjaro Linux 18.0.4
  * Alt Education 9.1
* OpenBSD 7.2 (i386)
* FreeBSD 12.1 (i386)

## Packages

Packages available [here](https://blackbox.oberon.org/download)

## Build

### Install dependencies

#### Ubuntu 18.04 LTS Bionic Beaver / Linux Mint 19.X (amd64)

	sudo dpkg --add-architecture i386
	sudo apt update
	sudo apt install libgtk2.0-0:i386 gtk2-engines:i386 gtk2-engines-murrine:i386 libcanberra-gtk-module:i386 gnome-themes-extra:i386

Linux Mint 20 has no package 'gnome-themes-extra:i386', it can be safly ignored. There will be warning in console.

#### Ubuntu 16.04 LTS (i386)

	sudo apt-get install libgtk2.0-0 gtk2-engines gtk2-engines-murrine libcanberra-gtk-module gnome-icon-theme-full

#### Ubuntu 16.04 LTS (amd64)

	sudo dpkg --add-architecture i386
	sudo apt-get update
	sudo apt-get install libgtk2.0-0:i386 gtk2-engines:i386 gtk2-engines-murrine:i386 libcanberra-gtk-module:i386 gnome-icon-theme-full

#### Debian 9 (amd64)

	sudo dpkg --add-architecture i386
	sudo apt update
	sudo apt install libgtk2.0-0:i386 gtk2-engines:i386 gtk2-engines-murrine:i386 libcanberra-gtk-module:i386 gtk2-engines-pixbuf:i386 libatk-adaptor:i386 libgail-common:i386 gnome-themes-standard:i386

#### Arch-based systems (amd64)

	sudo pacman -S multilib/lib32-gtk2

#### Alt Education 9 (amd64)

	apt-get install i586-libgtk+2-devel.32bit

#### OpenBSD (i386)

Use 'wxallowed' flag in mount options for the partition to build or start BlackBox from.

### Build

#### Build GNU/Linux version

	./build-linux

#### Build OpenBSD version

	./build-openbsd

#### Build FreeBSD version

	./build-freebsd

#### Build Windows version

	./build-windows

On Windows, these commands can be run from [MSYS2](https://www.msys2.org/)

There is also a *build-windows.bat* script that can be used to build the Windows version from Windows or Wine

## Install

	./export <outputDirectory>

## Run

### BlackBox Component Builder

On Windows:

	BlackBox.exe

On other operating systems:

	./run-BlackBox

### BlackBox Component Builder interpreter

On Windows:

	BlackBoxInterp.exe

On other operating systems:

	./run-BlackBoxInterp

## Authors

* Oberon microsystems AG
* BlackBox Framework Center
* Ivan Denisov
* Igor Dehtyarenko
* Anton Dmitriev
* Boris Rumshin
* Alexander Shiryaev
