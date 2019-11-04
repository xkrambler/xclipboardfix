# xclipboardfix

xfce-desktop/thunar - nautilus copy/paste fix
Licensed under GPLv3 - https://www.gnu.org/licenses/gpl-3.0.txt
(c)2019 mr.xkr https://mr.xkr.es/

This application will fix the error copying or cutting files from xfce-desktop or thunar to nautilus file managers.

Its based to change the format of x-special/gnome-copied-files that badly generates xfce applications, removing 0x0A characters (\r\n to \n) and terminating it always in \0 (another bad thing nautilus do).

You need to run it as a service for current user.

## Required package
	xclip
		apt-get install xclip

## Compiling
	make

## Running
	./xclipboardfix

## Command line examples of xclip

This is not part of xclipboardfix, but if someone wants to cut or copy a file to clipboard from the command line, this is the right format using xclip application:

	echo -e "copy\nfile:///data/desk/PAGI-master.zip\nfile:///data/desk/doc.pdf\0" | xclip -i -selection clipboard -t x-special/gnome-copied-files
	echo -e "cut\nfile:///data/desk/PAGI-master.zip\nfile:///data/desk/doc.pdf\0" | xclip -i -selection clipboard -t x-special/gnome-copied-files
