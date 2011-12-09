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

int RandomGenerator::randomizeInt(int Min, int Max)
{
	if (Min > Max)
	{
		return ((qrand()%(Min-Max+1))+Max);
	}
	return ((qrand()%(Max-Min+1))+Min);
	//qrand() Returns a value between 0 and RAND_MAX = 0x7fff -> 32767(dec)
}

bool RandomGenerator::randomizeList()
{
	int nListCount = this->count();
	for(int i=(nListCount-1);i>0;--i)
	{
		int nRandom = qrand() % nListCount;
		this->swap(i,nRandom);
		//QString str = strList[i,nRandom];
		//strList[i] = strList[nRandom];
		//strList[random] = str;
	}
	return true;

	//qDebug() << strList;
}