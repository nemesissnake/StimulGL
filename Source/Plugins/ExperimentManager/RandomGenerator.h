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
#include "randomc.h"

#ifdef Q_OS_WIN
	#include "windows.h"
#endif

#define RANDOMIZE_DATETIME_FORMAT			"HHmmsszzz"

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
		int iinterval = max - min + 1;
		if (iinterval <= 0) 
			return 0x80000000; // error
		int r = int(iinterval * Random()); // truncate
		if (r >= iinterval) 
			return max;//r = iinterval-1;
		return min + r;
	}

	double DRandom(double min, double max)
	{// output random double
		if(min == max)
			return min;
		// get double random number in desired interval
		double iinterval = max - min + 1;
		if (iinterval <= 0) 
			return 0x80000000; // error
		double r = double(iinterval * Random()); // truncate
		if (r >= iinterval) 
			return max;//	r = iinterval-1;
		return min + r;
	}
};

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
	int randomizeInt(int nMin, int nMax) {return tCombinedRandGen->IRandom(nMin,nMax);};											//just for <int> randomization
	int randomizeDouble(double nMin, double nMax) {return tCombinedRandGen->DRandom(nMin,nMax);};									//just for <double> randomization
	bool randomizeList(RandomGenerator_RandomizeMethod rMethod = RandomGenerator_RandomizeStandard, QStringList *sList = NULL);		//Randomizes the class inherited QStringList

private:
	TRandomCombined<CRandomMersenne,CRandomMother> *tCombinedRandGen;
	int nSeed;
};
#endif // RANDOMGENERATOR_H
