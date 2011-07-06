#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
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
	//void setShape(Shape shape);
	void setAntialiased(bool antialiased);

protected:
	void paintEvent(QPaintEvent *event);

private:
	//Shape shape;
	//QPen pen;
	//QBrush brush;
	bool bAntialiased;	
};

#endif // RENDERAREA_H
