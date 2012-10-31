//ExperimentManagerplugin
//Copyright (C) 2012  Sven Gijsen
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


#include "retinomap_glwidget.h"
#include <QDomNodeList>
#include <QGraphicsRectItem>
#include <QtCore/qmath.h>
#include <QFile>
#include "ExperimentManager.h"

Q_DECLARE_METATYPE(QPixmap*)
Q_DECLARE_METATYPE(QPainter*)

RetinoMap_glwidget::RetinoMap_glwidget(QWidget *parent) : GLWidgetWrapper(parent)
{
	customScriptHandlerFunction = NULL;
	initialize();
	GLWidgetWrapper::setupLayout(this);
	GLWidgetWrapper::setDoubleBufferCheck(true);
}

RetinoMap_glwidget::~RetinoMap_glwidget()
{
	if (customScriptHandlerFunction)
	{
		delete customScriptHandlerFunction;
		customScriptHandlerFunction = NULL;
	}
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

bool RetinoMap_glwidget::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
{
	if (engine)
	{
		QScriptEngine *currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		//QObject *someObject = this;//new MyObject;
		QScriptValue objectValue = currentScriptEngine->newQObject(this);
		currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		setScriptEngine(currentScriptEngine);
		return true;
	}
	return false;
}

void RetinoMap_glwidget::initialize()
{
	//Place your custom initialization code here:
	
	//setVerticalSyncSwap();
	//Default values if none defined.
	expSnapshot.currExpBlock = 0;
	expSnapshot.currExpBlockTrialFrame = 0;
	expSnapshot.currExpBlockTrialTrigger = 0;
	expSnapshot.currExpBlockTrialInternalTriggerAmount = 0;
	expSnapshot.currExpTrial = 0;
	expSnapshot.currExpInternalTrigger = 0;
	expSnapshot.currExpExternalTrigger = 0;
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
	fCalculatedCortMagFacTimingConst = 1.0f;
}

void RetinoMap_glwidget::parseExperimentObjectBlockParameters(bool bInit)
{	
	int tmpInteger = -1;
	if (bInit)
	{	
		QString tmpString = "";
		//getCurrentExperimentProgressSnapshot(&expSnapshot);
		//refreshRate = 0;
		//insertExpObjectBlockParameter(nRetinoID,GLWWRAP_WIDGET_DISPLAY_REFRESHRATE,QString::number(refreshRate)); \\Already taking care of in GLWidgetWrapperObject
		polarWedgeSpan = 22.5;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_POLAR_WEDGE_SPAN,polarWedgeSpan);
		//insertExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_POLAR_WEDGE_SPAN,QString::number(polarWedgeSpan));
		//float *tmp = getExpObjectVariabelePointer<float>(nRetinoID,RETINOMAP_WIDGET_POLAR_WEDGE_SPAN);
		//*tmp = 11.7;
		polarWedgeNrChecks = 4;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_POLAR_CHECK_AMOUNT,polarWedgeNrChecks);
		polarWedgeNrRings = 20;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_POLAR_RING_AMOUNT,polarWedgeNrRings);
		polarRotationDirection = 1;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_POLAR_ROTATION_DIRECTION,polarRotationDirection);
		eccentricityNrChecks = 20;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_ECCENTRICITY_CHECK_AMOUNT,eccentricityNrChecks);
		eccentricityNrRings = 4;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_ECCENTRICITY_RING_AMOUNT,eccentricityNrRings);
		disableCortMagFac = false;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_DISABLECORTMAGFAC,disableCortMagFac);
		enableCortMagTiming = false;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_ECCENTRICITY_ENABLE_CORTMAG_TIMING,enableCortMagTiming);
		eccentricityDirection = 1;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_ECCENTRICITY_DIRECTION,eccentricityDirection);
		movingBarAngle = 45.0;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_ANGLE,movingBarAngle);
		movingBarCoverage = 0.5;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_COVERAGE,movingBarCoverage);
		movingBarHeight = 10.0;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_HEIGHT,movingBarHeight);
		//movingBarIncludeOppositeDirection = false;
		//insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_INCOPPDIR,movingBarIncludeOppositeDirection);
		movingBarHeightCheckAmount = 4;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_HEIGTH_CHECK_AMOUNT,movingBarHeightCheckAmount);
		movingBarDirection = 1;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_DIRECTION,movingBarDirection);
		movingDotsColor = QColor(255,255,255);
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_COLOR,movingDotsColor);
		currentExpType = RETINOMAP_WIDGET_PATTERN_POLARANGLE;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_PATTERN,currentExpType);
		colorBackground = QColor(QColor(87,87,87));//gives "#575757";
		brushBackground = QBrush(colorBackground);
		insertExpObjectParameter(nRetinoID,GLWIDGET_BACKGROUNDCOLOR,colorBackground);
		fixationColor = QColor(255, 0, 0);
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_FIXATIONCOLOR,fixationColor);
		cCheckerColor1 = QColor(255, 255, 255);
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_CHECKERCOLOR1,cCheckerColor1);
		cCheckerColor2 = QColor(0, 0, 0);
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_CHECKERCOLOR2,cCheckerColor2);
		showFixationPoint = true;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_SHOWFIXPOINT,showFixationPoint);
		fixationSize = 8;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_FIXSIZE,fixationSize);
		stimHeigthPixelAmount = rectScreenRes.height();
		insertExpObjectParameter(nRetinoID,GLWIDGET_HEIGHT_PIXEL_AMOUNT,stimHeigthPixelAmount);
		stimWidthPixelAmount = stimHeigthPixelAmount;
		insertExpObjectParameter(nRetinoID,GLWIDGET_WIDTH_PIXEL_AMOUNT,stimWidthPixelAmount);
		cycleTriggerAmount = 1;
		if(getCurrentExpBlockTrialInternalTriggerAmount(tmpInteger))
			cycleTriggerAmount = tmpInteger;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_CYCLE_TRIGGER_AMOUNT,cycleTriggerAmount);
		flickrSpeedFreq = 7.5;//5 times per second
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_FLICKRSPEED_HZ,flickrSpeedFreq);
		flickrThreshold = 1000 / flickrSpeedFreq;//First at 200 mSecs
		cortMagFactor = 0.2f;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_CORTMAG_FACTOR,cortMagFactor);
		triggerDurationMsec = 2000;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_TRIGGERDURATION_MSEC,triggerDurationMsec);
		emptyTriggerSteps = 0;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_EMPTYTRIGGERSTEPS,emptyTriggerSteps);
		gapDiameter = 20.0;//aperture in the middle left blank
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_GAP_DIAMETER,gapDiameter);
		outputTriggerFrame = false;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_OUTPUTTRIGGERFRAME,outputTriggerFrame);
		discreteTriggerSteps = false;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_DISCRETETRIGGERSTEPS,discreteTriggerSteps);
		randomizeTriggerSteps = false;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_RANDOMIZETRIGGERSTEPS,randomizeTriggerSteps);
		antiAliasing = true;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_ANTIALIASING,antiAliasing);
		retinoOutputType = RETINOMAP_WIDGET_OUTPUTTYPE_FRAME;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_OUTPUTFRAMETYPE,retinoOutputType);
		retinoOutputFormat = RETINOMAP_WIDGET_OUTPUTFORMAT_DAT;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_OUTPUTFRAMEFORMAT,retinoOutputFormat);
		movingDotsMaxMoveSpeed = 4;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_MAX_MOVESPEED,movingDotsMaxMoveSpeed);
		movingDotsMinMoveSpeed = 4;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_MIN_MOVESPEED,movingDotsMinMoveSpeed);
		movingDotsMinMoveAngle = 0;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_MIN_MOVEANGLE,movingDotsMinMoveAngle);
		movingDotsMaxMoveAngle = 359;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_MAX_MOVEANGLE,movingDotsMaxMoveAngle);
		movingDotsHemifieldPos = RETINOMAP_WIDGET_POS_BOTH;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_HEMIFIELD,movingDotsHemifieldPos);
		movingDotsPixelFromCenter = 100;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_PIXELFROMCENTER,movingDotsPixelFromCenter);
		movingDotsIsStationary = false;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_STATIONAIRY,movingDotsIsStationary);
		movingDotsDotSize = 12;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_DOTSIZE,movingDotsDotSize);
		movingDotsNrOfDots = 1000;
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_NROFDOTS,movingDotsNrOfDots);
		////movingDotsRetPosition = 0;		
		////insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_RETINALPOSITION,movingDotsRetPosition);
		movingDotsHemiFieldWidth = stimWidthPixelAmount/2;		
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_HEMIFIELDWIDTH,movingDotsHemiFieldWidth);
		movingDotsHemiFieldHeight = stimHeigthPixelAmount;		
		insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_HEMIFIELDHEIGHT,movingDotsHemiFieldHeight);
	} 
	else
	{
		ParsedParameterDefinition pParDef;

		if (randStimStateGenerator == NULL)
			randStimStateGenerator = new RandomGenerator();
		if (randEmptyStimGenerator == NULL)
			randEmptyStimGenerator = new RandomGenerator();
		if (previousRandEmptyStimGenerator == NULL)
			previousRandEmptyStimGenerator = new RandomGenerator();	
		pParDef = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_PATTERN,RETINOMAP_WIDGET_PATTERN_POLARANGLE);
		currentExpType = pParDef.sValue.toLower();
		
		pParDef = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_SHOWFIXPOINT,TYPE_BOOL_TRUE);
		if(pParDef.sValue.toLower() == TYPE_BOOL_FALSE)
		{
			showFixationPoint = false;
		}
		else //if (tmpString == TYPE_BOOL_TRUE)
		{
			showFixationPoint = true;
		}
		stimHeigthPixelAmount = getExpObjectBlockParameter(nRetinoID,GLWIDGET_HEIGHT_PIXEL_AMOUNT,QString::number(stimHeigthPixelAmount)).sValue.toFloat();
		stimWidthPixelAmount = getExpObjectBlockParameter(nRetinoID,GLWIDGET_WIDTH_PIXEL_AMOUNT,QString::number(stimWidthPixelAmount)).sValue.toFloat();
		setStimuliResolution(stimWidthPixelAmount,stimHeigthPixelAmount);
		fixationSize = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_FIXSIZE,QString::number(fixationSize)).sValue.toInt();
		
		pParDef = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_CYCLE_TRIGGER_AMOUNT,QString::number(cycleTriggerAmount));
		if (pParDef.bHasChanged)
		{
			cycleTriggerAmount = pParDef.sValue.toInt();
		} 
		else
		{
			if(getCurrentExpBlockTrialInternalTriggerAmount(tmpInteger))
				cycleTriggerAmount = tmpInteger;
		}

		flickrSpeedFreq = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_FLICKRSPEED_HZ,QString::number(flickrSpeedFreq)).sValue.toFloat();
		flickrThreshold = 1000 / flickrSpeedFreq;
		cortMagFactor = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_CORTMAG_FACTOR,QString::number(cortMagFactor)).sValue.toFloat();
		triggerDurationMsec = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_TRIGGERDURATION_MSEC,QString::number(triggerDurationMsec)).sValue.toInt();
		emptyTriggerSteps = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_EMPTYTRIGGERSTEPS,QString::number(emptyTriggerSteps)).sValue.toInt();		
		gapDiameter = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_GAP_DIAMETER,QString::number(gapDiameter)).sValue.toInt();
		colorBackground = QColor(getExpObjectBlockParameter(nRetinoID,GLWIDGET_BACKGROUNDCOLOR,colorBackground.name()).sValue.toLower());
		brushBackground = QBrush(colorBackground);
		fixationColor = QColor(getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_FIXATIONCOLOR,fixationColor.name()).sValue.toLower());
		cCheckerColor1 = QColor(getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_CHECKERCOLOR1,cCheckerColor1.name()).sValue.toLower());
		cCheckerColor2 = QColor(getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_CHECKERCOLOR2,cCheckerColor2.name()).sValue.toLower());		
		pParDef = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_OUTPUTTRIGGERFRAME,TYPE_BOOL_FALSE);
		if (pParDef.sValue.toLower() == TYPE_BOOL_TRUE)
			outputTriggerFrame = true;
		else //if(tmpString == TYPE_BOOL_FALSE)
			outputTriggerFrame = false;
		pParDef = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_DISCRETETRIGGERSTEPS,TYPE_BOOL_FALSE);
		if (pParDef.sValue.toLower() == TYPE_BOOL_TRUE)
			discreteTriggerSteps = true;
		else// if(pParDef.sValue.toLower() == TYPE_BOOL_FALSE)
			discreteTriggerSteps = false;
		pParDef = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_RANDOMIZETRIGGERSTEPS,TYPE_BOOL_FALSE);
		if (pParDef.sValue.toLower() == TYPE_BOOL_TRUE)
			randomizeTriggerSteps = true;
		else// if(pParDef.sValue.toLower() == TYPE_BOOL_FALSE)
			randomizeTriggerSteps = false;
		pParDef = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_ANTIALIASING,TYPE_BOOL_TRUE);
		if(pParDef.sValue.toLower() == TYPE_BOOL_FALSE)
			antiAliasing = false;
		else //(tmpString == TYPE_BOOL_TRUE)
			antiAliasing = true;
		pParDef = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_OUTPUTFRAMETYPE,RETINOMAP_WIDGET_OUTPUTTYPE_FRAME);
		retinoOutputType = pParDef.sValue.toLower();
		pParDef = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_OUTPUTFRAMEFORMAT,RETINOMAP_WIDGET_OUTPUTFORMAT_DAT);
		retinoOutputFormat = pParDef.sValue.toLower();
		pParDef = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_DISABLECORTMAGFAC,TYPE_BOOL_FALSE);
		if (pParDef.sValue.toLower() == TYPE_BOOL_TRUE)
			disableCortMagFac = true;
		else //if(pParDef.sValue.toLower() == TYPE_BOOL_FALSE)
			disableCortMagFac = false;

		//Specific ExperimentType Parameters
		if(currentExpType == RETINOMAP_WIDGET_PATTERN_POLARANGLE)
		{
			polarWedgeSpan = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_POLAR_WEDGE_SPAN,QString::number(polarWedgeSpan)).sValue.toFloat();
			polarWedgeNrChecks = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_POLAR_CHECK_AMOUNT,QString::number(polarWedgeNrChecks)).sValue.toInt();
			polarWedgeNrRings = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_POLAR_RING_AMOUNT,QString::number(polarWedgeNrRings)).sValue.toInt();
			polarRotationDirection = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_POLAR_ROTATION_DIRECTION,QString::number(polarRotationDirection)).sValue.toInt();
		}
		else if(currentExpType == RETINOMAP_WIDGET_PATTERN_ECCENTRICITY)
		{
			eccentricityNrChecks = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_ECCENTRICITY_CHECK_AMOUNT,QString::number(eccentricityNrChecks)).sValue.toInt();
			eccentricityNrRings = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_ECCENTRICITY_RING_AMOUNT,QString::number(eccentricityNrRings)).sValue.toInt();
			eccentricityDirection = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_ECCENTRICITY_DIRECTION,QString::number(eccentricityDirection)).sValue.toInt();
			pParDef = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_ECCENTRICITY_ENABLE_CORTMAG_TIMING,TYPE_BOOL_FALSE);
			if (pParDef.sValue.toLower() == TYPE_BOOL_TRUE)
				enableCortMagTiming = true;
			else //if(tmpString == TYPE_BOOL_FALSE)
				enableCortMagTiming = false;
		}
		else if(currentExpType == RETINOMAP_WIDGET_PATTERN_MOVINGBAR)
		{
			movingBarAngle = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_ANGLE,QString::number(movingBarAngle)).sValue.toFloat();
			movingBarCoverage = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_COVERAGE,QString::number(movingBarCoverage)).sValue.toFloat();
			movingBarHeight = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_HEIGHT,QString::number(movingBarHeight)).sValue.toFloat();
			movingBarHeightCheckAmount = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_HEIGTH_CHECK_AMOUNT,QString::number(movingBarHeightCheckAmount)).sValue.toInt();
			movingBarDirection = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_DIRECTION,QString::number(movingBarDirection)).sValue.toInt();
			//tmpString = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_INCOPPDIR,TYPE_BOOL_FALSE);
			//if (tmpString == TYPE_BOOL_TRUE)
			//	movingBarIncludeOppositeDirection = true;
			//else //if(tmpString == TYPE_BOOL_FALSE)
			//	movingBarIncludeOppositeDirection = false;	
		}
		else if(currentExpType == RETINOMAP_WIDGET_PATTERN_MOVINGDOTS)
		{
			movingDotsColor = QColor(getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_COLOR,movingDotsColor.name()).sValue.toLower());
			movingDotsMinMoveAngle = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_MIN_MOVEANGLE,QString::number(movingDotsMinMoveAngle)).sValue.toFloat();
			movingDotsMaxMoveAngle = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_MAX_MOVEANGLE,QString::number(movingDotsMaxMoveAngle)).sValue.toFloat();
			movingDotsMaxMoveSpeed = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_MAX_MOVESPEED,QString::number(movingDotsMaxMoveSpeed)).sValue.toFloat();
			movingDotsMinMoveSpeed = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_MIN_MOVESPEED,QString::number(movingDotsMinMoveSpeed)).sValue.toFloat();
			pParDef = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_HEMIFIELD,RETINOMAP_WIDGET_POS_BOTH);
			movingDotsHemifieldPos = pParDef.sValue.toLower();
			movingDotsPixelFromCenter = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_PIXELFROMCENTER,QString::number(movingDotsPixelFromCenter)).sValue.toInt();
			pParDef = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_STATIONAIRY,TYPE_BOOL_FALSE);
			if (pParDef.sValue.toLower() == TYPE_BOOL_TRUE)
				movingDotsIsStationary = true;
			else //if(pParDef.sValue.toLower() == TYPE_BOOL_FALSE)
				movingDotsIsStationary = false;			
			movingDotsDotSize = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_DOTSIZE,QString::number(movingDotsDotSize)).sValue.toInt();
			movingDotsNrOfDots = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_NROFDOTS,QString::number(movingDotsNrOfDots)).sValue.toInt();
			//movingDotsRetPosition = getExperimentObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_RETINALPOSITION,QString::number(movingDotsRetPosition)).toInt();
			
			//We have initialized the parameter(movingDotsHemiFieldWidth) the first time(before the experiment actually started), but because the default value is dependent of another 
			//parameter(stimWidthPixelAmount) that might have changed again we might have to reinitialize this parameter again. But this applies only when we fetch the 
			//parameter for the first time and that the parameter is also not re-defined by the user. When we reinitialize the parameter here its important to set the bIsInitializing
			//from the function insertExpObjectParameter() to false!
			pParDef = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_HEMIFIELDWIDTH,QString::number(stimWidthPixelAmount/2));
			if ((pParDef.bIsInitialized) && (pParDef.bHasChanged == false))
			{
				int tmpBuffer = movingDotsHemiFieldWidth;
				movingDotsHemiFieldWidth = QString::number(stimWidthPixelAmount/2).toInt();
				if(insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_HEMIFIELDWIDTH,movingDotsHemiFieldWidth,false))
					movingDotsHemiFieldWidth = QString::number(stimWidthPixelAmount/2).toInt();
				movingDotsHemiFieldWidth = tmpBuffer;
			}
			else
			{
				movingDotsHemiFieldWidth = pParDef.sValue.toInt();
			}
			//Same counts for the next parameter
			pParDef = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_HEMIFIELDHEIGHT,QString::number(stimHeigthPixelAmount));
			if ((pParDef.bIsInitialized) && (pParDef.bHasChanged == false))
			{
				int tmpBuffer = movingDotsHemiFieldHeight;
				movingDotsHemiFieldHeight = QString::number(stimHeigthPixelAmount).toInt();
				if(insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_MOVINGDOTS_HEMIFIELDHEIGHT,movingDotsHemiFieldHeight,false))
					movingDotsHemiFieldHeight = QString::number(stimHeigthPixelAmount).toInt();
				else
					movingDotsHemiFieldHeight = tmpBuffer;
			}
			else
			{
				movingDotsHemiFieldHeight = pParDef.sValue.toInt();
			}

			initializeMovingDotsStructures();
		}
		else if(currentExpType == RETINOMAP_WIDGET_PATTERN_FIXATION)
		{

		}
		else if(currentExpType == RETINOMAP_WIDGET_PATTERN_CUSTOM)
		{

		}
		else
		{
			qDebug() << __FUNCTION__ << "::Unknown RetinoMapping type!";
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
	//emit LogToOutputWindow("Started");
	return true;
}

