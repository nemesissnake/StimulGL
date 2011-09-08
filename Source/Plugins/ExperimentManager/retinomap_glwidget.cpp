#include "retinomap_glwidget.h"
//#include <QPaintEvent>

RetinoMap_glwidget::RetinoMap_glwidget(QWidget *parent) : GLWidgetWrapper(parent)
{
	initialize();
	GLWidgetWrapper::SetupLayout(this);
}

RetinoMap_glwidget::~RetinoMap_glwidget()
{

}

void RetinoMap_glwidget::initialize()
{
	//Place your custom initialization code here:

}

//////////////////////////////////////////////////////////////////////////Virtual overridden member functions

bool RetinoMap_glwidget::start()
{
	//Virtual, don't forget to call the base member first!
	if(!GLWidgetWrapper::start())
		return false;
	//Additional code:

	if(!this->format().doubleBuffer())// check whether we have double buffering, otherwise cancel
	{
		//qDebug() << "RetinoMap_glwidget::start::No Double Buffering available!";
		//stimContainerDlg->deleteLater();//Schedules this object for deletion, the object will be deleted when control returns to the event loop
		return false;
	}
	return true;
}

//void RetinoMap_glwidget::init()
//{
//	//Virtual, don't forget to call the base member first!
//	GLWidgetWrapper::init();
//	//Additional code:
//
//}

//bool RetinoMap_glwidget::stop()
//{
//	//Virtual, don't forget to call the base member first!
//	if(!GLWidgetWrapper::stop())
//		return false;
//	//Additional code:
//
//	return true;
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

void RetinoMap_glwidget::initBlockTrial()
{
	//Virtual, don't forget to call the base member first!
	GLWidgetWrapper::initBlockTrial();
	//Additional code:

	background = QBrush(QColor(87, 87, 87)); // to do change into variable settings for user
	textPen = QPen(Qt::white);
	textFont.setPixelSize(20);
	textContent = "";
	xwidth = 1024;
	ywidth = 768;
	x_size_stim = 760;//BlockTrialParamTable[0].at(6).toInt(); // read by user TODO
	y_size_stim = 760;//BlockTrialParamTable[0].at(7).toInt(); // read by user TODO
	cycle_dur = 32;//BlockTrialParamTable[0].at(0).toFloat();
	flickr_speed = 1000 / 5;//BlockTrialParamTable[0].at(5).toInt();
	flickr_switch = 0;
	//if(ExpType < 1) // for eccentricity // TODO
	//{
	//	xstart_rel = (xwidth - x_size_stim) / 2.0;
	//	ystart_rel = (ywidth - y_size_stim) / 2.0;
	//}
	//else // for all polar wedges
	//{
		xstart_rel = (xwidth - x_size_stim) / 2.0;
		ystart_rel = (ywidth - y_size_stim) / 2.0;
	//}
	trialTime.start();
}

