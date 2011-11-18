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
	if (randGen)
	{
		delete randGen;
		randGen = NULL;
	}
}

void RetinoMap_glwidget::initialize()
{
	//Place your custom initialization code here:
	
	//Default values if none defined.
	nRetinoID = -1;
	rectScreenRes = getScreenResolution();
	firstBlockTrialPaintFrame = false;	
	lastTriggerNumber = -1;
	flickrSwitch = 0;
	textContent = "";
	colorBackground = QColor(0,0,0);
	brushBackground = QBrush(colorBackground);//(87, 87, 87));
	textPen = QPen(Qt::white);
	textFont.setPixelSize(20);
	fixationColor = QColor(255, 0, 0);
	textContent = "";
	StimulusResultImageFrame = NULL;
	StimulusActivationMap = NULL;
	activationPainter = NULL;
	currExpConfStruct = NULL;
	randGen = NULL;
	elapsedTrialTime = 0;
	//debugString = "";
	//debugElapsedTime = 0;
	whiteColor = QColor(255,255,255);
	blackColor = QColor(0,0,0);
	color1 = QColor(255, 255, 255);	//For the stimuli
	color2 = QColor(0, 0, 0);		//For the stimuli
	style = Qt::SolidLine;			
	flatCap = Qt::FlatCap;
	roundCap = Qt::RoundCap;
}

void RetinoMap_glwidget::parseExperimentObjectBlockParameters(bool bInit)
{	
	if (bInit)
	{
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
		eccentricityDirection = 1;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_ECCENTRICITY_DIRECTION,QString::number(eccentricityDirection));
		movingBarAngle = 45.0;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_ANGLE,QString::number(movingBarAngle));
		movingBarCoverage = 0.5;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_COVERAGE,QString::number(movingBarCoverage));
		movingBarWidthCheckAmount = 20;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_WIDTH_CHECK_AMOUNT,QString::number(movingBarWidthCheckAmount));
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
		flickrSpeedFreq = 5;//5 times per second
		flickrThreshold = 1000 / flickrSpeedFreq;//First at 200 mSecs
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_FLICKRSPEED_HZ,QString::number(flickrSpeedFreq));
		cortMagFactor = 0.2f;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_CORTMAG_FACTOR,QString::number(cortMagFactor));
		triggerDurationMsec = 2000;
		insertExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_TRIGGERDURATION_MSEC,QString::number(triggerDurationMsec));
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
		QString tmpString = "";
		randGen = new RandomGenerator();//Here we can initialize the generator so each block we have it correctly constructed
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
		fixationSize = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_FIXSIZE,QString::number(fixationSize)).toInt();
		cycleTriggerAmount = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_CYCLE_TRIGGER_AMOUNT,QString::number(cycleTriggerAmount)).toFloat();
		flickrSpeedFreq = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_FLICKRSPEED_HZ,QString::number(flickrSpeedFreq)).toInt();
		flickrThreshold = 1000 / flickrSpeedFreq;
		cortMagFactor = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_CORTMAG_FACTOR,QString::number(cortMagFactor)).toFloat();
		triggerDurationMsec = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_TRIGGERDURATION_MSEC,QString::number(triggerDurationMsec)).toInt();
		gapDiameter = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_GAP_DIAMETER,QString::number(gapDiameter)).toInt();
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
			movingBarWidthCheckAmount = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_WIDTH_CHECK_AMOUNT,QString::number(movingBarWidthCheckAmount)).toInt();
			movingBarHeightCheckAmount = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_HEIGTH_CHECK_AMOUNT,QString::number(movingBarHeightCheckAmount)).toInt();
			movingBarDirection = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_DIRECTION,QString::number(movingBarDirection)).toInt();
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
}

//void RetinoMap_glwidget::initExperimentObject()
//{
//	//Virtual, don't forget to call the base member first!
//	GLWidgetWrapper::initExperimentObject();
//	//Additional code:	
//}

//bool RetinoMap_glwidget::loadBlockTrial()
//{
//	//Virtual, don't forget to call the base member first!
//	if (!GLWidgetWrapper::loadBlockTrial())
//		return false;
//	//Additional code:
//	
//	return true;
//}

bool RetinoMap_glwidget::initExperimentObject()
{
	//Virtual, don't forget to call the base member first!
	if(!GLWidgetWrapper::initExperimentObject()) 
		return false;
	//Additional code:
	
	parseExperimentObjectBlockParameters(true);
	return true;
}

