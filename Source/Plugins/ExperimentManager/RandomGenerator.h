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


#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QtScript>
#include <QScriptEngine>
#include "randomc.h"

#ifdef Q_OS_WIN
	#include "windows.h"
#endif

#define RANDOMIZE_DATETIME_FORMAT			"HHmmsszzz"
//RANDOMGENERATOR_NAME

// This template class combines two different random number generators
// for improved randomness. R1 and R2 are any two different random number
// generator classes.
template <class RG1, class RG2>
class TRandomCombined : private RG1, private RG2 
{
public:
	TRandomCombined(int seed) : RG1(seed), RG2(seed+1) {};

	void RandomInit(int seed) {        // re-seed
		RG1::RandomInit(seed);
		RG2::RandomInit(seed+1);
	}

	double Random() {
		double r = RG1::Random() + RG2::Random();
		if (r >= 1.) r -= 1.;
		return r;
	}

	int IRandom(int min, int max)
	{// output random integer
		if(min == max)
			return min;
		// get integer random number in desired interval
		int iinterval = max - min;// + 1;
		if (iinterval < 0)//<= 0) 
			return 0x80000000; // error
		int r = int((iinterval+1) * Random()); // truncate, Random() returns <1.0
		//if (r >= iinterval) 
		//	return max;//r = iinterval-1;
		return min + r;
	}

	double DRandom(double min, double max)
	{// output random double
		if(min == max)
			return min;
		// get double random number in desired interval
		double iinterval = max - min;// + 1;
		if (iinterval <= 0)//<= 0) 
			return 0x80000000; // error
		double dsmall = DBL_MIN;
		double r = (iinterval+DBL_MIN) * Random(); // truncate
		//if (r >= iinterval) 
		//	return max;//	r = iinterval-1;
		return min + r;
	}
};

enum RandomGenerator_RandomizeMethod
{
	RandomGenerator_RandomizeStandard = 0,
	RandomGenerator_RandomizePreservedIndexes = 1,
	RandomGenerator_RandomizePreservedValues = 2
};

//!  The Random Generator class. 
/*!
The Random Generator can be used to create and edit randomized lists and numbers. For the randomization it combines two 
different random number generators (Mersenne twister and Mother-of-All (Multiply with carry).
*/
class RandomGenerator : public QObject, public QStringList
{
	Q_OBJECT

public:
	RandomGenerator(QObject *parent = NULL);
	RandomGenerator(const RandomGenerator& other ){};//TODO fill in copy constructor, should be declared for the Q_DECLARE_METATYPE macro
	~RandomGenerator();

	static QScriptValue ctor__randomGenerator(QScriptContext* context, QScriptEngine* engine);

	bool randomizeList(RandomGenerator_RandomizeMethod rMethod = RandomGenerator_RandomizeStandard, QStringList *sList = NULL);		//Randomizes the class inherited QStringList
	void setScriptEngine(QScriptEngine* engine);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script

	int randomizeInt(int nMin, int nMax);									//just for <int> randomization
	double randomizeDouble(double dMin, double dMax);						//just for <double> randomization

	QStringList toStringList();
	QStringList randomize(int rMethod  = (int)RandomGenerator_RandomizeStandard);
	const QString at(int i) const;
	int count() const;
	int count(const QString &sValue) const;
	bool isEmpty() const;
	void append(const QString &sValue);
	int appendStringList(const QStringList &sList);
	QString takeFirst();
	QString takeLast();
	QString takeAt(int i);
	void clear();
	int removeAll(const QString &sValue);
	void removeAt(int i);
	int indexOf(const QString &sValue, int nFrom = 0) const;
	void swap(int i, int j);
	void insert(int i, const QString &sValue);

private:
	QScriptEngine* currentScriptEngine;
	TRandomCombined<CRandomMersenne,CRandomMother> *tCombinedRandGen;
	int nSeed;
};

Q_DECLARE_METATYPE(RandomGenerator*)

#endif // RANDOMGENERATOR_H
