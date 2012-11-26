//ExperimentManagerplugin
//Copyright (C) 2012  Sven Gijsen
//
//This file is part of StimulGL.
//StimulGL is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


#include "RandomGenerator.h"
#include <QDateTime>
#include "randomc/mersenne.cpp"
#include "randomc/mother.cpp"

RandomGenerator::RandomGenerator(QObject *parent):QObject(parent)
{
	currentScriptEngine = NULL;
	tCombinedRandGen == NULL;
	nSeed = QDateTime::currentDateTime().toString(RANDOMIZE_DATETIME_FORMAT).toInt();
	tCombinedRandGen = new TRandomCombined<CRandomMersenne,CRandomMother>(nSeed);
}

QScriptValue RandomGenerator::ctor__randomGenerator(QScriptContext* context, QScriptEngine* engine)
{
	RandomGenerator *RandomGeneratorObj = new RandomGenerator();
	RandomGeneratorObj->setScriptEngine(engine);
	return engine->newQObject(RandomGeneratorObj, QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
}

void RandomGenerator::setScriptEngine(QScriptEngine* engine)
{
	currentScriptEngine = engine;
}

bool RandomGenerator::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
{
	if (engine)
	{
		currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		//QObject *someObject = this;//new MyObject;
		QScriptValue objectValue = currentScriptEngine->newQObject(this);
		currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		return true;
	}
	return false;
}

RandomGenerator::~RandomGenerator()
{
	if(tCombinedRandGen != NULL)
	{
		delete tCombinedRandGen;
		tCombinedRandGen = NULL;
	}
}

//		for (int i = 0; i < 20; i++) {
//			// generate 20 random floating point numbers and 20 random integers
//			//printf("\n%14.10f   %2i",  RG.Random(),  RG.IRandom(0,99));
//			float a = tCombinedRandGen->Random();
//			int b = tCombinedRandGen->IRandom(0,99);
//			b = b;

int RandomGenerator::randomizeInt(int nMin, int nMax) 
{//nMin>nMax? --> return 0x80000000;
	return tCombinedRandGen->IRandom(nMin,nMax);
}

double RandomGenerator::randomizeDouble(double nMin, double nMax) 
{
	return tCombinedRandGen->DRandom(nMin,nMax);
}

QScriptValue RandomGenerator::randomize(int rMethod)//, QScriptValue sList)
{
	if(currentScriptEngine == NULL)
		return NULL;
	RandomGenerator_RandomizeMethod rndMethod = (RandomGenerator_RandomizeMethod) rMethod;
	//QStringList tmpList = currentScriptEngine->fromScriptValue<QStringList>(sList);

	if(randomizeList(rndMethod))//&tmpList))
	{
		//tmpList << "1" << "2" << "3";
		return currentScriptEngine->toScriptValue((QStringList)*this);//tmpList);
	}
	else
	{
		return currentScriptEngine->undefinedValue();
	}
}

QScriptValue RandomGenerator::toScriptArray()
{
	if(currentScriptEngine == NULL)
		return NULL;
	if(QStringList::isEmpty() == false)
	{
		return currentScriptEngine->toScriptValue((QStringList)*this);
	}
	else
	{
		return currentScriptEngine->undefinedValue();
	}
}

bool RandomGenerator::randomizeList(RandomGenerator_RandomizeMethod rMethod, QStringList *sList)
{
	//int a = randomizeInt(0,700);
	//InitRandomizer();
	bool bRetVal = false;
	int nListCount = QList::count();
	int i,j;
	int nIndexFound;
	int nRandom;

	switch (rMethod)
	{
	case RandomGenerator_RandomizeStandard:
		////just randomize
		for(i=(nListCount-1);i>0;--i)
		{
			nRandom = tCombinedRandGen->IRandom(0,nListCount-1);
			QList::swap(i,nRandom);
		}
		bRetVal = true;
		break;
	case RandomGenerator_RandomizePreservedValues:
		//sList[] contains the values of the indexes that should be preserved(should not be randomized)!
		for(i=(nListCount-1);i>0;--i)
		{//first start with an randomize
			nRandom = tCombinedRandGen->IRandom(0,nListCount-1);
			QList::swap(i,nRandom);
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
			nIndexFound = QList::indexOf(sList->at(i));
			if (nIndexFound == -1)
			{
				QList::replace(i,sList->at(i));
			} 
			else if(i!=nIndexFound)
			{
				QList::swap(i,nIndexFound);
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
				nRandom = tCombinedRandGen->IRandom(0,nListCount-1);
				QList::swap(i,nRandom);
			}
			bRetVal = true;
			break;
		}
		else if (sList->count() < 1)
		{//just randomize
			for(i=(nListCount-1);i>0;--i)
			{
				nRandom = tCombinedRandGen->IRandom(0,nListCount-1);
				QList::swap(i,nRandom);
			}
			bRetVal = true;
			break;
		}
		else if(sList->count() >= (QList::count()-1))
		{//nothing to do
			bRetVal = true;
			break;
		}
		else
		{
			QStringList tmpCopyLst;
			tmpCopyLst = QStringList::mid(0);//copy the current list
			//first start with an randomize
			for(i=(nListCount-1);i>0;--i)
			{
				nRandom = tCombinedRandGen->IRandom(0,nListCount-1);
				QList::swap(i,nRandom);
			}

			int nRecoverCount = sList->count();
			if (nRecoverCount > 0)
			{
				//recover the preserved items
				for(j=0;j<nRecoverCount;j++)
				{
					nIndexFound = QList::indexOf(tmpCopyLst.at(sList->at(j).toInt()));
					if(nIndexFound >= 0)//is the preserved item still in the current list?
						QList::swap(sList->at(j).toInt(),nIndexFound);				
				}
				if (nRecoverCount > 1)
				{
					//randomize the recovered values
					for(i=(nRecoverCount-1);i>0;--i)
					{
						QList::swap(sList->at(i).toInt(),sList->at(tCombinedRandGen->IRandom(0,nRecoverCount-1)).toInt());
					}
				}
			}
			bRetVal = true;
			break;
		}
	}
	return bRetVal;
}
