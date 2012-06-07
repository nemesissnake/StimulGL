//PrtFormatManager
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

#ifndef PRTFORMATMANAGER_H
#define PRTFORMATMANAGER_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QtScript>
#include <QScriptable>
#include <QColor>

#define PRT_DEFAULT_VERSION				2
#define PRT_UNDEF_VALUE_STRING			"<undefined value>"
#define PRT_UNDEF_DESC_STRING			"<undefined description>"
#define PRT_UNDEF_NAME_STRING			"<undefined name>"
#define PRT_UNDEF_COLOR_STRING			"192 192 192"
#define PRT_LAST_PARAM_NAME				"NrOfConditions"
#define PRT_COLOR_PARAM_NAME			"Color"

typedef struct strcPRTParameters
{
	strcPRTParameters()
	{
		sValue =						PRT_UNDEF_VALUE_STRING;
		sDescription =					PRT_UNDEF_DESC_STRING;
	}
	QString sValue;
	QString sDescription;
} PRTDefinitionParameters;
typedef QHash<QString, PRTDefinitionParameters> PRTDefinitionParameterList;

typedef struct strcIntervals
{
	strcIntervals()
	{
		nBegin =						0;
		nEnd =							1;
	}
	int nBegin;
	int nEnd;
} IntervalsDefinition;

typedef struct strcConditions
{
	strcConditions()
	{
		sName =							PRT_UNDEF_NAME_STRING;
		sColor =						PRT_UNDEF_COLOR_STRING;
	}
	QString sName;
	QString sColor;
	QList<IntervalsDefinition> Intervals;
} ConditionsDefinition;

typedef QList<ConditionsDefinition> PRTConditionsDefinitionList;

class PrtFormatManager : public QObject, protected QScriptable
{
	Q_OBJECT

public:
	PrtFormatManager(QObject *parent = NULL);
	PrtFormatManager(const PrtFormatManager& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro
	~PrtFormatManager();

	static QScriptValue ctor__PrtFormatManager(QScriptContext* context, QScriptEngine* engine);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script
	bool loadFile(const QString sFileName);
	bool saveFile(const QString sFileName, const bool bOverWrite = false);
	bool setParameterValue(const QString sParamName, const QString sParamValue = PRT_UNDEF_VALUE_STRING, const QString sParamDescription = PRT_UNDEF_DESC_STRING);
	QString getParameterValue(const QString sParamName);
	QString getParameterDescription(const QString sParamName);
	bool clearConditions();
	bool clearParameters() {return setDefaultParameters(PRT_DEFAULT_VERSION);};
	bool clearAll() {return (clearConditions() && clearParameters());}; 
	int appendCondition(const QString sConditionName = PRT_UNDEF_NAME_STRING, const QString sConditionColor = PRT_UNDEF_COLOR_STRING);
	int appendInterval(const int nConditionIndex = 0, const int nBegin = 0, const int nEnd = 1);
	QString getRGBPaletteColorString(const int nSteps, const int nIndex);
	bool SortandMergeAllConditionIntervals();

private:
	bool setDefaultParameters(const int nVersion = PRT_DEFAULT_VERSION);
	PRTDefinitionParameters getParameter(const QString sParamName);
	bool setConditionColor(const int nConditionIndex, const QString sColor);

	QScriptEngine* currentScriptEngine;

	PRTDefinitionParameterList m_PRTParameters;
	PRTConditionsDefinitionList m_PRTConditions;
	QStringList m_AllowedParameterNames;

	IntervalsDefinition tmpIntervalsDefinition;
	ConditionsDefinition tmpConditionsDefinition;
	PRTDefinitionParameters tmpPRTDefinitionParameters;
};

Q_DECLARE_METATYPE(PrtFormatManager*)

#endif // PRTFORMATMANAGER_H
