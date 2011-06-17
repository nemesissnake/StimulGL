//This file defines the script binding interface, all below function are scriptable except for the destructor

#ifndef Retinotopic_Mapping_H
#define Retinotopic_Mapping_H
#include <QObject>
#include <QString>
#include <QtScript>
#include <QScriptable>
#include <QGLWidget>
#include <QTime>
#include <QPen>
#ifdef Q_OS_WIN
#include <windows.h>
#endif
#include "Retinotopic_Mapping_dialog.h"
#include "globals.h"
#include "renderWdg.h"

class ContainerDlg;
class GLWidget;
class Retinotopic_Mapping_DialogClass;

class Retinotopic_Mapping : public QObject, protected QScriptable
{
	Q_OBJECT
	//Q_PROPERTY( short ExampleProperty WRITE setExampleProperty READ getExampleProperty )

signals:
	void ExperimentStopped();

public:
	Retinotopic_Mapping(QWidget *parent = 0, Ui::Retinotopic_Mapping_DialogClass *pRetMapDlg = NULL);//QObject
	~Retinotopic_Mapping();
	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);

	QStringList BlockTrialFiles;
	QList<int> BlockTrialsDurationInTRs;
	QTime experimentRunningTime;
	int currentBlockTrial;
	int nextTimeThresholdTRs;
	int completedTR;
	int m_TriggerCount;
	bool m_RunFullScreen;
	bool m_DebugMode; // print debug info - at present update duration (16/17 if 60 Hz and retrace used (not related to compilation in release or debug mode)
	int	m_TrTime;
	int m_TriggerMode;

	bool checkForNextBlockTrial();
	bool eventFilter(QObject *target, QEvent *event);
	void updateTotalNrOfTRs();
	void updateExperimentFromTable();
	bool saveExperiment();
	bool cleanupExperiment(bool bDestruct = false);

public slots:
	//void setExampleProperty( short sExampleProperty );
	//short getExampleProperty() const;
	bool setExperimentFileName(const QString qstrExpFileName);
	QString getExperimentFileName();
	bool loadExperiment();
	bool runExperiment();
	void abortExperiment();
	int incrementTriggerCount();
	
	bool setFullScreenMode(const bool bFullScreen);
	bool setDebugMode(const bool bDebugMode);
	bool setTrTime(const int nTrTimeInMillSec);
	bool setExperimentType(const int nExpType);
	bool setTriggerMode(const int nTriggerMode);
	bool ClearBlockTrials();
	bool AppendBlockTrial(const QString qstrFile, const int nDurationInTRs);
	void loadBlockTrialFile(const QString blockTrialFileName, const bool readForRendering=false);

private:
	//short m_ExampleProperty;
	Ui::Retinotopic_Mapping_DialogClass *pRetinotopicMappingDlg;
	QWidget *WidgetParent;

	QTimer *stimTimer;
	ContainerDlg *stimContainerDlg;
	GLWidget *renderWdg;
	int m_ExpType;
	QString m_ExpFileName;
	QString m_ExpFolder;
	bool m_ForceExperimentToStop;
};

class ContainerDlg : public QDialog
{
	Q_OBJECT

public:
	ContainerDlg(QWidget *parent = NULL);
	ContainerDlg::~ContainerDlg();

	private slots:
		void reject();

protected:
	void closeEvent(QCloseEvent *e);
};

#endif // Retinotopic_Mapping_H
