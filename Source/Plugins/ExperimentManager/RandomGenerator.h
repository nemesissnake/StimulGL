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
	void RandomGenerator::InitRandomizer();
	int nPrevRandInitResult;

};

#endif // RANDOMGENERATOR_H
