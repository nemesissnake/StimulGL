#ifndef RETINOMAP_GLWIDGET_H
#define RETINOMAP_GLWIDGET_H

#include "GLWidgetWrapper.h"
#include <QPointF>

//Below defines must be all in lower case!
//Shared defines
#define GLWWRAP_WIDGET_DISPLAY_REFRESHRATE				"displayrefreshrate"
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
#define RETINOMAP_WIDGET_POS_LEFT						"left"
#define RETINOMAP_WIDGET_POS_RIGHT						"right"
#define RETINOMAP_WIDGET_POS_BOTH						"both"
#define RETINOMAP_WIDGET_GAP_DIAMETER					"gapdiameter"
#define RETINOMAP_WIDGET_FLICKRSPEED_HZ					"flickrfrequency"
#define RETINOMAP_WIDGET_WIDTH_PIXEL_AMOUNT				"stimuluswidthspan"
#define RETINOMAP_WIDGET_HEIGHT_PIXEL_AMOUNT			"stimulusheightspan"
#define RETINOMAP_WIDGET_CORTMAG_FACTOR					"corticalmagnitudefactor"
#define RETINOMAP_WIDGET_TRIGGERDURATION_MSEC			"internaltriggerduration"
#define RETINOMAP_WIDGET_DISCRETETRIGGERSTEPS			"discretetriggersteps"
#define RETINOMAP_WIDGET_RANDOMIZETRIGGERSTEPS			"randomizetriggersteps"
#define RETINOMAP_WIDGET_EMPTYTRIGGERSTEPS				"emptytriggersteps"
#define RETINOMAP_WIDGET_OUTPUTTRIGGERFRAME				"outputtriggerframe"
#define RETINOMAP_WIDGET_OUTPUTFRAMETYPE				"outputframetype"
#define RETINOMAP_WIDGET_OUTPUTTYPE_FRAME				"frame"
#define RETINOMAP_WIDGET_OUTPUTTYPE_MASK				"mask"
#define RETINOMAP_WIDGET_OUTPUTFRAMEFORMAT				"outputframeformat"
#define RETINOMAP_WIDGET_OUTPUTFORMAT_DAT				"dat"
#define RETINOMAP_WIDGET_OUTPUTFORMAT_PNG				"png"
#define RETINOMAP_WIDGET_ANTIALIASING					"antialiasing"
#define RETINOMAP_WIDGET_DISABLECORTMAGFAC				"disablecortmagfac"
#define RETINOMAP_WIDGET_CHECKERCOLOR1					"checkercolor1"
#define RETINOMAP_WIDGET_CHECKERCOLOR2					"checkercolor2"
#define RETINOMAP_WIDGET_BACKGROUNDCOLOR				"backgroundcolor"
#define RETINOMAP_WIDGET_FIXATIONCOLOR					"fixationcolor"
//PolarAngle specific defines
#define RETINOMAP_WIDGET_POLAR_RING_AMOUNT				"polarringamount"
#define RETINOMAP_WIDGET_POLAR_ROTATION_DIRECTION		"polarrotationdirection"
#define RETINOMAP_WIDGET_POLAR_WEDGE_SPAN				"polarwedgespan"
#define RETINOMAP_WIDGET_POLAR_CHECK_AMOUNT				"polarcheckamount"
//Eccentricity specific defines
#define RETINOMAP_WIDGET_ECCENTRICITY_DIRECTION			"eccentricitydirection"
#define RETINOMAP_WIDGET_ECCENTRICITY_CHECK_AMOUNT		"eccentricitycheckamount"
#define RETINOMAP_WIDGET_ECCENTRICITY_RING_AMOUNT		"eccentricityringamount"
#define RETINOMAP_WIDGET_ECCENTRICITY_ENABLE_CORTMAG_TIMING		"enablecortmagtiming"//Not documented!
//MovingBar specific defines
#define RETINOMAP_WIDGET_MOVINGBAR_ANGLE				"movingbarangle" 
#define RETINOMAP_WIDGET_MOVINGBAR_COVERAGE				"movingbarcoverage"
#define RETINOMAP_WIDGET_MOVINGBAR_HEIGHT				"movingbarheight"
#define RETINOMAP_WIDGET_MOVINGBAR_DIRECTION			"movingbardirection"
//#define RETINOMAP_WIDGET_MOVINGBAR_INCOPPDIR			"movingbarincludeoppositedirection"
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

enum RetinoMapHemifieldPos //The position within the hemifield
{
	RetinoMap_HemifieldPos_Left		= 0,
	RetinoMap_HemifieldPos_Right	= 1,
	RetinoMap_HemifieldPos_Both		= 2
};

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
	RetinoMap_LockedState	= 1,
	RetinoMap_PolarAngle	= 2,
	RetinoMap_Eccentricity	= 3,
	RetinoMap_MovingBar		= 4,
	RetinoMap_MovingDots	= 5
};

typedef struct{
	QList<QPointF> Pos;
	QList<QPointF> OldPos;
	QList<QLineF> Mov;
	QList<float> MirrorXPos;
} MovingDotsStructure;

