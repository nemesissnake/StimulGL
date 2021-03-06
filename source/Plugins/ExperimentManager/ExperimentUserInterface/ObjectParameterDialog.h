//ExperimentManagerplugin
//Copyright (C) 2014  Sven Gijsen
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

#ifndef ObjectParameterDialog_H
#define ObjectParameterDialog_H

#include <QDialog>
#include "ExperimentTreeModel.h"
#include "ExperimentParameterDefinition.h"

namespace Ui {class ObjectParameterDialog;};

class ExperimentParameterVisualizer;
class ObjectParameterDialog : public QDialog
{
	Q_OBJECT

signals:
	//void newParametersAdded(QMap<QString,QVariant> parameters);

public:
	ObjectParameterDialog(QWidget *parent = NULL);
	~ObjectParameterDialog();

	bool parseExperimentTreeModel(ExperimentTreeModel *pExperimentTreeModel);

private slots:
	void parameterTypeChanged();
	void updateParameter();
	void resetParameter();
	void selectedObjectChanged(int nIndex);
	void selectedBlockChanged(int nIndex);
	void selectedParameterChanged(int nIndex);
	void derivedItemEditFinished(const QString& sParamName, const QString& sNewValue);

private:
	bool parseParameters(const int &nObjectID, const int &nBlockID, const bool &bIsCustom = false);
	void clearAllParsedStructures();

	Ui::ObjectParameterDialog *ui;

	struct strcParameterDefinition 
	{
		bool bIsChanged;
		QString sFullDisplayName;
		QString sValue;
		ExperimentParameterDefinitionStrc ParamDef;

		strcParameterDefinition()
		{
			bIsChanged = false;
			sFullDisplayName = "";
			sValue = "";
		}
	};

	bool bIsParsing;
	QWidget *pParameterEditWidget;

	QHash<int, int> hObjectControlIndexToObjectID;
	QHash<int, int> hBlockControlIndexToBlockID;
	ExperimentParameterVisualizer *expParamVizualizer;
	ExperimentTreeModel *pCurrentExpTree;
	cExperimentStructure *pCurrentExpStructure;
	QList<ExperimentStructuresNameSpace::strcExperimentObject> lCurrentExpObjects;
	QList<ExperimentTreeItem*> lCurrentExpTreeItems;
	QMap<QString,strcParameterDefinition> mapUniqueHexParamIDToParamDef;
	QHash<int,QString> hashParamControlIndexToUniqueHexParamID;
	QHash<QString,int> hashParamNameToParamID;
};

#endif // ObjectParameterDialog_H
