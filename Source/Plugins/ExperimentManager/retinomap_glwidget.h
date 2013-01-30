//ExperimentManagerplugin
//Copyright (C) 2013  Sven Gijsen
//
//This file is part of StimulGL.
//StimulGL is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef RETINOMAP_GLWIDGET_H
#define RETINOMAP_GLWIDGET_H

#include "GLWidgetWrapper.h"
#include <QPointF>
#include <QScriptEngine>

//Below defines must be all in lower case!

/*!
 * \defgroup RetinoUniversalParamDefs Retinotopic (Universal) parameter Names
 * @{
 */
#define RETINOMAP_WIDGET_PATTERN_POLARANGLE					"polarangle"					/*!< "polarangle" */
#define RETINOMAP_WIDGET_PATTERN_ECCENTRICITY				"eccentricity"					/*!< "eccentricity" */
#define RETINOMAP_WIDGET_PATTERN_MOVINGBAR					"movingbar"						/*!< "movingbar" */
#define RETINOMAP_WIDGET_PATTERN_MOVINGDOTS					"movingdots"					/*!< "movingdots" */
#define RETINOMAP_WIDGET_PATTERN_FIXATION					"fixation"						/*!< "fixation" */
#define RETINOMAP_WIDGET_PATTERN_CUSTOM						"custom"						/*!< "custom" */
#define RETINOMAP_WIDGET_PATTERN							"retinopattern"					/*!< "retinopattern" */
#define RETINOMAP_WIDGET_CYCLE_TRIGGER_AMOUNT				"cycletriggeramount"			/*!< "cycletriggeramount" */
#define RETINOMAP_WIDGET_SHOWFIXPOINT						"showfixpoint"					/*!< "showfixpoint" */
#define RETINOMAP_WIDGET_FIXSIZE							"fixationsize"					/*!< "fixationsize" */
#define RETINOMAP_WIDGET_POS_LEFT							"left"							/*!< "left" */
#define RETINOMAP_WIDGET_POS_RIGHT							"right"							/*!< "right" */
#define RETINOMAP_WIDGET_POS_BOTH							"both"							/*!< "both" */
#define RETINOMAP_WIDGET_GAP_DIAMETER						"gapdiameter"					/*!< "gapdiameter" */
#define RETINOMAP_WIDGET_FLICKRSPEED_HZ						"flickrfrequency"				/*!< "flickrfrequency" */
#define RETINOMAP_WIDGET_CORTMAG_FACTOR						"corticalmagnitudefactor"		/*!< "corticalmagnitudefactor" */
#define RETINOMAP_WIDGET_TRIGGERDURATION_MSEC				"internaltriggerduration"		/*!< "internaltriggerduration" */
#define RETINOMAP_WIDGET_DISCRETETRIGGERSTEPS				"discretetriggersteps"			/*!< "discretetriggersteps" */
#define RETINOMAP_WIDGET_RANDOMIZETRIGGERSTEPS				"randomizetriggersteps"			/*!< "randomizetriggersteps" */
#define RETINOMAP_WIDGET_EMPTYTRIGGERSTEPS					"emptytriggersteps"				/*!< "emptytriggersteps" */
#define RETINOMAP_WIDGET_OUTPUTTRIGGERFRAME					"outputtriggerframe"			/*!< "outputtriggerframe" */
#define RETINOMAP_WIDGET_OUTPUTFRAMETYPE					"outputframetype"				/*!< "outputframetype" */
#define RETINOMAP_WIDGET_OUTPUTTYPE_FRAME					"frame"							/*!< "frame" */
#define RETINOMAP_WIDGET_OUTPUTTYPE_MASK					"mask"							/*!< "mask" */
#define RETINOMAP_WIDGET_OUTPUTFRAMEFORMAT					"outputframeformat"				/*!< "outputframeformat" */
#define RETINOMAP_WIDGET_OUTPUTFORMAT_DAT					"dat"							/*!< "dat" */
#define RETINOMAP_WIDGET_OUTPUTFORMAT_PNG					"png"							/*!< "png" */
#define RETINOMAP_WIDGET_ANTIALIASING						"antialiasing"					/*!< "antialiasing" */
#define RETINOMAP_WIDGET_DISABLECORTMAGFAC					"disablecortmagfac"				/*!< "disablecortmagfac" */
#define RETINOMAP_WIDGET_CHECKERCOLOR1						"checkercolor1"					/*!< "checkercolor1" */
#define RETINOMAP_WIDGET_CHECKERCOLOR2						"checkercolor2"					/*!< "checkercolor2" */
#define RETINOMAP_WIDGET_FIXATIONCOLOR						"fixationcolor"					/*!< "fixationcolor" */
/**@}*/
/*!
 * \defgroup RetinoPolarAngleParamDefs Retinotopic (PolarAngle) parameter Names
 * @{
 */