void RetinoMap_glwidget::paintEvent(QPaintEvent *event)
{
	//Virtual, don't forget to call the base member first!
	GLWidgetWrapper::paintEvent(event);
	//Additional code:

	//if(BlockTrialParamTable.size() == 0)
	//	return;
	QPainter painter;
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.fillRect(event->rect(), background);
	painter.setPen(textPen);
	painter.setFont(textFont);
	//if(bDebugMode)
	//{
		//textContent = QString("Elapsed: %1").arg(elapsed); // "Retinotopic Mapping";
	//	textContent = QString::number(elapsed);
	//	painter.drawText(QRect(10, 0, 200, 100), Qt::AlignLeft, textContent);
		//if(pMainObject->currentBlockTrial < pMainObject->BlockTrialFiles.size())
		//{
		//	textContent = QString::number(pMainObject->completedTR) + " " + pMainObject->BlockTrialFiles[pMainObject->currentBlockTrial];
		//	painter.drawText(QRect(10, 50, 200, 100), Qt::AlignLeft, textContent);
		//}
	//}
	// colors of wedges TODO specify by user
	QColor color1 = QColor(255, 255, 255);
	QColor color2 = QColor(0, 0, 0);
	// access trial/block parameters
	float wedge_deg = 22.5;//BlockTrialParamTable[0].at(1).toFloat();
	float wedge_nr_checks = 4;//BlockTrialParamTable[0].at(2).toFloat();
	float wedge_nr_rings = 20;//BlockTrialParamTable[0].at(3).toFloat();
	int rot_dir = -1;//BlockTrialParamTable[0].at(4).toInt();
	float cort_mag_factor = 0.2;//BlockTrialParamTable[0].at(8).toFloat();
	int cent_gap = 20.0; // aperture in the middle left blank
	Qt::PenStyle style = Qt::SolidLine; // Qt::NoPen
	Qt::PenCapStyle cap = Qt::FlatCap;
	int draw_page, disp_page;
	draw_page = nFrameCounter % 2;
	disp_page = (nFrameCounter+1) % 2;
	int i,k; 
	if(trialTime.elapsed() >= flickr_speed)//Can we change the flickr?
	{
		if(flickr_switch == 0)
			flickr_switch = 1;
		else
			flickr_switch = 0;
		flickr_speed += 1000 / 5;//BlockTrialParamTable[0].at(5).toInt();//Calculate the next flickr moment in time
	}
	//if(ExpType > 0) // polar angle
	//{
		float xWedge = wedge_deg / wedge_nr_checks * 16.0f;
		float yWedge = ((y_size_stim - cent_gap)) * cort_mag_factor;
		float cur_ysize = y_size_stim - (yWedge / 2.0f);
		float cur_xpt = (xwidth - cur_ysize) / 2.0f;
		float cur_ypt = (ywidth - cur_ysize) / 2.0f;
		if(rot_dir == -1)
		{
			startAngle = ((360.0f) / (cycle_dur * 1000 * TR)) * trialTime.elapsed();
		}
		else
		{
			startAngle = ((360.0f) / (cycle_dur * 1000 * TR)) * -(trialTime.elapsed());
		}
		for(i=1; i<wedge_nr_rings+1;i++)
		{
			float startAngle_temp = startAngle * 16.0f;
			for(k=0; k<wedge_nr_checks;k++)
			{
				if(flickr_switch==1)
				{
					if((k+i)%2==0)
					{
						painter.setPen(QPen(color1, yWedge/2.0, style, cap));
					}
					else
					{
						painter.setPen(QPen(color2, yWedge/2.0, style, cap));
					}
				}
				else
				{
					if((k+i)%2!=0)
					{
						painter.setPen(QPen(color1, yWedge/2.0, style, cap));
					}
					else
					{
						painter.setPen(QPen(color2, yWedge/2.0, style, cap));
					}
				}
				painter.drawArc(cur_xpt, cur_ypt, cur_ysize, cur_ysize, startAngle_temp, xWedge);
				startAngle_temp = startAngle_temp + xWedge;
			}
			cur_ysize = cur_ysize - (yWedge / 2.0f);
			if(i==wedge_nr_rings-1)
			{yWedge = (cur_ysize-cent_gap);}
			else
			{yWedge = (cur_ysize-cent_gap) * cort_mag_factor;}
			cur_ysize = cur_ysize - (yWedge / 2.0f);
			cur_xpt = (xwidth - cur_ysize) / 2.0f;
			cur_ypt = (ywidth - cur_ysize) / 2.0f;
		}
	//}
	//else // eccentricity
	//{
	//	float xWedge = wedge_deg / wedge_nr_checks * 16.0f;
	//	float yWedge = ((y_size_stim - cent_gap)) * cort_mag_factor - (((y_size_stim - cent_gap) / (cycle_dur * 1000 * TR)) * trialTime.elapsed()) * cort_mag_factor;
	//	float cur_ysize = (y_size_stim - yWedge/2.0f) - (((y_size_stim - cent_gap) / (cycle_dur * 1000 * TR)) * trialTime.elapsed());
	//	float cur_xpt = (xwidth - cur_ysize) / 2.0f;
	//	float cur_ypt = (ywidth - cur_ysize) / 2.0f;
	//	for(i=1; i<wedge_nr_rings+1;i++)
	//	{
	//		float startAngle = 0.0f * 16.0f;
	//		for(k=0; k<wedge_nr_checks; k++)
	//		{
	//			if(flickr_switch ==1)
	//			{
	//				if((k+i)%2==0)
	//				{
	//					painter.setPen(QPen(color1, yWedge/2.0, style, cap));
	//				}
	//				else
	//				{
	//					painter.setPen(QPen(color2, yWedge/2.0, style, cap));
	//				}
	//			}
	//			else
	//			{
	//				if((k+i)%2!=0)
	//				{
	//					painter.setPen(QPen(color1, yWedge/2.0, style, cap));
	//				}
	//				else
	//				{
	//					painter.setPen(QPen(color2, yWedge/2.0, style, cap));
	//				}
	//			}
	//			painter.drawArc(cur_xpt, cur_ypt, cur_ysize, cur_ysize, startAngle, xWedge);
	//			startAngle = startAngle + xWedge;
	//		}
	//		cur_ysize = cur_ysize - (yWedge / 2.0f);
	//		cur_ysize = cur_ysize - (yWedge / 2.0f);
	//		cur_xpt = (xwidth - cur_ysize) / 2.0f;
	//		cur_ypt = (ywidth - cur_ysize) / 2.0f;
	//	}
	//}

	if(1) // show fix cross
	{
		QColor fix_color = QColor(255, 0, 0);
		int fix_width =  8;
		painter.setPen(QPen(fix_color, fix_width, style, Qt::RoundCap));
		painter.drawPoint(xwidth/2, ywidth/2);
	}
	painter.end();

	
	//Before ending we need to call the base finalizePaintEvent();
	GLWidgetWrapper::finalizePaintEvent();
}
