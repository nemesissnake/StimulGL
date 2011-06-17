#ifndef RENDERWDG_H
#define RENDERWDG_H

#include <QGLWidget>
#include <QDialog>
#include <QTime>
#include <QPen>

class Retinotopic_Mapping_Dialog;
class Retinotopic_Mapping;
class QPaintEvent;
class ContainerDlg;
class GLWidget;
class QTimer;
class QTextStream;

class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLWidget(QGLFormat fmt, QWidget *parent, Retinotopic_Mapping *pParentControlObject);
	~GLWidget();

	void initBlockTrial();

	//Retinotopic_Mapping_Dialog *pMainDialog;
	Retinotopic_Mapping *pMainObject;
	QTime trialTime, frameTime;
	int MinUpdateTime;
	bool bForceToStop;

public slots:
	void onAnimate();

protected:
	void paintEvent(QPaintEvent *event);

private:
	QBrush background;
	QBrush circleBrush;
	QFont textFont;
	QPen circlePen;
	QPen textPen;
	QString textContent;

public:
	int elapsed;
    int frame_counter, ExpType, flickr_speed, flickr_switch;
    float xwidth, ywidth, xstart_rel, ystart_rel, startAngle, cycle_dur, TR;
    float x_size_stim, y_size_stim;
	QVector< QVector <float> > xvalue, yvalue;
	QString BlockTrialConditionType; // this identifies what to do with parameters
	QVector< QStringList > BlockTrialParamTable; // values per row are stored as strings (type conversion done either via type (see below) or via expectation)
	QStringList BlockTrialParamTypes;
};

#endif // RENDERWDG_H