bool RetinoMap_glwidget::startExperimentObject()
{
	//Virtual, don't forget to call the base member first!
	if(!GLWidgetWrapper::startExperimentObject()) 
		return false;
	//Additional code:
	//if (isDebugMode())
	//{
	//	screenShotWidget = new Screenshot();
	//	screenShotWidget->show();
	//}
	lastTriggerNumber = -1;
	emit LogToOutputWindow("Started");
	return true;
}

//bool RetinoMap_glwidget::abortExperimentObject()
//{
//	//Virtual, don't forget to call the base member first!
//	if(!GLWidgetWrapper::abortExperimentObject()) 
//		return false;
//	//Additional code:
//
//	return true;
//}

//bool RetinoMap_glwidget::stopExperimentObject()
//{
//	//Virtual, don't forget to call the base member first!
//	if(!GLWidgetWrapper::stopExperimentObject())
//		return false;
//	//Additional code:
//
//	return true;
//}

//bool RetinoMap_glwidget::setBlockTrialDomNodeList(QDomNodeList *pDomNodeList)
//{
//	//Virtual, don't forget to call the base member first!
//	if(!GLWidgetWrapper::setBlockTrialDomNodeList(pDomNodeList))
//		return false;
//	//Additional code:
//
//	return true;
//}

bool RetinoMap_glwidget::setExperimentConfiguration(ExperimentConfiguration *pExpConfStruct)
{
	//Virtual, don't forget to call the base member first!
	if(!GLWidgetWrapper::setExperimentConfiguration(pExpConfStruct))
		return false;
	//Additional code:
	currExpConfStruct = pExpConfStruct;

	return true;
}

bool RetinoMap_glwidget::setExperimentObjectID(int nObjID)
{
	//Virtual, don't forget to call the base member first!
	if(!GLWidgetWrapper::setExperimentObjectID(nObjID))
		return false;
	//Additional code:

	nRetinoID = getObjectID();//Because we need it often we'll buffer it here
	return true;
}

void RetinoMap_glwidget::initBlockTrial()
{
	//debugTime.start();
	//Virtual, don't forget to call the base member first!
	GLWidgetWrapper::initBlockTrial();
	//Additional code:

	//int currentExpBlock = getCurrentExperimentBlock();
	//QString tmpParamValue;
	//debugString = "";
	parseExperimentObjectBlockParameters(false);

	//Some variable initializations
	flickrSwitch = 0;
	StimulusResultImageFrame = new QPixmap(stimWidthPixelAmount,stimHeigthPixelAmount);
	StimulusActivationMap = new QPixmap(stimWidthPixelAmount,stimHeigthPixelAmount);
	//debugUsedTestSamples = -1;
	//debugTotalElapsedTime = 0;
	//debugString = "";
	firstBlockTrialPaintFrame = true;
	//debugInitBlockTrialTime = debugTime.restart();
	trialTime.start();
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
		movingDotsXValue[0][i] = randGen->randomize(movingDotsXStartRel,movingDotsXStartRel+(movingDotsXWidth-1)); //movingDotsXStartRel + myRandom(movingDotsXWidth-1) + 1;
		movingDotsYValue[0][i] = randGen->randomize(movingDotsYStartRel,movingDotsYStartRel+(movingDotsYWidth-1)); //movingDotsYStartRel + myRandom(movingDotsYWidth-1) + 1;
		movingDotsBGXDir[i] =    cos((float)randGen->randomize(1,360) * (180.0f/DOUBLE_PI_VALUE));//cos((float)randGen->randomize(1,6)); //cos((float)myRandom(6)+1); 
		movingDotsBGXDir[i] *=   movingDotsMoveSpeed;
		movingDotsBGYDir[i] =    -1.0f * sin((float)randGen->randomize(1,360) * (180.0f/DOUBLE_PI_VALUE));//-1.0f * sin((float)randGen->randomize(1,6)); //-1.0f * sin((float)myRandom(6)+1); 
		movingDotsBGYDir[i] *=   movingDotsMoveSpeed;
	}
}

