/****************************************************************************
**
** Copyright (C) 20011 Rainer Goebel / Jan Zimmermann
** All rights reserved.
** Contact: Rainer Goebel (rainergoebel@mac.com)
**
**
** GNU General Public License
** This file may be used under the terms of the GNU General Public
** License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
****************************************************************************/

#include "renderWdg.h"
#include "Retinotopic_Mapping_dialog.h"
#include "globals.h"

#include <QPicture>
#include <QPainter>
#include <QPaintEvent>
#include <QCloseEvent>
#include <QTimer>
#include <QDesktopWidget>
#include <math.h>
#include <time.h>

GLWidget::GLWidget(QGLFormat fmt, QWidget *parent, Retinotopic_Mapping *pParentControlObject) : QGLWidget(fmt, parent) // initialize openGL window
{
    setFixedSize(1024, 768);
    setAutoFillBackground(false);
    setAutoBufferSwap(false); // in order to have control over time point for buffer swap
    pMainObject = pParentControlObject;
	bForceToStop = false;
}

GLWidget::~GLWidget()
{
}

void GLWidget::initBlockTrial()
{
	bForceToStop = false;
    background = QBrush(QColor(87, 87, 87)); // to do change into variable settings for user
    textPen = QPen(Qt::white);
    textFont.setPixelSize(20);
    textContent = "";

    frame_counter = 0;
#ifdef Q_WS_MACX
    MinUpdateTime = 1; // make param in interface and recommend per platform
#else
    MinUpdateTime = 1; // on Win (with recent OGL drivers), "swapBuffers" will wait for n retraces as indicated by "setSwapInterval" command (works like DX "flip")
#endif

    xwidth = 1024;
    ywidth = 768;
    x_size_stim = BlockTrialParamTable[0].at(6).toInt(); // read by user TODO
    y_size_stim = BlockTrialParamTable[0].at(7).toInt(); // read by user TODO
    cycle_dur = BlockTrialParamTable[0].at(0).toFloat();
    flickr_speed = 1000 / BlockTrialParamTable[0].at(5).toInt();
    flickr_switch = 0;

    if(ExpType < 1) // for eccentricity // TODO
    {
        xstart_rel = (xwidth - x_size_stim) / 2.0;
        ystart_rel = (ywidth - y_size_stim) / 2.0;
    }
    else // for all polar wedges
    {
        xstart_rel = (xwidth - x_size_stim) / 2.0;
        ystart_rel = (ywidth - y_size_stim) / 2.0;
    }
    trialTime.start();
}

void GLWidget::onAnimate()
{
    elapsed = frameTime.elapsed();//Calculate the elapsed time since started
    frameTime.restart();
    repaint();//calls the below void GLWidget::paintEvent(QPaintEvent *event)
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    if(BlockTrialParamTable.size() == 0)
        return;
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), background);
    painter.setPen(textPen);
    painter.setFont(textFont);
    frame_counter += 1;
    if(pMainObject->m_DebugMode)
    {
        //textContent = QString("Elapsed: %1").arg(elapsed); // "Retinotopic Mapping";
        textContent = QString::number(elapsed);
        painter.drawText(QRect(10, 0, 200, 100), Qt::AlignLeft, textContent);
        if(pMainObject->currentBlockTrial < pMainObject->BlockTrialFiles.size())
        {
            textContent = QString::number(pMainObject->completedTR) + " " + pMainObject->BlockTrialFiles[pMainObject->currentBlockTrial];
            painter.drawText(QRect(10, 50, 200, 100), Qt::AlignLeft, textContent);
        }
    }
    // colors of wedges TODO specify by user
    QColor color1 = QColor(255, 255, 255);
    QColor color2 = QColor(0, 0, 0);
    // access trial/block parameters
    float wedge_deg = BlockTrialParamTable[0].at(1).toFloat();
    float wedge_nr_checks = BlockTrialParamTable[0].at(2).toFloat();
    float wedge_nr_rings = BlockTrialParamTable[0].at(3).toFloat();
    int rot_dir = BlockTrialParamTable[0].at(4).toInt();
    float cort_mag_factor = BlockTrialParamTable[0].at(8).toFloat();
    int cent_gap = 20.0; // aperture in the middle left blank
    Qt::PenStyle style = Qt::SolidLine; // Qt::NoPen
    Qt::PenCapStyle cap = Qt::FlatCap;
    int draw_page, disp_page;
    draw_page = frame_counter % 2;
    disp_page = (frame_counter+1) % 2;
    int i,k; 
    if(trialTime.elapsed() >= flickr_speed)
    {
        if(flickr_switch == 0)
            flickr_switch = 1;
        else
            flickr_switch = 0;
        flickr_speed += 1000 / BlockTrialParamTable[0].at(5).toInt();
    }
    if(ExpType > 0) // polar angle
    {
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
    }
    else // eccentricity
    {
        float xWedge = wedge_deg / wedge_nr_checks * 16.0f;
        float yWedge = ((y_size_stim - cent_gap)) * cort_mag_factor - (((y_size_stim - cent_gap) / (cycle_dur * 1000 * TR)) * trialTime.elapsed()) * cort_mag_factor;
        float cur_ysize = (y_size_stim - yWedge/2.0f) - (((y_size_stim - cent_gap) / (cycle_dur * 1000 * TR)) * trialTime.elapsed());
        float cur_xpt = (xwidth - cur_ysize) / 2.0f;
        float cur_ypt = (ywidth - cur_ysize) / 2.0f;
        for(i=1; i<wedge_nr_rings+1;i++)
        {
            float startAngle = 0.0f * 16.0f;
            for(k=0; k<wedge_nr_checks; k++)
            {
                if(flickr_switch ==1)
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
                painter.drawArc(cur_xpt, cur_ypt, cur_ysize, cur_ysize, startAngle, xWedge);
                startAngle = startAngle + xWedge;
            }
            cur_ysize = cur_ysize - (yWedge / 2.0f);
            cur_ysize = cur_ysize - (yWedge / 2.0f);
            cur_xpt = (xwidth - cur_ysize) / 2.0f;
            cur_ypt = (ywidth - cur_ysize) / 2.0f;
        }
    }

    if(1) // show fix cross
    {
        QColor fix_color = QColor(255, 0, 0);
        int fix_width =  8;
        painter.setPen(QPen(fix_color, fix_width, style, Qt::RoundCap));
        painter.drawPoint(xwidth/2, ywidth/2);
    }
    painter.end();
    if(1)
    {
        while(1)//Don't go too fast...
        {
            elapsed = frameTime.elapsed();
            if(elapsed > MinUpdateTime)
                break;
        }
        swapBuffers();
        if( !pMainObject->checkForNextBlockTrial() ) //Check whether we need to prepare for an new block Trial, otherwise directly call onAnimate()
		{
			if(bForceToStop)
			{
				return;
			}
			// if next trial/block reached, the function will prepare for it and calls "onAnimate()" when ready, otherwise we compute next frame
			QTimer::singleShot(1, this, SLOT(onAnimate()));
		}
    }
}