bool RetinoMap_glwidget::setObjectConfiguration(QObject *pExpConfStruct)//ExperimentConfiguration *pExpConfStruct)
{
	currExpConfStruct = reinterpret_cast<ExperimentConfiguration *>(pExpConfStruct);
	//currExpConfStruct->pExperimentManager->getExperimentObjectBlockParameter()
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
	//fLastTrialTimeProgress = 0.0f;
	previousRandEmptyStimGenerator->clear();
	//randStimStateGenerator->clear();
	return true;
}

QImage RetinoMap_glwidget::fractalFillCheckeredImage(float fWidth, float fHeigth, float fSize, int nflickr)
{
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
	return image;
}

void RetinoMap_glwidget::initializeMovingDotsStructures()
{
	if (movingDotsHemifieldPos == RETINOMAP_WIDGET_POS_RIGHT)
	{
		movingDotsFirstHemiVisibleArea.setP1(QPointF((stimWidthPixelAmount/2)+movingDotsPixelFromCenter, (stimHeigthPixelAmount-movingDotsHemiFieldHeight)/2));
		movingDotsFirstHemiVisibleArea.setP2(QPointF(stimWidthPixelAmount - ((stimWidthPixelAmount/2)-movingDotsHemiFieldWidth), stimHeigthPixelAmount-(movingDotsPixelFromCenter, (stimHeigthPixelAmount-movingDotsHemiFieldHeight)/2)));
	}
	else//(RETINOMAP_WIDGET_POS_LEFT || RETINOMAP_WIDGET_POS_BOTH)
	{
		movingDotsFirstHemiVisibleArea.setP1(QPointF(((stimWidthPixelAmount/2)-movingDotsHemiFieldWidth), (stimHeigthPixelAmount-movingDotsHemiFieldHeight)/2));
		movingDotsFirstHemiVisibleArea.setP2(QPointF((stimWidthPixelAmount/2)-movingDotsPixelFromCenter, stimHeigthPixelAmount-movingDotsFirstHemiVisibleArea.p1().y()));
	}
	QPointF tmpPoint;
	QLineF tmpLine;
	movingDots.Pos.clear();
	movingDots.Mov.clear();
	//movingDots.MirrorXPos.clear();
	movingDots.OldPos.clear();
	for (int i=0; i<movingDotsNrOfDots; i++)
	{
		tmpPoint.setX(randStimStateGenerator->randomizeInt(movingDotsFirstHemiVisibleArea.p1().x(),movingDotsFirstHemiVisibleArea.p2().x()));
		tmpPoint.setY(randStimStateGenerator->randomizeInt(movingDotsFirstHemiVisibleArea.p1().y(),movingDotsFirstHemiVisibleArea.p2().y()));
		movingDots.OldPos.append(tmpPoint);//The initial position will be copied from here
		movingDots.Pos.append(QPointF(0,0));
		tmpLine.setP1(QPointF(0,0));//Point1 of Movement Vector
		tmpLine.setP2(QPointF(1,0));//The base speed
		tmpLine.setAngle(randStimStateGenerator->randomizeInt(movingDotsMinMoveAngle,movingDotsMaxMoveAngle));//The randomized movement direction
		tmpLine.setLength(randStimStateGenerator->randomizeDouble(movingDotsMinMoveSpeed,movingDotsMaxMoveSpeed));//The randomized speed of the movement  
		movingDots.Mov.append(tmpLine);
		//movingDots.MirrorXPos.append(0.0f);//The Mirrored X Position
	}
}

