#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <QObject>
#include <QStringList>
#ifdef Q_OS_WIN
	#include "windows.h"
#endif

enum RandomGenerator_RandomizeMethod
{
	RandomGenerator_RandomizeStandard = 0,
	RandomGenerator_RandomizePreservedIndexes = 1,
	RandomGenerator_RandomizePreservedValues = 2
};

class RandomGenerator : public QObject, public QStringList
{
	Q_OBJECT

public:
	RandomGenerator(QObject *parent = NULL);
	~RandomGenerator();

public slots:
	int randomizeInt(int Min, int Max);					//just for <int> randomization
	bool randomizeList(RandomGenerator_RandomizeMethod rMethod = RandomGenerator_RandomizeStandard, QStringList *sList = NULL);//Randomizes the class inherited QStringList

private:

};

#endif // RANDOMGENERATOR_H
