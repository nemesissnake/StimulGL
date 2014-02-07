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

#ifndef EXPERIMENTBLOCKPARAMETERVIEW_H
#define EXPERIMENTBLOCKPARAMETERVIEW_H

#include <QTableWidgetItem>
#include "ExperimentStructures.h"

#define BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER		";"
#define BLOCKPARAMVIEW_DEFAULTBLOCKHEADER_COUNT		4
//#define BLOCKPARAMVIEW_BLOCKNUMBER_COLUMNINDEX		0
#define BLOCKPARAMVIEW_BLOCKNAME_COLUMNINDEX		0
#define BLOCKPARAMVIEW_BLOCKTRIALS_COLUMNINDEX		1
#define BLOCKPARAMVIEW_BLOCKINTTRGS_COLUMNINDEX		2
#define BLOCKPARAMVIEW_BLOCKEXTTRGS_COLUMNINDEX		3

class ExperimentParameterDefinitionContainer;
class cExperimentStructure;
class QDomNodeList;

class ExperimentBlockParameterView : public QTableWidget
{
	Q_OBJECT

signals:
	void destroyed(QWidget*);

public:
	ExperimentBlockParameterView(QWidget *parent = NULL);
	~ExperimentBlockParameterView();

	bool parseExperimentStructure(cExperimentStructure *ExpStruct);
	bool parseExperimentBlockParameters(const QDomNodeList &tmpDomNodeList);
	bool setExperimentObjects(const QList<ExperimentStructuresNameSpace::strcExperimentObject> &lExperimentObjects);

public slots:
	void resizeView(const int &nWidth, const int &nHeight);

private slots:
	void cellItemChanged(const int &nRow, const int &nColumn);
	void cellItemEditFinished(const QString&sParamName, const QString&sNewValue);
	//void widgetItemChanged(QWidget *pWidget, const QString &sNewValue);	

protected:
	void currentChanged(const QModelIndex &current, const QModelIndex &previous);
	void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
	void configureEditHandling(const bool &bEnable);
	bool appendExperimentBlockParameterChanges();
	void initTableSetup();

	struct strcParameterBlockChanges
	{
		int nBlockID;
		QString sValue;
		strcParameterBlockChanges()
		{
			nBlockID = -1;
			sValue = "";
		}
	};

	struct strcColumnInfo
	{
		int nColumnIndex;
		QString sHeader;
		strcColumnInfo()
		{
			nColumnIndex = -1;
			sHeader = "";
		}
	};

	struct strcExperimentObjectInfo
	{
		ExperimentStructuresNameSpace::strcExperimentObject ObjectGlobalInfo;
		ExperimentParameterDefinitionContainer *pObjectParamDefContainer;
		strcExperimentObjectInfo()
		{
			pObjectParamDefContainer = NULL;
		}
	};

	cExperimentStructure *parsedExpStruct;
	QStringList lColumnHeaders;	
	QStringList lVerticalHeaders;	
	bool bEditHandlingEnabled;
	QMutex mutexEditHandlingEnabled;

	QHash<int, strcExperimentObjectInfo> hashObjectIdExperimentObjectInfo;
	QHash<QString, QList<strcParameterBlockChanges>> hashExpParamBlockChanges;
	QHash<int, int> hashBlockIdRowIndex;
	QHash<int, int> hashRowIndexBlockId;
	QHash<int, QString> hashColumnIndexObjectIDParamName;
	QHash<QString, strcColumnInfo> hashObjectParameterColumnIndex;
};

#endif // EXPERIMENTBLOCKPARAMETERVIEW_H