bool RetinoMap_glwidget::paintObject(int paintFlags, QObject *paintEventObject)
{
	if(isDebugMode())
		currExpConfStruct->pExperimentManager->logExperimentObjectData(nRetinoID,0,__FUNCTION__,"","Starting to paint the object");
	QPaintEvent *event = reinterpret_cast<QPaintEvent *>(paintEventObject);//qobject_cast<QPaintEvent *>(paintEventObject);
	GLWidgetPaintFlags currentPaintFlags = (GLWidgetPaintFlags)paintFlags;
	bool bRenderStimuli = true;
	QString tmpStr = "";
	tmpParamValue = "";
	fStimulusDiameter = 0.0f;
	fTrialTimeProgress = 0.0f;
	if (currentPaintFlags & GLWidgetPaintFlags_LockedState)
	{
		bCreateActivationMap = false;
		currentExpType = RETINOMAP_WIDGET_PATTERN_FIXATION;
	}
	else
	{
		bCreateActivationMap = (outputTriggerFrame) && (retinoOutputType==RETINOMAP_WIDGET_OUTPUTTYPE_MASK);
	}
	getCurrentExperimentProgressSnapshot(&expSnapshot);
	int elapsedTrialTime = (int)expSnapshot.elapsedTrialTime;
	if((elapsedTrialTime >= ((triggerDurationMsec * (expSnapshot.currExpBlockTrialTrigger+1)))) &&(currentExpType != RETINOMAP_WIDGET_PATTERN_FIXATION))//If the TrialTime exceeds the maximum time for the current Block-Trial Time
		elapsedTrialTime = (triggerDurationMsec * (expSnapshot.currExpBlockTrialTrigger+1))-1;
	else if((elapsedTrialTime <= ((triggerDurationMsec * (expSnapshot.currExpBlockTrialTrigger)))) &&(currentExpType != RETINOMAP_WIDGET_PATTERN_FIXATION))//If the TrialTime is less than the minimum time for the current Block-Trial Time
		elapsedTrialTime = (triggerDurationMsec * (expSnapshot.currExpBlockTrialTrigger))+1;
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
	if (currentExpType != RETINOMAP_WIDGET_PATTERN_FIXATION)
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
				//if(isDebugMode())
					//emit LogToOutputWindow("EmptyRandomList(steps) = " + tmpStr);
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
			//if(isDebugMode())
				//emit LogToOutputWindow("StimRandomList = " + tmpStr);
		}	

		if (discreteTriggerSteps)
		{	
			int numberOfSteps = 1;
			if (cycleTriggerAmount > 1)
			{
				if (currentExpType == RETINOMAP_WIDGET_PATTERN_POLARANGLE) 
				{
					//0-->1 travels from 0 to 360(which is also 0!) degrees
					//To use a full scale spread we'll use the following:
					numberOfSteps = cycleTriggerAmount;
				}
				//else if ((currentExpType == RETINOMAP_WIDGET_PATTERN_MOVINGBAR) && (movingBarIncludeOppositeDirection))
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

				//if ((currentExpType == RETINOMAP_WIDGET_PATTERN_MOVINGBAR) && (movingBarIncludeOppositeDirection) && ((nTrialIndex*2)>=cycleTriggerAmount))
				//	fTrialTimeProgress = (float)(nTrialIndex-1) / numberOfSteps;
				//else
					fTrialTimeProgress = (float)(nTrialIndex) / numberOfSteps;
			}
			else
			{
				fTrialTimeProgress = (float)expSnapshot.currExpBlockTrialTrigger / cycleTriggerAmount;
			}

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
			fTrialTimeProgress = fTrialTimeProgress - ((int)fTrialTimeProgress);//Revert cycle to beginning (subtract the full cycles so that 0<=fTrialTimeProgress>=1 counts)
	
		if (nextNewCycleEntered)
			emptyTriggerStepCount = emptyTriggerSteps;

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
	//if(currentExpType != RETINOMAP_WIDGET_PATTERN_MOVINGDOTS)
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

	if (currentExpType == RETINOMAP_WIDGET_PATTERN_POLARANGLE)
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
	}
	else if (currentExpType == RETINOMAP_WIDGET_PATTERN_ECCENTRICITY)
	{
		if (bRenderStimuli)
		{		
			QPen oldPen;
			wedgeSpanAngle = 360.0f / eccentricityNrChecks * 16.0f;		
			if((enableCortMagTiming)&&(cortMagFactor > 0.0f)&&(cortMagFactor < 1.0f))
			{
				//Matlab
				//CortMagFac = 0.2
				//CycleTriggerAmount = 8
				//CortMagFacTimingFactor = (100-(1/(((log(-CortMagFac+1)/(10/(CycleTriggerAmount-1)))-1)/-100)))/(CortMagFac*100)
				//time=[0:0.01:15];
				//Decrease
				//y=  ((-100+(CortMagFac*CortMagFacTimingFactor*100))*exp(((CortMagFac*CortMagFacTimingFactor*100)/(-100+(CortMagFac*CortMagFacTimingFactor*100)))*  time)    +100-(CortMagFac*CortMagFacTimingFactor*100))/(100-(CortMagFac*CortMagFacTimingFactor*100));
				//Increase
				//y=1-((-100+(CortMagFac*CortMagFacTimingFactor*100))*exp(((CortMagFac*CortMagFacTimingFactor*100)/(-100+(CortMagFac*CortMagFacTimingFactor*100)))*(-time+10))+100-(CortMagFac*CortMagFacTimingFactor*100))/(100-(CortMagFac*CortMagFacTimingFactor*100));
				//plot(time,y)
				//additional --> s = solve('CycleTriggerAmount=20','CortMagFac=0.3','t=(10/(CycleTriggerAmount-1))','((-100+(CortMagFac*answer*100))*exp(((CortMagFac*answer*100)/(-100+(CortMagFac*answer*100)))*t)+100-(CortMagFac*answer*100))/(100-(CortMagFac*answer*100))=CortMagFac')
				if(eccentricityDirection == -1)//Decrease ring diameter
				{			
					fCalculatedCortMagFacTimingConst =(100-(1/(((qLn(-cortMagFactor+1)/(10/(float)(cycleTriggerAmount-1)))-1)/-100)))/(cortMagFactor*100);
					float fTau = (float)(cortMagFactor * fCalculatedCortMagFacTimingConst*100)/(-100 + (cortMagFactor * fCalculatedCortMagFacTimingConst*100));
					fCortMagTrialTimeProgress = ((-100 + (cortMagFactor * fCalculatedCortMagFacTimingConst*100))*qExp(fTau *(fTrialTimeProgress*10))+100-(cortMagFactor * fCalculatedCortMagFacTimingConst*100))/(100-(cortMagFactor * fCalculatedCortMagFacTimingConst*100));
				}
				else
				{
					fCalculatedCortMagFacTimingConst =(100-(1/(((qLn(-cortMagFactor+1)/(10/(float)(cycleTriggerAmount-1)))-1)/-100)))/(cortMagFactor*100);
					float fTau = (float)(cortMagFactor * fCalculatedCortMagFacTimingConst*100)/(-100 + (cortMagFactor * fCalculatedCortMagFacTimingConst*100));
					fCortMagTrialTimeProgress = 1-((-100 + (cortMagFactor * fCalculatedCortMagFacTimingConst*100))*qExp(fTau *( (fTrialTimeProgress*-10)+10 ))+100-(cortMagFactor * fCalculatedCortMagFacTimingConst*100))/(100-(cortMagFactor * fCalculatedCortMagFacTimingConst*100));
				}				
			}
			else
			{
				fCortMagTrialTimeProgress = fTrialTimeProgress;
			}
			if (disableCortMagFac)
			{
				currentCompleteWedgeDiameter = ((nStimFrameHeight - gapDiameter) * cortMagFactor) / 2.0f;
				//currentCompleteWedgeDiameter = ((nStimFrameHeight - gapDiameter)) / 2.0f;
				currentWedgeDiameter = currentCompleteWedgeDiameter / eccentricityNrRings;
				if(eccentricityDirection == -1)//Decrease ring diameter
					currentOuterCompleteRingDiameter = nStimFrameHeight - ((nStimFrameHeight - gapDiameter - (2*currentCompleteWedgeDiameter)) * fCortMagTrialTimeProgress);
				else//Increase ring diameter
					currentOuterCompleteRingDiameter = (gapDiameter + (2*currentCompleteWedgeDiameter)) + ((nStimFrameHeight - gapDiameter - (2*currentCompleteWedgeDiameter)) * fCortMagTrialTimeProgress);
			} 
			else
			{
				if(eccentricityDirection == -1)//Decrease ring diameter
				{
					currentCompleteWedgeDiameter = (((nStimFrameHeight - gapDiameter) * cortMagFactor) - ((nStimFrameHeight - gapDiameter) * fCortMagTrialTimeProgress * cortMagFactor)) / 2.0f;
					currentWedgeDiameter = currentCompleteWedgeDiameter / eccentricityNrRings;
					currentOuterCompleteRingDiameter = nStimFrameHeight - ((nStimFrameHeight - gapDiameter) * fCortMagTrialTimeProgress);
					//if (fCortMagTrialTimeProgress > 0.0)
					//{
					//	int aa = 7;
					//}
				}
				else//Increase ring diameter
				{
					currentCompleteWedgeDiameter = (((nStimFrameHeight - gapDiameter) / 2.0f) * fCortMagTrialTimeProgress * cortMagFactor) ;
					currentWedgeDiameter = currentCompleteWedgeDiameter / eccentricityNrRings;
					currentOuterCompleteRingDiameter = gapDiameter + ((nStimFrameHeight - gapDiameter) * fCortMagTrialTimeProgress);
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
	}
	else if (currentExpType == RETINOMAP_WIDGET_PATTERN_MOVINGBAR)
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
	}
	else if (currentExpType == RETINOMAP_WIDGET_PATTERN_MOVINGDOTS)
	{
		if (bRenderStimuli)
		{
			imgPainter->setPen(QPen(movingDotsColor, movingDotsDotSize, style, roundCap));
			//fTrialTimeProgress = fTrialTimeProgress;//(float)elapsedTrialTime / (cycleTriggerAmount * triggerDurationMsec);
			//float fDiffTrialTimeProgress;
			//float fMovCorrFactor;
			//int nStimRefreshRate = getCurrentStimuliRefreshRate();
			//int nTrialDuration = (cycleTriggerAmount * triggerDurationMsec);

			//if ((fLastTrialTimeProgress < fTrialTimeProgress) && ((fLastTrialTimeProgress != 0.0)))
			//{
			//	fDiffTrialTimeProgress = fTrialTimeProgress - fLastTrialTimeProgress;
			//	//fDiffTrialTimeProgress = (1000.0/nStimRefreshRate)/nTrialDuration;//assume that it was fine...
			//} 
			//else
			//{				
			//	fDiffTrialTimeProgress = (1000.0/nStimRefreshRate)/nTrialDuration;//assume that it was fine...				
			//}	
			//fMovCorrFactor = (fDiffTrialTimeProgress * (nTrialDuration/1000.0)) * nStimRefreshRate;
			//fLastTrialTimeProgress = fTrialTimeProgress;

			for(int i=0; i<movingDotsNrOfDots; i++)
			{
				//nCurrentThreadNum = omp_get_thread_num();
				//nCurrentThreadAmount = omp_get_num_threads(); 
				//if ((nCurrentThreadNum != nDefaultThreadNum) )// || (nCurrentThreadAmount != nDefaultThreadAmount))
				//{
				//	nDefaultThreadNum = nCurrentThreadNum;
				//	nDefaultThreadAmount = nCurrentThreadAmount; 
				//}
				//if(isDebugMode())
				
				if(movingDotsIsStationary)
				{
					movingDots.Pos[i] = movingDots.OldPos[i];
				}
				else
				{
					//move the dots
					//#pragma omp atomic
					{
						//#pragma omp critical
						movingDots.Pos[i] = movingDots.OldPos.at(i) + (movingDots.Mov[i].p2());// * fMovCorrFactor);
						// check whether dot moves out of display field - x direction					
						if(movingDots.Pos[i].x() >= movingDotsFirstHemiVisibleArea.p2().x())
						{
							//#pragma omp atomic
							movingDots.Pos[i].setX(movingDots.Pos[i].x() - movingDotsFirstHemiVisibleArea.dx());
						}
						if(movingDots.Pos[i].x() <= movingDotsFirstHemiVisibleArea.p1().x())//
						{
							//#pragma omp atomic
							movingDots.Pos[i].setX(movingDots.Pos[i].x() + movingDotsFirstHemiVisibleArea.dx());
						}
						// check whether dot moves out of display field - y direction
						if(movingDots.Pos[i].y() >= movingDotsFirstHemiVisibleArea.p2().y())
						{
							//#pragma omp atomic
							movingDots.Pos[i].setY(movingDots.Pos[i].y() - movingDotsFirstHemiVisibleArea.dy());
						}
						if(movingDots.Pos[i].y() <= movingDotsFirstHemiVisibleArea.p1().y())//movingDotsYStartRel)
						{
							//#pragma omp atomic
							movingDots.Pos[i].setY(movingDots.Pos[i].y() + movingDotsFirstHemiVisibleArea.dy());
						}
					}
				}
			}
			//StimulusResultImageFrame->fill(Qt::transparent);
			//#pragma omp parallel for --> doesn't work in the drawing routines!
			for(int i=0; i<movingDotsNrOfDots; i++)
			{
				if(isDebugMode())
				{
					if (i==0)
					{
						imgPainter->save();
						imgPainter->setPen(QPen(QColor(255,0,0), movingDotsDotSize, style, roundCap));
						imgPainter->drawPoint(movingDots.Pos.at(i));
						imgPainter->restore();
					} 
					else
					{
						imgPainter->drawPoint(movingDots.Pos.at(i));
					}

				}
				else
				{
					imgPainter->drawPoint(movingDots.Pos.at(i));
				}

								
				//Draw the remaining parts of a dot (mirrored!)
				if (movingDots.Pos.at(i).x()>=movingDotsFirstHemiVisibleArea.p2().x()-movingDotsDotSize)
					imgPainter->drawPoint(movingDots.Pos.at(i).x() - movingDotsFirstHemiVisibleArea.dx(),movingDots.Pos.at(i).y());
				if (movingDots.Pos.at(i).x()<=movingDotsFirstHemiVisibleArea.p1().x()+movingDotsDotSize)
					imgPainter->drawPoint(movingDots.Pos.at(i).x() + movingDotsFirstHemiVisibleArea.dx(),movingDots.Pos.at(i).y());
				if (movingDots.Pos.at(i).y()>=movingDotsFirstHemiVisibleArea.p2().y()-movingDotsDotSize)
					imgPainter->drawPoint(movingDots.Pos.at(i).x(),movingDots.Pos.at(i).y() - movingDotsFirstHemiVisibleArea.dy());
				if (movingDots.Pos.at(i).y()<=movingDotsFirstHemiVisibleArea.p1().y()+movingDotsDotSize)
					imgPainter->drawPoint(movingDots.Pos.at(i).x(),movingDots.Pos.at(i).y() + movingDotsFirstHemiVisibleArea.dy());
				if (i==movingDotsNrOfDots-1)
				{
					//Remove the dot-parts that are outside the first hemisphere
					imgPainter->save();
					imgPainter->setPen(QPen(colorBackground, movingDotsDotSize*2, style, flatCap));
					imgPainter->drawLine(movingDotsFirstHemiVisibleArea.p2().x()+movingDotsDotSize,movingDotsFirstHemiVisibleArea.p1().y()-(2*movingDotsDotSize),movingDotsFirstHemiVisibleArea.p2().x()+movingDotsDotSize,movingDotsFirstHemiVisibleArea.p2().y()+(2*movingDotsDotSize));//Right border
					imgPainter->drawLine(movingDotsFirstHemiVisibleArea.p1().x()-(2*movingDotsDotSize),movingDotsFirstHemiVisibleArea.p1().y()-movingDotsDotSize,movingDotsFirstHemiVisibleArea.p2().x()+(2*movingDotsDotSize),movingDotsFirstHemiVisibleArea.p1().y()-movingDotsDotSize);//Top border				
					imgPainter->drawLine(movingDotsFirstHemiVisibleArea.p1().x()-movingDotsDotSize,movingDotsFirstHemiVisibleArea.p1().y()-(2*movingDotsDotSize),movingDotsFirstHemiVisibleArea.p1().x()-movingDotsDotSize,movingDotsFirstHemiVisibleArea.p2().y()+(2*movingDotsDotSize));//Left border				
					imgPainter->drawLine(movingDotsFirstHemiVisibleArea.p1().x()-(2*movingDotsDotSize),movingDotsFirstHemiVisibleArea.p2().y()+movingDotsDotSize,movingDotsFirstHemiVisibleArea.p2().x()+(2*movingDotsDotSize),movingDotsFirstHemiVisibleArea.p2().y()+movingDotsDotSize);//Bottom border				
					
					imgPainter->restore();
				}
				if(movingDotsHemifieldPos == RETINOMAP_WIDGET_POS_BOTH)
				{
					if (i==movingDotsNrOfDots-1)
					{
						mirrorImage = StimulusResultImageFrame->copy(movingDotsFirstHemiVisibleArea.p1().x(),movingDotsFirstHemiVisibleArea.p1().y(),movingDotsFirstHemiVisibleArea.dx(),movingDotsFirstHemiVisibleArea.dy()).toImage();
						//mirrorImage = mirrorImage.mirrored(true,false);
						//QPixmap mirror( QPixmap::fromImage( mirrorImage ) );						
						imgPainter->drawImage((stimWidthPixelAmount/2)+movingDotsPixelFromCenter,movingDotsFirstHemiVisibleArea.p1().y(),mirrorImage);//    10+movingDotsPixelFromCenter,0,mirrorImage);//movingDotsFirstHemiVisibleArea.p2().x()+
						//imgPainter->save();
						//imgPainter->setPen(Qt::NoPen);
						//imgPainter->setBrush(tmpPXMirrorMap);
						////imgPainter->setOpacity( 0.5 );
						////imgPainter->translate((stimWidthPixelAmount/2)+movingDotsPixelFromCenter, 0 );
						//imgPainter->translate(300,300);
						//int nCurrCompMode = imgPainter->compositionMode();
						//imgPainter->setCompositionMode(QPainter::CompositionMode_Plus);
						//nCurrCompMode = imgPainter->compositionMode();
						//imgPainter->drawRect(0,0,movingDotsFirstHemiVisibleArea.dx(),movingDotsFirstHemiVisibleArea.dy()); //mirrorImage->boundingRect() );
						//imgPainter->restore();
						//nCurrCompMode = imgPainter->compositionMode();	
					}
				}
				movingDots.OldPos[i] = movingDots.Pos.at(i);
			}
			//static bool firstUpdate = true;
			//if (firstUpdate)
			//{ 
			//	firstUpdate = false;
			//}
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
	}
	else if (currentExpType == RETINOMAP_WIDGET_PATTERN_FIXATION)
	{
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
	}
	else if (currentExpType == RETINOMAP_WIDGET_PATTERN_CUSTOM)
	{
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
	}
	if ((bCreateActivationMap) && (activationPainter))
		activationPainter->end();
	imgPainter->end();
	if (outputTriggerFrame)
	{
		if (expSnapshot.currExpInternalTrigger > lastTriggerNumber)
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
			QString fileName = outputDir + QString::number(nRetinoID) + "_" + QString::number(expSnapshot.currExpBlock) + QString("_") + QString::number(expSnapshot.currExpTrial) + QString("_") + QString::number(expSnapshot.currExpInternalTrigger);

			if(retinoOutputType == RETINOMAP_WIDGET_OUTPUTTYPE_FRAME)
			{
				if (retinoOutputFormat == RETINOMAP_WIDGET_OUTPUTFORMAT_PNG)
				{
					fileName = fileName + QString(".png");
					file.setFileName(fileName);
					if (file.open(QIODevice::WriteOnly))
					{
						StimulusResultImageFrame->save(&file, "PNG");
						file.close();
					}
				} 
				else if (retinoOutputFormat == RETINOMAP_WIDGET_OUTPUTFORMAT_DAT)
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
				}
			}
			else if(retinoOutputType == RETINOMAP_WIDGET_OUTPUTTYPE_MASK)
			{
				if (retinoOutputFormat == RETINOMAP_WIDGET_OUTPUTFORMAT_PNG)
				{
					fileName = fileName + QString(".png");
					file.setFileName(fileName);
					if (file.open(QIODevice::WriteOnly))
					{
						StimulusActivationMap->save(&file, "PNG");
						file.close();
					}
				} 
				else if (retinoOutputFormat == RETINOMAP_WIDGET_OUTPUTFORMAT_DAT)
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
	
	stimuliPainter->drawPixmap(0,0,*StimulusResultImageFrame);
	if (customScriptHandlerFunction && getScriptEngine())
	{
		scriptVal1 = getScriptEngine()->toScriptValue<QPainter *>(stimuliPainter);
		if (currentExpType == RETINOMAP_WIDGET_PATTERN_FIXATION)
		{
			customScriptHandlerFunction->call(QScriptValue(), QScriptValueList() << scriptVal1);
		}
		else
		{
			scriptVal2 = getScriptEngine()->toScriptValue<float>(fTrialTimeProgress);
			customScriptHandlerFunction->call(QScriptValue(), QScriptValueList() << scriptVal1 << scriptVal2);
		}
	}

	//if (customScriptHandlerFunction && currentScriptEngine)
	//{
	//	scriptVal = currentScriptEngine->toScriptValue<QPixmap *>(StimulusResultImageFrame);
	//	funcReturnValue = customScriptHandlerFunction->call(QScriptValue(), QScriptValueList() << scriptVal);
	//	tmpPixmap = NULL;
	//	tmpPixmap = currentScriptEngine->fromScriptValue<QPixmap *>(funcReturnValue);
	//	if(tmpPixmap)
	//		*StimulusResultImageFrame = tmpPixmap->copy();	
	//}		
	//stimuliPainter->drawPixmap(0,0,*StimulusResultImageFrame);

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
		textContent = "ExternalTriggerNr:" + QString::number(expSnapshot.currExpExternalTrigger) + ", InternalTriggerNr:" + QString::number(expSnapshot.currExpInternalTrigger) + ", TrialNr:" + QString::number(expSnapshot.currExpTrial) + ", BlockNr:" + QString::number(expSnapshot.currExpBlock);
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

bool RetinoMap_glwidget::installCustomScriptHandlerFunction(QString FuncName)
{
	if (getScriptEngine())
	{
		customScriptHandlerFunction = new QScriptValue(getScriptEngine()->globalObject().property(FuncName));
		if(customScriptHandlerFunction->isFunction())
		{
			return true;
		}
		else
		{
			if(customScriptHandlerFunction)
			{
				delete customScriptHandlerFunction;
				customScriptHandlerFunction = NULL;
			}
		}
	}
	return false;
}

bool RetinoMap_glwidget::parseExperimentObjectParameter(QString strParamName)
{
	//bool bRetval = insertExpObjectParameter(nRetinoID,RETINOMAP_WIDGET_FIXSIZE,QString::number(11));
	//fixationSize = getExpObjectBlockParameter(nRetinoID,RETINOMAP_WIDGET_FIXSIZE,QString::number(fixationSize)).sValue.toInt();
	//strParamName = strParamName;

	
	//ExperimentParameterContainer expParamCntnr;
	//ExperimentParameterContainer *expParamCntnr = new ExperimentParameterContainer();
	//TypedExperimentParameterContainer *typedExpParamCntnr = new TypedExperimentParameterContainer();
	
	currExpConfStruct->pExperimentManager->insertExperimentObjectVariabelePointer(nRetinoID,strParamName,fixationSize);
	////typedExpParamCntnr.insertExperimentParameter();
	int *tmpVar = NULL;
	tmpVar = currExpConfStruct->pExperimentManager->getExperimentObjectVariabelePointer<int>(nRetinoID,strParamName);
	////tmpVar = typedExpParamCntnr.getExperimentParameter<int>(RETINOMAP_WIDGET_FIXSIZE);
	*tmpVar = 88;
	int g = *tmpVar;

	//expParamCntnr.insertExperimentParameter(RETINOMAP_WIDGET_FIXSIZE,QVariant(fixationSize));
	
	
	//ExperimentParameter ep("Name");
	//TypedExperimentParameter <int>tep("fef",7);

	//std::vector<TypedExperimentParameter*> my_property_list;
	//for(unsigned int u=0; u<10; ++u)
	//	my_property_list.push_back(new TypedExperimentParameter(u));

	////QList<QPair<QString, ExperimentParameter>> list;
	////list.append(qMakePair("list1", ep));

	//QHash<QString, ExperimentParameter> hash;
	//hash.insert("defd",ep);

	//QHash<QString, TypedExperimentParameter> hash;
	//hash.insert("defd",ep);
	
	//typedef std::vector< std::shared_ptr<TypedExperimentParameter> > property_list_type;
	//typedef std::vector< std::shared_ptr<ExperimentParameter> > property_list_type;
	
	//std::vector<ExperimentParameter*> my_property_list;
	//for(unsigned int u=0; u<10; ++u)
	//	my_property_list.push_back(new ExperimentParameter(u));
	//
	////use_property_list(my_property_list);

	//for(std::vector<ExperimentParameter*>::iterator it=my_property_list.begin();
	//	it!=my_property_list.end(); ++it)
	//	delete *it;



	return true;
}