class RetinoMap_glwidget : public GLWidgetWrapper
{
	Q_OBJECT

public:
	RetinoMap_glwidget(QWidget *parent = NULL);
	~RetinoMap_glwidget();
	RetinoMap_glwidget(const RetinoMap_glwidget& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro

public slots:
	bool startObject();
	bool initObject();
	bool setObjectConfiguration(QObject *pExpConfStruct = NULL);//ExperimentConfiguration *pExpConfStruct = NULL);
	bool setObjectID(int nObjID);
	bool initObjectBlockTrial();
	bool paintObject(int paintFlags = 0, QObject *paintEventObject = NULL);

private:
	void initialize();
	void parseExperimentObjectBlockParameters(bool bInit = false);
	void initializeMovingDotsStructures();
	QImage RetinoMap_glwidget::fractalFillCheckeredImage(float fWidth, float fHeigth, float fSize, int nflickr);

	int nRetinoID;								//This variable stores the ObjectID used to identify the object
	RetinoMapExperimentType currentExpType;		//The experiment type used, see RetinoMapExperimentType
	bool firstBlockTrialPaintFrame;				//To determine whether it's the first frame to paint from a new Block trial
	//QTime trialTime;							//The elapsed trial time
	//int elapsedTrialTime;						//The elapsed trial time(snapshot)
	QColor colorBackground;						//The color of the background
	QBrush brushBackground;						//The background brush
	QFont textFont;								//The font used for text(debug mode)
	QPen textPen;								//The pen used for text(debug mode)
	QColor whiteColor;							//The color used for the activation map (active)
	QColor blackColor;							//The color used for the activation map (inactive)
	QString textContent;						//Variable to store the text string
	QRectF rectScreenRes;						//The screen resolution
	float flickrThreshold;						//Variable for keeping track of the next flickr switch threshold time
	float flickrSpeedFreq;						//The flickr speed frequency
	int flickrSwitch;							//Variable to store the current flickr state
	int cycleTriggerAmount;						//The total block duration in amount of triggers
	int triggerDurationMsec;					//The duration of one trigger in mSec
	float stimWidthPixelAmount;					//The amount of visible stimuli pixels(height)
	float stimHeigthPixelAmount;				//The amount of visible stimuli pixels(width)
	QColor fixationColor;						//The color of the fixation dot/cross
	int fixationSize;							//The width of the fixation dot/cross
	bool showFixationPoint;						//Should we show an fixation point?
	bool outputTriggerFrame;					//Defines whether we should output(write to a png file) the first frame
	bool discreteTriggerSteps;					//Defines whether the stimuli should run smooth or in discrete steps according to each trigger received
	bool randomizeTriggerSteps;					//Defines whether the stimuli should randomize after each trigger received
	bool antiAliasing;							//Defines whether the anti-aliasing filter should be used for painting to the screen
	int lastTriggerNumber;						//To keep track of the last trigger number
	int emptyTriggerSteps;						//Defines the number of Trigger steps in which no stimuli should be presented
	bool bNoChangesSinceLastFrame;
	ExperimentSnapshotStructure expSnapshot;

	QColor dotColor;
	QColor cCheckerColor1;
	QColor cCheckerColor2;
	Qt::PenStyle style;
	Qt::PenCapStyle flatCap;
	Qt::PenCapStyle roundCap;
	QString tmpParamValue;
	float fStimulusDiameter;
	float fTrialTimeProgress;
	float fCortMagTrialTimeProgress;
	bool bCreateActivationMap;
	int currExpBlockTrialCycle;
	bool nextNewCycleEntered;
	bool nextNewBlockEntered;
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
	bool disableCortMagFac;
	bool enableCortMagTiming;
	int emptyTriggerLastIndex;
	int emptyTriggerStepCount;
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
	float fCalculatedCortMagFacTimingConst;

	//MovingBar
	float movingBarAngle;
	float movingBarCoverage;
	float movingBarHeight;
	int movingBarWidthCheckAmount;
	int movingBarHeightCheckAmount;
	int movingBarDirection;
	//bool movingBarIncludeOppositeDirection;

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
	bool movingDotsIsStationary;
	RetinoMapHemifieldPos movingDotsHemifieldPos;
	int movingDotsPixelFromCenter;
	int movingDotsFieldWidth;
	int movingDotsFieldHeight;
	QVector< QVector <float> > movingDotsXValue;
	QVector< QVector <float> > movingDotsYValue;
	QVector< QVector <float> > movingDotsXValueMirror;
	
	MovingDotsStructure movingDots;

	RetinoMapOutputType retinoOutputType;
	RetinoMapOutputFormat retinoOutputFormat;
	QPixmap *StimulusResultImageFrame;
	QPixmap *StimulusActivationMap;
	QPainter *activationPainter;
	QPainter *stimuliPainter;
	QPainter *imgPainter;
	ExperimentConfiguration *currExpConfStruct;
	RandomGenerator *randStimStateGenerator;		 //To hold the Stimuli Trigger Step
	RandomGenerator *randEmptyStimGenerator;		 //To hold the Empty Block Trials
	RandomGenerator *previousRandEmptyStimGenerator; //To hold the processed Empty Block Trials
};
Q_DECLARE_METATYPE(RetinoMap_glwidget)
#endif // RETINOMAP_GLWIDGET_H
