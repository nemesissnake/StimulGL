#ifndef RETINOMAP_GLWIDGET_H
#define RETINOMAP_GLWIDGET_H

#include "GLWidgetWrapper.h"

//Below defines must be all in lower case!
//Shared defines
#define RETINOMAP_WIDGET_PATTERN_POLARANGLE				"polarangle"
#define RETINOMAP_WIDGET_PATTERN_ECCENTRICITY			"eccentricity"
#define RETINOMAP_WIDGET_PATTERN_MOVINGBAR				"movingbar"
#define RETINOMAP_WIDGET_PATTERN_MOVINGDOTS				"movingdots"
#define RETINOMAP_WIDGET_PATTERN_FIXATION				"fixation"
#define RETINOMAP_WIDGET_PATTERN						"retinopattern"
#define RETINOMAP_WIDGET_CYCLE_TRIGGER_AMOUNT			"cycletriggeramount"
#define RETINOMAP_WIDGET_SHOWFIXPOINT					"showfixpoint"
#define RETINOMAP_WIDGET_FIXSIZE						"fixationsize"
#define RETINOMAP_WIDGET_BOOL_TRUE						"true"
#define RETINOMAP_WIDGET_BOOL_FALSE						"false"
#define RETINOMAP_WIDGET_GAP_DIAMETER					"gapdiameter"
#define RETINOMAP_WIDGET_FLICKRSPEED_HZ					"flickrfrequency"
#define RETINOMAP_WIDGET_WIDTH_PIXEL_AMOUNT				"stimuluswidthspan"
#define RETINOMAP_WIDGET_HEIGHT_PIXEL_AMOUNT			"stimulusheightspan"
#define RETINOMAP_WIDGET_CORTMAG_FACTOR					"corticalmagnitudefactor"
#define RETINOMAP_WIDGET_TRIGGERDURATION_MSEC			"triggerduration"
#define RETINOMAP_WIDGET_DISCRETETRIGGERSTEPS			"discretetriggersteps"
#define RETINOMAP_WIDGET_OUTPUTTRIGGERFRAME				"outputtriggerframe"
#define RETINOMAP_WIDGET_OUTPUTFRAMETYPE				"outputframetype"
#define RETINOMAP_WIDGET_OUTPUTTYPE_FRAME				"frame"
#define RETINOMAP_WIDGET_OUTPUTTYPE_MASK				"mask"
#define RETINOMAP_WIDGET_OUTPUTFRAMEFORMAT				"outputframeformat"
#define RETINOMAP_WIDGET_OUTPUTFORMAT_DAT				"dat"
#define RETINOMAP_WIDGET_OUTPUTFORMAT_PNG				"png"
#define RETINOMAP_WIDGET_ANTIALIASING					"antialiasing"
//PolarAngle specific defines
#define RETINOMAP_WIDGET_POLAR_RING_AMOUNT				"polarringamount"
#define RETINOMAP_WIDGET_POLAR_ROTATION_DIRECTION		"polarrotationdirection"
#define RETINOMAP_WIDGET_POLAR_WEDGE_SPAN				"polarwedgespan"
#define RETINOMAP_WIDGET_POLAR_CHECK_AMOUNT				"polarcheckamount"
//Eccentricity specific defines
#define RETINOMAP_WIDGET_ECCENTRICITY_DIRECTION			"eccentricitydirection"
#define RETINOMAP_WIDGET_ECCENTRICITY_CHECK_AMOUNT		"eccentricitycheckamount"
#define RETINOMAP_WIDGET_ECCENTRICITY_RING_AMOUNT		"eccentricityringamount"
//MovingBar specific defines
#define RETINOMAP_WIDGET_MOVINGBAR_ANGLE				"movingbarangle"
#define RETINOMAP_WIDGET_MOVINGBAR_COVERAGE				"movingbarcoverage"
#define RETINOMAP_WIDGET_MOVINGBAR_DIRECTION			"movingbardirection"
#define RETINOMAP_WIDGET_MOVINGBAR_WIDTH_CHECK_AMOUNT	"movingbarwidthcheckamount"
#define RETINOMAP_WIDGET_MOVINGBAR_HEIGTH_CHECK_AMOUNT	"movingbarheightcheckamount"
//MovingDots specific defines
#define	RETINOMAP_WIDGET_MOVINGDOTS_MOVESPEED			"movingdotsmovespeed"
#define RETINOMAP_WIDGET_MOVINGDOTS_NROFDOTS			"movingdotsnrofdots"
#define RETINOMAP_WIDGET_MOVINGDOTS_DOTSIZE				"movingdotsdotsize"
#define RETINOMAP_WIDGET_MOVINGDOTS_HEMIFIELD			"movingdotshemifield"
#define RETINOMAP_WIDGET_MOVINGDOTS_PIXELFROMCENTER		"movingdotspixelfromcenter"
#define RETINOMAP_WIDGET_MOVINGDOTS_STATIONAIRY			"movingdotsstationairy"
//#define RETINOMAP_WIDGET_MOVINGDOTS_RETINALPOSITION	"movingdotsretinalposition"
#define RETINOMAP_WIDGET_MOVINGDOTS_FIELDWIDTH			"movingdotsfieldwidth"
#define RETINOMAP_WIDGET_MOVINGDOTS_FIELDHEIGHT			"movingdotsfieldheight"

