#include "retinomap_glwidget.h"
#include <QDomNodeList>
#include <QGraphicsRectItem>
#include <QtCore/qmath.h>
#include <QFile>
#include "ExperimentManager.h"

RetinoMap_glwidget::RetinoMap_glwidget(QWidget *parent) : GLWidgetWrapper(parent)
{
	initialize();
	GLWidgetWrapper::setupLayout(this);
}

RetinoMap_glwidget::~RetinoMap_glwidget()
{
	if (StimulusResultImageFrame)
	{
		delete StimulusResultImageFrame;
		StimulusResultImageFrame = NULL;
	}
	if (StimulusActivationMap)
	{
		delete StimulusActivationMap;
		StimulusActivationMap = NULL;
	}
	if (activationPainter)
	{
		delete activationPainter;
		activationPainter = NULL;
	}
	if (stimuliPainter)
	{
		delete stimuliPainter;
		stimuliPainter = NULL;
	}
	if (imgPainter)
	{
		delete imgPainter;
		imgPainter = NULL;
	}
	if (randStimStateGenerator)
	{
		delete randStimStateGenerator;
		randStimStateGenerator = NULL;
	}
	if (randEmptyStimGenerator)
	{
		delete randEmptyStimGenerator;
		randEmptyStimGenerator = NULL;
	}
	if (previousRandEmptyStimGenerator)
	{
		delete previousRandEmptyStimGenerator;
		previousRandEmptyStimGenerator = NULL;
	}
}

void RetinoMap_glwidget::initialize()
{
	//Place your custom initialization code here:
	
	//setVerticalSyncSwap();
	//Default values if none defined.
	expSnapshot.currExpBlock = 0;
	expSnapshot.currExpBlockTrialFrame = 0;
	expSnapshot.currExpBlockTrialTrigger = 0;
	expSnapshot.currExpBlockTrialTriggerAmount = 0;
	expSnapshot.currExpTrial = 0;
	expSnapshot.currExpTrigger = 0;
	expSnapshot.elapsedTrialTime = 0;

	bNoChangesSinceLastFrame = false;
	nRetinoID = -1;
	rectScreenRes = getScreenResolution();
	firstBlockTrialPaintFrame = false;	
	lastTriggerNumber = -1;
	flickrSwitch = 0;
	textContent = "";
	textContent = "";
	StimulusResultImageFrame = NULL;
	StimulusActivationMap = NULL;
	activationPainter = NULL;
	stimuliPainter = NULL;
	imgPainter = NULL;
	currExpConfStruct = NULL;
	randStimStateGenerator = NULL;
	randEmptyStimGenerator = NULL;
	previousRandEmptyStimGenerator = NULL;
	textPen = QPen(Qt::white);
	textFont.setPixelSize(20);
	whiteColor = QColor(255,255,255);//For the activity maps
	blackColor = QColor(0,0,0);//For the activity maps
	style = Qt::SolidLine;
	flatCap = Qt::FlatCap;
	roundCap = Qt::RoundCap;
	nextNewBlockEntered = false;
	nextNewCycleEntered = false;
}

