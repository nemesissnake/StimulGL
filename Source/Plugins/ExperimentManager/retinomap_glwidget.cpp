#include "retinomap_glwidget.h"
#include <QDomNodeList>
#include <QGraphicsRectItem>
#include <QtCore/qmath.h>
#include <QFile>

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
	}	
	//if (screenShotWidget!=NULL)
	//{
	//	delete screenShotWidget;
	//}
}

void RetinoMap_glwidget::initialize()
{
	//Place your custom initialization code here:
	
	//Default values if none defined.
	rectScreenRes = getScreenResolution();
	float fAngle = 45.0;
	float fScreenCoverageRange = 0.5;
	stimWidthPixelAmount = rectScreenRes.height();
	totalWedgeScreenSpanSize = rectScreenRes.height();
	blockDurationInTriggers = 1;
	flickrSpeedFreq = 5;//5 times per second
	flickrThreshold = 1000 / flickrSpeedFreq;//First at 200 mSecs
	flickrSwitch = 0;
	textContent = "";
	colorBackground = QColor(0,0,0);
	brushBackground = QBrush(colorBackground);//(87, 87, 87));
	textPen = QPen(Qt::white);
	textFont.setPixelSize(20);
	totalWedgeDeg = 22.5;
	wedgeNrChecks = 4;
	wedgeNrRings = 20;
	rotationDirection = -1;
	cortMagFactor = 0.2;
	triggerDuration = 2000;
	centerGapWidth = 20.0;//aperture in the middle left blank
	fixationColor = QColor(255, 0, 0);
	fixationWidth =  8;
	textContent = "";
	nRetinoID = -1;
	currentExpType = RetinoMap_PolarAngle;
	showFixationPoint = true;
	StimulusResultImageFrame = NULL;
	elapsedTrialTime = 0;
	debugTestSamples = 10;
	debugUsedTestSamples = -1;
	debugTotalElapsedTime = 0;
	debugString = "";
	debugElapsedTime = 0;
	firstBlockTrialPaintFrame = false;
	outputTriggerFrame = false;
	lastTriggerNumber = -1;
	//screenShotWidget = NULL;
}

//void RetinoMap_glwidget::initExperimentObject()
//{
//	//Virtual, don't forget to call the base member first!
//	GLWidgetWrapper::initExperimentObject();
//	//Additional code:
//
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
	emit LogToExperimentManager("Started");
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