#define RETINOMAP_WIDGET_OUTPUT_SUBFOLDER				"/RetinoWidget/"

enum RetinoMapOutputType //The output type
{
	RetinoMap_OutputType_Frame	= 0,
	RetinoMap_OutputType_Mask	= 1
};

enum RetinoMapOutputFormat //The output format
{
	RetinoMap_OutputFormat_PNG	= 0,
	RetinoMap_OutputFormat_DAT	= 1
};

enum RetinoMapExperimentType //The state of the main experiment object
{
	RetinoMap_Fixation		= 0,
	RetinoMap_PolarAngle	= 1,
	RetinoMap_Eccentricity	= 2,
	RetinoMap_MovingBar		= 3,
	RetinoMap_MovingDots	= 4
};

class RetinoMap_glwidget : public GLWidgetWrapper
{
	Q_OBJECT

public:
	RetinoMap_glwidget(QWidget *parent = NULL);
	~RetinoMap_glwidget();
	RetinoMap_glwidget(const RetinoMap_glwidget& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro

public slots:
	bool startExperimentObject();
	bool initExperimentObject();
	//bool abortExperimentObject();
	//bool stopExperimentObject();
	//bool setBlockTrialDomNodeList(QDomNodeList *pDomNodeList = NULL);
	bool setExperimentConfiguration(ExperimentConfiguration *pExpConfStruct = NULL);
	bool setExperimentObjectID(int nObjID);

protected:
	void initBlockTrial();
	void paintEvent(QPaintEvent *event);

private:
	void initialize();
	void parseExperimentObjectBlockParameters(bool bInit = false);
	void initializeMovingDotsStructures();

	int nRetinoID;								//This variable stores the ObjectID used to identify the object
	RetinoMapExperimentType currentExpType;		//The experiment type used, see RetinoMapExperimentType
	bool firstBlockTrialPaintFrame;				//To determine whether it's the first frame to paint from a new Block trial
	QTime trialTime;							//The elapsed trial time
	int elapsedTrialTime;						//The elapsed trial time(snapshot)
	QColor colorBackground;						//The color of the background
	QBrush brushBackground;						//The background brush
	QFont textFont;								//The font used for text(debug mode)
	QPen textPen;								//The pen used for text(debug mode)
	QColor whiteColor;							//The color used for the activation map (active)
	QColor blackColor;							//The color used for the activation map (inactive)
	QString textContent;						//Variable to store the text string
	QRectF rectScreenRes;						//The screen resolution
	int flickrThreshold;						//Variable for keeping track of the next flickr switch threshold time
	int flickrSpeedFreq;						//The flickr speed frequency
	int flickrSwitch;							//Variable to store the current flickr state
	float cycleTriggerAmount;					//The total block duration in amount of triggers
	int triggerDurationMsec;					//The duration of one trigger in mSec
	float stimWidthPixelAmount;					//The amount of visible stimuli pixels(height)
	float stimHeigthPixelAmount;				//The amount of visible stimuli pixels(width)
	QColor fixationColor;						//The color of the fixation dot/cross
	int fixationSize;							//The width of the fixation dot/cross
	bool showFixationPoint;						//Should we show an fixation point?
	bool outputTriggerFrame;					//Defines whether we should output(write to a png file) the first frame
	bool discreteTriggerSteps;					//Defines whether the stimuli should run smooth or in discrete steps according to each trigger received
	bool antiAliasing;							//Defines whether the anti-aliasing filter should be used for painting to the screen
	int lastTriggerNumber;						//To keep track of the last trigger number

	QColor color1;
	QColor color2;
	Qt::PenStyle style;
	Qt::PenCapStyle flatCap;
	Qt::PenCapStyle roundCap;
	QString tmpParamValue;
	float fStimulusDiameter;
	float fTrialTimeProgress; 
	bool bCreateActivationMap;
	int currExpTrigger;
	int currExpBlockTrialTrigger;
	int currExpBlockTrialFrame;
	int currExpTrial;
	int currExpBlock;
	float cortMagFactor;
	int gapDiameter;
	float wedgeSpanAngle;
	float currentWedgeDiameter;
	float currentCompleteWedgeDiameter;
	float currentSize;
	float currentOuterCompleteRingDiameter;
	float currentXPoint;
	float currentYPoint;
	float currentStartAngle;
	//QTime debugTime;							//For debugging purpose...
	//int debugElapsedTime;						//For debugging purpose...
	//int debugTestSamples;						//For debugging purpose...
	//int debugUsedTestSamples;					//For debugging purpose...
	//int debugTotalElapsedTime;				//For debugging purpose...
	//int debugInitBlockTrialTime;				//For debugging purpose...
	//QString debugString;						//For debugging purpose...

	//Polar
	float polarWedgeSpan;
	int polarWedgeNrChecks;
	int polarWedgeNrRings;
	int polarRotationDirection;

	//Eccentricity
	int eccentricityNrChecks;
	int eccentricityNrRings;
	int eccentricityDirection;

	//MovingBar
	float movingBarAngle;
	float movingBarCoverage;
	int movingBarWidthCheckAmount;
	int movingBarHeightCheckAmount;
	int movingBarDirection;

	//MovingDots
	float movingDotsMoveSpeed;
	QVector<float> movingDotsBGXDir;
	QVector<float> movingDotsBGYDir;
	int movingDotsDotSize;
	int movingDotsXWidth;
	int movingDotsYWidth;
	int movingDotsXStartRel;
	int movingDotsYStartRel;
	int movingDotsNrOfDots;
	int movingDotsIsStationary;
	int movingDotsHemifield;
	int movingDotsPixelFromCenter;
	int movingDotsFieldWidth;
	int movingDotsFieldHeight;
	QVector< QVector <float> > movingDotsXValue;
	QVector< QVector <float> > movingDotsYValue;
	QVector< QVector <float> > movingDotsXValueMirror;
	//int movingDotsRetPosition;

	RetinoMapOutputType retinoOutputType;
	RetinoMapOutputFormat retinoOutputFormat;
	QPixmap *StimulusResultImageFrame;
	QPixmap *StimulusActivationMap;
	QPainter *activationPainter;
	ExperimentConfiguration *currExpConfStruct;
	RandomGenerator *randGen;
};
Q_DECLARE_METATYPE(RetinoMap_glwidget)
#endif // RETINOMAP_GLWIDGET_H
