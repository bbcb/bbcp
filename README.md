BlackBox Component Builder for Windows, GNU/Linux, OpenBSD, FreeBSD

Tested on:
* Windows XP, 7, 10, 11
* OpenBSD 6.6
* Fedora Core 17
* Ubuntu 16.04/18.04/20.04 LTS
* Debian 9.3, 9.4, 10.4 Xfce/Mate/GNOME/Cinnamon
* Linux Mint 19.1/19.2/19.3/20 Cinnamon
* CentOS 8
* Arch Linux 4.9.6
* Manjaro Linux 18.0.4, 21.2.0
* Red Hat Enterprise Linux Server 6.3
* FreeBSD 12.1
* Alt Education 9.1

Download: https://blackbox.oberon.org/download


Installation of required packages in Debian-based systems:

	For Alt Education 9.1

		apt-get install i586-libgtk+2-devel.32bit

	Ubuntu 18/20 LTS, Mint 19/20:

		sudo dpkg --add-architecture i386
		sudo apt update
		sudo apt install libgtk2.0-0:i386 gtk2-engines:i386 gtk2-engines-murrine:i386 libcanberra-gtk-module:i386 gnome-themes-extra:i386

	Linux Mint 20 has no package 'gnome-themes-extra:i386', it can be safly ignored. There will be warning in console.

	Debian 9.X/10.X GNOME/Xfce/KDE

		sudo dpkg --add-architecture i386
		sudo apt update
		sudo apt install libgtk2.0-0:i386 gtk2-engines:i386 gtk2-engines-murrine:i386 libcanberra-gtk-module:i386 gtk2-engines-pixbuf:i386 libatk-adaptor:i386 libgail-common:i386 gnome-themes-standard:i386

	Arch-based systems amd64:

		sudo pacman -S multilib/lib32-gtk2
		sudo pacman -Rc lib32-librsvg

	In OpenBSD use 'wxallowed' flag in mount options for the partition to start BlackBox from.


To build BlackBox:

	./build-linux
	./build-windows

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

