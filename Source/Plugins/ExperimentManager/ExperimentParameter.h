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

#ifndef EXPERIMENTPARAMETER_H
#define EXPERIMENTPARAMETER_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QVariant>

class ExperimentParameter : public QObject
{
	Q_OBJECT
	
	public:
		ExperimentParameter(const QString &strName) : m_name(strName) {};
		virtual ~ExperimentParameter() {};

	private:
		QString m_name;
};

template< typename T >
class TypedExperimentParameter : public ExperimentParameter
{
	public:
		TypedExperimentParameter(const QString &strName, const T& data) : ExperimentParameter(strName), m_data(data) {};

	private:
		T m_data;
};

class TypedExperimentParameterContainer
{
public:
	TypedExperimentParameterContainer() {};
	//TypedExperimentParameterContainer(const TypedExperimentParameterContainer& other ) {};

	template< typename T > bool insertExperimentParameter(const QString &strKeyName, T *pExpParam = NULL)
	{
		if(pExpParam == NULL)
			return false;
		if (typeid(T) == typeid(int))
		{
			hIntContainer.insert(strKeyName,pExpParam);
			return true;
		}
		return false;
	}

	template< typename T > T *getExperimentParameter(const QString &strKeyName)
	{
		if (typeid(T) == typeid(int))
		{
			return hIntContainer.value(strKeyName);
		}
		return NULL;
	};

private:
	//QHash<QString, TypedExperimentParameter<int>> hIntContainer;
	QHash<QString, int*> hIntContainer;
};

class ExperimentParameterContainer : public QObject
{
	Q_OBJECT

public:
	ExperimentParameterContainer() {};
	//ExperimentParameterContainer(const ExperimentParameterContainer& other ) {};

	bool insertExperimentParameter(const QString &strKeyName, QVariant vExpParam)
	{
	//	if(pExpParam == NULL)
	//		return false;
	//	if (typeid(T) == typeid(int))
	//	{
	//		hIntContainer.insert(strKeyName,pExpParam);
			return true;
	//	}
	//	return false;
	}

	//template< typename T > T *getExperimentParameter(const QString &strKeyName)
	//{
	//	if (typeid(T) == typeid(int))
	//	{
	//		return hIntContainer.value(strKeyName);// insert(strKeyName,pExpParam);
	//	}
	//	return NULL;
	//};

private:
	//QHash<QString, TypedExperimentParameter<int>> hIntContainer;
	QHash<QString, int*> hIntContainer;
};

#endif // EXPERIMENTPARAMETER_H