#ifndef RETINOMAP_GLWIDGET_H
#define RETINOMAP_GLWIDGET_H

#include "GLWidgetWrapper.h"

class RetinoMap_glwidget : public GLWidgetWrapper
{
	Q_OBJECT

public:
	RetinoMap_glwidget(QWidget *parent = NULL);
	~RetinoMap_glwidget();
	RetinoMap_glwidget(const RetinoMap_glwidget& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro

public slots:
	bool start();
	//bool stop();

protected:
	//void init();
	void initBlockTrial();
	//bool loadBlockTrial();
	void paintEvent(QPaintEvent *event);

private:
	void initialize();

	QBrush background;
	//QBrush circleBrush;
	QFont textFont;
	//QPen circlePen;
	QPen textPen;
	QString textContent;

public:
	QTime trialTime;
	int flickr_speed;
	int flickr_switch;
	float xwidth;
	float ywidth;
	float xstart_rel;
	float ystart_rel;
	float startAngle;
	float cycle_dur;
	float TR;
	float x_size_stim;
	float y_size_stim;
	//QVector< QVector <float> > xvalue, yvalue;
	//QString BlockTrialConditionType; // this identifies what to do with parameters
	//QVector< QStringList > BlockTrialParamTable; // values per row are stored as strings (type conversion done either via type (see below) or via expectation)
	//QStringList BlockTrialParamTypes;

	//int m_nStep;
	//int m_nScreenPosOffsetX;
	//int m_nScreenPosOffsetY;
	//QBrush m_bBackground;
	//QFont m_fTextFont;
	//QPen m_pTextPen;
	//QString m_sTextContent;
};

Q_DECLARE_METATYPE(RetinoMap_glwidget)

#endif // RETINOMAP_GLWIDGET_H