void RetinoMap_glwidget::parseExperimentObjectBlockParameters(bool bInit)
{	
	QString tmpString = "";
	if (bInit)
	{		
		//refreshRate = 0;
		//insertExperimentObjectBlockParameter(nRetinoID,GLWWRAP_WIDGET_DISPLAY_REFRESHRATE,QString::number(refreshRate));
		polarWedgeSpan = 22.5;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_POLAR_WEDGE_SPAN,QString::number(polarWedgeSpan));
		polarWedgeNrChecks = 4;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_POLAR_CHECK_AMOUNT,QString::number(polarWedgeNrChecks));
		polarWedgeNrRings = 20;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_POLAR_RING_AMOUNT,QString::number(polarWedgeNrRings));
		polarRotationDirection = 1;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_POLAR_ROTATION_DIRECTION,QString::number(polarRotationDirection));
		eccentricityNrChecks = 20;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_ECCENTRICITY_CHECK_AMOUNT,QString::number(eccentricityNrChecks));
		eccentricityNrRings = 4;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_ECCENTRICITY_RING_AMOUNT,QString::number(eccentricityNrRings));
		disableCortMagFac = false;
		if (disableCortMagFac)
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_DISABLECORTMAGFAC,RETINOMAP_WIDGET_BOOL_TRUE);
		else
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_DISABLECORTMAGFAC,RETINOMAP_WIDGET_BOOL_FALSE);		
		eccentricityDirection = 1;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_ECCENTRICITY_DIRECTION,QString::number(eccentricityDirection));
		movingBarAngle = 45.0;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_ANGLE,QString::number(movingBarAngle));
		movingBarCoverage = 0.5;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_COVERAGE,QString::number(movingBarCoverage));
		movingBarHeight = 10.0;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_HEIGHT,QString::number(movingBarHeight));
		//movingBarIncludeOppositeDirection = false;
		//if (movingBarIncludeOppositeDirection)
		//	insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_INCOPPDIR,RETINOMAP_WIDGET_BOOL_TRUE);
		//else
		//	insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_INCOPPDIR,RETINOMAP_WIDGET_BOOL_FALSE);
		movingBarHeightCheckAmount = 4;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_HEIGTH_CHECK_AMOUNT,QString::number(movingBarHeightCheckAmount));
		movingBarDirection = 1;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_DIRECTION,QString::number(movingBarDirection));
		currentExpType = RetinoMap_PolarAngle;
		switch (currentExpType)
		{
		case RetinoMap_PolarAngle :
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_PATTERN,RETINOMAP_WIDGET_PATTERN_POLARANGLE);
			break;
		case RetinoMap_Eccentricity:
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_PATTERN,RETINOMAP_WIDGET_PATTERN_ECCENTRICITY);
			break;
		case RetinoMap_MovingBar :
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_PATTERN,RETINOMAP_WIDGET_PATTERN_MOVINGBAR);
			break;
		case RetinoMap_MovingDots :
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_PATTERN,RETINOMAP_WIDGET_PATTERN_MOVINGDOTS);
			break;
		case RetinoMap_Fixation:
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_PATTERN,RETINOMAP_WIDGET_PATTERN_FIXATION);
			break;
		}
		tmpString = QColor(87,87,87).name();//gives "#575757";
		colorBackground = QColor(tmpString);
		brushBackground = QBrush(colorBackground);
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_BACKGROUNDCOLOR,tmpString);
		tmpString = QColor(255, 0, 0).name();
		fixationColor = QColor(tmpString);
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_FIXATIONCOLOR,tmpString);
		tmpString = QColor(255, 255, 255).name();
		cCheckerColor1 = QColor(tmpString);
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_CHECKERCOLOR1,tmpString);
		tmpString = QColor(0, 0, 0).name();
		cCheckerColor2 = QColor(tmpString);
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_CHECKERCOLOR2,tmpString);
		showFixationPoint = true;
		if (showFixationPoint)
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_SHOWFIXPOINT,RETINOMAP_WIDGET_BOOL_TRUE);
		else
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_SHOWFIXPOINT,RETINOMAP_WIDGET_BOOL_FALSE);
		fixationSize = 8;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_FIXSIZE,QString::number(fixationSize));
		stimHeigthPixelAmount = rectScreenRes.height();
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_HEIGHT_PIXEL_AMOUNT,QString::number(stimHeigthPixelAmount));
		stimWidthPixelAmount = stimHeigthPixelAmount;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_WIDTH_PIXEL_AMOUNT,QString::number(stimWidthPixelAmount));
		cycleTriggerAmount = 1;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_CYCLE_TRIGGER_AMOUNT,QString::number(cycleTriggerAmount));
		flickrSpeedFreq = 7.5;//5 times per second
		flickrThreshold = 1000 / flickrSpeedFreq;//First at 200 mSecs
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_FLICKRSPEED_HZ,QString::number(flickrSpeedFreq));
		cortMagFactor = 0.2f;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_CORTMAG_FACTOR,QString::number(cortMagFactor));
		triggerDurationMsec = 2000;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_TRIGGERDURATION_MSEC,QString::number(triggerDurationMsec));
		emptyTriggerSteps = 0;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_EMPTYTRIGGERSTEPS,QString::number(emptyTriggerSteps));
		gapDiameter = 20.0;//aperture in the middle left blank
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_GAP_DIAMETER,QString::number(gapDiameter));
		outputTriggerFrame = false;
		if (outputTriggerFrame)
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_OUTPUTTRIGGERFRAME,RETINOMAP_WIDGET_BOOL_TRUE);
		else
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_OUTPUTTRIGGERFRAME,RETINOMAP_WIDGET_BOOL_FALSE);
		discreteTriggerSteps = false;
		if (discreteTriggerSteps)
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_DISCRETETRIGGERSTEPS,RETINOMAP_WIDGET_BOOL_TRUE);
		else
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_DISCRETETRIGGERSTEPS,RETINOMAP_WIDGET_BOOL_FALSE);
		randomizeTriggerSteps = false;
		if (randomizeTriggerSteps)
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_RANDOMIZETRIGGERSTEPS,RETINOMAP_WIDGET_BOOL_TRUE);
		else
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_RANDOMIZETRIGGERSTEPS,RETINOMAP_WIDGET_BOOL_FALSE);
		antiAliasing = true;
		if (antiAliasing)
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_ANTIALIASING,RETINOMAP_WIDGET_BOOL_TRUE);
		else
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_ANTIALIASING,RETINOMAP_WIDGET_BOOL_FALSE);
		retinoOutputType = RetinoMap_OutputType_Frame;
		switch (retinoOutputType)
		{
		case RetinoMap_OutputType_Frame :
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_OUTPUTFRAMETYPE,RETINOMAP_WIDGET_OUTPUTTYPE_FRAME);
			break;
		case RetinoMap_OutputType_Mask:
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_OUTPUTFRAMETYPE,RETINOMAP_WIDGET_OUTPUTTYPE_MASK);
			break;
		}
		retinoOutputFormat = RetinoMap_OutputFormat_DAT;
		switch (retinoOutputFormat)
		{
		case RetinoMap_OutputFormat_DAT:
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_OUTPUTFRAMEFORMAT,RETINOMAP_WIDGET_OUTPUTFORMAT_DAT);
			break;
		case RetinoMap_OutputFormat_PNG:
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_OUTPUTFRAMEFORMAT,RETINOMAP_WIDGET_OUTPUTFORMAT_PNG);
			break;
		}

		movingDotsMoveSpeed = 1;// in case you want to specify the move speed of dots to be homogeneous
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_MOVESPEED,QString::number(movingDotsMoveSpeed));
		movingDotsHemifieldPos = RetinoMap_HemifieldPos_Both;
		switch (movingDotsHemifieldPos)
		{
		case RetinoMap_HemifieldPos_Left:
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_HEMIFIELD,RETINOMAP_WIDGET_POS_LEFT);
			break;
		case RetinoMap_HemifieldPos_Right:
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_HEMIFIELD,RETINOMAP_WIDGET_POS_RIGHT);
			break;
		case RetinoMap_HemifieldPos_Both:
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_HEMIFIELD,RETINOMAP_WIDGET_POS_BOTH);
			break;
		}
		movingDotsPixelFromCenter = 100;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_PIXELFROMCENTER,QString::number(movingDotsPixelFromCenter));
		movingDotsIsStationary = false;
		if (movingDotsIsStationary)
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_STATIONAIRY,RETINOMAP_WIDGET_BOOL_TRUE);
		else
			insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_STATIONAIRY,RETINOMAP_WIDGET_BOOL_FALSE);
		movingDotsDotSize = 12;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_DOTSIZE,QString::number(movingDotsDotSize));
		movingDotsNrOfDots = 1000;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_NROFDOTS,QString::number(movingDotsNrOfDots));
		//movingDotsRetPosition = 0;		
		//insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_RETINALPOSITION,QString::number(movingDotsRetPosition));
		movingDotsFieldWidth = stimWidthPixelAmount;		
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_FIELDWIDTH,QString::number(movingDotsFieldWidth));
		movingDotsFieldHeight = stimHeigthPixelAmount;		
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_FIELDHEIGHT,QString::number(movingDotsFieldHeight));
		//initializeMovingDotsStructures();
	} 
	else
	{
		randStimStateGenerator = new RandomGenerator();//Here we can initialize the generator so each block we have it correctly constructed
		randEmptyStimGenerator = new RandomGenerator();
		previousRandEmptyStimGenerator = new RandomGenerator();
		tmpString = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_PATTERN,RETINOMAP_WIDGET_PATTERN_POLARANGLE);
		if(tmpString == RETINOMAP_WIDGET_PATTERN_ECCENTRICITY)
			currentExpType = RetinoMap_Eccentricity;
		else if(tmpString == RETINOMAP_WIDGET_PATTERN_MOVINGBAR)
			currentExpType = RetinoMap_MovingBar;
		else if(tmpString == RETINOMAP_WIDGET_PATTERN_FIXATION)
			currentExpType = RetinoMap_Fixation;
		else if(tmpString == RETINOMAP_WIDGET_PATTERN_MOVINGDOTS)
			currentExpType = RetinoMap_MovingDots; 
		else// (tmpString == RETINOMAP_WIDGET_PATTERN_POLARANGLE)
			currentExpType = RetinoMap_PolarAngle;

		tmpString = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_SHOWFIXPOINT,RETINOMAP_WIDGET_BOOL_TRUE);
		if(tmpString == RETINOMAP_WIDGET_BOOL_FALSE)
			showFixationPoint = false;
		else //if (tmpString == RETINOMAP_WIDGET_BOOL_TRUE)
			showFixationPoint = true;
		stimHeigthPixelAmount = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_HEIGHT_PIXEL_AMOUNT,QString::number(stimHeigthPixelAmount)).toFloat();
		stimWidthPixelAmount = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_WIDTH_PIXEL_AMOUNT,QString::number(stimWidthPixelAmount)).toFloat();
		setStimuliResolution(stimWidthPixelAmount,stimHeigthPixelAmount);
		fixationSize = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_FIXSIZE,QString::number(fixationSize)).toInt();
		cycleTriggerAmount = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_CYCLE_TRIGGER_AMOUNT,QString::number(cycleTriggerAmount)).toInt();
		flickrSpeedFreq = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_FLICKRSPEED_HZ,QString::number(flickrSpeedFreq)).toFloat();
		flickrThreshold = 1000 / flickrSpeedFreq;
		cortMagFactor = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_CORTMAG_FACTOR,QString::number(cortMagFactor)).toFloat();
		triggerDurationMsec = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_TRIGGERDURATION_MSEC,QString::number(triggerDurationMsec)).toInt();
		emptyTriggerSteps = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_EMPTYTRIGGERSTEPS,QString::number(emptyTriggerSteps)).toInt();		
		gapDiameter = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_GAP_DIAMETER,QString::number(gapDiameter)).toInt();
		colorBackground = QColor(getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_BACKGROUNDCOLOR,colorBackground.name()));
		brushBackground = QBrush(colorBackground);
		fixationColor = QColor(getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_FIXATIONCOLOR,fixationColor.name()));
		cCheckerColor1 = QColor(getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_CHECKERCOLOR1,cCheckerColor1.name()));
		cCheckerColor2 = QColor(getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_CHECKERCOLOR2,cCheckerColor2.name()));		
		tmpString = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_OUTPUTTRIGGERFRAME,RETINOMAP_WIDGET_BOOL_FALSE);
		if (tmpString == RETINOMAP_WIDGET_BOOL_TRUE)
			outputTriggerFrame = true;
		else //if(tmpString == RETINOMAP_WIDGET_BOOL_FALSE)
			outputTriggerFrame = false;
		tmpString = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_DISCRETETRIGGERSTEPS,RETINOMAP_WIDGET_BOOL_FALSE);
		if (tmpString == RETINOMAP_WIDGET_BOOL_TRUE)
			discreteTriggerSteps = true;
		else// if(tmpString == RETINOMAP_WIDGET_BOOL_FALSE)
			discreteTriggerSteps = false;
		tmpString = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_RANDOMIZETRIGGERSTEPS,RETINOMAP_WIDGET_BOOL_FALSE);
		if (tmpString == RETINOMAP_WIDGET_BOOL_TRUE)
			randomizeTriggerSteps = true;
		else// if(tmpString == RETINOMAP_WIDGET_BOOL_FALSE)
			randomizeTriggerSteps = false;
		tmpString = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_ANTIALIASING,RETINOMAP_WIDGET_BOOL_TRUE);
		if(tmpString == RETINOMAP_WIDGET_BOOL_FALSE)
			antiAliasing = false;
		else //(tmpString == RETINOMAP_WIDGET_BOOL_TRUE)
			antiAliasing = true;
		tmpString = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_OUTPUTFRAMETYPE,RETINOMAP_WIDGET_OUTPUTTYPE_FRAME);
		if (tmpString == RETINOMAP_WIDGET_OUTPUTTYPE_MASK)
			retinoOutputType = RetinoMap_OutputType_Mask;
		else //(tmpString == RETINOMAP_WIDGET_OUTPUTTYPE_FRAME)
			retinoOutputType = RetinoMap_OutputType_Frame;
		tmpString = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_OUTPUTFRAMEFORMAT,RETINOMAP_WIDGET_OUTPUTFORMAT_DAT);
		if (tmpString == RETINOMAP_WIDGET_OUTPUTFORMAT_PNG)
			retinoOutputFormat = RetinoMap_OutputFormat_PNG;
		else //(tmpString == RETINOMAP_WIDGET_OUTPUTFRAMEFORMAT)
			retinoOutputFormat = RetinoMap_OutputFormat_DAT;
		tmpString = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_DISABLECORTMAGFAC,RETINOMAP_WIDGET_BOOL_FALSE);
		if (tmpString == RETINOMAP_WIDGET_BOOL_TRUE)
			disableCortMagFac = true;
		else //if(tmpString == RETINOMAP_WIDGET_BOOL_FALSE)
			disableCortMagFac = false;

		//Specific ExperimentType Parameters
		switch (currentExpType)
		{
		case RetinoMap_PolarAngle :
			polarWedgeSpan = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_POLAR_WEDGE_SPAN,QString::number(polarWedgeSpan)).toFloat();
			polarWedgeNrChecks = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_POLAR_CHECK_AMOUNT,QString::number(polarWedgeNrChecks)).toInt();
			polarWedgeNrRings = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_POLAR_RING_AMOUNT,QString::number(polarWedgeNrRings)).toInt();
			polarRotationDirection = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_POLAR_ROTATION_DIRECTION,QString::number(polarRotationDirection)).toInt();
			break;
		case RetinoMap_Eccentricity:
			eccentricityNrChecks = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_ECCENTRICITY_CHECK_AMOUNT,QString::number(eccentricityNrChecks)).toInt();
			eccentricityNrRings = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_ECCENTRICITY_RING_AMOUNT,QString::number(eccentricityNrRings)).toInt();
			eccentricityDirection = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_ECCENTRICITY_DIRECTION,QString::number(eccentricityDirection)).toInt();
			break;
		case RetinoMap_MovingBar :
			movingBarAngle = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_ANGLE,QString::number(movingBarAngle)).toFloat();
			movingBarCoverage = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_COVERAGE,QString::number(movingBarCoverage)).toFloat();
			movingBarHeight = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_HEIGHT,QString::number(movingBarHeight)).toFloat();
			movingBarHeightCheckAmount = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_HEIGTH_CHECK_AMOUNT,QString::number(movingBarHeightCheckAmount)).toInt();
			movingBarDirection = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_DIRECTION,QString::number(movingBarDirection)).toInt();
			//tmpString = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_INCOPPDIR,RETINOMAP_WIDGET_BOOL_FALSE);
			//if (tmpString == RETINOMAP_WIDGET_BOOL_TRUE)
			//	movingBarIncludeOppositeDirection = true;
			//else //if(tmpString == RETINOMAP_WIDGET_BOOL_FALSE)
			//	movingBarIncludeOppositeDirection = false;	
			break;
		case RetinoMap_MovingDots :
			movingDotsMoveSpeed = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_MOVESPEED,QString::number(movingDotsMoveSpeed)).toFloat();
			tmpString = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_HEMIFIELD,RETINOMAP_WIDGET_POS_BOTH);
			if (tmpString == RETINOMAP_WIDGET_POS_LEFT)
				movingDotsHemifieldPos = RetinoMap_HemifieldPos_Left;
			else if (tmpString == RETINOMAP_WIDGET_POS_RIGHT)
				movingDotsHemifieldPos = RetinoMap_HemifieldPos_Right;
			else //if (tmpString == RETINOMAP_WIDGET_POS_BOTH)
				movingDotsHemifieldPos = RetinoMap_HemifieldPos_Both;
			movingDotsPixelFromCenter = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_PIXELFROMCENTER,QString::number(movingDotsPixelFromCenter)).toInt();
			tmpString = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_STATIONAIRY,RETINOMAP_WIDGET_BOOL_FALSE);
			if (tmpString == RETINOMAP_WIDGET_BOOL_TRUE)
				movingDotsIsStationary = true;
			else //if(tmpString == RETINOMAP_WIDGET_BOOL_FALSE)
				movingDotsIsStationary = false;			
			movingDotsDotSize = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_DOTSIZE,QString::number(movingDotsDotSize)).toInt();
			movingDotsNrOfDots = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_NROFDOTS,QString::number(movingDotsNrOfDots)).toInt();
			//movingDotsRetPosition = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_RETINALPOSITION,QString::number(movingDotsRetPosition)).toInt();
			movingDotsFieldWidth = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_FIELDWIDTH,QString::number(movingDotsFieldWidth)).toInt();
			movingDotsFieldHeight = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_FIELDHEIGHT,QString::number(movingDotsFieldHeight)).toInt();
			initializeMovingDotsStructures();
			break;
		case RetinoMap_Fixation:
			break;
		}
	}
	bNoChangesSinceLastFrame = false;
}

