# BlackBox Component Builder cross-platform

## Supported operating systems

List of supported operating systems:
* Windows
* GNU/Linux (amd64, i386)
* OpenBSD (i386)
* FreeBSD (i386)

Tested on:
* Windows XP, 7, 10, 11
* GNU/Linux:
  * Ubuntu 16.04/18.04/20.04 LTS
  * Debian 9.3, 9.4, 10.4 (Xfce/Mate/GNOME/Cinnamon)
  * Linux Mint 19.1/19.2/19.3/20 (Cinnamon)
  * Fedora 31
  * Red Hat Enterprise Linux Server 6.3
  * CentOS 8
  * Arch Linux 4.9.6
  * Manjaro Linux 18.0.4, 21.2.0
  * Alt Education 9.1
* OpenBSD 7.2 (i386)
* FreeBSD 12.1, 13.0, 13.2 (i386)

## Packages

Packages available [here](https://blackbox.oberon.org/download)

## Build

### Install dependencies

#### Ubuntu 20/22 LTS, Mint 20/22 (amd64)

	sudo dpkg --add-architecture i386
	sudo apt update
	sudo apt install libgtk2.0-0:i386 gtk2-engines:i386 gtk2-engines-murrine:i386 libcanberra-gtk-module:i386

#### Debian 9/10 (amd64)

	sudo dpkg --add-architecture i386
	sudo apt update
	sudo apt install libgtk2.0-0:i386 gtk2-engines:i386 gtk2-engines-murrine:i386 libcanberra-gtk-module:i386 gtk2-engines-pixbuf:i386 libatk-adaptor:i386 libgail-common:i386 gnome-themes-standard:i386

#### Arch-based systems (amd64)

	sudo pacman -S multilib/lib32-gtk2
	sudo pacman -Rc lib32-librsvg

#### Fedora (amd64)

	sudo dnf install gtk2.i686 gtk2-devel.i686

#### Alt Education 9.1 (amd64)

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

### BlackBox Component Builder (GUI)

#### On Windows

	BlackBox.exe

#### On other operating systems

	./run-BlackBox

Symbolic link to this script can be created to run from any directory:

	ln -s `readlink -f run-BlackBox` ~/bin/blackbox

And then BlackBox Component Builder (GUI) can be run with *blackbox* command from anywhere

### BlackBox Component Builder (command line interpreter)

#### On Windows

	BlackBoxInterp.exe

#### On other operating systems

	./run-BlackBoxInterp

Symbolic link to this script can be created to run from any directory:

	ln -s `readlink -f run-BlackBox` ~/bin/blackbox-cli

And then BlackBox Component Builder (command line interpreter) can be run with *blackbox-cli* command from anywhere

## Authors

* Oberon microsystems AG
* BlackBox Framework Center
* OberonCore
* Ivan Denisov
* Igor Dehtyarenko
* Anton Dmitriev
* Alexander Shiryaev
* Ketmar Dark
