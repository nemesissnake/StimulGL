#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include "Global.h"
//#include <QBrush>
//#include <QPen>
//#include <QPixmap>

class RenderArea : public QWidget
{
	Q_OBJECT

public:
	RenderArea(QWidget *parent = NULL);
	~RenderArea();

public slots:
	void UpdatePosition(float XPos, float YPos, bool bShowRef = false);
	void setAntialiased(bool antialiased);

protected:
	void paintEvent(QPaintEvent *event);

private:
	//Shape shape;
	//QPen pen;
	//QBrush brush;
	bool m_bAntialiased;
	bool m_bShowRefLines;
	float m_fXPos;
	float m_fYPos;
};

#endif // RENDERAREA_H
