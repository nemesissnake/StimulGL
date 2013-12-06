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
#include "ExperimentParameterDefinition.h"

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

	bool addProperty(const ExperimentParameterDefinitionStrc *expParamDef, const QVariant &vValue);
	bool parseDependencies(QtVariantProperty *variantProperty = NULL);
	bool addDependency(QtVariantProperty *variantProperty, const ExperimentParameterDefinitionDependencyStrc &dependencyParamDef);
	void setAutoDepencyParsing(bool bEnable);

private slots:
	void propertyValueChanged(QtProperty *property, const QVariant &value);

private:
	Ui::ExperimentParameterVisualizer *ui;
	QVBoxLayout *mainLayout;

	struct propertyGroupStruct
	{
		QtGroupPropertyManager* manager;
		QList<QtProperty *> lProperties;
	};

	struct propertyDependency
	{
		QtVariantProperty *vProperty;
		ExperimentParameterDefinitionDependencyStrc definition;
	};

	QtAbstractPropertyBrowser *propertyEditor;
	propertyGroupStruct lGroupProperties;
	QtVariantPropertyManager* lVariantPropertyManager;
	QList<propertyDependency> lPropertyDependencies;
	bool bAutoDepencyParsing;

	int getPropertyGroupIndex(const QString &sPropertyGroupName);
};

#endif // EXPERIMENTPARAMETERVISUALIZER_H
