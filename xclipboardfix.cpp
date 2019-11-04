/*

	xclipboardfix
	xfce-desktop/thunar - nautilus copy/paste fix
	Licensed under GPLv3 - https://www.gnu.org/licenses/gpl-3.0.txt
	(c)2019 mr.xkr https://mr.xkr.es/

	Required package:
		xclip
			apt-get install xclip

	Compiling:
		make

	Running:
		./xclipboardfix

	Command line examples of xclip:
		echo -e "copy\nfile:///data/desk/PAGI-master.zip\nfile:///data/desk/doc.pdf\0" | xclip -i -selection clipboard -t x-special/gnome-copied-files
		echo -e "cut\nfile:///data/desk/PAGI-master.zip\nfile:///data/desk/doc.pdf\0" | xclip -i -selection clipboard -t x-special/gnome-copied-files

*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <X11/Xlib.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <signal.h>

//#define DEBUG // uncomment this for debugging

using namespace std;

Display *display;
Window window;
string clipboard="";

/*
string urlDecode(string SRC) {
    string ret;
    char ch;
    int i, ii;
    for (i=0; i<SRC.length(); i++) {
        if (int(SRC[i])==37) {
            sscanf(SRC.substr(i+1,2).c_str(), "%x", &ii);
            ch=static_cast<char>(ii);
            ret+=ch;
            i=i+2;
        } else {
            ret+=SRC[i];
        }
    }
    return (ret);
}
*/

// replace all ocurrences in a string
void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if(from.empty()) return;
	size_t start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

// convert selection
Bool convertSelection(Display *display, Window window, const char *bufname, const char *fmtname) {

	char *result;
	unsigned long ressize, restail;
	int resbits;
	Atom bufid = XInternAtom(display, bufname, False),
	fmtid = XInternAtom(display, fmtname, False),
	propid = XInternAtom(display, "XSEL_DATA", False),
	incrid = XInternAtom(display, "INCR", False);
	XEvent event;

	XConvertSelection(display, bufid, fmtid, propid, window, CurrentTime);
	do {
		XNextEvent(display, &event);
	} while (event.type != SelectionNotify || event.xselection.selection != bufid);

	if (event.xselection.property) {

		XGetWindowProperty(display, window, propid, 0, LONG_MAX/4, False, AnyPropertyType,
		&fmtid, &resbits, &ressize, &restail, (unsigned char**)&result);

		if (fmtid == incrid)
			printf("Buffer is too large and INCR reading is not implemented yet.\n");
		else {
			clipboard=(string)result;

			// its a cutted/copied file by xfce (has file:// and has \r returns)
			std::size_t f1=clipboard.find("\nfile://");
			std::size_t f2=clipboard.find("\r");
			if (f1!=std::string::npos && f2!=std::string::npos) {

				// remove last \n
				clipboard=clipboard.substr(0, clipboard.length()-1);

				#ifdef DEBUG
					printf("OLD:%.*s\n", (int)ressize, result);
				#endif

				//clipboard=urlDecode(clipboard.substr(7));
				replaceAll(clipboard, "\r", ""); // remove all \r
				replaceAll(clipboard, "\n", "\\n"); // replace all \n with escape secuence

				//result=(char *)clipboard.c_str();
				//ressize=clipboard.length();
				//for (int i=0; i<ressize; i++) printf("[%d]", (int)result[i]);

				#ifdef DEBUG
					//printf("NEW:%.*s\n", (int)ressize, result);
				#endif

				// used xclip to replace XChangeProperty
				string cmd="bash -c 'echo -e \"";
				cmd+=clipboard;
				cmd+="\\0\" | xclip -i -selection clipboard -t x-special/gnome-copied-files'";
				#ifdef DEBUG
					printf("CLIPBOARD:%s\n\n", cmd.c_str());
				#endif
				system(cmd.c_str());

				// didnt work for me
				/*XChangeProperty(
					display,
					window,
					propid,
					8,
					8, // fmtid,
					PropModeReplace,
					(unsigned char *) result,
					(int) ressize
				);*/

			}

		}

		XFree(result);
		return True;

	}

	// request failed, e.g. owner can't convert to the target format
	return False;

}

// quit handler
void quit_handler(int s){
	XDestroyWindow(display, window);
	XCloseDisplay(display);
	exit(0);
}

// main app
int main() {

	// open display and window
	display = XOpenDisplay(NULL);
	unsigned long color = BlackPixel(display, DefaultScreen(display));
	window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0,0, 1,1, 0, color, color);

	// set quit handler
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = quit_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	// main loop (yes, it's not event oriented, but it works for me and I don't know about X11 event programming, that will be a big improvement... but this is a fix!)
	while (true) {
		Bool result = convertSelection(display, window, "CLIPBOARD", "x-special/gnome-copied-files"); // TARGETS UTF8_STRING STRING
		usleep(50000);
	}

	return 0;

}
