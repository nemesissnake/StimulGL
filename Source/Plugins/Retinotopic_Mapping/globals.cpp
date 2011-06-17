/****************************************************************************
**
** Copyright (C) 2011 Rainer Goebel / Jan Zimmermann
** All rights reserved.
** Contact: Rainer Goebel (rainergoebel@mac.com)
**
**
** GNU General Public License
** This file may be used under the terms of the GNU General Public
** License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
****************************************************************************/

#include "globals.h"

#include <time.h>
#include <cstdlib>

//
// global helper functions
//
int my_random(int x)
{
	return(rand()%x);
}

void my_randomize()
{
	srand((unsigned)time(NULL));
	//srand((unsigned)read_pentium_timer());
}

QString combinePathAndFileName(const QString& path, const QString& filename)
{
	QString FullFileName;

	if(path == "")
		return filename;

	if(path.at(path.length()-1) == '/')  // In case path contains only disk (i.e. "C:/"), a "/" will be present at the end, otherwise not (e.g.. "C:/BrainVoyager")
		FullFileName = path + filename;
	else
		FullFileName = path + "/" + filename;

	return FullFileName;
}

bool readStringInQuotes(QString &token, QTextStream &ar)
{
	QChar delimiter = '"';
	QChar qch;
	int i;

	token = "";

	for(;;)
	{
		ar >> qch;
		if(ar.atEnd())
		{
			return false;
		}

		if(qch == delimiter)
			break;
	}

	// Now read token until ending " char
	for(i=0;;i++)
	{
		ar >> qch;

		if(ar.atEnd())
			return false;

		if(qch == delimiter)
			break;

		token.append( qch );
	}
	return true;
}