void RetinoMap_glwidget::paintEvent(QPaintEvent *event)
{
	//Virtual, don't forget to call the base member first!
	GLWidgetWrapper::paintEvent(event);
	//Additional code:

	tmpParamValue = "";
	fStimulusDiameter = 0.0f;
	fTrialTimeProgress = 0.0f; 
	bCreateActivationMap = (outputTriggerFrame) && (retinoOutputType==RetinoMap_OutputType_Mask);
	currExpTrigger = getCurrentExperimentTrigger();
	currExpBlockTrialTrigger = getCurrentExperimentBlockTrialTrigger();//This is different!! It is now the current trigger within a Block Trial
	currExpTrial = getCurrentExperimentTrial();
	currExpBlock = getCurrentExperimentBlock();
	currExpBlockTrialFrame = getCurrentExperimentBlockTrialFrame();
	//int currExpBlockTrialTriggerAmount = getExperimentBlockTrialTriggerAmount(currExpBlock,currExpTrial);//The amount of triggers within the current trial

	elapsedTrialTime = trialTime.elapsed();
	//debugElapsedTime = debugTime.restart();
	
	if (discreteTriggerSteps)
	{
		if (currentExpType == RetinoMap_PolarAngle)
		{
			//0-->1 travels from 0 to 360(which is also 0!) degrees
			//To use a full scale spread we'll use the following:
			fTrialTimeProgress = currExpBlockTrialTrigger / cycleTriggerAmount;
		} 
		else
		{
			//0-->1 travels from 0 to 100(which is not 0!) procent
			//To use a full scale spread we'll use the following:
			fTrialTimeProgress = currExpBlockTrialTrigger / (cycleTriggerAmount-1);
		}		
	} 
	else
	{
		fTrialTimeProgress = elapsedTrialTime / (cycleTriggerAmount * triggerDurationMsec);
	}
	StimulusResultImageFrame->fill(colorBackground);
	QPainter imgPainter(StimulusResultImageFrame);
	if (bCreateActivationMap)
	{
		StimulusActivationMap->fill(blackColor);//The activation map should be first filled with 0's
		activationPainter = new QPainter(StimulusActivationMap);
		activationPainter->begin(this);//For the StimulusActivationMap
	}
	imgPainter.begin(this);
	if (antiAliasing)
		imgPainter.setRenderHint(QPainter::Antialiasing);
	imgPainter.setPen(textPen);
	imgPainter.setFont(textFont);
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
	switch (currentExpType)
	{
	case RetinoMap_PolarAngle:
		float startAngle;
		wedgeSpanAngle = polarWedgeSpan / polarWedgeNrChecks * 16.0f;
		currentWedgeDiameter = ((nStimFrameHeight - gapDiameter)) / 2.0f * cortMagFactor;
		currentSize = nStimFrameHeight - currentWedgeDiameter;
		currentXPoint = (nStimFrameWidth - currentSize) / 2.0f;
		currentYPoint = (nStimFrameHeight - currentSize) / 2.0f;
		if(polarRotationDirection == 1)//Clockwise
		{
			startAngle = -360.0f * fTrialTimeProgress;
		}
		else//Counterclockwise 
		{
			startAngle = 360.0f * fTrialTimeProgress;
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
						imgPainter.setPen(QPen(color1, currentWedgeDiameter, style, flatCap));
					}
					else
					{
						imgPainter.setPen(QPen(color2, currentWedgeDiameter, style, flatCap));
					}
				}
				else
				{
					if((k+i)%2!=0)
					{
						imgPainter.setPen(QPen(color1, currentWedgeDiameter, style, flatCap));
					}
					else
					{
						imgPainter.setPen(QPen(color2, currentWedgeDiameter, style, flatCap));
					}
				}
				//if((i==1) || (i==5) || (i==10)) //(i%2)==0)
				imgPainter.drawArc(currentXPoint, currentYPoint, currentSize, currentSize, currentStartAngle, wedgeSpanAngle);//Same as drawing an partial ellipse
				currentStartAngle = currentStartAngle + wedgeSpanAngle;
			}
			//if(isDebugMode() && (debugUsedTestSamples==debugTestSamples))
			//{
			//	imgPainter.setPen(QPen(QColor(255,0,0), 1, style, cap));
			//	imgPainter.drawEllipse(currentXPoint, currentYPoint, currentSize, currentSize);
			//}

			currentSize = currentSize - currentWedgeDiameter;//First subtract the first wedge diameter
			if(i==(polarWedgeNrRings-1))//Pre-last loop to prepare last wedge?
				currentWedgeDiameter = (currentSize-gapDiameter) / 2;
			else
				currentWedgeDiameter = ((currentSize - currentWedgeDiameter) - gapDiameter) / 2 * cortMagFactor;			
			currentSize = currentSize - currentWedgeDiameter;//First subtract the second wedge diameter
			currentXPoint = (nStimFrameWidth - currentSize) / 2.0f;
			currentYPoint = (nStimFrameHeight - currentSize) / 2.0f;
		}
		if(showFixationPoint) // show fix cross
		{
			imgPainter.setPen(QPen(fixationColor, fixationSize, style, roundCap));
			imgPainter.drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			if(bCreateActivationMap)
			{
				activationPainter->setPen(QPen(whiteColor, fixationSize, style, roundCap));
				activationPainter->drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			}
		}
		break;
	case RetinoMap_Eccentricity:
		wedgeSpanAngle = 360.0f / eccentricityNrChecks * 16.0f;
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
		currentXPoint = (nStimFrameWidth - currentOuterCompleteRingDiameter + currentWedgeDiameter) / 2.0f;
		currentYPoint = (nStimFrameHeight - currentOuterCompleteRingDiameter + currentWedgeDiameter) / 2.0f;
		if(bCreateActivationMap)
		{
			activationPainter->setPen(QPen(whiteColor, currentCompleteWedgeDiameter, style, flatCap));
			float fTemp = currentOuterCompleteRingDiameter - (currentWedgeDiameter*eccentricityNrRings);
			activationPainter->drawArc((nStimFrameWidth - fTemp) / 2.0f, (nStimFrameHeight - fTemp) / 2.0f, fTemp, fTemp, 0.0f, 5760.0f);			
		}
		for(int i=1; i<eccentricityNrRings+1;i++)
		{
			float startAngle = 0.0f * 16.0f;
			for(int k=0; k<eccentricityNrChecks; k++)
			{
				if(flickrSwitch ==1)
				{
					if((k+i)%2==0)
					{
						imgPainter.setPen(QPen(color1, currentWedgeDiameter, style, flatCap));
					}
					else
					{
						imgPainter.setPen(QPen(color2, currentWedgeDiameter, style, flatCap));
					}
				}
				else
				{
					if((k+i)%2!=0)
					{
						imgPainter.setPen(QPen(color1, currentWedgeDiameter, style, flatCap));
					}
					else
					{
						imgPainter.setPen(QPen(color2, currentWedgeDiameter, style, flatCap));
					}
				}
				imgPainter.drawArc(currentXPoint, currentYPoint, currentOuterCompleteRingDiameter - currentWedgeDiameter, currentOuterCompleteRingDiameter - currentWedgeDiameter, startAngle, wedgeSpanAngle);
				startAngle = startAngle + wedgeSpanAngle;
			}
			//if(isDebugMode() && (debugUsedTestSamples==debugTestSamples))
			//{
			//	imgPainter.setPen(QPen(QColor(255,0,0), 1, style, cap));
			//	imgPainter.drawEllipse(currentXPoint, currentYPoint, currentSize, currentSize);//To draw this it takes about 2ms additional time!!
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
		if(showFixationPoint) // show fix cross
		{
			imgPainter.setPen(QPen(fixationColor, fixationSize, style, roundCap));
			imgPainter.drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			if(bCreateActivationMap)
			{
				activationPainter->setPen(QPen(whiteColor, fixationSize, style, roundCap));
				activationPainter->drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			}
		}
		break;
	case RetinoMap_MovingBar:
		float fYOffset;
		if(showFixationPoint) // show fix cross
		{
			imgPainter.setPen(QPen(fixationColor, fixationSize, style, roundCap));
			imgPainter.drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			if(bCreateActivationMap)
			{				
				activationPainter->setPen(QPen(whiteColor, fixationSize, style, roundCap));
				activationPainter->drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			}
		}
		//if(isDebugMode() && (debugUsedTestSamples==debugTestSamples))
		//{
		//	imgPainter.setPen(QPen(QColor(255,0,0), 0, style, cap));
		//	imgPainter.drawRect((nStimFrameWidth/2)-(nStimFrameWidth/2),(nStimFrameHeight/2)-(nStimFrameHeight/2),nStimFrameWidth,nStimFrameHeight);
		//}
		fStimulusDiameter = qSqrt(qPow(nStimFrameWidth,2) + qPow(nStimFrameHeight,2));//qSqrt(qPow(nStimFrameWidth,2) + qPow(nStimFrameHeight,2));
		imgPainter.translate(nStimFrameWidth/2, nStimFrameHeight/2);
		imgPainter.rotate(movingBarAngle);
		if(bCreateActivationMap)
		{
			activationPainter->translate(nStimFrameWidth/2, nStimFrameHeight/2);
			activationPainter->rotate(movingBarAngle);
		}
		if(movingBarDirection == -1)//Down->Up (When 0 <= movingBarAngle >= 180 degrees)
		{
			currentSize = fStimulusDiameter/movingBarWidthCheckAmount;
			fYOffset = (((0.5 * movingBarCoverage * fStimulusDiameter) - (0.5 * currentSize * movingBarHeightCheckAmount) + (0.5 * currentSize)) - (((movingBarCoverage * fStimulusDiameter) - (currentSize * movingBarHeightCheckAmount) ) * fTrialTimeProgress));
			imgPainter.translate(0,fYOffset);
			if(bCreateActivationMap)
				activationPainter->translate(0,fYOffset);
		}
		else//Up->Down (When 0 <= movingBarAngle >= 180 degrees)
		{
			currentSize = fStimulusDiameter/movingBarWidthCheckAmount;
			fYOffset = (((-0.5 * movingBarCoverage * fStimulusDiameter) + (0.5 * currentSize * movingBarHeightCheckAmount) + (0.5 * currentSize)) + (((movingBarCoverage * fStimulusDiameter) - (currentSize * movingBarHeightCheckAmount) ) * fTrialTimeProgress));
			//float fYOffset = ((-0.5 * movingBarCoverage * fStimulusDiameter) + ( movingBarCoverage * fStimulusDiameter * fTrialTimeProgress));
			imgPainter.translate(0,fYOffset);
			if(bCreateActivationMap)
				activationPainter->translate(0,fYOffset);
		}
		currentSize = fStimulusDiameter/movingBarWidthCheckAmount;//nStimFrameHeight - (currentWedgeDiameter / 2.0f);
		currentYPoint = (-1 * movingBarHeightCheckAmount * currentSize) / 2.0f;
		if(bCreateActivationMap)
		{
			activationPainter->setPen(QPen(whiteColor, currentSize*movingBarHeightCheckAmount, style, flatCap));
			float fTemp1 = -fStimulusDiameter/2;
			float fTemp2 = -0.5 * currentSize;//2.0f
			activationPainter->drawLine(fTemp1, fTemp2, fTemp1+(currentSize*movingBarWidthCheckAmount), fTemp2);
		}
		for (int i=1; i<movingBarHeightCheckAmount+1;i++)
		{
			currentXPoint = -fStimulusDiameter/2;
			for(int k=0; k<movingBarWidthCheckAmount;k++)
			{
				if(flickrSwitch==1)
				{
					if((k+i)%2==0)
					{	
						//if(isDebugMode() && (k==(movingBarWidthCheckAmount/2)))
						//{
						//	imgPainter.setPen(QPen(QColor(255,0,0), 0, style, cap));
						//	imgPainter.setBrush(QColor(255,0,0));
						//}
						//else
						//{
						imgPainter.setPen(QPen(color1, currentSize, style, flatCap));
						imgPainter.setBrush(color1);
						//}
					}
					else
					{
						//if(isDebugMode()&&(k==(movingBarWidthCheckAmount/2)))
						//{
						//	imgPainter.setPen(QPen(QColor(255,0,0), 0, style, cap));
						//	imgPainter.setBrush(QColor(255,0,0));
						//}
						//else
						//{
						imgPainter.setPen(QPen(color2, currentSize, style, flatCap));
						imgPainter.setBrush(color2);
						//}
					}

				}
				else
				{
					if((k+i)%2!=0)
					{
						//if(isDebugMode()&&(k==(movingBarWidthCheckAmount/2)))
						//{
						//	imgPainter.setPen(QPen(QColor(255,0,0), 0, style, cap));
						//	imgPainter.setBrush(QColor(255,0,0));
						//}
						//else
						//{
						imgPainter.setPen(QPen(color1,currentSize, style, flatCap));
						imgPainter.setBrush(color1);
						//}
					}
					else
					{
						//if(isDebugMode()&&(k==(movingBarWidthCheckAmount/2)))
						//{
						//	imgPainter.setPen(QPen(QColor(255,0,0), 0, style, cap));
						//	imgPainter.setBrush(QColor(255,0,0));
						//}
						//else
						//{
						imgPainter.setPen(QPen(color2, currentSize, style, flatCap));
						imgPainter.setBrush(color2);
						//}
					}
				}

				//if(flickrSwitch==1)
				//{
				//	imgPainter.drawRect(currentXPoint, currentYPoint, currentSize, currentSize);
				//}
				//else
				//{
				imgPainter.drawLine(currentXPoint, currentYPoint, currentXPoint+currentSize, currentYPoint);
				currentXPoint = currentXPoint + currentSize;
			}
			if(movingBarHeightCheckAmount>1)
			{
				currentYPoint = currentYPoint + currentSize;
			}
		}
		break;
	case RetinoMap_MovingDots:
	{
		int i;
		QColor color = QColor(255, 255, 230);
		imgPainter.setPen(QPen(color, movingDotsDotSize, style, roundCap));
		int draw_page, disp_page;
		disp_page = currExpBlockTrialFrame % 2;
		draw_page = (currExpBlockTrialFrame+1) % 2;

		//double dPI = DOUBLE_PI_VALUE;
		//float PI = (float)dPI;
		//float ToRad = PI / 180.0f;

		for(i=0; i<movingDotsNrOfDots; i++)
		{
			if(movingDotsIsStationary)
			{
				movingDotsXValue[draw_page][i] = movingDotsXValue[disp_page][i];
				movingDotsYValue[draw_page][i] = movingDotsYValue[disp_page][i];
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
					imgPainter.drawPoint(px);
					imgPainter.drawPoint(px_mirror);
				//}
				//else // draw with int points
				//	// imgPainter.drawPoint(movingDotsXValue[draw_page][i], movingDotsYValue[draw_page][i]); // Jan - I deleted this line, otherwise next line would be called always (outside of if/else)
				//	imgPainter.drawPoint(movingDotsXValueMirror[draw_page][i], movingDotsYValue[draw_page][i]);
			}
			else
			{
				//if(1) // draw with float points
				//{
					QPointF px = QPointF(movingDotsXValue[draw_page][i], movingDotsYValue[draw_page][i]);
					imgPainter.drawPoint(px);
				//}
				//else // draw with int points
				//	imgPainter.drawPoint(movingDotsXValue[draw_page][i], movingDotsYValue[draw_page][i]);
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
		//	imgPainter.setBrush(brushBackground);
		//	imgPainter.setPen(colorBackground);//(87, 87, 87));
		//	imgPainter.drawPath(ret_apeture);
		//}
		if(showFixationPoint) // show fix cross
		{
			imgPainter.setPen(QPen(fixationColor, fixationSize, style, roundCap));
			imgPainter.drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
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
			imgPainter.setPen(QPen(fixationColor, fixationSize, style, roundCap));
			imgPainter.drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			if(bCreateActivationMap)
			{
				activationPainter->setPen(QPen(whiteColor, fixationSize, style, roundCap));			
				activationPainter->drawPoint(nStimFrameWidth/2, nStimFrameHeight/2);
			}
		}
		break;
	}
	if (bCreateActivationMap)
		activationPainter->end();
	imgPainter.end();
	if (outputTriggerFrame)
	{
		if (currExpTrigger > lastTriggerNumber)
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
			QString fileName = outputDir + QString::number(nRetinoID) + "_" + QString::number(currExpBlock) + QString("_") + QString::number(currExpTrial) + QString("_") + QString::number(currExpTrigger);

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
						output.status() == QDataStream::Ok;
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
						output.status() == QDataStream::Ok;
						file.close();
					}
					break;
				}
			}
			lastTriggerNumber++;
		}
	}
	//QPixmap pixmap;
	//pixmap.load("D:\\output.png");
	
	QPainter painter(this);
	painter.begin(this);
	//painter.setRenderHint(QPainter::Antialiasing);
	painter.fillRect(event->rect(), brushBackground);
	painter.setPen(textPen);
	painter.setFont(textFont);
	painter.drawPixmap((rectScreenRes.width()-nStimFrameWidth)/2,(rectScreenRes.height()-nStimFrameHeight)/2,*StimulusResultImageFrame);

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
		textContent = "TriggerNr:" + QString::number(currExpTrigger) + ", TrialNr:" + QString::number(currExpTrial) + ", BlockNr:" + QString::number(currExpBlock);
		painter.drawText(QRect(10, 50, rectScreenRes.width(), 100), Qt::AlignLeft, textContent);
		//debugUsedTestSamples++;
		//}
		//else if (debugUsedTestSamples==(debugTestSamples+1))//During this sample we reset all the information for the speed measurement again
		//{
		//	debugTotalElapsedTime = 0;
		//	debugUsedTestSamples = 0;
		//}
	}
	painter.end();
	if(isDebugMode())
	{
		currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,"paintEvent():painter.end()");
		//emit LogExpObjData(nRetinoID,0,"paintEvent():painter.end()2");//0 is the default experiment timer
	}
	GLWidgetWrapper::finalizePaintEvent();
}
