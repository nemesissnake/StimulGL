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

#ifndef EXPERIMENTPARAMETER_H
#define EXPERIMENTPARAMETER_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QColor>

class TypedExperimentParameterContainer
{
public:
	TypedExperimentParameterContainer() {};
	~TypedExperimentParameterContainer() 
	{
		hIntContainer.clear();
		hDoubleContainer.clear();
		hFloatContainer.clear();
		hBoolContainer.clear();
		hQStringContainer.clear();
		hQColorContainer.clear();
	};

	template< typename T1 > bool insertExperimentParameter(const QString &strKeyName, T1 *pExpParam = NULL)
	{
		if(pExpParam == NULL)
			return false;
		if (typeid(T1) == typeid(int))
		{
			hIntContainer.insert(strKeyName.toLower(),(int *)pExpParam);
			return true;
		}
		else if (typeid(T1) == typeid(double))
		{
			hDoubleContainer.insert(strKeyName.toLower(),(double *)pExpParam);
			return true;
		}
		else if (typeid(T1) == typeid(float))
		{
			hFloatContainer.insert(strKeyName.toLower(),(float *)pExpParam);
			return true;
		}
		else if (typeid(T1) == typeid(bool))
		{
			hBoolContainer.insert(strKeyName.toLower(),(bool *)pExpParam);
			return true;
		}
		else if (typeid(T1) == typeid(QString))
		{
			hQStringContainer.insert(strKeyName.toLower(),(QString *)pExpParam);
			return true;
		}
		else if (typeid(T1) == typeid(QColor))
		{
			hQColorContainer.insert(strKeyName.toLower(),(QColor *)pExpParam);
			return true;
		}
		else
		{
			QString strUnknownTypeName = typeid(T1).name();
			if (strUnknownTypeName.contains("char",Qt::CaseInsensitive))
			{
				hQStringContainer.insert(strKeyName.toLower(),(QString *)pExpParam);
				return true;
			}
			else
			{
				qDebug() << __FUNCTION__ << "::Could not cast type (" + strUnknownTypeName + ")!";
				return false;
			}
		}
		return false;
	}

	template< typename T2 > T2 *getExperimentParameter(const QString &strKeyName)
	{
		if (typeid(T2) == typeid(int))
		{
			return (T2*)hIntContainer.value(strKeyName.toLower());
		}
		else if (typeid(T2) == typeid(double))
		{
			return (T2*)hDoubleContainer.value(strKeyName.toLower());
		}
		else if (typeid(T2) == typeid(float))
		{
			return (T2*)hFloatContainer.value(strKeyName.toLower());
		}
		else if (typeid(T2) == typeid(bool))
		{
			return (T2*)hBoolContainer.value(strKeyName.toLower());
		}
		else if (typeid(T2) == typeid(QString))
		{
			return (T2*)hQStringContainer.value(strKeyName.toLower());
		}
		else if (typeid(T2) == typeid(QColor))
		{
			return (T2*)hQColorContainer.value(strKeyName.toLower());
		}
		else
		{
			return NULL;
		}
		return NULL;
	};

	bool getExperimentParameter(const QString &strKeyName, QScriptValue &scriptVal)
	{
		QString strKeyNameLow = strKeyName.toLower();
		if (hIntContainer.contains(strKeyNameLow))
		{
			scriptVal = QScriptValue(*hIntContainer.value(strKeyNameLow));
			return true;
		} 
		else if (hDoubleContainer.contains(strKeyNameLow))
		{
			scriptVal = QScriptValue(*hDoubleContainer.value(strKeyNameLow));
			return true;
		}
		else if (hFloatContainer.contains(strKeyNameLow))
		{
			scriptVal = QScriptValue(*hFloatContainer.value(strKeyNameLow));
			return true;
		}
		else if (hBoolContainer.contains(strKeyNameLow))
		{
			scriptVal = QScriptValue(*hBoolContainer.value(strKeyNameLow));
			return true;
		}
		else if (hQStringContainer.contains(strKeyNameLow))
		{
			scriptVal = QScriptValue(*hQStringContainer.value(strKeyNameLow));
			return true;
		}
		else if (hQColorContainer.contains(strKeyNameLow))
		{
			scriptVal = QScriptValue((*hQColorContainer.value(strKeyNameLow)).name());
			return true;
		}
		else
		{
			return false;
		}
		return false;
	};

	bool setExperimentParameter(const QString &strKeyName, const QScriptValue &scriptVal)
	{
		QString strKeyNameLow = strKeyName.toLower();
		if (hIntContainer.contains(strKeyNameLow))
		{
			int *tmpInt = hIntContainer.value(strKeyNameLow);
			*tmpInt = scriptVal.toInteger();
			return true;
		} 
		else if (hDoubleContainer.contains(strKeyNameLow))
		{
			double *tmpDouble = hDoubleContainer.value(strKeyNameLow);
			*tmpDouble = scriptVal.toNumber();
			return true;
		}
		else if (hFloatContainer.contains(strKeyNameLow))
		{
			float *tmpFloat = hFloatContainer.value(strKeyNameLow);
			*tmpFloat = scriptVal.toNumber();//Creates a double
			return true;
		}
		else if (hBoolContainer.contains(strKeyNameLow))
		{
			bool *tmpBool = hBoolContainer.value(strKeyNameLow);
			*tmpBool = scriptVal.toBool();
			return true;
		}
		else if (hQStringContainer.contains(strKeyNameLow))
		{
			QString *tmpString = hQStringContainer.value(strKeyNameLow);
			*tmpString = scriptVal.toString();
			return true;
		}
		else if (hQColorContainer.contains(strKeyNameLow))
		{
			QColor tmpColor;
			QString test = scriptVal.toString();
			tmpColor.setNamedColor(test);
			if(tmpColor.isValid())
			{
				*hQColorContainer.value(strKeyNameLow) = tmpColor;
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
		return false;
	};

private:
	QHash<QString, int*> hIntContainer;
	QHash<QString, double*> hDoubleContainer;
	QHash<QString, float*> hFloatContainer;
	QHash<QString, bool*> hBoolContainer;
	QHash<QString, QColor*> hQColorContainer;
	QHash<QString, QString*> hQStringContainer;
};

#endif // EXPERIMENTPARAMETER_H