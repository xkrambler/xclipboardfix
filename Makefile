all: xclipboardfix

xclipboardfix:
	gcc -o xclipboardfix xclipboardfix.cpp -lstdc++ -lX11
