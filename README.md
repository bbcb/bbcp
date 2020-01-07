BlackBox Component Builder for Windows, GNU/Linux, OpenBSD, FreeBSD

Tested on:
* Windows XP, 7, 10
* OpenBSD 6.6
* Fedora Core 17
* Ubuntu 16.04 LTS, 18.04 LTS
* Debian 9.3, 9.4 Xfce/Mate/GNOME/Cinnamon
* Linux Mint 19.1/19.2/19.3 Cinnamon
* CentOS 8
* Arch Linux 4.9.6, Manjaro Linux 18.0.4
* Red Hat Enterprise Linux Server 6.3
* FreeBSD 11.0

Download: https://blackbox.oberon.org/download

Directories structure:

	BlackBox/
		universal development environment for Component Pascal programming language
	BlackBox/_*
		platform-specific files

	dev/
		files for continuous integration system & technical notes for framework developers

	examples/
		append/  - append string to .odc file
		encoder/ - encoding subsystems by StdCoder from the command line
		server/  - static web-server

Prepare BlackBox from repository on Windows:

	copy _Windows_, _Windows_GUI and __GUI content to the BlackBox directory
	remove other directories starting with "_"
	run build.bat


Installation of required packages in Debian-based systems:

	Ubuntu 18.04 LTS Bionic Beaver / Linux Mint 19.X:

		sudo dpkg --add-architecture i386
		sudo apt update
		sudo apt install libgtk2.0-0:i386 gtk2-engines:i386 gtk2-engines-murrine:i386 libcanberra-gtk-module:i386 gnome-themes-extra:i386

	Debian 9.X GNOME/Xfce/KDE
		sudo dpkg --add-architecture i386
		sudo apt update
		sudo apt install libgtk2.0-0:i386 gtk2-engines:i386 gtk2-engines-murrine:i386 libcanberra-gtk-module:i386 gtk2-engines-pixbuf:i386 libatk-adaptor:i386 libgail-common:i386 gnome-themes-standard:i386

	Ubuntu 16.04 LTS i386:

		sudo apt-get install libgtk2.0-0 gtk2-engines gtk2-engines-murrine libcanberra-gtk-module gnome-icon-theme-full

	Ubuntu 16.04 LTS amd64:

		sudo dpkg --add-architecture i386
		sudo apt-get update
		sudo apt-get install libgtk2.0-0:i386 gtk2-engines:i386 gtk2-engines-murrine:i386 libcanberra-gtk-module:i386 gnome-icon-theme-full


Installation of required packages in Arch-based systems:

	64-bit version:

		sudo pacman -S multilib/lib32-gtk2


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
* Anton A. Dmitriev
