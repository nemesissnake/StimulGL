#include <QtGui>
#include "renderarea.h"

RenderArea::RenderArea(QWidget *parent)	: QWidget(parent)
{
	//shape = Polygon;
	bAntialiased = false;
	//transformed = false;
	
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);
}

RenderArea::~RenderArea()
{

}

//void RenderArea::setShape(Shape shape)
//{
	//this->shape = shape;
	//update();
//}

void RenderArea::setAntialiased(bool antialiased)
{
	this->bAntialiased = antialiased;
	update();
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
	QPoint CrossHair((width()/100)*70 ,(height()/100)*30);
	QRect rectCrossHair(CrossHair.x()-2,CrossHair.y()-2,4,4);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, false);
	QPen pen(Qt::black, 1, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	QRect rect(10, 10, width()-20, height()-20);	
	painter.drawRect(rect);
	//painter.drawLine(rect.bottomLeft(), rect.topRight());
	//painter.drawText(rect, Qt::AlignCenter, tr("Qt by\nNokia"));
	
	painter.drawRect(rectCrossHair);
	//painter.drawLine(CrossHair,CrossHair);
	//painter.drawLine(CrossHair,CrossHair);
	painter.setPen(palette().dark().color());
	rect.adjust(10,10,-10,-10);
	painter.drawRect(rect);
	painter.drawRect(QRect(0, 0, width() - 1, height() - 1));



	//static const QPoint points[4] = {
	//	QPoint(10, 80),
	//	QPoint(20, 10),
	//	QPoint(80, 30),
	//	QPoint(90, 70)
	//};
	//painter.setPen(pen);
	//painter.setBrush(brush);
	//if (antialiased)
	//	painter.setRenderHint(QPainter::Antialiasing, true);

	//for (int x = 0; x < width(); x += 100) {
	//	for (int y = 0; y < height(); y += 100) {
			//painter.save();
			//painter.translate(x, y);
	//		if (transformed) {
	//			painter.translate(50, 50);
	//			painter.rotate(60.0);
	//			painter.scale(0.6, 0.9);
	//			painter.translate(-50, -50);
	//		}
	//		switch (shape) {
	//		 case Line:
				 //painter.drawLine(rect.bottomLeft(), rect.topRight());
	//			 break;
	//		 case Points:
				 //painter.drawPoints(points, 4);
	//			 break;
	//		 case Polyline:
	//			 painter.drawPolyline(points, 4);
	//			 break;
	//		 case Polygon:
	//			 painter.drawPolygon(points, 4);
	//			 break;
	//		 case Rect:
	//			 painter.drawRect(rect);
	//			 break;
	//		 case RoundedRect:
	//			 painter.drawRoundedRect(rect, 25, 25, Qt::RelativeSize);
	//			 break;
	//		 case Ellipse:
	//			 painter.drawEllipse(rect);
	//			 break;
	//		 case Arc:
	//			 painter.drawArc(rect, startAngle, arcLength);
	//			 break;
	//		 case Chord:
	//			 painter.drawChord(rect, startAngle, arcLength);
	//			 break;
	//		 case Pie:
	//			 painter.drawPie(rect, startAngle, arcLength);
	//			 break;
	//		 case Path:
	//			 painter.drawPath(path);
	//			 break;
	//		 case Text:
	//			 painter.drawText(rect, Qt::AlignCenter, tr("Qt by\nNokia"));
	//			 break;
	//		 case Pixmap:
	//			 painter.drawPixmap(10, 10, pixmap);
	//		}
			//painter.restore();
		//}
	//}
}