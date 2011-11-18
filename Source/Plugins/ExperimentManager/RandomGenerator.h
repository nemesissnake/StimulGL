#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <QObject>

class RandomGenerator : public QObject
{
	Q_OBJECT

public:
	RandomGenerator(QObject *parent = NULL);
	~RandomGenerator();

public slots:
	int randomize(int Min, int Max);

private:
};

#endif // RANDOMGENERATOR_H
