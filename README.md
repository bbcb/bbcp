BlackBox Component Builder for Windows, GNU/Linux, OpenBSD, FreeBSD

Tested on:
* Windows XP, 7, 10
* OpenBSD 6.2
* Fedora Core 17
* Ubuntu 14.04, 16.04 LTS
* Ubuntu GNOME 17.04, 17.10
* Arch Linux 4.9.6
* Debian 9.3 Xfce/Mate/GNOME
* Red Hat Enterprise Linux Server 6.3
* FreeBSD 11.0

Download: http://blackbox.obertone.ru/download

Directories structure:

	BlackBox/
		universal development environment for Component Pascal programing language

	dev/
		files for continuous integration system & technical notes for framework developers

	examples/
		append/  - append string to .odc file
		encoder/ - encoding subsystems by StdCoder from the command line
		server/  - static web-server

Installation of required packages in Ubuntu (should work also with other Debian-based OS):

	32-bit version:

		sudo apt-get install libgtk2.0-0 gtk2-engines gtk2-engines-murrine libcanberra-gtk-module gnome-icon-theme-full

	64-bit version:

		sudo dpkg --add-architecture i386
		sudo apt-get update
		sudo apt-get install libgtk2.0-0:i386 gtk2-engines:i386 gtk2-engines-murrine:i386 libcanberra-gtk-module:i386 gnome-icon-theme-full

To build BlackBox:

	cd BlackBox
	./switch-target `uname -s` GUI
	./build

To run BlackBox (after build):

	./run-BlackBox

for Linux version also you can run

	./blackbox

To create BlackBox assembly (after build):

	./export <outputDirectory>

To clean the BlackBox folder:

	./switch-target none
	./clean

Example: build, create assembly and run BlackBox from assembly:

	cd BlackBox
	./switch-target `uname -s` GUI
	./build
	./export ../`uname -s`_GUI
	./switch-target none
	./clean
	cd ../`uname -s`_GUI
	./run-BlackBox

Authors:
* Oberon microsystems AG
* BlackBox Framework Center
* Alexander V. Shiryaev
* Igor A. Dehtyarenko
* Ivan A. Denisov
