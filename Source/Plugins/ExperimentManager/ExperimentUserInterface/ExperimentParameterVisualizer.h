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

#include "ExperimentParameter.h"
#include "ExperimentParameterDefinition.h"
#include "ParameterPropertyExtensions.h"

namespace Ui {class ExperimentParameterVisualizer;};

class ExperimentParameterVisualizer : public QWidget
{
	Q_OBJECT

signals:
	void destroyed(QWidget*);

private:
	struct propertyContainerItem
	{
		QtProperty *lGroupProperty;
		QList<propertyContainerItem> *pSubItems;
		propertyContainerItem()
		{
			lGroupProperty = NULL;
			pSubItems = NULL;
		}
	};

	struct propertyContainerItems
	{
		QList<propertyContainerItem> propItem;
	};

	struct propertyDependencyStruct
	{
		QtVariantProperty *vProperty;
		QList<ExperimentParameterDefinitionDependencyStrc> definitions;
		propertyDependencyStruct()
		{
			vProperty = NULL;
		}
	};

public:
	explicit ExperimentParameterVisualizer(QWidget *parent = NULL);
	ExperimentParameterVisualizer(const ExperimentParameterVisualizer& other){Q_UNUSED(other)};
	~ExperimentParameterVisualizer();

	bool addParameterProperty(const ExperimentParameterDefinitionStrc *expParamDef, const QVariant &vValue);
	bool addGroupProperties(const QList<ExperimentGroupDefinitionStrc> *expParamDef);
	bool parseDependencies(QtVariantProperty *variantProperty = NULL);
	bool addDependency(QtVariantProperty *variantProperty, const ExperimentParameterDefinitionDependencyStrc &dependencyParamDef);
	void setAutoDepencyParsing(bool bEnable);

public slots:
	void resizeParameterView(const int &nWidth, const int &nHeight);

private slots:
	void propertyValueChanged(QtProperty *property, const QVariant &value);

private:
	Ui::ExperimentParameterVisualizer *ui;
	QVBoxLayout *mainLayout;
	QtTreePropertyBrowser *propertyEditor;
	QtGroupPropertyManager* groupManager;
	propertyContainerItems lGroupPropertyCollection;
	VariantExtensionPropertyManager* lVariantPropertyManager;
	QList<propertyDependencyStruct> lPropertyDependencies;
	bool bAutoDepencyParsing;

	bool addPropertyToSubGroup(const QString &sPropertyGroupNames, QtVariantProperty *item1, QList<propertyContainerItem> *pRootGroupPropertyItemList, QString &sSandPath = QString(""));
	void deleteSubGroupProperties(QList<propertyContainerItem> *pRootGroupPropertyItemList);
};

#endif // EXPERIMENTPARAMETERVISUALIZER_H
