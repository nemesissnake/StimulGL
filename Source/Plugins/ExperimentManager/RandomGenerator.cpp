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

bool RandomGenerator::randomizeList(RandomGenerator_RandomizeMethod rMethod, QStringList *sList)
{
	bool bRetVal = false;
	int nListCount = this->count();
	int i,j;
	int nIndexFound;

	switch (rMethod)
	{
	case RandomGenerator_RandomizeStandard:
		////just randomize
		for(i=(nListCount-1);i>0;--i)
		{
			int nRandom = qrand() % nListCount;
			this->swap(i,nRandom);
		}
		bRetVal = true;
		break;
	case RandomGenerator_RandomizePreservedValues:
		//sList[] contains the values of the indexes that should be preserved(should not be randomized)!
		for(i=(nListCount-1);i>0;--i)
		{//first start with an randomize
			int nRandom = qrand() % nListCount;
			this->swap(i,nRandom);
		}
		if (sList == NULL)
		{
			bRetVal = true;
			break;
		}
		if (sList->count() < 1)
		{
			bRetVal = true;
			break;
		}
		for(i=0;i<sList->count();i++)
		{
			nIndexFound = this->indexOf(sList->at(i));
			if (nIndexFound == -1)
			{
				this->replace(i,sList->at(i));
			} 
			else if(i!=nIndexFound)
			{
				this->swap(i,nIndexFound);
			}
		}
		bRetVal = true;
		break;
	case RandomGenerator_RandomizePreservedIndexes:
		//sList[] contains the indexes that should be preserved(should not be randomized)!
		if (sList == NULL)
		{//just randomize
			for(i=(nListCount-1);i>0;--i)
			{
				int nRandom = qrand() % nListCount;
				this->swap(i,nRandom);
			}
			bRetVal = true;
			break;
		}
		else if (sList->count() < 1)
		{//just randomize
			for(i=(nListCount-1);i>0;--i)
			{
				int nRandom = qrand() % nListCount;
				this->swap(i,nRandom);
			}
			bRetVal = true;
			break;
		}
		else if(sList->count() >= (this->count()-1))
		{//nothing to do
			bRetVal = true;
			break;
		}
		else
		{
			QStringList tmpCopyLst;
			tmpCopyLst = this->mid(0);//copy the current list
			//first start with an randomize
			for(i=(nListCount-1);i>0;--i)
			{
				int nRandom = qrand() % nListCount;
				this->swap(i,nRandom);
			}
			//recover the preserved items
			for(j=0;j<sList->count();j++)
			{
				nIndexFound = this->indexOf(tmpCopyLst.at(sList->at(j).toInt()));
				if(nIndexFound >= 0)//is the preserved item still in the current list?
					this->swap(sList->at(j).toInt(),nIndexFound);				
			}
			bRetVal = true;
			break;
		}
	}
	return bRetVal;
}