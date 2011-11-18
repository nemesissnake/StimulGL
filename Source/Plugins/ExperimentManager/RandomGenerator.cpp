#include "RandomGenerator.h"
#include <QTime>

RandomGenerator::RandomGenerator(QObject *parent)
	: QObject(parent)
{
	qsrand(QTime::currentTime().msec());
}

RandomGenerator::~RandomGenerator()
{

}

int RandomGenerator::randomize(int Min, int Max)
{
	if (Min > Max)
	{
		return ((qrand()%(Min-Max+1))+Max);
	}
	return ((qrand()%(Max-Min+1))+Min);
	//qrand() Returns a value between 0 and RAND_MAX = 0x7fff -> 32767(dec)
}