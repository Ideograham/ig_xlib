#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/Xos.h>
//#include <X11/Xatom.h>

int
main (int argc, char **args)
{
	Display *dpy;
	int screen;
	Window win, root_win;
	XEvent event;

	unsigned int depth;
	XSetWindowAttributes attrs;

	printf("haven't crashed yet...\n");

	dpy = XOpenDisplay(NULL);

	if(dpy == NULL)
	{
		fprintf(stderr, "Cannot open display\n");
		exit(1);
	} else { fprintf(stderr, "connected to display\n"); }


	screen = DefaultScreen(dpy);
	depth = DefaultDepth(dpy, screen);
	root_win = RootWindow(dpy, screen);

	win = XCreateSimpleWindow(dpy, root_win, 
			100, 100, 500, 300,
			1, BlackPixel(dpy, screen), WhitePixel(dpy, screen));

	XSelectInput(dpy, win, ExposureMask | KeyPressMask);
	XMapWindow(dpy, win);

	while (1)
	{
		XNextEvent(dpy, &event);
	}

	return 0;
}