#define RETINOMAP_WIDGET_POLAR_RING_AMOUNT					"polarringamount"				/*!< "polarringamount" */
#define RETINOMAP_WIDGET_POLAR_ROTATION_DIRECTION			"polarrotationdirection"		/*!< "polarrotationdirection" */
#define RETINOMAP_WIDGET_POLAR_WEDGE_SPAN					"polarwedgespan"				/*!< "polarwedgespan" */
#define RETINOMAP_WIDGET_POLAR_CHECK_AMOUNT					"polarcheckamount"				/*!< "polarcheckamount" */
/**@}*/
/*!
 * \defgroup RetinoEccentricityParamDefs Retinotopic (Eccentricity) parameter Names
 * @{
 */
#define RETINOMAP_WIDGET_ECCENTRICITY_DIRECTION				"eccentricitydirection"			/*!< "eccentricitydirection" */
#define RETINOMAP_WIDGET_ECCENTRICITY_CHECK_AMOUNT			"eccentricitycheckamount"		/*!< "eccentricitycheckamount" */
#define RETINOMAP_WIDGET_ECCENTRICITY_RING_AMOUNT			"eccentricityringamount"		/*!< "eccentricityringamount" */
#define RETINOMAP_WIDGET_ECCENTRICITY_ENABLE_CORTMAG_TIMING	"enablecortmagtiming"			//Not documented!
/**@}*/
/*!
 * \defgroup RetinoMovingBarParamDefs Retinotopic (MovingBar) parameter Names
 * @{
 */
#define RETINOMAP_WIDGET_MOVINGBAR_ANGLE					"movingbarangle"				/*!< "movingbarangle" */
#define RETINOMAP_WIDGET_MOVINGBAR_COVERAGE					"movingbarcoverage"				/*!< "movingbarcoverage" */
#define RETINOMAP_WIDGET_MOVINGBAR_HEIGHT					"movingbarheight"				/*!< "movingbarheight" */
#define RETINOMAP_WIDGET_MOVINGBAR_DIRECTION				"movingbardirection"			/*!< "movingbardirection" */
//#define RETINOMAP_WIDGET_MOVINGBAR_INCOPPDIR				"movingbarincludeoppositedirection"
#define RETINOMAP_WIDGET_MOVINGBAR_HEIGTH_CHECK_AMOUNT		"movingbarheightcheckamount"	/*!< "movingbarheightcheckamount" */
//MovingDots specific defines
#define	RETINOMAP_WIDGET_MOVINGDOTS_COLOR					"movingdotscolor"				/*!< "movingdotscolor" */
#define	RETINOMAP_WIDGET_MOVINGDOTS_MIN_MOVESPEED			"movingdotsminmovespeed"		/*!< "movingdotsminmovespeed" */
#define	RETINOMAP_WIDGET_MOVINGDOTS_MAX_MOVESPEED			"movingdotsmaxmovespeed"		/*!< "movingdotsmaxmovespeed" */
#define	RETINOMAP_WIDGET_MOVINGDOTS_MIN_MOVEANGLE			"movingdotsminmoveangle"		/*!< "movingdotsminmoveangle" */
#define	RETINOMAP_WIDGET_MOVINGDOTS_MAX_MOVEANGLE			"movingdotsmaxmoveangle"		/*!< "movingdotsmaxmoveangle" */
#define RETINOMAP_WIDGET_MOVINGDOTS_NROFDOTS				"movingdotsnrofdots"			/*!< "movingdotsnrofdots" */
#define RETINOMAP_WIDGET_MOVINGDOTS_DOTSIZE					"movingdotsdotsize"				/*!< "movingdotsdotsize" */
#define RETINOMAP_WIDGET_MOVINGDOTS_HEMIFIELD				"movingdotshemifield"			/*!< "movingdotshemifield" */
#define RETINOMAP_WIDGET_MOVINGDOTS_PIXELFROMCENTER			"movingdotspixelfromcenter"		/*!< "movingdotspixelfromcenter" */
#define RETINOMAP_WIDGET_MOVINGDOTS_STATIONAIRY				"movingdotsstationairy"			/*!< "movingdotsstationairy" */
//#define RETINOMAP_WIDGET_MOVINGDOTS_RETINALPOSITION		"movingdotsretinalposition"
#define RETINOMAP_WIDGET_MOVINGDOTS_HEMIFIELDWIDTH			"movingdotshemifieldwidth"		/*!< "movingdotshemifieldwidth" */
#define RETINOMAP_WIDGET_MOVINGDOTS_HEMIFIELDHEIGHT			"movingdotshemifieldheight"		/*!< "movingdotshemifieldheight" */
/**@}*/