bool RetinoMap_glwidget::initObject()
{
	currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"","swapInterval() = ", QString::number(this->format().swapInterval()));
	parseExperimentObjectBlockParameters(true);
	return true;
}

bool RetinoMap_glwidget::startObject()
{
	lastTriggerNumber = -1;
	emit LogToOutputWindow("Started");
	return true;
}

bool RetinoMap_glwidget::setObjectConfiguration(QObject *pExpConfStruct)//ExperimentConfiguration *pExpConfStruct)
{
	currExpConfStruct = reinterpret_cast<ExperimentConfiguration *>(pExpConfStruct);
	return true;
}

bool RetinoMap_glwidget::setObjectID(int nObjID)
{
	nRetinoID = nObjID;//or use getObjectID();//Because we need it often we'll buffer it here
	return true;
}

bool RetinoMap_glwidget::initObjectBlockTrial()
{
	parseExperimentObjectBlockParameters(false);
	//Some variable initializations
	flickrSwitch = 0;
	StimulusResultImageFrame = new QPixmap(stimWidthPixelAmount,stimHeigthPixelAmount);
	StimulusActivationMap = new QPixmap(stimWidthPixelAmount,stimHeigthPixelAmount);
	firstBlockTrialPaintFrame = true;	
	currExpBlockTrialCycle = 0;
	emptyTriggerStepCount = 0;
	emptyTriggerLastIndex = -1;
	previousRandEmptyStimGenerator->clear();
	//randStimStateGenerator->clear();
	return true;
}

QImage RetinoMap_glwidget::fractalFillCheckeredImage(float fWidth, float fHeigth, float fSize, int nflickr)
{
	//QTime recTime;
	//recTime.restart();
	//imgPainter->setStyle(style);
	//imgPainter->setCapStyle(flatCap);
	//if(isDebugMode())
	//	currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"",QString("SubPainting the object"),QString("3c1"));

	int nWidth = (int)(fWidth + 0.5f);
	int nHeigth = (int)(fHeigth + 0.5f);
	int nSize = (int)(fSize + 1.0f);

	QImage image(nWidth, nHeigth, QImage::Format_RGB16);
	QImage base((int)((fSize*2)+0.5f), (int)((fSize*2)+0.5f), QImage::Format_RGB16);
	QPainter p1(&base); 
	if (antiAliasing)
		p1.setRenderHint(QPainter::Antialiasing);
	p1.setCompositionMode(QPainter::CompositionMode_Source);

	if (nflickr == 1)
	{
		p1.fillRect(0, 0, nSize, nSize, cCheckerColor1);// Qt::gray);
		p1.fillRect(nSize, 0, nSize, nSize, cCheckerColor2);// Qt::white);
		p1.fillRect(0, nSize, nSize, nSize, cCheckerColor2);//Qt::white);
		p1.fillRect(nSize, nSize, nSize, nSize, cCheckerColor1);//Qt::gray);
	}
	else
	{
		p1.fillRect(0, 0, nSize, nSize, cCheckerColor2);// Qt::gray);
		p1.fillRect(nSize, 0, nSize, nSize, cCheckerColor1);// Qt::white);
		p1.fillRect(0, nSize, nSize, nSize, cCheckerColor1);//Qt::white);
		p1.fillRect(nSize, nSize, nSize, nSize, cCheckerColor2);//Qt::gray);
	}

	QPainter p(&image);
	if (antiAliasing)
		p.setRenderHint(QPainter::Antialiasing);
	p.setCompositionMode(QPainter::CompositionMode_Source);
	p.drawImage(0, 0, base);

	const int imageW = image.width();
	const int imageH = image.height();
	int w = base.width();
	int h = base.height();
	while (w < imageW || h < imageH) {
		if (w < imageW) {
			p.drawImage(QRect(w, 0, w, h), image, QRect(0, 0, w, h));
			w *= 2;
		}
		if (h < imageH) {
			p.drawImage(QRect(0, h, w, h), image, QRect(0, 0, w, h));
			h *= 2;
		}
	}
	//if(isDebugMode())
	//	currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"",QString("SubPainting the object"),QString("3c2"));
	//int a = recTime.elapsed();
	return image;
}

void RetinoMap_glwidget::initializeMovingDotsStructures()
{
	switch(movingDotsHemifieldPos)
	{
	case RetinoMap_HemifieldPos_Both:
		movingDotsXStartRel = ((stimWidthPixelAmount/2)-movingDotsFieldWidth) - movingDotsPixelFromCenter;
		break;
	case RetinoMap_HemifieldPos_Right:
		movingDotsXStartRel = (stimWidthPixelAmount/2) + movingDotsPixelFromCenter;
		break;
	case RetinoMap_HemifieldPos_Left:
		movingDotsXStartRel = ((stimWidthPixelAmount/2)-movingDotsFieldWidth) - movingDotsPixelFromCenter;
		break;
	}
	movingDotsXWidth = movingDotsFieldWidth;
	movingDotsYWidth = movingDotsFieldHeight;
	movingDotsYStartRel = ((stimHeigthPixelAmount-movingDotsFieldHeight)/2);


	QPointF tmpPoint;
	QLineF tmpLine;
	movingDots.Pos.clear();
	movingDots.Mov.clear();
	movingDots.MirrorXPos.clear();
	movingDots.OldPos.clear();
	for (int i=0; i<movingDotsNrOfDots; i++)
	{
		movingDots.OldPos.append(QPointF(0,0));
		tmpPoint.setX(randStimStateGenerator->randomizeInt(movingDotsXStartRel,movingDotsXStartRel+(movingDotsXWidth-1)));//X Position
		tmpPoint.setY(randStimStateGenerator->randomizeInt(movingDotsYStartRel,movingDotsYStartRel+(movingDotsYWidth-1)));//Y Position
		movingDots.Pos.append(tmpPoint);
		tmpLine.setP1(QPointF(0,0));//Point1 of Movement Vector
		tmpLine.setP2(QPointF(1,0));//The base speed
		tmpLine.setAngle(randStimStateGenerator->randomizeInt(1,360));//The movement direction
		tmpLine.setLength(2);//The additional extra speed of the movement
		movingDots.Mov.append(tmpLine);
		//movingDots.MirrorXPos//The Mirrored X Position
	}

	movingDotsXValue.resize(2); 
	movingDotsXValue[0].resize(movingDotsNrOfDots); 
	movingDotsXValue[1].resize(movingDotsNrOfDots);
	movingDotsYValue.resize(2); 
	movingDotsYValue[0].resize(movingDotsNrOfDots); 
	movingDotsYValue[1].resize(movingDotsNrOfDots);
	movingDotsBGXDir.resize(movingDotsNrOfDots);
	movingDotsBGYDir.resize(movingDotsNrOfDots);
	for (int i=0; i<movingDotsNrOfDots; i++)
	{
		movingDotsXValue[0][i] = randStimStateGenerator->randomizeInt(movingDotsXStartRel,movingDotsXStartRel+(movingDotsXWidth-1)); //movingDotsXStartRel + myRandom(movingDotsXWidth-1) + 1;
		movingDotsYValue[0][i] = randStimStateGenerator->randomizeInt(movingDotsYStartRel,movingDotsYStartRel+(movingDotsYWidth-1)); //movingDotsYStartRel + myRandom(movingDotsYWidth-1) + 1;
		movingDotsBGXDir[i] =    cos((float)randStimStateGenerator->randomizeInt(1,360) / (180.0f/DOUBLE_PI_VALUE));//cos((float)randStimStateGenerator->randomize(1,6)); //cos((float)myRandom(6)+1); 
		movingDotsBGXDir[i] *=   movingDotsMoveSpeed;
		movingDotsBGYDir[i] =    sin((float)randStimStateGenerator->randomizeInt(1,360) / (180.0f/DOUBLE_PI_VALUE));//-1.0f * sin((float)randStimStateGenerator->randomize(1,6)); //-1.0f * sin((float)myRandom(6)+1); 
		movingDotsBGYDir[i] *=   movingDotsMoveSpeed;
	}
}



