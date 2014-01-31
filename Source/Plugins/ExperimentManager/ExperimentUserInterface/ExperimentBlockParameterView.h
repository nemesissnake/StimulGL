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

#define BLOCKPARAMVIEW_BLOCKNUMBER_HEADER			"Number"
#define BLOCKPARAMVIEW_BLOCKNAME_HEADER				"Name"
#define BLOCKPARAMVIEW_BLOCKTRIALS_HEADER			"Trials"
#define BLOCKPARAMVIEW_BLOCKINTTRIGGERS_HEADER		"Int. Triggers"
#define BLOCKPARAMVIEW_BLOCKEXTTRIGGERS_HEADER		"Ext. Triggers"

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

public slots:
	void resizeView(const int &nWidth, const int &nHeight);

private slots:
	void viewEdited(const int &nRow, const int &nColumn);

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

	cExperimentStructure *parsedExpStruct;
	QStringList lColumnHeaders;	
	bool bEditHandlingEnabled;
	QMutex mutexEditHandlingEnabled;

	QHash<QString, QList<strcParameterBlockChanges>> hashExpParamBlockChanges;
	QHash<int, int> hashBlockIdRowIndex;
	QHash<QString, strcColumnInfo> hashObjectParameterColumnIndex;
};

#endif // EXPERIMENTBLOCKPARAMETERVIEW_H
