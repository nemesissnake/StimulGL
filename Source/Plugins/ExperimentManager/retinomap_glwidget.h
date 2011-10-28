#ifndef RETINOMAP_GLWIDGET_H
#define RETINOMAP_GLWIDGET_H

#include "GLWidgetWrapper.h"
//#include "screenshot.h"

#define RETINOMAP_WIDGET_PATTERN_POLARANGLE			"polarangle"
#define RETINOMAP_WIDGET_PATTERN_ECCENTRICITY		"eccentricity"
#define RETINOMAP_WIDGET_PATTERN_MOVINGBAR			"movingbar"
#define RETINOMAP_WIDGET_PATTERN_FIXATION			"fixation"
#define RETINOMAP_WIDGET_PATTERN					"Retino pattern"
#define RETINOMAP_WIDGET_CYCLEDURATION_TR			"duration of one cycle in TR"
#define RETINOMAP_WIDGET_WEDGESPAN_DEGREES			"wedge span in degrees"
#define RETINOMAP_WIDGET_CHECK_AMOUNT				"nr of checks"
#define RETINOMAP_WIDGET_RING_AMOUNT				"nr of rings"
#define RETINOMAP_WIDGET_ROTATION_DIRECTION			"rotation direction"
#define RETINOMAP_WIDGET_FLICKRSPEED_HZ				"flickr speed in Hz"
#define RETINOMAP_WIDGET_XPIXEL_AMOUNT				"stimulus pixel no x"
#define RETINOMAP_WIDGET_YPIXEL_AMOUNT				"stimulus pixel no y"
#define RETINOMAP_WIDGET_CORTMAG_FACTOR				"cortical mag factor"
#define RETINOMAP_WIDGET_TRDURATION_MSEC			"TR duration in msec"
#define RETINOMAP_WIDGET_SHOWFIXPOINT				"show fixation point"
#define RETINOMAP_WIDGET_MOVINGBAR_ANGLE			"angle"
#define RETINOMAP_WIDGET_MOVINGBAR_COVERAGE			"coverage"
#define RETINOMAP_WIDGET_BOOL_TRUE					"true"
#define RETINOMAP_WIDGET_BOOL_FALSE					"false"
#define RETINOMAP_WIDGET_GAP_DIAMETER				"gap diameter"
#define RETINOMAP_WIDGET_OUTPUTTRIGGERFRAME			"output trigger frame"

enum RetinoMapExperimentType //The state of the main experiment object
{
	RetinoMap_Fixation		= 0,
	RetinoMap_PolarAngle	= 1,
	RetinoMap_Eccentricity	= 2,
	RetinoMap_MovingBar		= 3
};

class RetinoMap_glwidget : public GLWidgetWrapper
{
	Q_OBJECT

signals:
	void LogToExperimentManager(const QString &strText2Write);

public:
	RetinoMap_glwidget(QWidget *parent = NULL);
	~RetinoMap_glwidget();
	RetinoMap_glwidget(const RetinoMap_glwidget& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro

public slots:
	bool startExperimentObject();
	//bool abortExperimentObject();
	//bool stopExperimentObject();
	//bool setBlockTrialDomNodeList(QDomNodeList *pDomNodeList = NULL);
	//bool setExperimentConfiguration(ExperimentConfiguration *pExpConfStruct = NULL);
	bool setExperimentObjectID(int nObjID);

protected:
	//void init();
	void initBlockTrial();
	//bool loadBlockTrial();
	void paintEvent(QPaintEvent *event);

private:
	void initialize();

	RetinoMapExperimentType currentExpType;		//The experiment type used, see RetinoMapExperimentType
	bool firstBlockTrialPaintFrame;				//To determine whether it's the first frame to paint from a new Block trial
	QTime trialTime;							//The elapsed trial time
	int elapsedTrialTime;						//The elapsed trial time(snapshot)
	QColor colorBackground;						//The color of the background
	QBrush brushBackground;						//The background brush
	QFont textFont;								//The font used for text(debug mode)
	QPen textPen;								//The pen used for text(debug mode)
	QString textContent;						//Variable to store the text string
	QRectF rectScreenRes;						//The screen resolution
	int flickrThreshold;						//Variable for keeping track of the next flickr switch threshold time
	int flickrSpeedFreq;						//The flickr speed frequency
	int flickrSwitch;							//Variable to store the current flickr state
	float blockDurationInTriggers;				//The total block duration in amount of triggers
	int triggerDuration;						//The duration of one trigger in mSec
	float stimWidthPixelAmount;					//The amount of visible stimuli pixels(height)
	float totalWedgeScreenSpanSize;				//The amount of visible stimuli pixels(width)
	QColor fixationColor;						//The color of the fixation dot/cross
	int fixationWidth;							//The width of the fixation dot/cross
	bool showFixationPoint;						//Should we show an fixation point?
	bool outputTriggerFrame;					//Defines whether we should output(write to a png file) the first frame
	int nRetinoID;								//This variable stores the ObjectID used to identify the object
	int lastTriggerNumber;						//To keep track of the last trigger number
	QTime debugTime;							//For debugging purpose...
	int debugElapsedTime;						//For debugging purpose...
	int debugTestSamples;						//For debugging purpose...
	int debugUsedTestSamples;					//For debugging purpose...
	int debugTotalElapsedTime;					//For debugging purpose...
	int debugInitBlockTrialTime;				//For debugging purpose...
	QString debugString;						//For debugging purpose...

	float totalWedgeDeg;
	int wedgeNrChecks;
	int wedgeNrRings;
	int rotationDirection;
	float cortMagFactor;
	int centerGapWidth;
	float wedgeSpanAngle;
	float currentWedgeDiameter;
	float currentCompleteWedgeDiameter;
	float currentSize;
	float currentXPoint;
	float currentYPoint;
	float currentStartAngle;
	float fAngle;
	float fScreenCoverageRange;

	QPixmap *StimulusResultImageFrame;//QImage *StimulusResultImageFrame;

	//Screenshot *screenShotWidget;	
};
Q_DECLARE_METATYPE(RetinoMap_glwidget)
#endif // RETINOMAP_GLWIDGET_H
