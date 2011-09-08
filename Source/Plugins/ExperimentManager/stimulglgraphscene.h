#ifndef STIMULGLGRAPHSCENE_H
#define STIMULGLGRAPHSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QKeyEvent>

class StimulGLGraphScene : public QGraphicsScene
{
	Q_OBJECT

signals:
	void UserWantsToClose();

public:
	StimulGLGraphScene(QObject *parent);
	~StimulGLGraphScene();

	bool Init();

protected:
	void keyPressEvent(QKeyEvent * keyEvent);

private:
	QGraphicsRectItem *m_GraphRectItem;
	QGraphicsItem *m_GraphItem;	
};

#endif // STIMULGLGRAPHSCENE_H