//bool RetinoMap_glwidget::setExperimentConfiguration(ExperimentConfiguration *pExpConfStruct)
//{
//	//Virtual, don't forget to call the base member first!
//	if(!GLWidgetWrapper::setExperimentConfiguration(pExpConfStruct))
//		return false;
//	//Additional code:
//
//	return true;
//}

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
	debugTime.start();
	//Virtual, don't forget to call the base member first!
	GLWidgetWrapper::initBlockTrial();
	//Additional code:


	//RectItems = new QVector<QRect>();
	//for (int i=0;i<20;i++)
	//{
	//	RectItems->append(QRect(50,50,60*i,60*i));
	//}


	QString tmpParamValue;
	debugString = "";

	if (getExperimentBlockParameter(getCurrentExperimentBlock(),nRetinoID,RETINOMAP_WIDGET_PATTERN,tmpParamValue))
	{
		if (tmpParamValue == RETINOMAP_WIDGET_PATTERN_POLARANGLE)
		{
			currentExpType = RetinoMap_PolarAngle;
		}
		else if (tmpParamValue == RETINOMAP_WIDGET_PATTERN_ECCENTRICITY)
		{
			currentExpType = RetinoMap_Eccentricity;
		}
		else if (tmpParamValue == RETINOMAP_WIDGET_PATTERN_MOVINGBAR)
		{
			currentExpType = RetinoMap_MovingBar;
		}
		else if (tmpParamValue == RETINOMAP_WIDGET_PATTERN_FIXATION)
		{
			currentExpType = RetinoMap_Fixation;
		}
	}

	if(currentExpType == RetinoMap_MovingBar)
	{
		if (getExperimentBlockParameter(getCurrentExperimentBlock(),nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_ANGLE,tmpParamValue))
			fAngle = tmpParamValue.toFloat();
		if (getExperimentBlockParameter(getCurrentExperimentBlock(),nRetinoID,RETINOMAP_WIDGET_MOVINGBAR_COVERAGE,tmpParamValue))
			fScreenCoverageRange = tmpParamValue.toFloat();
	}

	if (getExperimentBlockParameter(getCurrentExperimentBlock(),nRetinoID,RETINOMAP_WIDGET_SHOWFIXPOINT,tmpParamValue))
	{
		if (tmpParamValue == RETINOMAP_WIDGET_BOOL_TRUE)
		{
			showFixationPoint = true;
		} 
		else if (tmpParamValue == RETINOMAP_WIDGET_BOOL_FALSE)
		{
			showFixationPoint = false;
		}
	}

	if (getExperimentBlockParameter(getCurrentExperimentBlock(),nRetinoID,RETINOMAP_WIDGET_XPIXEL_AMOUNT,tmpParamValue))
		stimWidthPixelAmount = tmpParamValue.toFloat();
	if (getExperimentBlockParameter(getCurrentExperimentBlock(),nRetinoID,RETINOMAP_WIDGET_YPIXEL_AMOUNT,tmpParamValue))
		totalWedgeScreenSpanSize = tmpParamValue.toFloat();
	if (getExperimentBlockParameter(getCurrentExperimentBlock(),nRetinoID,RETINOMAP_WIDGET_CYCLEDURATION_TR,tmpParamValue))
		blockDurationInTriggers = tmpParamValue.toFloat();
	if (getExperimentBlockParameter(getCurrentExperimentBlock(),nRetinoID,RETINOMAP_WIDGET_FLICKRSPEED_HZ,tmpParamValue))
	{
		if(tmpParamValue.toFloat()>0)
			flickrSpeedFreq = tmpParamValue.toFloat();			
	}
	flickrThreshold = 1000 / flickrSpeedFreq;
	if (getExperimentBlockParameter(getCurrentExperimentBlock(),nRetinoID,RETINOMAP_WIDGET_WEDGESPAN_DEGREES,tmpParamValue))
		totalWedgeDeg = tmpParamValue.toFloat();
	if (getExperimentBlockParameter(getCurrentExperimentBlock(),nRetinoID,RETINOMAP_WIDGET_CHECK_AMOUNT,tmpParamValue))
		wedgeNrChecks = tmpParamValue.toFloat();
	if (getExperimentBlockParameter(getCurrentExperimentBlock(),nRetinoID,RETINOMAP_WIDGET_RING_AMOUNT,tmpParamValue))
		wedgeNrRings = tmpParamValue.toFloat();
	if (getExperimentBlockParameter(getCurrentExperimentBlock(),nRetinoID,RETINOMAP_WIDGET_ROTATION_DIRECTION,tmpParamValue))
		rotationDirection = tmpParamValue.toInt();
	if (getExperimentBlockParameter(getCurrentExperimentBlock(),nRetinoID,RETINOMAP_WIDGET_CORTMAG_FACTOR,tmpParamValue))
		cortMagFactor = tmpParamValue.toFloat();
	if (getExperimentBlockParameter(getCurrentExperimentBlock(),nRetinoID,RETINOMAP_WIDGET_TRDURATION_MSEC,tmpParamValue))
		triggerDuration = tmpParamValue.toInt();
	if (getExperimentBlockParameter(getCurrentExperimentBlock(),nRetinoID,RETINOMAP_WIDGET_GAP_DIAMETER,tmpParamValue))
		centerGapWidth = tmpParamValue.toInt();
	if (getExperimentBlockParameter(getCurrentExperimentBlock(),nRetinoID,RETINOMAP_WIDGET_OUTPUTTRIGGERFRAME,tmpParamValue))
	{
		if (tmpParamValue == RETINOMAP_WIDGET_BOOL_TRUE)
		{
			outputTriggerFrame = true;
		} 
		else if (tmpParamValue == RETINOMAP_WIDGET_BOOL_FALSE)
		{
			outputTriggerFrame = false;
		}
	}
	
	flickrSwitch = 0;
	StimulusResultImageFrame = new QPixmap(stimWidthPixelAmount,totalWedgeScreenSpanSize);//new QImage(stimWidthPixelAmount,totalWedgeScreenSpanSize, QImage::Format_ARGB32_Premultiplied);
	//The image is stored using a pre-multiplied 32-bit ARGB format (0xAARRGGBB), i.e. the red, green and 
	//blue channels are multiplied by the alpha component divided by 255. (If RR, GG, or BB has a higher 
	//value than the alpha channel, the results are undefined.) Certain operations (such as image 
	//composition using alpha blending) are faster using pre-multiplied ARGB32 than with plain ARGB32.

	debugUsedTestSamples = -1;
	debugTotalElapsedTime = 0;
	debugString = "";
	firstBlockTrialPaintFrame = true;
	debugInitBlockTrialTime = debugTime.restart();
	trialTime.start();//(Re)start the trial time
}

