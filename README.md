BlackBox Component Builder for Windows, GNU/Linux, OpenBSD, FreeBSD

Tested on:
* Windows XP, 7, 10, 11
* Ubuntu 16.04/18.04/20.04 LTS
* Linux Mint 19.1/19.2/19.3/20 Cinnamon
* Debian 9.3, 9.4, 10.4 Xfce/Mate/GNOME/Cinnamon
* Alt Education 9.1
* OpenBSD 6.8, 7.2 i386
* Fedora 31
* CentOS 8
* Arch Linux 4.9.6
* Manjaro Linux 18.0.4, 21.2.0
* Red Hat Enterprise Linux Server 6.3
* FreeBSD 12.1, 13.0, 13.2 i386


Download: https://blackbox.oberon.org/download


Installation of required packages in Debian-based systems:

	For Alt Education 9.1

		apt-get install i586-libgtk+2-devel.32bit

	Ubuntu 20/22 LTS, Mint 20/22:

		sudo dpkg --add-architecture i386
		sudo apt update
		sudo apt install libgtk2.0-0:i386 gtk2-engines:i386 gtk2-engines-murrine:i386 libcanberra-gtk-module:i386

	Debian 9.X/10.X GNOME/Xfce/KDE

		sudo dpkg --add-architecture i386
		sudo apt update
		sudo apt install libgtk2.0-0:i386 gtk2-engines:i386 gtk2-engines-murrine:i386 libcanberra-gtk-module:i386 gtk2-engines-pixbuf:i386 libatk-adaptor:i386 libgail-common:i386 gnome-themes-standard:i386

	Arch-based systems amd64:

		sudo pacman -S multilib/lib32-gtk2
		sudo pacman -Rc lib32-librsvg

	Fedora:

		sudo dnf install gtk2.i686 gtk2-devel.i686


	In OpenBSD use 'wxallowed' flag in mount options for the partition to start BlackBox from.


To build BlackBox:

	./build-windows
	./build-linux
	./build-obsd

To run GUI:

	./blackbox

To run GUI in Ubuntu:

	./run-BlackBox

To clean reopsitory:

	./clean


Authors:
* Oberon microsystems AG
* BlackBox Framework Center
* OberonCore
* Alexander V. Shiryaev
* Igor A. Dehtyarenko
* Ivan A. Denisov
* Anton A. Dmitriev
* Ketmar Dark
