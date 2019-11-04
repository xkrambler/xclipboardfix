# xclipboardfix

xfce-desktop/thunar - nautilus copy/paste fix
Licensed under GPLv3 - https://www.gnu.org/licenses/gpl-3.0.txt
(c)2019 mr.xkr https://mr.xkr.es/

## Required package
	xclip
		apt-get install xclip

## Compiling
	make

## Running
	./xclipboardfix

## Command line examples of xclip
	echo -e "copy\nfile:///data/desk/PAGI-master.zip\nfile:///data/desk/doc.pdf\0" | xclip -i -selection clipboard -t x-special/gnome-copied-files
	echo -e "cut\nfile:///data/desk/PAGI-master.zip\nfile:///data/desk/doc.pdf\0" | xclip -i -selection clipboard -t x-special/gnome-copied-files