void RetinoMap_glwidget::paintEvent(QPaintEvent *event)
{
	//Virtual, don't forget to call the base member first!
	GLWidgetWrapper::paintEvent(event);
	//Additional code:


	//const QVector<QRect>
	QString tmpParamValue;
	float tmpFloatVal = 0;
	elapsedTrialTime = trialTime.elapsed();
	debugElapsedTime = debugTime.restart();
	QColor color1 = QColor(255, 255, 255);	//White
	QColor color2 = QColor(0, 0, 0);		//Black
	Qt::PenStyle style = Qt::SolidLine;		//Qt::NoPen
	Qt::PenCapStyle cap = Qt::FlatCap;

	StimulusResultImageFrame->fill(colorBackground);
	QPainter imgPainter(StimulusResultImageFrame);
	imgPainter.begin(this);
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
	if(currentExpType == RetinoMap_PolarAngle)//Polar Angle
	{
		float startAngle;
		wedgeSpanAngle = totalWedgeDeg / wedgeNrChecks * 16.0f;
		currentWedgeDiameter = ((StimulusResultImageFrame->height() - centerGapWidth)) / 2.0f * cortMagFactor;
		currentSize = StimulusResultImageFrame->height() - currentWedgeDiameter;
		currentXPoint = (StimulusResultImageFrame->width() - currentSize) / 2.0f;
		currentYPoint = (StimulusResultImageFrame->height() - currentSize) / 2.0f;
		tmpFloatVal = elapsedTrialTime / (blockDurationInTriggers * triggerDuration);
		if(rotationDirection == 1)//Clockwise
		{
			startAngle = -360.0f * tmpFloatVal;
		}
		else//Counterclockwise 
		{
			startAngle = 360.0f * tmpFloatVal;
		}
		for(int i=1; i<(wedgeNrRings+1);i++)
		{
			currentStartAngle = startAngle * 16.0f;
			for(int k=0; k<wedgeNrChecks;k++)
			{
				if(flickrSwitch==1)
				{
					if((k+i)%2==0)
					{
						imgPainter.setPen(QPen(color1, currentWedgeDiameter, style, cap));
					}
					else
					{
						imgPainter.setPen(QPen(color2, currentWedgeDiameter, style, cap));
					}
				}
				else
				{
					if((k+i)%2!=0)
					{
						imgPainter.setPen(QPen(color1, currentWedgeDiameter, style, cap));
					}
					else
					{
						imgPainter.setPen(QPen(color2, currentWedgeDiameter, style, cap));
					}
				}
				//if((i==1) || (i==5) || (i==10)) //(i%2)==0)
				imgPainter.drawArc(currentXPoint, currentYPoint, currentSize, currentSize, currentStartAngle, wedgeSpanAngle);//Same as drawing an partial ellipse
				currentStartAngle = currentStartAngle + wedgeSpanAngle;
			}
			if(isDebugMode() && (debugUsedTestSamples==debugTestSamples))
			{
				imgPainter.setPen(QPen(QColor(255,0,0), 1, style, cap));
				imgPainter.drawEllipse(currentXPoint, currentYPoint, currentSize, currentSize);
			}
			
			currentSize = currentSize - currentWedgeDiameter;//First subtract the first wedge diameter
			if(i==(wedgeNrRings-1))//Pre-last loop to prepare last wedge?
				currentWedgeDiameter = (currentSize-centerGapWidth) / 2;
			else
				currentWedgeDiameter = ((currentSize - currentWedgeDiameter) - centerGapWidth) / 2 * cortMagFactor;			
			currentSize = currentSize - currentWedgeDiameter;//First subtract the second wedge diameter
			currentXPoint = (StimulusResultImageFrame->width() - currentSize) / 2.0f;
			currentYPoint = (StimulusResultImageFrame->height() - currentSize) / 2.0f;
		}
		if(showFixationPoint) // show fix cross
		{
			imgPainter.setPen(QPen(fixationColor, fixationWidth, style, Qt::RoundCap));
			imgPainter.drawPoint(StimulusResultImageFrame->width()/2, StimulusResultImageFrame->height()/2);
		}
	}
	else if(currentExpType == RetinoMap_Eccentricity)//Eccentricity
	{
		wedgeSpanAngle = 360.0f / wedgeNrChecks * 16.0f;
		tmpFloatVal = elapsedTrialTime / (blockDurationInTriggers * triggerDuration);
		if(rotationDirection == -1)//Decrease ring diameter
		{
			currentCompleteWedgeDiameter = (((StimulusResultImageFrame->height() - centerGapWidth) * cortMagFactor) - ((StimulusResultImageFrame->height() - centerGapWidth) * tmpFloatVal * cortMagFactor)) / 2.0f;
			currentWedgeDiameter = currentCompleteWedgeDiameter / wedgeNrRings;
			currentSize = (StimulusResultImageFrame->height() - currentWedgeDiameter) - ((StimulusResultImageFrame->height() - currentWedgeDiameter - centerGapWidth) * tmpFloatVal);			
		}
		else//Increase ring diameter
		{
			currentCompleteWedgeDiameter = ((StimulusResultImageFrame->height() - centerGapWidth) * tmpFloatVal * cortMagFactor) / 2.0f;
			currentWedgeDiameter = currentCompleteWedgeDiameter / wedgeNrRings;
			currentSize = centerGapWidth + ((StimulusResultImageFrame->height() - currentWedgeDiameter - centerGapWidth) * tmpFloatVal);
		}
		currentXPoint = (StimulusResultImageFrame->width() - currentSize) / 2.0f;
		currentYPoint = (StimulusResultImageFrame->height() - currentSize) / 2.0f;
		for(int i=1; i<wedgeNrRings+1;i++)
		{
			float startAngle = 0.0f * 16.0f;
			for(int k=0; k<wedgeNrChecks; k++)
			{
				if(flickrSwitch ==1)
				{
					if((k+i)%2==0)
					{
						imgPainter.setPen(QPen(color1, currentWedgeDiameter, style, cap));
					}
					else
					{
						imgPainter.setPen(QPen(color2, currentWedgeDiameter, style, cap));
					}
				}
				else
				{
					if((k+i)%2!=0)
					{
						imgPainter.setPen(QPen(color1, currentWedgeDiameter, style, cap));
					}
					else
					{
						imgPainter.setPen(QPen(color2, currentWedgeDiameter, style, cap));
					}
				}
				imgPainter.drawArc(currentXPoint, currentYPoint, currentSize, currentSize, startAngle, wedgeSpanAngle);
				startAngle = startAngle + wedgeSpanAngle;
			}
			if(isDebugMode() && (debugUsedTestSamples==debugTestSamples))
			{
				imgPainter.setPen(QPen(QColor(255,0,0), 1, style, cap));
				imgPainter.drawEllipse(currentXPoint, currentYPoint, currentSize, currentSize);//To draw this it takes about 2ms additional time!!
			}
			if(rotationDirection == -1)//Decrease ring diameter
			{
				currentSize = currentSize - (2*currentWedgeDiameter);
			}
			else//Increase ring diameter
			{
				currentSize = currentSize - (2*currentWedgeDiameter);
			}
			currentXPoint = (StimulusResultImageFrame->width() - currentSize) / 2.0f;
			currentYPoint = (StimulusResultImageFrame->height() - currentSize) / 2.0f;
		}		
		if(showFixationPoint) // show fix cross
		{
			imgPainter.setPen(QPen(fixationColor, fixationWidth, style, Qt::RoundCap));
			imgPainter.drawPoint(StimulusResultImageFrame->width()/2, StimulusResultImageFrame->height()/2);
		}
	}
	else if(currentExpType == RetinoMap_MovingBar)//MovingBar
	{

		
		//graphItem->at(0)->setRect(QRect..);
		//QPainter painter(&resultImage);
		//painter.setCompositionMode(QPainter::CompositionMode_Source);
		//painter.fillRect(resultImage.rect(), Qt::transparent);
		//painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
		//painter.drawImage(0, 0, destinationImage);
		//painter.setCompositionMode(mode);
		//painter.drawImage(0, 0, sourceImage);
		//painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
		//painter.fillRect(resultImage.rect(), Qt::white);
		//painter.end();

		//resultLabel->setPixmap(QPixmap::fromImage(resultImage));

		//QPainterPath tmpPainterPath;
		tmpFloatVal = elapsedTrialTime / (blockDurationInTriggers * triggerDuration);

		if(showFixationPoint) // show fix cross
		{
			imgPainter.setPen(QPen(fixationColor, fixationWidth, style, Qt::RoundCap));
			imgPainter.drawPoint(StimulusResultImageFrame->width()/2, StimulusResultImageFrame->height()/2);
		}

		//screenx = StimulusResultImageFrame->width();
		//screeny = StimulusResultImageFrame->height();
		if(isDebugMode() && (debugUsedTestSamples==debugTestSamples))
		{
			imgPainter.setPen(QPen(QColor(255,0,0), 0, style, cap));
			imgPainter.drawRect((StimulusResultImageFrame->width()/2)-(StimulusResultImageFrame->width()/2),(StimulusResultImageFrame->height()/2)-(StimulusResultImageFrame->height()/2),StimulusResultImageFrame->width(),StimulusResultImageFrame->height());
		}

		float fStimulusDiameter = qSqrt(qPow(StimulusResultImageFrame->width(),2) + qPow(StimulusResultImageFrame->height(),2));//qSqrt(qPow(StimulusResultImageFrame->width(),2) + qPow(StimulusResultImageFrame->height(),2));
		imgPainter.translate(StimulusResultImageFrame->width()/2, StimulusResultImageFrame->height()/2);
		imgPainter.rotate(fAngle);
		if(rotationDirection == -1)//Down->Up (When 0 <= fAngle >= 180 degrees)
		{
			imgPainter.translate(0,((0.5 * fScreenCoverageRange * fStimulusDiameter) - ( fScreenCoverageRange * fStimulusDiameter * tmpFloatVal)));
		}
		else//Up->Down (When 0 <= fAngle >= 180 degrees)
		{
			imgPainter.translate(0,((-0.5 * fScreenCoverageRange * fStimulusDiameter) + ( fScreenCoverageRange * fStimulusDiameter * tmpFloatVal)));
		}
		currentSize = fStimulusDiameter/wedgeNrChecks;//StimulusResultImageFrame->height() - (currentWedgeDiameter / 2.0f);
		currentYPoint = (-1 * wedgeNrRings * currentSize) / 2.0f;
		for (int i=1; i<wedgeNrRings+1;i++)
		{
			currentXPoint = -fStimulusDiameter/2;
			for(int k=0; k<wedgeNrChecks;k++)
			{
				if(flickrSwitch==1)
				{
					if((k+i)%2==0)
					{	
						//if(isDebugMode() && (k==(wedgeNrChecks/2)))
						//{
						//	imgPainter.setPen(QPen(QColor(255,0,0), 0, style, cap));
						//	imgPainter.setBrush(QColor(255,0,0));
						//}
						//else
						//{
							imgPainter.setPen(QPen(color1, currentSize, style, cap));
							imgPainter.setBrush(color1);
						//}
					}
					else
					{
						//if(isDebugMode()&&(k==(wedgeNrChecks/2)))
						//{
						//	imgPainter.setPen(QPen(QColor(255,0,0), 0, style, cap));
						//	imgPainter.setBrush(QColor(255,0,0));
						//}
						//else
						//{
							imgPainter.setPen(QPen(color2, currentSize, style, cap));
							imgPainter.setBrush(color2);
						//}
					}

				}
				else
				{
					if((k+i)%2!=0)
					{
						//if(isDebugMode()&&(k==(wedgeNrChecks/2)))
						//{
						//	imgPainter.setPen(QPen(QColor(255,0,0), 0, style, cap));
						//	imgPainter.setBrush(QColor(255,0,0));
						//}
						//else
						//{
							imgPainter.setPen(QPen(color1,currentSize, style, cap));
							imgPainter.setBrush(color1);
						//}
					}
					else
					{
						//if(isDebugMode()&&(k==(wedgeNrChecks/2)))
						//{
						//	imgPainter.setPen(QPen(QColor(255,0,0), 0, style, cap));
						//	imgPainter.setBrush(QColor(255,0,0));
						//}
						//else
						//{
							imgPainter.setPen(QPen(color2,currentSize, style, cap));
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
				//}
				
				//tmpPainterPath.moveTo(0,0);
				//if((flickrSwitch==1) && ((k+i)%2==0))
				//{
				//	tmpPainterPath.addRect(currentXPoint, currentYPoint, currentSize, currentSize);//(500,500,200,200);
				//}
				currentXPoint = currentXPoint + currentSize;//(xwidth - currentSize) / 2.0f;
			}
			//QPen pen;
			//pen.setStyle(Qt::CustomDashLine);
			//pen.setDashPattern([10, 10, 10, 10]);
			//imgPainter.setPen(pen);
			//imgPainter.drawRect(currentXPoint, currentYPoint, currentSize, currentSize);
			
			//imgPainter.drawRects(*RectItems);

			//QPen myPen(color2, 0, style, cap);
			//QRectF myRectangle(100,100,200,200);
			//QRectF myRectangle2(200,200,300,400);

			//
			//tmpPainterPath.addRect(myRectangle);
			//tmpPainterPath.addRect(myRectangle2);

			//imgPainter.setBrush(color2);
			//imgPainter.setPen(myPen);
			//imgPainter.drawPath(tmpPainterPath);
			//tmpPainterPath = QPainterPath();//Rest the path again for re-usage


			if(wedgeNrRings>1)
			{
				currentYPoint = currentYPoint + currentSize;
			}
		}
	}	
	else if(currentExpType == RetinoMap_Fixation)//Fixation
	{
		//tmpFloatVal = elapsedTrialTime / (blockDurationInTriggers * triggerDuration);
		if(showFixationPoint) // show fix cross
		{
			imgPainter.setPen(QPen(fixationColor, fixationWidth, style, Qt::RoundCap));
			imgPainter.drawPoint(StimulusResultImageFrame->width()/2, StimulusResultImageFrame->height()/2);
		}
	}
	imgPainter.end();

	if (outputTriggerFrame)
	{
		QString outputDir = MainAppInfo::outputsDirPath() + "/outputs/";
		if(QDir(outputDir).exists()==false)
		{
			QDir().mkdir(outputDir);
		}
		outputDir = outputDir + "RetinoWidget/";
		if(QDir(outputDir).exists()==false)
		{
			QDir().mkdir(outputDir);
		}
		outputDir = outputDir + getLastLoggedObjectStateTime(Experiment_SubObject_Started) + "/";
		if(QDir(outputDir).exists()==false)
		{
			QDir().mkdir(outputDir);
		}
		int currentTrigger = getCurrentExperimentTriggers();
		if (currentTrigger > lastTriggerNumber)
		{
			int currentTrial = getCurrentExperimentTrial();
			int currentBlock = getCurrentExperimentBlock();
			QString fileName = outputDir + QString::number(nRetinoID) + "_" + QString::number(currentBlock) + QString("_") + QString::number(currentTrial) + QString("_") + QString::number(currentTrigger) + QString(".png");
			QFile file(fileName);
			if (file.open(QIODevice::WriteOnly))
			{
				StimulusResultImageFrame->save(&file, "PNG");
				file.close();
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

	if(isDebugMode())
	{
		if (debugUsedTestSamples == -1)//This is the first experiment sample and is ignored for the speed measurement
		{
			debugTotalElapsedTime = 0;
			debugUsedTestSamples++;
		}
		else if (debugUsedTestSamples<debugTestSamples)//During these samples we collect the information for the speed measurement
		{
			debugTotalElapsedTime = debugTotalElapsedTime + debugElapsedTime;
			debugUsedTestSamples++;
			debugString = debugString + ", " + QString::number(debugElapsedTime);
		}
		else if (debugUsedTestSamples==debugTestSamples)//During this sample all the debugging functionality is processed and also ignored for the speed measurement
		{
			float fAverageElapsed = debugTotalElapsedTime/debugUsedTestSamples;
			textContent = QString("Average elapsed time: %1").arg(fAverageElapsed) + ": " + debugString; //"Retinotopic Mapping";
			painter.drawText(QRect(10, 0, rectScreenRes.width(), 100), Qt::AlignLeft, textContent);//Takes about 5ms additional total drawing time!
			textContent = "TriggerNr:" + QString::number(getCurrentExperimentTriggers()) + ", TrialNr:" + QString::number(getCurrentExperimentTrial()) + ", BlockNr:" + QString::number(getCurrentExperimentBlock());
			painter.drawText(QRect(10, 50, rectScreenRes.width(), 100), Qt::AlignLeft, textContent);
			debugUsedTestSamples++;
		}
		else if (debugUsedTestSamples==(debugTestSamples+1))//During this sample we reset all the information for the speed measurement again
		{
			debugTotalElapsedTime = 0;
			debugUsedTestSamples = 0;
		}
	}
	painter.drawPixmap((rectScreenRes.width()-StimulusResultImageFrame->width())/2,(rectScreenRes.height()-StimulusResultImageFrame->height())/2,*StimulusResultImageFrame);
	//QPainter::CompositionMode mode = QPainter::CompositionMode_SourceIn;
	//painter.setPen(Qt::NoPen);
	//painter.setBrush(Qt::SolidPattern);
	//painter.drawRect(100,100,500,500);
	painter.end();
	GLWidgetWrapper::finalizePaintEvent();
}
