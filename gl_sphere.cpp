#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
//#include <X11/Xutil.h>
#include <X11/Xos.h>
//#include <X11/Xatom.h>

#include <GL/glu.h>
#include <GL/glx.h>

void drawSphere(Display *dpy, Window &win) {
  XWindowAttributes winattr;
  XGetWindowAttributes(dpy, win, &winattr);

  static GLint angle;
  static GLfloat pos;

  GLfloat theta, phi;
  const GLfloat angled = 0.05f;

  glViewport(0, 0, winattr.width, winattr.height);

  glClearColor(0.7f, 0.7f, 0.7f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(0.0f, 0.0f, 0.0f);

  glPushMatrix();
  glRotatef(angle, 0.5f, 1.0f, 0.0f);
  // glScalef(pos, pos, 0.0f);

    static int loopcount = 0;
  for (phi = -M_PI_2; phi < M_PI_2; phi += angled) {
    glBegin(GL_QUAD_STRIP);
    for (theta = 0.0f; theta <= (2.0f * M_PI )+angled; theta += angled) {
        if(loopcount++ % 2 == 0)
            glColor3f(abs(sinf(phi)), abs(cosf(phi)), abs(cosf(phi)));
      glVertex3f(cosf(theta) * cosf(phi), sinf(phi), sinf(theta) * cosf(phi));
      glVertex3f(cosf(theta + angled) * cosf(phi + angled), sinf(phi + angled),
                 sinf(theta + angled) * cosf(phi + angled));
    }
    glEnd();
  }

  glPopMatrix();

  glXSwapBuffers(dpy, win);

  angle += 1;
  if (angle >= 359)
    angle = 0;

  pos += 0.001f;
  if (pos >= 1.0f)
    pos = 0.0f;
}

void drawTriangle(Display *dpy, Window &win) {
  XWindowAttributes winattr;
  XGetWindowAttributes(dpy, win, &winattr);

  static GLint angle;
  static GLfloat pos;
  glViewport(0, 0, winattr.width, winattr.height);

  glClearColor(0.7f, 0.7f, 0.7f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // glColor3f(0.0f, 0.0f, 0.0f);

  glPushMatrix();
  glRotatef(angle, 0.5f, 1.0f, 0.0f);
  // glScalef(pos, pos, 0.0f);

  glBegin(GL_QUADS);
  glColor3f(0.0f, 0.9f, 1.0f);

  glVertex3f(0.5f, 0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glVertex3f(0.5f, -0.5f, 0.5f);

  glColor3f(0.0f, 0.0f, 1.0f);

  glVertex3f(0.5f, 0.5f, -0.5f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glVertex3f(0.5f, -0.5f, -0.5f);

  glColor3f(0.0f, 1.0f, 0.0f);

  glVertex3f(0.5f, 0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glVertex3f(0.5f, 0.5f, -0.5f);

  glEnd();
  glPopMatrix();

  glXSwapBuffers(dpy, win);

  angle += 1;
  if (angle >= 360)
    angle = 0;

  pos += 0.001f;
  if (pos >= 1.0f)
    pos = 0.0f;
}

int main(int argc, char **args) {
  Display *dpy;
  int screen;
  Window win, root_win;
  XEvent event;
  unsigned int depth;
  XSetWindowAttributes attrs;

  GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
  XVisualInfo *visual;
  GLXContext glc;

  dpy = XOpenDisplay(NULL);
  if (dpy == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  } else {
    fprintf(stderr, "connected to display\n");
  }

  screen = DefaultScreen(dpy);
  depth = DefaultDepth(dpy, screen);
  root_win = RootWindow(dpy, screen);

  visual = glXChooseVisual(dpy, screen, att);

  attrs.border_pixel = BlackPixel(dpy, screen);
  attrs.background_pixel = WhitePixel(dpy, screen);
  attrs.override_redirect = True;
  attrs.colormap = XCreateColormap(dpy, root_win, visual->visual, AllocNone);
  attrs.event_mask = ExposureMask | KeyPressMask;

  win = XCreateWindow(dpy, root_win, 200, 200, 500, 300, 0, visual->depth,
                      InputOutput, visual->visual,
                      CWBackPixel | CWColormap | CWBorderPixel | CWEventMask,
                      &attrs);

  XMapWindow(dpy, win);
  glc = glXCreateContext(dpy, visual, NULL, GL_TRUE);
  glXMakeCurrent(dpy, win, glc);

  glEnable(GL_DEPTH_TEST);

  while (1) {

    if (XPending(dpy) == 0) {
      // drawTriangle(dpy, win);
      drawSphere(dpy, win);
      usleep(5000);
      continue;
    }

    XNextEvent(dpy, &event);
    if (event.type == Expose) {
      fprintf(stderr, "Event: Expose\n");
    }
    if (event.type == KeyPress) {
      XDestroyWindow(dpy, win);
      XCloseDisplay(dpy);
      break;
    }
  }

  return 0;
}
