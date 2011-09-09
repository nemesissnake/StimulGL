#include "glthread.h"
#include "ThreadedGLWidgetWrapper.h"
#include <QtOpenGL>

GLThread::GLThread(ThreadedGLWidgetWrapper *glWidgetWrapper) : QThread(), glw(glWidgetWrapper)
{
	doRendering = true;
	doResize = false;
}

void GLThread::stop()
{
	doRendering = false;
}

void GLThread::resizeViewport(const QSize &size)
{
	w = size.width();
	h = size.height();
	doResize = true;
}    

void GLThread::run()
{
	srand(QTime::currentTime().msec());
	rotAngle = rand() % 360;

	glw->makeCurrent();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();        
	glOrtho(-5.0, 5.0, -5.0, 5.0, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, 200, 200);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	while (doRendering) {
		if (doResize) {
			glViewport(0, 0, w, h);
			doResize = false;
		}
		// Rendering code goes here
		glBegin(GL_QUADS);
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1,0,0);
		glBegin(GL_POLYGON);
		glVertex2f(0,0);
		glVertex2f(100,500);
		glVertex2f(500,100);
		glVertex2f(20, 200);
		glVertex2f(30, 200);
		glVertex2f(100, 100);
		glVertex2f(50, 60);
		glEnd();
		glw->swapBuffers();
		msleep(40);
	}

	//QPainter painter;
	//painter.begin(glw);
	//painter.setRenderHint(QPainter::Antialiasing);
	//painter.end();
}
