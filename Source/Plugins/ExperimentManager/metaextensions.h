#ifndef METAEXTENSIONS_H
#define METAEXTENSIONS_H

//#include <QObject>
#include <QWidget>
#include <QTimer>

class TriggerTimer : public QTimer
{
public:
	TriggerTimer(QWidget *parent = 0){};
	TriggerTimer(const TriggerTimer& other ){};//TODO fill in copy constructor, should be declared for the Q_DECLARE_METATYPE macro
	//~TriggerTimer(){};
};

#endif // METAEXTENSIONS_H
