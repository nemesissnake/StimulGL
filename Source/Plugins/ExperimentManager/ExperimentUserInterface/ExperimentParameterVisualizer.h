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

#ifndef EXPERIMENTPARAMETERVISUALIZER_H
#define EXPERIMENTPARAMETERVISUALIZER_H

#include <QWidget>
#include <QVBoxLayout>
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qteditorfactory.h"
#include "qttreepropertybrowser.h"
//#include "qtbuttonpropertybrowser.h"
//#include "qtgroupboxpropertybrowser.h"

#include "ExperimentParameter.h"

namespace Ui {class ExperimentParameterVisualizer;};

class ExperimentParameterVisualizer : public QWidget
{
	Q_OBJECT

signals:
	void destroyed(QWidget*);

public:
	explicit ExperimentParameterVisualizer(QWidget *parent = NULL);
	ExperimentParameterVisualizer(const ExperimentParameterVisualizer& other){Q_UNUSED(other)};
	~ExperimentParameterVisualizer();

	bool addProperty(const ExperimentParameterDefinition *expParamDef, const QVariant &vValue);

private:
	Ui::ExperimentParameterVisualizer *ui;
	QVBoxLayout *mainLayout;

	struct propertyGroupStruct
	{
		QtGroupPropertyManager* manager;
		QList<QtProperty *> lProperties;
	};

	QtAbstractPropertyBrowser *propertyEditor;
	propertyGroupStruct lGroupProperties;
	QList<QtBoolPropertyManager*> lBoolPropertyManagers;
	//QList<QtStringPropertyManager*> lStringPropertyManagers;
	QList<QtEnumPropertyManager*> lEnumPropertyManagers;
	QList<QtIntPropertyManager*> lIntPropertyManagers;
	QList<QtVariantPropertyManager*> lVariantPropertyManagers;

	int getPropertyGroupIndex(const QString &sPropertyGroupName);
};

#endif // EXPERIMENTPARAMETERVISUALIZER_H
