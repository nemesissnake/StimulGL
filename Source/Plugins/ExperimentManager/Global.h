#ifndef GLWIDGET_GLOBAL_H
#define GLWIDGET_GLOBAL_H

enum ExperimentObjectState
{
	ExperimentObject_Initialized	= 0,
	ExperimentObject_Started		= 1,
	ExperimentObject_Stopped		= 2	
};

class GLWidget_Global
{
public:
	static QGLFormat getDefaultGLFormat()
	{
		QGLFormat StimulGLQGLFormat;
		StimulGLQGLFormat.setSwapInterval(1); // sync with vertical refresh
		StimulGLQGLFormat.setSampleBuffers(true);
		return StimulGLQGLFormat;
	}
};

#endif //GLWIDGET_GLOBAL_H