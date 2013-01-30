//ExperimentManagerplugin
//Copyright (C) 2013  Sven Gijsen
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

Q_DECLARE_METATYPE(QStringList)

/*! \brief The RandomGenerator constructor.
*
*   You do not need to specify the parent object. 
*	The StimulGL script engine automatically retrieves the parent role
*/
RandomGenerator::RandomGenerator(QObject *parent):QObject(parent)
{
	currentScriptEngine = NULL;
	tCombinedRandGen == NULL;
	nSeed = QDateTime::currentDateTime().toString(RANDOMIZE_DATETIME_FORMAT).toInt();
	tCombinedRandGen = new TRandomCombined<CRandomMersenne,CRandomMother>(nSeed);
}

QScriptValue RandomGenerator::ctor__randomGenerator(QScriptContext* context, QScriptEngine* engine)
{
	//QString sArgs;
	//int nArgCount = context->argumentCount();
	//bool bIsArray;
	//for(int i=0;i<nArgCount;i++)
	//{
	//	bIsArray = context->argument(i).isArray();
	//	sArgs = sArgs + context->argument(i).toString();
	//	if((i+1)<nArgCount)
	//		sArgs = sArgs + " , ";
	//	
	//}
	RandomGenerator *RandomGeneratorObj = new RandomGenerator();
	if(context->argumentCount()>0)
	{
		if(context->argument(0).isArray())
		{
			RandomGeneratorObj->appendStringList(qscriptvalue_cast<QStringList> (context->argument(0)));
		}
	}
	RandomGeneratorObj->setScriptEngine(engine);
	return engine->newQObject(RandomGeneratorObj, QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
}

void RandomGenerator::setScriptEngine(QScriptEngine* engine)
{
	currentScriptEngine = engine;
	qScriptRegisterSequenceMetaType<QStringList>(currentScriptEngine);
}

bool RandomGenerator::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
{
	if (engine)
	{
		currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		QScriptValue objectValue = currentScriptEngine->newQObject(this);
		currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		return true;
	}
	return false;
}

/*! \brief The RandomGenerator destructor.
*
*   You do not need call the destructor. 
*	The StimulGL script engine automatically performs the garbage collection after you set the object to NULL and the script ends
*/
RandomGenerator::~RandomGenerator()
{
	if(tCombinedRandGen != NULL)
	{
		delete tCombinedRandGen;
		tCombinedRandGen = NULL;
	}
}

int RandomGenerator::randomizeInt(int nMin, int nMax) 
{
/*! \brief Creates a random Integer value.
 *
 *  This function creates a random integer value between (and including) a defined range.
 * @param nMin an integer value that determines the minimal integer value of the range where a random number is picked from.
 * @param nMax an integer value that determines the maximal integer value of the range where a random number is picked from.
 * @return an random integer value between (and including) a defined range. If the range is not correct it returns 0x80000000;
 */
	return tCombinedRandGen->IRandom(nMin,nMax);
}

double RandomGenerator::randomizeDouble(double dMin, double dMax) 
{
/*! \brief Creates a random Double value.
 *
 *  This function creates a random double value between (and including) a defined range.
 * @param dMin an integer value that determines the minimal double value of the range where a random number is picked from.
 * @param dMax an integer value that determines the maximal double value of the range where a random number is picked from.
 * @return an random integer value between (and including) a defined range. If the range is not correct it returns 0x80000000;
 */
	return tCombinedRandGen->DRandom(dMin,dMax);
}

const QString RandomGenerator::at(int i) const 
{
/*! \brief Returns the String at index position i in the StringList.
 *
 *  This function returns the String at index position i in the list, 
 *  i must be a valid index position in the list (i.e., 0 <= i < RandomGenerator::count()).
 *  This function is very fast (constant time).
 * @param i an integer value that holds the index position.
 * @return an String value containing the item at index position in the list.
 */
	return QStringList::at(i);
}

int RandomGenerator::count() const 
{
/*! \brief Returns the number of String items in the list.
 *
 * @return an integer value containing the number of String items in the list.
 */
	return QStringList::count();
}

int RandomGenerator::count(const QString &sValue) const 
{
/*! \brief Returns the number of a particular String occurrences in the list.
 *
 * @param sValue an String value that holds the String to search for in the list.
 * @return an integer value containing the number of a particular String occurrences in the list.
 */
	return QStringList::count(sValue);
}

bool RandomGenerator::isEmpty() const 
{
/*! \brief Returns true if the list contains no Strings; otherwise returns false.
 *
 * @return an boolean value representing whether the list was empty or not.
 */
	return QStringList::isEmpty();
}

void RandomGenerator::append(const QString &sValue) 
{
/*! \brief Inserts a defined String at the end of the list.
 *
 * This operation is typically very fast (constant time), because the list preallocates extra space on both sides of its internal buffer to allow for fast growth at both ends of the list.
 * @param sValue an String value that should be inserted at the end of the list.
 */
	QStringList::append(sValue);
}

QString RandomGenerator::takeFirst() 
{
/*! \brief Removes the first String in the list and returns it.
 * 
 * This function assumes the list is not empty. To avoid failure, call RandomGenerator::isEmpty() before calling this function. This operation takes constant time.
 * @return a String value containing the removed String from the list.
 */
	return QStringList::takeFirst();
}

QString RandomGenerator::takeLast() 
{
/*! \brief Removes the last String in the list and returns it.
 * 
 * This function assumes the list is not empty. To avoid failure, call RandomGenerator::isEmpty() before calling this function. This operation takes constant time.
 * @return a String value containing the removed String from the list.
 */
	return QStringList::takeLast();
}

QString RandomGenerator::takeAt(int i) 
{
/*! \brief Removes the String at a defined index position in the list and returns it.
 * 
 * This function assumes the list is not empty. To avoid failure, call RandomGenerator::isEmpty() before calling this function. This operation takes constant time.
 * @param i an integer value holding the index position.
 * @return a String value containing the removed String from the list.
 */
	return QStringList::takeAt(i);
}

void RandomGenerator::clear() 
{
/*! \brief Removes all items from the list.
 *
 * This function removes all items from the list.
 */
	QStringList::clear();
}

int RandomGenerator::removeAll(const QString &sValue) 
{
/*! \brief Removes all occurrences of a String in the list and returns the number of entries removed.
 * 
 * @param sValue the String value that should be removed (all) from the list.
 * @return an integer value representing the number of entries that were removed.
 */
	return QStringList::removeAll(sValue);
}

void RandomGenerator::removeAt(int i) 
{
/*! \brief Removes the String at a defined index position in the list.
 * 
 * This function assumes the list is not empty. To avoid failure, call RandomGenerator::isEmpty() before calling this function. This operation takes constant time.
 * @param i an integer value holding the index position.
 */
	return QStringList::removeAt(i);
}

int RandomGenerator::indexOf(const QString &sValue, int nFrom) const 
{
/*! \brief Returns the index position of the first occurrence of the defined String in the list.
 * 
 * This function Returns the index position of the first occurrence of the defined String in the list, searching forward from index position from. Note that QList uses 0-based indexes.
 * @param sValue the String value to search for in the list.
 * @nFrom an integer value holding the starting index position for the search operation. This value is 0 if not defined.
 * @return an integer holding the index of the first occurrence of the defined String in the list. This function returns -1 if no item matched.
 */
	return QStringList::indexOf(sValue,nFrom);
}

void RandomGenerator::swap(int i, int j) 
{
/*! \brief Exchange the String value at index position i with the String value at index position j.
 * 
 * This function assumes that both i and j are at least 0 but less than RandomGenerator::count(). To avoid failure, test that both i and j are at least 0 and less than RandomGenerator::count().
 * @param i an integer value holding the first index position.
 * @param j an integer value holding the second index position.
 */
	QStringList::swap(i,j);
}

void RandomGenerator::insert(int i, const QString &sValue) 
{
/*! \brief Inserts a defined String value at specific index position i.
 * 
 * This function inserts a String value at index position i in the list. If i is 0, the value is prepended to the list. If i is RandomGenerator::count(), the value is appended to the list.
 * @param i an integer value holding the index position.
 * @param sValue the String value to insert to the list.
 */
	QStringList::insert(i,sValue);
}

QStringList RandomGenerator::randomize(int rMethod)
{
/*! \brief Randomizes and return the String List.
 * 
 * This function randomizes and then returns the randomized String List.
 * @param rMethod <todo>.
 * @return a randomized StringList (array).
 */
	RandomGenerator_RandomizeMethod rndMethod = (RandomGenerator_RandomizeMethod) rMethod;

	if(randomizeList(rndMethod))
	{
		return (QStringList)*this;
	}
	else
	{
		return QStringList();
	}
}

QStringList RandomGenerator::toStringList()
{
/*! \brief Returns the String List.
 * 
 * This function returns the String List.
 * @return the StringList (array).
 */
	return (QStringList)*this;
}

int RandomGenerator::appendStringList(const QStringList &sList)
{
/*! \brief Inserts a defined StringList at the end of the list.
 *
 * This function inserts a defined StringList to the end of the list and returns the number of Strings in the list.
 * @param sList an StringList (array) that should be inserted at the end of the list.
 * @return an integer value representing the number of Strings in the list.
 */
	QStringList::append(sList);
	return QStringList::count();
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