#define RETINOMAP_WIDGET_OUTPUT_SUBFOLDER					"/RetinoWidget/"

typedef struct strcMovingDotsStructure
{
	QPolygonF Pos; //A QPolygonF is a QVector<QPointF>. The easiest way to add points to a QPolygonF is to use its streaming operator: polygon << QPointF(10.4, 20.5) << QPointF(20.2, 30.2);
	QPolygonF OldPos;
	QList<QLineF> Mov;
} MovingDotsStructure;

//!  The RetinoMap_glwidget class. 
/*!
  The RetinoMap_glwidget class can be used in combination with the ExperimentManager for presenting 
  various retinotopic (mapping) stimuli.
*/
class RetinoMap_glwidget : public GLWidgetWrapper
{
	Q_OBJECT

public:
	RetinoMap_glwidget(QWidget *parent = NULL);
	~RetinoMap_glwidget();
	RetinoMap_glwidget(const RetinoMap_glwidget& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script
	bool startObject();
	bool initObject();
	bool setObjectID(int nObjID);
	bool initObjectBlockTrial();
	bool paintObject(int paintFlags = 0, QObject *paintEventObject = NULL);
	bool installCustomScriptHandlerFunction(QString FuncName);
	bool setExperimentManager(ExperimentManager *expManager);

private:
	void initialize();
	void parseExperimentObjectBlockParameters(bool bInit = false);
	void initializeMovingDotsStructures();
	QImage RetinoMap_glwidget::fractalFillCheckeredImage(float fWidth, float fHeigth, float fSize, int nflickr);

	QScriptValue *customScriptHandlerFunction;
	int nRetinoID;								//This variable stores the ObjectID used to identify the object
	QString currentExpType;						//The experiment type used, see RetinoMapExperimentType
	bool firstBlockTrialPaintFrame;				//To determine whether it's the first frame to paint from a new Block trial
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
	bool outputTriggerFrame;					//Defines whether we should output(write to a *.png file) the first frame
	bool discreteTriggerSteps;					//Defines whether the stimuli should run smooth or in discrete steps according to each trigger received
	bool randomizeTriggerSteps;					//Defines whether the stimuli should randomize after each trigger received
	bool antiAliasing;							//Defines whether the anti-aliasing filter should be used for painting to the screen
	int lastTriggerNumber;						//To keep track of the last trigger number
	int emptyTriggerSteps;						//Defines the number of Trigger steps in which no stimuli should be presented
	bool bNoChangesSinceLastFrame;
	ExperimentManager *experimentManager;

	QColor movingDotsColor;
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

	float movingDotsMinMoveSpeed;
	float movingDotsMaxMoveSpeed;
	int movingDotsMinMoveAngle;
	int movingDotsMaxMoveAngle;
	int movingDotsDotSize;
	QLineF movingDotsFirstHemiVisibleArea; //The absolute area of the first visible hemisphere from the left-top to the right-bottom
	int movingDotsNrOfDots;
	bool movingDotsIsStationary;
	//RetinoMapHemifieldPos movingDotsHemifieldPos;
	QString movingDotsHemifieldPos;
	int movingDotsPixelFromCenter;
	int movingDotsHemiFieldWidth;
	int movingDotsHemiFieldHeight;
	MovingDotsStructure movingDots;
	//RetinoMapOutputType retinoOutputType;
	QString retinoOutputType;
	//RetinoMapOutputFormat retinoOutputFormat;
	QString retinoOutputFormat;
	QImage mirrorImage;
	QPixmap StimulusResultImageFrame;
	QPixmap StimulusActivationMap;
	QPainter activationPainter;
	QPainter stimuliPainter;
	QPainter imgPainter;
	RandomGenerator *randStimStateGenerator;		 //To hold the Stimuli Trigger Step
	RandomGenerator *randEmptyStimGenerator;		 //To hold the Empty Block Trials
	RandomGenerator *previousRandEmptyStimGenerator; //To hold the processed Empty Block Trials
	QScriptValue scriptVal1;
	QScriptValue scriptVal2;
};
Q_DECLARE_METATYPE(RetinoMap_glwidget)
#endif // RETINOMAP_GLWIDGET_H