bool RetinoMap_glwidget::paintObject(int paintFlags, QObject *paintEventObject)
{
	if(isDebugMode())
		currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"","Starting to paint the object");
	QPaintEvent *event = reinterpret_cast<QPaintEvent *>(paintEventObject);//qobject_cast<QPaintEvent *>(paintEventObject);
	GLWidgetPaintFlags currentPaintFlags = (GLWidgetPaintFlags)paintFlags;
	//int nPaintIndex = 0;
	bool bRenderStimuli = true;
	QString tmpStr = "";
	tmpParamValue = "";
	fStimulusDiameter = 0.0f;
	fTrialTimeProgress = 0.0f;
	if (currentPaintFlags & GLWidgetPaintFlags_LockedState)
	{
		bCreateActivationMap = false;
		currentExpType = RetinoMap_Fixation;
	}
	else
	{
		bCreateActivationMap = (outputTriggerFrame) && (retinoOutputType==RetinoMap_OutputType_Mask);
	}
	getCurrentExperimentProgressSnapshot(&expSnapshot);
	int elapsedTrialTime = (int)expSnapshot.elapsedTrialTime;

	if((elapsedTrialTime >= ((triggerDurationMsec * (expSnapshot.currExpBlockTrialTrigger+1))))&&(currentExpType != RetinoMap_Fixation))//To make sure that the elapsed TrialTime doesn't exceed the maximum time of one Block-TrialTime
		elapsedTrialTime = (triggerDurationMsec * (expSnapshot.currExpBlockTrialTrigger+1))-1;
	else if((elapsedTrialTime <= ((triggerDurationMsec * (expSnapshot.currExpBlockTrialTrigger))))&&(currentExpType != RetinoMap_Fixation))
		elapsedTrialTime = (triggerDurationMsec * (expSnapshot.currExpBlockTrialTrigger))+1;
	//debugElapsedTime = debugTime.restart();
	if (currExpBlockTrialCycle < (int)(expSnapshot.currExpBlockTrialTrigger/cycleTriggerAmount))
	{
		currExpBlockTrialCycle++;
		nextNewCycleEntered = true;
	}
	if (expSnapshot.currExpBlockTrialFrame == 0)
	{
		nextNewCycleEntered = true;
		nextNewBlockEntered = true;
		randStimStateGenerator->clear();
	}
	//if(isDebugMode())
	//	currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"",QString("SubPainting the object"),QString::number(0));//nPaintIndex++));
	if (currentExpType != RetinoMap_Fixation)
	{
		if (randEmptyStimGenerator->isEmpty() && (currExpBlockTrialCycle == 0))//Initialize the "Empty" random list
		{
			int i,j;
			if ((emptyTriggerSteps > 0) && (cycleTriggerAmount>=emptyTriggerSteps))//We have to make sure that the Empty item occur in a block next to each other
			{
				RandomGenerator tmpGenerator;
				for (j=0;j<(cycleTriggerAmount/emptyTriggerSteps);j++)
				{
					tmpGenerator.append(QString::number(j));
				}
				tmpGenerator.randomizeList();
				for (j=0;j<tmpGenerator.count();j++)//Create random Empty trigger steps within the Cycle
				{
					for (i=0;i<emptyTriggerSteps;i++)
					{
						randEmptyStimGenerator->append(QString::number((tmpGenerator.at(j).toInt()*emptyTriggerSteps)+i));
					}				
				}
				if (cycleTriggerAmount%emptyTriggerSteps>0)//Do we need some additional steps?
				{
					for (i=0;i<(cycleTriggerAmount%emptyTriggerSteps);i++)
					{
						randEmptyStimGenerator->append(QString::number((emptyTriggerSteps*((int)cycleTriggerAmount/emptyTriggerSteps))+i));
					}
				}
			}
			else//Just fill the list
			{
				for (j=0;j<cycleTriggerAmount;j++)//Create random Empty trigger steps within the Cycle
				{
					randEmptyStimGenerator->append(QString::number(j));
				}
				randEmptyStimGenerator->randomizeList();
			}

			if ((emptyTriggerSteps > 0))
			{
				currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"","EmptyRandomList Filled(Block:" + QString::number(expSnapshot.currExpBlock) + ", Trial:" + QString::number(expSnapshot.currExpTrial) + ")");
				tmpStr = "";
				for (j=0;j<cycleTriggerAmount;j++)
				{
					if (j==0)
						tmpStr = randEmptyStimGenerator->at(j);
					else
						tmpStr = tmpStr + QString(",") + randEmptyStimGenerator->at(j);
				}
				currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"","EmptyRandomList(steps) = ",tmpStr);
			}
			emptyTriggerStepCount = emptyTriggerSteps;
		}

		if (randomizeTriggerSteps && nextNewCycleEntered)
		{
			int j;
			if(randStimStateGenerator->count() != cycleTriggerAmount)
			{
				randStimStateGenerator->clear();
				for (j=0;j<cycleTriggerAmount;j++)//Create random Stimuli trigger steps within a cycle
				{
					randStimStateGenerator->append(QString::number(j));
				}
			}
			randStimStateGenerator->randomizeList(RandomGenerator_RandomizePreservedIndexes,previousRandEmptyStimGenerator);

			//currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"","StimRandomList Filled(Block:" + QString::number(expSnapshot.currExpBlock) + ", Trial:" + QString::number(expSnapshot.currExpTrial) + ")");
			tmpStr = "";
			for (j=0;j<cycleTriggerAmount;j++)
			{
				if (j==0)
					tmpStr = randStimStateGenerator->at(j);
				else
					tmpStr = tmpStr + QString(",") + randStimStateGenerator->at(j);
			}
			currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"","StimRandomList = ",tmpStr);
		}	

		if (discreteTriggerSteps)
		{	
			int numberOfSteps = 1;
			if (cycleTriggerAmount > 1)
			{
				if (currentExpType == RetinoMap_PolarAngle) 
				{
					//0-->1 travels from 0 to 360(which is also 0!) degrees
					//To use a full scale spread we'll use the following:
					numberOfSteps = cycleTriggerAmount;
				}
				//else if ((currentExpType == RetinoMap_MovingBar) && (movingBarIncludeOppositeDirection))
				//{
				//	numberOfSteps = cycleTriggerAmount-2;
				//}
				else
				{
					//0-->1 travels from 0 to 100(which is not 0!) procent
					//To use a full scale spread we'll use the following:
					numberOfSteps = cycleTriggerAmount-1;
				}
				int nTrialIndex;
				if (randomizeTriggerSteps)
					nTrialIndex = randStimStateGenerator->at(expSnapshot.currExpBlockTrialTrigger%cycleTriggerAmount).toInt()%cycleTriggerAmount;
				else
					nTrialIndex = expSnapshot.currExpBlockTrialTrigger%cycleTriggerAmount;

				//if ((currentExpType == RetinoMap_MovingBar) && (movingBarIncludeOppositeDirection) && ((nTrialIndex*2)>=cycleTriggerAmount))
				//	fTrialTimeProgress = (float)(nTrialIndex-1) / numberOfSteps;
				//else
					fTrialTimeProgress = (float)(nTrialIndex) / numberOfSteps;
			}
			else
				fTrialTimeProgress = (float)expSnapshot.currExpBlockTrialTrigger / cycleTriggerAmount;

			//qDebug() << fTrialTimeProgress;
		} 
		else
		{
			if (randomizeTriggerSteps)
			{
				float subElapsedTrialTime = elapsedTrialTime%(triggerDurationMsec);
				float newElapsedTrialTime = (float)subElapsedTrialTime + (randStimStateGenerator->at(expSnapshot.currExpBlockTrialTrigger%cycleTriggerAmount).toFloat() * triggerDurationMsec);//(randStimStateGenerator->at(currExpBlockTrialTrigger).toFloat() * triggerDurationMsec);
				fTrialTimeProgress = (float)newElapsedTrialTime / (cycleTriggerAmount * triggerDurationMsec);
			}
			else
			{
				fTrialTimeProgress = (float)elapsedTrialTime / (cycleTriggerAmount * triggerDurationMsec);
			}
		}
		if (fTrialTimeProgress>1.0f)//Maximum for this cycle overridden?
			fTrialTimeProgress = fTrialTimeProgress - ((int)fTrialTimeProgress);//Revert cycle to beginning
	
		if (nextNewCycleEntered)
			emptyTriggerStepCount = emptyTriggerSteps;

		//if (  (  (emptyTriggerSteps==0)  ||  ((emptyTriggerStepCount<1)&&(emptyTriggerLastIndex!=expSnapshot.currExpBlockTrialTrigger)       )  )  == false)
		//if (  (  (emptyTriggerSteps >0)  &&  ((emptyTriggerStepCount<1)&&(emptyTriggerLastIndex!=expSnapshot.currExpBlockTrialTrigger)==false)  )  )
		if   (  (  (emptyTriggerSteps >0)  &&  ((emptyTriggerStepCount>0)||(emptyTriggerLastIndex==expSnapshot.currExpBlockTrialTrigger)       )  )  )
		{
			if (emptyTriggerLastIndex != expSnapshot.currExpBlockTrialTrigger) 
			{
				if(emptyTriggerStepCount>0)
				{
					for (int k=0;k<emptyTriggerStepCount;k++)//Check if one of the empty steps is equal to the current trigger
					{
						if (randEmptyStimGenerator->count() > k)
						{
							int nSelectedBlockTrialStep;
							//if (randomizeTriggerSteps)
							//	nSelectedBlockTrial = randStimStateGenerator->at(expSnapshot.currExpBlockTrialTrigger%cycleTriggerAmount).toInt();
							//else
							//	nSelectedBlockTrial = expSnapshot.currExpBlockTrialTrigger%(cycleTriggerAmount);

							nSelectedBlockTrialStep = expSnapshot.currExpBlockTrialTrigger%cycleTriggerAmount;

							if (randEmptyStimGenerator->at(k).toInt()==nSelectedBlockTrialStep)
							{
								bRenderStimuli = false;
								emptyTriggerLastIndex = expSnapshot.currExpBlockTrialTrigger;
								emptyTriggerStepCount--;
								previousRandEmptyStimGenerator->append(randEmptyStimGenerator->takeAt(k));
								currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"",QString("Removed a empty at (step=")+QString::number(nSelectedBlockTrialStep)+QString(", size=")+QString::number(randEmptyStimGenerator->count())+QString(")"),previousRandEmptyStimGenerator->last());
								if (previousRandEmptyStimGenerator->count() == cycleTriggerAmount)
								{
									previousRandEmptyStimGenerator->clear();
								}
								break;
							}
						}
					}
				}
			}
			else//This one should not be shown, the trigger is still equal to the last one where it was empty
			{
				bRenderStimuli = false;
			}
		}
		else if(emptyTriggerLastIndex==expSnapshot.currExpBlockTrialTrigger)//This one should not be shown, the trigger is still equal to the last one where it was empty
		{
			bRenderStimuli = false;
		}
	}

	//if(isDebugMode())
	//	currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"",QString("SubPainting the object"),QString::number(1));

	StimulusResultImageFrame->fill(colorBackground);

	if (imgPainter == NULL)
		imgPainter = new QPainter(StimulusResultImageFrame);//Constructor automatically calls begin()
	else
		imgPainter->begin(StimulusResultImageFrame);
	if (bCreateActivationMap)
	{
		StimulusActivationMap->fill(blackColor);//The activation map should be first filled with 0's
		if (activationPainter == NULL)
			activationPainter = new QPainter(StimulusActivationMap);//Constructor automatically calls begin()
		else
			activationPainter->begin(StimulusActivationMap);//For the StimulusActivationMap
	}
	if (antiAliasing)
		imgPainter->setRenderHint(QPainter::Antialiasing);
	imgPainter->setPen(textPen);
	imgPainter->setFont(textFont);
	if(elapsedTrialTime >= flickrThreshold)//Can we already switch the flickr state?
	{
		if(flickrSwitch == 0)
			flickrSwitch = 1;    
		else
			flickrSwitch = 0;
		
		flickrThreshold += (1000 / flickrSpeedFreq);//Calculate the next flickr threshold moment in time
	}
	int nStimFrameHeight = StimulusResultImageFrame->height();
	int nStimFrameWidth = StimulusResultImageFrame->width();

	//if(isDebugMode())
	//	currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"",QString("SubPainting the object"),QString::number(2));

	switch (currentExpType)
	{
	case RetinoMap_PolarAngle:
	{		
		if (bRenderStimuli)
		{
			float startAngle;
			wedgeSpanAngle = polarWedgeSpan / polarWedgeNrChecks * 16.0f;
			if((disableCortMagFac==false) && (cortMagFactor > 0))
				currentWedgeDiameter = ((nStimFrameHeight - gapDiameter)) / 2.0f * cortMagFactor;
			else
				currentWedgeDiameter = ((nStimFrameHeight - gapDiameter)) / 2.0f / polarWedgeNrRings;		
			currentSize = nStimFrameHeight - currentWedgeDiameter;
			currentXPoint = (nStimFrameWidth - currentSize) / 2.0f;
			currentYPoint = (nStimFrameHeight - currentSize) / 2.0f;
			if(polarRotationDirection == 1)//Clockwise
			{
				startAngle = (-360.0f * fTrialTimeProgress) - (polarWedgeSpan);
			}
			else//Counterclockwise 
			{
				startAngle = (360.0f * fTrialTimeProgress);
			}
			if(bCreateActivationMap)
			{
				activationPainter->setPen(QPen(whiteColor,(nStimFrameHeight - gapDiameter) / 2.0f, style, flatCap));
				float fHalfWedgeLenght = (nStimFrameHeight - gapDiameter) / 2.0f;
				activationPainter->drawArc(fHalfWedgeLenght/2, fHalfWedgeLenght/2, (nStimFrameHeight - (fHalfWedgeLenght)), (nStimFrameHeight - (fHalfWedgeLenght)), (startAngle * 16.0f), (polarWedgeSpan * 16.0f));
			}
			for(int i=1; i<(polarWedgeNrRings+1);i++)
			{
				currentStartAngle = startAngle * 16.0f;
				for(int k=0; k<polarWedgeNrChecks;k++)
				{
					if(flickrSwitch==1)
					{
						if((k+i)%2==0)
						{
							imgPainter->setPen(QPen(cCheckerColor1, currentWedgeDiameter, style, flatCap));
						}
						else
						{
							imgPainter->setPen(QPen(cCheckerColor2, currentWedgeDiameter, style, flatCap));
						}
					}
					else
					{
						if((k+i)%2!=0)
						{
							imgPainter->setPen(QPen(cCheckerColor1, currentWedgeDiameter, style, flatCap));
						}
						else
						{
							imgPainter->setPen(QPen(cCheckerColor2, currentWedgeDiameter, style, flatCap));
						}
					}
					//if((i==1) || (i==5) || (i==10)) //(i%2)==0)
					
					if(k==0)//draw a full wide wedge
					{
						imgPainter->drawArc(currentXPoint, currentYPoint, currentSize, currentSize, currentStartAngle, wedgeSpanAngle*polarWedgeNrChecks);//Same as drawing an partial ellipse
					}					
					else if (k%2!=0)
					{
						imgPainter->drawArc(currentXPoint, currentYPoint, currentSize, currentSize, currentStartAngle, wedgeSpanAngle);//Same as drawing an partial ellipse
					}
					currentStartAngle = currentStartAngle + wedgeSpanAngle;
				}
				//if(isDebugMode() && (debugUsedTestSamples==debugTestSamples))
				//{
				//	imgPainter->setPen(QPen(QColor(255,0,0), 1, style, cap));
				//	imgPainter->drawEllipse(currentXPoint, currentYPoint, currentSize, currentSize);
				//}

				currentSize = currentSize - currentWedgeDiameter;//First subtract the first wedge diameter
				if((disableCortMagFac==false) && (cortMagFactor > 0))
				{
					if(i==(polarWedgeNrRings-1))//Pre-last loop to prepare last wedge?
						currentWedgeDiameter = (currentSize-gapDiameter) / 2;
					else
						currentWedgeDiameter = ((currentSize - currentWedgeDiameter) - gapDiameter) / 2 * cortMagFactor;
				}
				currentSize = currentSize - currentWedgeDiameter;//First subtract the second wedge diameter
				currentXPoint = (nStimFrameWidth - currentSize) / 2.0f;
				currentYPoint = (nStimFrameHeight - currentSize) / 2.0f;
			}
		}
		//if(isDebugMode())
		//	currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"",QString("SubPainting the object"),QString::number(3));
		if(showFixationPoint) // show fix cross
		{
			imgPainter->setPen(QPen(fixationColor, fixationSize, style, roundCap));
			imgPainter->drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			if(bCreateActivationMap)
			{
				activationPainter->setPen(QPen(whiteColor, fixationSize, style, roundCap));
				activationPainter->drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			}
		}
		break;
	}
	case RetinoMap_Eccentricity:
		if (bRenderStimuli)
		{		
			QPen oldPen;
			wedgeSpanAngle = 360.0f / eccentricityNrChecks * 16.0f;
			if (disableCortMagFac)
			{
				currentCompleteWedgeDiameter = ((nStimFrameHeight - gapDiameter) * cortMagFactor) / 2.0f;
				//currentCompleteWedgeDiameter = ((nStimFrameHeight - gapDiameter)) / 2.0f;
				currentWedgeDiameter = currentCompleteWedgeDiameter / eccentricityNrRings;
				if(eccentricityDirection == -1)//Decrease ring diameter
					currentOuterCompleteRingDiameter = nStimFrameHeight - ((nStimFrameHeight - gapDiameter - (2*currentCompleteWedgeDiameter)) * fTrialTimeProgress);
				else//Increase ring diameter
					currentOuterCompleteRingDiameter = (gapDiameter + (2*currentCompleteWedgeDiameter)) + ((nStimFrameHeight - gapDiameter - (2*currentCompleteWedgeDiameter)) * fTrialTimeProgress);
			} 
			else
			{
				if(eccentricityDirection == -1)//Decrease ring diameter
				{
					currentCompleteWedgeDiameter = (((nStimFrameHeight - gapDiameter) * cortMagFactor) - ((nStimFrameHeight - gapDiameter) * fTrialTimeProgress * cortMagFactor)) / 2.0f;
					currentWedgeDiameter = currentCompleteWedgeDiameter / eccentricityNrRings;
					currentOuterCompleteRingDiameter = nStimFrameHeight - ((nStimFrameHeight - gapDiameter) * fTrialTimeProgress);
				}
				else//Increase ring diameter
				{
					currentCompleteWedgeDiameter = (((nStimFrameHeight - gapDiameter) / 2.0f) * fTrialTimeProgress * cortMagFactor) ;
					currentWedgeDiameter = currentCompleteWedgeDiameter / eccentricityNrRings;
					currentOuterCompleteRingDiameter = gapDiameter + ((nStimFrameHeight - gapDiameter) * fTrialTimeProgress);
				}
			}
			currentXPoint = (nStimFrameWidth - currentOuterCompleteRingDiameter + currentWedgeDiameter) / 2.0f;
			currentYPoint = (nStimFrameHeight - currentOuterCompleteRingDiameter + currentWedgeDiameter) / 2.0f;
			if(bCreateActivationMap)
			{
				activationPainter->setPen(QPen(whiteColor, currentCompleteWedgeDiameter, style, flatCap));
				float fTemp = currentOuterCompleteRingDiameter - (currentWedgeDiameter*eccentricityNrRings);
				activationPainter->drawEllipse((nStimFrameWidth - fTemp) / 2.0f, (nStimFrameHeight - fTemp) / 2.0f, fTemp, fTemp);			
			}
			for(int i=1;i<eccentricityNrRings+1;i++)
			{
				float startAngle = 0.0f * 16.0f;
				for(int k=0; k<eccentricityNrChecks; k++)
				{
					if(flickrSwitch ==1)
					{
						if((k+i)%2==0)
						{
							imgPainter->setPen(QPen(cCheckerColor1, currentWedgeDiameter, style, flatCap));
						}
						else
						{
							imgPainter->setPen(QPen(cCheckerColor2, currentWedgeDiameter, style, flatCap));
						}
					}
					else
					{
						if((k+i)%2!=0)
						{
							imgPainter->setPen(QPen(cCheckerColor1, currentWedgeDiameter, style, flatCap));
						}
						else
						{
							imgPainter->setPen(QPen(cCheckerColor2, currentWedgeDiameter, style, flatCap));
						}
					}
					/*
					if((k==0) && (i==1))//draw a full complete ring
					{
						//imgPainter->setPen(QPen(QColor(255,0,0), currentCompleteWedgeDiameter, style, flatCap));//cCheckerColor1
						//imgPainter->drawEllipse(currentXPoint, currentYPoint, currentOuterCompleteRingDiameter - currentWedgeDiameter, currentOuterCompleteRingDiameter - currentWedgeDiameter, startAngle, 360.0f * 16.0f);

						imgPainter->setPen(QPen(QColor(255,0,0), currentCompleteWedgeDiameter, style, flatCap));
						float fTemp = currentOuterCompleteRingDiameter - (currentWedgeDiameter*eccentricityNrRings);
						imgPainter->drawArc((nStimFrameWidth - fTemp) / 2.0f, (nStimFrameHeight - fTemp) / 2.0f, fTemp, fTemp, 0.0f, 5760.0f);

						imgPainter->drawArc(currentXPoint, currentYPoint, currentOuterCompleteRingDiameter - currentWedgeDiameter, currentOuterCompleteRingDiameter - currentWedgeDiameter, startAngle, 360.0f * 16.0f);
					}					
					else if((k+i)%2!=0)//old --> else if (k%2!=0)//Only odds
					{
						imgPainter->drawArc(currentXPoint, currentYPoint, currentOuterCompleteRingDiameter - currentWedgeDiameter, currentOuterCompleteRingDiameter - currentWedgeDiameter, startAngle, wedgeSpanAngle);
					}
					*/

					if(k==0)//first checker? 
					{
						if(i==1)//first ring?
						{//draw a full complete ring
							oldPen = imgPainter->pen();
							imgPainter->setPen(QPen(cCheckerColor1, currentCompleteWedgeDiameter, style, flatCap));
							float fTemp = currentOuterCompleteRingDiameter - (currentWedgeDiameter*eccentricityNrRings);
							//imgPainter->drawArc((nStimFrameWidth - fTemp) / 2.0f, (nStimFrameHeight - fTemp) / 2.0f, fTemp, fTemp, 0.0f, 5760.0f);
							imgPainter->drawEllipse((nStimFrameWidth - fTemp) / 2.0f, (nStimFrameHeight - fTemp) / 2.0f, fTemp, fTemp);
							imgPainter->setPen(oldPen);
						}
						imgPainter->drawEllipse(currentXPoint, currentYPoint, currentOuterCompleteRingDiameter - currentWedgeDiameter, currentOuterCompleteRingDiameter - currentWedgeDiameter);
					}					
					else if (k%2!=0)
					{
						imgPainter->drawArc(currentXPoint, currentYPoint, currentOuterCompleteRingDiameter - currentWedgeDiameter, currentOuterCompleteRingDiameter - currentWedgeDiameter, startAngle, wedgeSpanAngle);
					}
					startAngle = startAngle + wedgeSpanAngle;
				}
				//if(isDebugMode() && (debugUsedTestSamples==debugTestSamples))
				//{
				//	imgPainter->setPen(QPen(QColor(255,0,0), 1, style, cap));
				//	imgPainter->drawEllipse(currentXPoint, currentYPoint, currentSize, currentSize);//To draw this it takes about 2ms additional time!!
				//}
				if(eccentricityDirection == -1)//Decrease ring diameter
				{
					currentOuterCompleteRingDiameter = currentOuterCompleteRingDiameter - (2*currentWedgeDiameter);
				}
				else//Increase ring diameter
				{
					currentOuterCompleteRingDiameter = currentOuterCompleteRingDiameter - (2*currentWedgeDiameter);
				}
				currentXPoint = (nStimFrameWidth - currentOuterCompleteRingDiameter + currentWedgeDiameter) / 2.0f;
				currentYPoint = (nStimFrameHeight - currentOuterCompleteRingDiameter + currentWedgeDiameter) / 2.0f;
			}		
		}
		//if(isDebugMode())
		//	currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"",QString("SubPainting the object"),QString::number(3));
		if(showFixationPoint) // show fix cross
		{
			imgPainter->setPen(QPen(fixationColor, fixationSize, style, roundCap));
			imgPainter->drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			if(bCreateActivationMap)
			{
				activationPainter->setPen(QPen(whiteColor, fixationSize, style, roundCap));
				activationPainter->drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			}
		}
		break;
	case RetinoMap_MovingBar:
		{
		//if(showFixationPoint) // show fix cross
		//{
		//	imgPainter->setPen(QPen(fixationColor, fixationSize, style, roundCap));
		//	imgPainter->drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
		//	if(bCreateActivationMap)
		//	{				
		//		activationPainter->setPen(QPen(whiteColor, fixationSize, style, roundCap));
		//		activationPainter->drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
		//	}
		//}
		//if(isDebugMode() && (debugUsedTestSamples==debugTestSamples))
		//{
		//	imgPainter->setPen(QPen(QColor(255,0,0), 0, style, cap));
		//	imgPainter->drawRect((nStimFrameWidth/2)-(nStimFrameWidth/2),(nStimFrameHeight/2)-(nStimFrameHeight/2),nStimFrameWidth,nStimFrameHeight);
		//}
		float fYOffset = 0.0f;
		if (bRenderStimuli)
		{
			fStimulusDiameter = qSqrt(qPow(nStimFrameWidth,2) + qPow(nStimFrameHeight,2));//qSqrt(qPow(nStimFrameWidth,2) + qPow(nStimFrameHeight,2));
			imgPainter->translate(nStimFrameWidth/2, nStimFrameHeight/2);
			imgPainter->rotate(movingBarAngle);
			if(bCreateActivationMap)
			{
				activationPainter->translate(nStimFrameWidth/2, nStimFrameHeight/2);
				activationPainter->rotate(movingBarAngle);
			}
			currentSize = (movingBarCoverage * fStimulusDiameter)/(movingBarHeight*movingBarHeightCheckAmount);
			if(movingBarDirection == -1)//Down->Up (When 0 <= movingBarAngle >= 180 degrees)
			{				
				//if (movingBarIncludeOppositeDirection)
				//{
				//	if (fTrialTimeProgress < 0.5f)
				//		fYOffset = (((0.5 * movingBarCoverage * fStimulusDiameter) - (0.5 * currentSize * movingBarHeightCheckAmount) + (0.5 * currentSize)) - (((movingBarCoverage * fStimulusDiameter) - (currentSize * movingBarHeightCheckAmount) ) * (fTrialTimeProgress*2)));
				//	else
				//		fYOffset = (((-0.5 * movingBarCoverage * fStimulusDiameter) + (0.5 * currentSize * movingBarHeightCheckAmount) + (0.5 * currentSize)) + (((movingBarCoverage * fStimulusDiameter) - (currentSize * movingBarHeightCheckAmount) ) * ((fTrialTimeProgress-0.5f)*2)));
				//} 
				//else
					fYOffset = (((0.5 * movingBarCoverage * fStimulusDiameter) - (0.5 * currentSize * movingBarHeightCheckAmount) + (0.5 * currentSize)) - (((movingBarCoverage * fStimulusDiameter) - (currentSize * movingBarHeightCheckAmount) ) * fTrialTimeProgress));
			}
			else//Up->Down (When 0 <= movingBarAngle >= 180 degrees)
			{
				//if (movingBarIncludeOppositeDirection)
				//{
				//	if (fTrialTimeProgress < 0.5f)
				//		fYOffset = (((-0.5 * movingBarCoverage * fStimulusDiameter) + (0.5 * currentSize * movingBarHeightCheckAmount) + (0.5 * currentSize)) + (((movingBarCoverage * fStimulusDiameter) - (currentSize * movingBarHeightCheckAmount) ) * (fTrialTimeProgress*2)));
				//	else
				//		fYOffset = (((0.5 * movingBarCoverage * fStimulusDiameter) - (0.5 * currentSize * movingBarHeightCheckAmount) + (0.5 * currentSize)) - (((movingBarCoverage * fStimulusDiameter) - (currentSize * movingBarHeightCheckAmount) ) * ((fTrialTimeProgress-0.5f)*2)));
				//} 
				//else
					fYOffset = (((-0.5 * movingBarCoverage * fStimulusDiameter) + (0.5 * currentSize * movingBarHeightCheckAmount) + (0.5 * currentSize)) + (((movingBarCoverage * fStimulusDiameter) - (currentSize * movingBarHeightCheckAmount) ) * fTrialTimeProgress));
			}

			//if(isDebugMode())
			//	currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"",QString("SubPainting the object"),QString("3a"));
			imgPainter->translate(0,fYOffset);
			//if(isDebugMode())
			//	currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"",QString("SubPainting the object"),QString("3b"));
			if(bCreateActivationMap)
				activationPainter->translate(0,fYOffset);
			currentYPoint = (-1 * movingBarHeightCheckAmount * currentSize) / 2.0f;
			movingBarWidthCheckAmount = ((int)(fStimulusDiameter/currentSize)+1);
			if(bCreateActivationMap)
			{
				activationPainter->setPen(QPen(whiteColor, currentSize*movingBarHeightCheckAmount, style, flatCap));
				activationPainter->drawLine(-fStimulusDiameter/2, -0.5 * currentSize, -fStimulusDiameter/2+(currentSize*movingBarWidthCheckAmount), -0.5 * currentSize);
			}
			//if(isDebugMode())
			//	currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"",QString("SubPainting the object"),QString("3c"));
			imgPainter->drawImage(-fStimulusDiameter/2,currentYPoint-(0.5 * currentSize),fractalFillCheckeredImage(currentSize*movingBarWidthCheckAmount,movingBarHeightCheckAmount * currentSize,currentSize,flickrSwitch));
		}
		//if(isDebugMode())
		//	currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"",QString("SubPainting the object"),QString("3d"));
		if(showFixationPoint) // show fix cross
		{
			imgPainter->setPen(QPen(fixationColor, fixationSize, style, roundCap));
			if (bRenderStimuli)
				imgPainter->drawPoint(0.0f,-fYOffset);
			else
				imgPainter->drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			if(bCreateActivationMap)
			{				
				activationPainter->setPen(QPen(whiteColor, fixationSize, style, roundCap));
				if (bRenderStimuli)
					activationPainter->drawPoint(0.0f,-fYOffset);
				else
					activationPainter->drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			}
		}
		break;
	}
	case RetinoMap_MovingDots:
	{
		if (bRenderStimuli)
		{
		//if ((emptyTriggerSteps==0)||(emptyTriggerStepCount<1)||((emptyTriggerStepCount>0)&&(randEmptyStimGenerator->at(0).toInt()!=currExpBlockTrialTrigger%((int)cycleTriggerAmount))))
		//{
		//	emptyTriggerStepCount--;
		//	randEmptyStimGenerator->removeFirst();
		//if (randEmptyStimGenerator->at(currExpBlockTrialTrigger%((int)cycleTriggerAmount)) == RETINOMAP_WIDGET_BOOL_FALSE)
		//{
			int i;
			imgPainter->setPen(QPen(dotColor, movingDotsDotSize, style, roundCap));
			int draw_page, disp_page;
			disp_page = expSnapshot.currExpBlockTrialFrame % 2;
			draw_page = (expSnapshot.currExpBlockTrialFrame+1) % 2;

			//double dPI = DOUBLE_PI_VALUE;
			//float PI = (float)dPI;
			//float ToRad = PI / 180.0f;

			for(i=0; i<movingDotsNrOfDots; i++)
			{
				if(movingDotsIsStationary)
				{
					movingDotsXValue[draw_page][i] = movingDotsXValue[disp_page][i];
					movingDotsYValue[draw_page][i] = movingDotsYValue[disp_page][i];
					movingDots.Pos[i] = movingDots.OldPos[i];
						//.setX(movingDots.Pos.at(i).x());
					//movingDots.OldPos.at(i).setY(movingDots.Pos.at(i).y());
				}
				else
				{
					// move dot forward
					movingDotsXValue[draw_page][i] = movingDotsXValue[disp_page][i] + movingDotsBGXDir[i];
					movingDotsYValue[draw_page][i] = movingDotsYValue[disp_page][i] + movingDotsBGYDir[i];
					// check whether dot moves out of display field - x direction
					if(movingDotsXValue[draw_page][i] >= (movingDotsXStartRel + (movingDotsXWidth)))
						movingDotsXValue[draw_page][i] = movingDotsXStartRel + (movingDotsXValue[draw_page][i] - (movingDotsXStartRel + movingDotsXWidth)) + movingDotsBGXDir[i];
					if(movingDotsXValue[draw_page][i] <= movingDotsXStartRel)
						movingDotsXValue[draw_page][i] = (movingDotsXStartRel + movingDotsXWidth) - (movingDotsXStartRel - movingDotsXValue[draw_page][i]) - movingDotsBGXDir[i];
					// check whether dot moves out of display field - y direction
					if(movingDotsYValue[draw_page][i] >= (movingDotsYStartRel + movingDotsYWidth))
						movingDotsYValue[draw_page][i] = movingDotsYStartRel + (movingDotsYValue[draw_page][i] - (movingDotsYStartRel + movingDotsYWidth)) + movingDotsBGYDir[i];
					if(movingDotsYValue[draw_page][i] <= movingDotsYStartRel)
						movingDotsYValue[draw_page][i] = (movingDotsYStartRel + movingDotsYWidth) - (movingDotsYStartRel - movingDotsYValue[draw_page][i]) - movingDotsBGYDir[i];

					// move dot forward
					movingDots.Pos[i] = movingDots.OldPos[i] + movingDots.Mov[i].p2();
					// check whether dot moves out of display field - x direction					
					if(movingDots.Pos.at(i).x() >= (movingDotsXStartRel + (movingDotsXWidth)))
						movingDots.Pos[i].setX(movingDotsXStartRel + (movingDots.Pos.at(i).x() - (movingDotsXStartRel + movingDotsXWidth)) + movingDots.Mov.at(i).p2().x());
					if(movingDots.Pos.at(i).x() <= movingDotsXStartRel)
						movingDots.Pos[i].setX((movingDotsXStartRel + movingDotsXWidth) - (movingDotsXStartRel - movingDots.Pos.at(i).x()) - movingDots.Mov.at(i).p2().x());
					// check whether dot moves out of display field - y direction
					if(movingDots.Pos.at(i).y() >= (movingDotsYStartRel + movingDotsYWidth))
						movingDots.Pos[i].setY(movingDotsYStartRel + (movingDots.Pos.at(i).y() - (movingDotsYStartRel + movingDotsYWidth)) + movingDots.Mov.at(i).p2().y());
					if(movingDots.Pos.at(i).y() <= movingDotsYStartRel)
						movingDots.Pos[i].setY((movingDotsYStartRel + movingDotsYWidth) - (movingDotsYStartRel - movingDots.Pos.at(i).y()) - movingDots.Mov.at(i).p2().y());
				}
			}

			if(movingDotsHemifieldPos == RetinoMap_HemifieldPos_Both)
			{
				movingDotsXValueMirror.resize(2); 
				movingDotsXValueMirror[0].resize(movingDotsNrOfDots); 
				movingDotsXValueMirror[1].resize(movingDotsNrOfDots);
				for(i=0; i<movingDotsNrOfDots; i++)
				{
					movingDotsXValueMirror[draw_page][i] = movingDotsXValue[draw_page][i] + movingDotsXWidth + 2*movingDotsPixelFromCenter;
				}
			}
			for(i=0; i<movingDotsNrOfDots; i++)
			{
				if(movingDotsHemifieldPos== RetinoMap_HemifieldPos_Both)
				{
					//if(1) // draw with float points
					//{
						QPointF px = QPointF(movingDotsXValue[draw_page][i], movingDotsYValue[draw_page][i]);
						QPointF px_mirror = QPointF(movingDotsXValueMirror[draw_page][i], movingDotsYValue[draw_page][i]);
						imgPainter->drawPoint(px);
						imgPainter->drawPoint(px_mirror);
					//}
					//else // draw with int points
					//	// imgPainter->drawPoint(movingDotsXValue[draw_page][i], movingDotsYValue[draw_page][i]); // Jan - I deleted this line, otherwise next line would be called always (outside of if/else)
					//	imgPainter->drawPoint(movingDotsXValueMirror[draw_page][i], movingDotsYValue[draw_page][i]);
				}
				else
				{
					//if(1) // draw with float points
					//{
						QPointF px = QPointF(movingDotsXValue[draw_page][i], movingDotsYValue[draw_page][i]);
						imgPainter->drawPoint(px);
					//}
					//else // draw with int points
					//	imgPainter->drawPoint(movingDotsXValue[draw_page][i], movingDotsYValue[draw_page][i]);
				}
			}		
			//if(movingDotsRetPosition>0)
			//{
			//	QPainterPath ret_apeture;
			//	switch(movingDotsRetPosition)
			//	{
			//	case 1:
			//		ret_apeture.moveTo(300,0);
			//		ret_apeture.lineTo(300,rectScreenRes.height());
			//		ret_apeture.lineTo(rectScreenRes.width(),rectScreenRes.height());
			//		ret_apeture.lineTo(rectScreenRes.width(),0);
			//		ret_apeture.lineTo(rectScreenRes.width()/2,rectScreenRes.height()/2);
			//		ret_apeture.lineTo(rectScreenRes.width()/2,0);
			//		ret_apeture.lineTo(300,0);
			//		break;
			//	case 2:
			//		ret_apeture.moveTo(300,0);
			//		ret_apeture.lineTo(300,rectScreenRes.height());
			//		ret_apeture.lineTo(rectScreenRes.width(),rectScreenRes.height());
			//		ret_apeture.lineTo(rectScreenRes.width(),rectScreenRes.height()/2);
			//		ret_apeture.lineTo(rectScreenRes.width()/2,rectScreenRes.height()/2);
			//		ret_apeture.lineTo(rectScreenRes.width(),0);
			//		ret_apeture.lineTo(300,0);
			//		break;
			//	case 3:
			//		ret_apeture.moveTo(300,0);
			//		ret_apeture.lineTo(300,rectScreenRes.height());
			//		ret_apeture.lineTo(rectScreenRes.width(),rectScreenRes.height());
			//		ret_apeture.lineTo(rectScreenRes.width()/2,rectScreenRes.height()/2);
			//		ret_apeture.lineTo(rectScreenRes.width(),rectScreenRes.height()/2);
			//		ret_apeture.lineTo(rectScreenRes.width(),0);
			//		ret_apeture.lineTo(300,0);
			//		break;
			//	case 4:
			//		ret_apeture.moveTo(300,0);
			//		ret_apeture.lineTo(300,rectScreenRes.height());
			//		ret_apeture.lineTo(rectScreenRes.width()/2,rectScreenRes.height());
			//		ret_apeture.lineTo(rectScreenRes.width()/2,rectScreenRes.height()/2);
			//		ret_apeture.lineTo(rectScreenRes.width(),rectScreenRes.height());
			//		ret_apeture.lineTo(rectScreenRes.width(),0);
			//		ret_apeture.lineTo(300,0);
			//		break;
			//	}
			//	ret_apeture.closeSubpath();
			//	imgPainter->setBrush(brushBackground);
			//	imgPainter->setPen(colorBackground);//(87, 87, 87));
			//	imgPainter->drawPath(ret_apeture);
			//}
		}
		//if(isDebugMode())
		//	currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"",QString("SubPainting the object"),QString::number(3));
		if(showFixationPoint) // show fix cross
		{
			imgPainter->setPen(QPen(fixationColor, fixationSize, style, roundCap));
			imgPainter->drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			if(bCreateActivationMap)
			{
				activationPainter->setPen(QPen(whiteColor, fixationSize, style, roundCap));
				activationPainter->drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			}
		}
		break;
	}
	case RetinoMap_Fixation:
		if(showFixationPoint) // show fix cross
		{
			imgPainter->setPen(QPen(fixationColor, fixationSize, style, roundCap));
			imgPainter->drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			if(bCreateActivationMap)
			{
				activationPainter->setPen(QPen(whiteColor, fixationSize, style, roundCap));			
				activationPainter->drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			}
		}
		break;
	}
	if ((bCreateActivationMap) && (activationPainter))
		activationPainter->end();
	imgPainter->end();
	if (outputTriggerFrame)
	{
		if (expSnapshot.currExpTrigger > lastTriggerNumber)
		{
			QFile file;
			QString outputDir = MainAppInfo::outputsDirPath();
			outputDir = outputDir + RETINOMAP_WIDGET_OUTPUT_SUBFOLDER;
			if(QDir(outputDir).exists()==false)
			{
				QDir().mkdir(outputDir);
			}
			outputDir = outputDir + getLastLoggedObjectStateTime(Experiment_SubObject_Started) + "/";
			if(QDir(outputDir).exists()==false)
			{
				QDir().mkdir(outputDir);
			}
			QString fileName = outputDir + QString::number(nRetinoID) + "_" + QString::number(expSnapshot.currExpBlock) + QString("_") + QString::number(expSnapshot.currExpTrial) + QString("_") + QString::number(expSnapshot.currExpTrigger);

			switch (retinoOutputType)
			{
			case RetinoMap_OutputType_Frame:
				if (retinoOutputFormat == RetinoMap_OutputFormat_PNG)
				{
					fileName = fileName + QString(".png");
					file.setFileName(fileName);
					if (file.open(QIODevice::WriteOnly))
					{
						StimulusResultImageFrame->save(&file, "PNG");
						file.close();
					}
					break;
				} 
				else if (retinoOutputFormat == RetinoMap_OutputFormat_DAT)
				{
					fileName = fileName + QString(".dat");
					file.setFileName(fileName);
					if (file.open(QIODevice::WriteOnly))
					{
						QImage imgWrite = StimulusResultImageFrame->toImage().convertToFormat(QImage::Format_ARGB32);
						QDataStream output(&file);
						quint32 magic = ARGB_FORMAT_HEADER;
						quint32 width = imgWrite.width();
						quint32 height = imgWrite.height();
						output << magic << width << height;
						for (quint32 y = 0; y < height; ++y) 
						{
							QRgb *scanLine = (QRgb *)imgWrite.scanLine(y);
							for (quint32 x = 0; x < width; ++x)
							{
								output << scanLine[x];
							}
						}
						//bool bResult = 
						//output.status() == QDataStream::Ok;
						file.close();
					}
					break;
				}
			case RetinoMap_OutputType_Mask:
				if (retinoOutputFormat == RetinoMap_OutputFormat_PNG)
				{
					fileName = fileName + QString(".png");
					file.setFileName(fileName);
					if (file.open(QIODevice::WriteOnly))
					{
						StimulusActivationMap->save(&file, "PNG");
						file.close();
					}
					break;
				} 
				else if (retinoOutputFormat == RetinoMap_OutputFormat_DAT)
				{
					fileName = fileName + QString(".dat");
					file.setFileName(fileName);
					if (file.open(QIODevice::WriteOnly))
					{
						QImage imgWrite = StimulusActivationMap->toImage().convertToFormat(QImage::Format_ARGB32);
						QDataStream output(&file);
						quint32 magic = ARGB_FORMAT_HEADER;
						quint32 width = imgWrite.width();
						quint32 height = imgWrite.height();
						output << magic << width << height;
						for (quint32 y = 0; y < height; ++y) 
						{
							QRgb *scanLine = (QRgb *)imgWrite.scanLine(y);
							for (quint32 x = 0; x < width; ++x)
							{
								output << scanLine[x];
							}
						}
						//bool bResult = 
						//output.status() == QDataStream::Ok;
						file.close();
					}
					break;
				}
			}
			lastTriggerNumber++;
		}
	}
	//if(isDebugMode())
	//	currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"",QString("SubPainting the object"),QString::number(4));
	if (stimuliPainter == NULL)
		stimuliPainter = new QPainter(this);//Constructor automatically calls begin()
	else
		stimuliPainter->begin(this);
	stimuliPainter->fillRect(event->rect(), brushBackground);
	stimuliPainter->setPen(textPen);
	stimuliPainter->setFont(textFont);
	//painter.drawPixmap((rectScreenRes.width()-nStimFrameWidth)/2,(rectScreenRes.height()-nStimFrameHeight)/2,*StimulusResultImageFrame);
	stimuliPainter->drawPixmap(0,0,*StimulusResultImageFrame);

	//if(isDebugMode())
	//	currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"",QString("SubPainting the object"),QString::number(5));

	if(isDebugMode())
	{
		//if (debugUsedTestSamples == -1)//This is the first experiment sample and is ignored for the speed measurement
		//{
		//	debugTotalElapsedTime = 0;
		//	debugUsedTestSamples++;
		//}
		//else if (debugUsedTestSamples<debugTestSamples)//During these samples we collect the information for the speed measurement
		//{
		//	debugTotalElapsedTime = debugTotalElapsedTime + debugElapsedTime;
		//	debugUsedTestSamples++;
		//	debugString = debugString + ", " + QString::number(debugElapsedTime);
		//}
		//else if (debugUsedTestSamples==debugTestSamples)//During this sample all the debugging functionality is processed and also ignored for the speed measurement
		//{
		//float fAverageElapsed = debugTotalElapsedTime/debugUsedTestSamples;
		//textContent = QString("Average elapsed time: %1").arg(fAverageElapsed); //+ ": " + debugString; //"Retinotopic Mapping";
		//painter.drawText(QRect(10, 0, rectScreenRes.width(), 100), Qt::AlignLeft, textContent);//Takes about 5ms additional total drawing time!
		textContent = "TriggerNr:" + QString::number(expSnapshot.currExpTrigger) + ", TrialNr:" + QString::number(expSnapshot.currExpTrial) + ", BlockNr:" + QString::number(expSnapshot.currExpBlock);
		stimuliPainter->drawText(QRect(10, 50, stimWidthPixelAmount, 100), Qt::AlignLeft, textContent);
		//debugUsedTestSamples++;
		//}
		//else if (debugUsedTestSamples==(debugTestSamples+1))//During this sample we reset all the information for the speed measurement again
		//{
		//	debugTotalElapsedTime = 0;
		//	debugUsedTestSamples = 0;
		//}
	}
	stimuliPainter->end();
	//if(isDebugMode())
	//{
	//	currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"","painter.end()");
	//	//emit LogExpObjData(nRetinoID,0,"paintEvent():painter.end()2");//0 is the default experiment timer
	//}
	//if(isDebugMode())
	//	currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"",QString("SubPainting the object"),QString::number(6));
	nextNewCycleEntered = false;
	nextNewBlockEntered = false;
	if(isDebugMode())
		currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"","Finished painting the object");
	return true;
}

