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

#ifndef EXPERIMENTPARAMETERWIDGETS_H
#define EXPERIMENTPARAMETERWIDGETS_H

#include <QObject>
#include <QMutex>
#include "ExperimentParameterDefinition.h"

class ExperimentParameterVisualizer;

struct ExperimentParameterDefinitionCollection
{
	ExperimentParameterDefinitionContainer *cExperimentParameterDefinition;
	ExperimentParameterVisualizer *wWidget;
	QString sCollectionName;
	

	ExperimentParameterDefinitionCollection()
	{
		this->cExperimentParameterDefinition = NULL;
		this->wWidget = NULL;
	}
};

class ExperimentParameterWidgets : public QObject
{
	Q_OBJECT

public:
	static ExperimentParameterWidgets* instance()
	{
		static QMutex mutex;
		if (!m_Instance)
		{
			mutex.lock();

			if (!m_Instance)
			{
				m_Instance = new ExperimentParameterWidgets;

			}
			mutex.unlock();
		}
		return m_Instance;
	}

	static void drop()
	{
		static QMutex mutex;
		mutex.lock();
		delete m_Instance;
		m_Instance = 0;
		mutex.unlock();
	}

	ExperimentParameterVisualizer *getExperimentParameterWidget(const QString &sCollectionName);

private:
	ExperimentParameterWidgets();
	~ExperimentParameterWidgets();

	ExperimentParameterWidgets(const ExperimentParameterWidgets &); // hide copy constructor
	ExperimentParameterWidgets& operator=(const ExperimentParameterWidgets &); // hide assign operator
	//We leave just the declarations, so the compiler will warn us if we try to use those two functions by accident

	static ExperimentParameterWidgets* m_Instance;


	void fetchExperimentParameterDefinitions();
	void createExperimentParameterWidgets();
	ExperimentParameterDefinitionContainer *getExperimentParameterDefinition(const QString &sCollectionName);

	QList<ExperimentParameterDefinitionCollection> *lExperimentParameterDefinitions;
};

#endif // EXPERIMENTPARAMETERWIDGETS_H
