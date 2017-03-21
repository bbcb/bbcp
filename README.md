BlackBox Component Builder port for OpenBSD, GNU/Linux, FreeBSD

Repository includes version for Windows also.

Tested on:
* OpenBSD 6.0
* Fedora Core 17
* Ubuntu 14.04, 16.04 LTS
* FreeBSD 11.0

Directories structure:
	BlackBox/
		universal development environment based on BlackBox 1.7

	examples/
		encoder/ - encoding subsystems by StdCoder from the command line
		fpstest/ - loop and fonts test
		server/ - static single-thread web-server

Installation of required packages in Ubuntu (should work also with other Debian-based OS):

32-bit version:
	sudo apt-get install libgtk2.0-0 gtk2-engines gtk2-engines-murrine libcanberra-gtk-module
	sudo apt-get install libgnomeui-0 gnome-icon-theme-full

64-bit version:
	sudo dpkg --add-architecture i386
	sudo apt-get update
	sudo apt-get install libgtk2.0-0:i386 gtk2-engines:i386 gtk2-engines-murrine:i386 libcanberra-gtk-module:i386
	sudo apt-get install libgnomeui-0:i386 gnome-icon-theme-full

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
* [Oberon microsystems AG](http://www.oberon.ch/blackbox.html)
* [BlackBox Framework Center](http://blackboxframework.org)
* Alexander V. Shiryaev, Igor A. Dehtyarenko, Ivan A. Denisov

