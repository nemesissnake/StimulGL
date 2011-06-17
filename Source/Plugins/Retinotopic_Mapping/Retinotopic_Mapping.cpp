#include "Retinotopic_Mapping.h"

QScriptValue Retinotopic_Mapping::ctor__extensionname(QScriptContext* context, QScriptEngine* engine)
{
	//this function gets called first whenever a new object is constructed trough the script

	//	if (context->isCalledAsConstructor()) {
	//		// initialize the new object
	//		//context->thisObject().setProperty("bar", ...);
	//		// ...
	//		// return a non-object value to indicate that the
	//		// thisObject() should be the result of the "new Foo()" expression
	//		//return engine->undefinedValue();

	return engine->newQObject(new Retinotopic_Mapping(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
} 

Retinotopic_Mapping::Retinotopic_Mapping(QWidget *parent, Ui::Retinotopic_Mapping_DialogClass *pRetMapDlg) : QObject(parent), pRetinotopicMappingDlg(pRetMapDlg), WidgetParent(parent)
{
	renderWdg = NULL;
	stimTimer = NULL;
	stimContainerDlg = NULL;
	m_RunFullScreen = true;
	if(pRetinotopicMappingDlg)
		pRetinotopicMappingDlg->FullScreenToggle->setChecked(true);
	m_ExpType = 0;
	m_TrTime = 1000;
	nextTimeThresholdTRs = 0;
	completedTR = 0;
	m_ExpFileName = "";
	m_DebugMode = false;
	m_ForceExperimentToStop = false;
	m_TriggerMode = 0;
	m_TriggerCount = 0;
}

Retinotopic_Mapping::~Retinotopic_Mapping()
{
	cleanupExperiment(true);
}

//void Retinotopic_Mapping::setExampleProperty( short sExampleProperty )
//{
//	m_ExampleProperty = sExampleProperty;
//	emit ExampleSignalTriggered(m_ExampleProperty);//Signal that the value has been changed
//}

//short Retinotopic_Mapping::getExampleProperty() const
//{
//	return m_ExampleProperty;
//}

void Retinotopic_Mapping::updateTotalNrOfTRs()
{
	int i, sumTRs = 0;
	for(i=0; i<BlockTrialsDurationInTRs.size(); i++)
	{
		sumTRs += BlockTrialsDurationInTRs[i];
	}
	if(pRetinotopicMappingDlg)
		pRetinotopicMappingDlg->NrOfTRsEd->setText(QString::number(sumTRs));
}

void Retinotopic_Mapping::updateExperimentFromTable()
{
	if(pRetinotopicMappingDlg)
	{
		// read potentially modified exp table and then save to disk:
		int i;
		for(i=0; i<BlockTrialFiles.size(); i++)
		{
			BlockTrialFiles[i]          = pRetinotopicMappingDlg->ExpContentTableWdg->item(i, 0)->text();
			BlockTrialsDurationInTRs[i] = pRetinotopicMappingDlg->ExpContentTableWdg->item(i, 1)->text().toInt();
		}
		updateTotalNrOfTRs();
	}
}

bool Retinotopic_Mapping::eventFilter(QObject *target, QEvent *event)
{
	if (target == stimContainerDlg) 
	{
		if (event->type() == QEvent::KeyPress) 
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			switch (keyEvent->key())
			{
			case Qt::Key_Escape://abort the experiment
				abortExperiment();
				break;
			//default :
			//	break;
			}

			//if(keyEvent->text() == ui.USBCharEdit->text()) // for fMRI triggering via USB port (simulated keyboard)
			//{
			//	updateTriggerCount();
			//	return true;
			//}
		}
	}
	//return QDialog::eventFilter(target, event);
	return true;
}

void Retinotopic_Mapping::abortExperiment()
{
	m_ForceExperimentToStop = true;
	QThread::currentThread()->setPriority(QThread::NormalPriority);
}

int Retinotopic_Mapping::incrementTriggerCount()
{
	m_TriggerCount++;
	return m_TriggerCount;
}

bool Retinotopic_Mapping::cleanupExperiment(bool bDestruct)
{
	if (stimTimer)
	{
		delete stimTimer;
		stimTimer = NULL;
	}
	if (renderWdg)
	{
		renderWdg->bForceToStop = true;
		if(bDestruct)
		{
			delete renderWdg;
			renderWdg = NULL;
		}
	}
	if (stimContainerDlg)
	{
		stimContainerDlg->close();
		if(bDestruct)
		{
			delete stimContainerDlg;
			stimContainerDlg = NULL;
		}
	}
	m_TriggerCount = 0;
	return true;
}

bool Retinotopic_Mapping::checkForNextBlockTrial()
{
	if (m_ForceExperimentToStop)
	{		
		cleanupExperiment();
		emit ExperimentStopped();
		return false;
	}
	int total_elapsed_time = experimentRunningTime.elapsed();//The Total time past since the experiment started
	if(m_TriggerMode == 0)
	{
		completedTR = total_elapsed_time / m_TrTime;//The time past in number of Tr's
	}
	else
	{
		completedTR = m_TriggerCount;
	}
	bool goToNextBlockTrial = false;
	if(currentBlockTrial == -1) //First Block Trial? (Start/Init)
	{
		goToNextBlockTrial = true;
	}
	else
	{
		goToNextBlockTrial = completedTR > nextTimeThresholdTRs;//Go to next Block Trial when the total completed Tr's is larger than the next Threshold Tr.
	}
	if(goToNextBlockTrial)//When we init/start or switch from a Block Trial 
	{
		currentBlockTrial++;//Increment the Block Trial Counter
		if(currentBlockTrial >= BlockTrialFiles.size())//No more Block Trials left?
		{
			cleanupExperiment();
			emit ExperimentStopped();
			return false;
		}
		nextTimeThresholdTRs += BlockTrialsDurationInTRs[currentBlockTrial];//increment the nextTimeThresholdTRs
		loadBlockTrialFile(BlockTrialFiles[currentBlockTrial], true);
		renderWdg->frame_counter = 0;
		renderWdg->initBlockTrial();
		renderWdg->frameTime.restart();
		stimTimer->singleShot(10, renderWdg, SLOT(onAnimate()));
		return true;
	}
	return false;
}

void Retinotopic_Mapping::loadBlockTrialFile(QString blockTrialFileName, bool readForRendering)
{
	if( readForRendering && (renderWdg == NULL))//There should be an renderWdg in case we Read For Rendering!
		return;
	QString filename = combinePathAndFileName(m_ExpFolder, blockTrialFileName);
	if(!QFile::exists(filename))
	{
		QMessageBox::warning(stimContainerDlg, " Retinotopic Mapping", "File not found", QMessageBox::Ok);
		return;
	}
	if( !readForRendering )
	{
		if(pRetinotopicMappingDlg)
		{	
			pRetinotopicMappingDlg->blockTrialContentDkWdg->setWindowTitle(" " + blockTrialFileName);
			pRetinotopicMappingDlg->blockTrialContentTbWdg->setRowCount(0);
		}
	}
	QFile fileB(filename);
	if(!fileB.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::warning(stimContainerDlg, " Retinotopic Mapping", "Can not open file", QMessageBox::Ok);
		return;
	}
	QTextStream ar(&fileB);
	int i, nrOfParams, nrOfEntries;
	QString str, conditionType, infoText;
	QStringList paramNames, paramTypes;
	ar >> str >> conditionType;
	ar >> str >> nrOfParams;
	ar >> str;
	for(i=0; i<nrOfParams; i++)
	{
		readStringInQuotes(str, ar);
		paramNames.append(str);
	}
	ar >> str;
	for(i=0; i<nrOfParams; i++)
	{
		ar >> str;
		paramTypes.append(str);
	}
	ar >> str;
	readStringInQuotes(infoText, ar);
	ar >> str >> nrOfEntries;
	if( readForRendering )
	{
		for(i=0; i<renderWdg->BlockTrialParamTable.size(); i++)
			renderWdg->BlockTrialParamTable[i].clear();

		renderWdg->BlockTrialParamTable.resize(nrOfEntries);
	}
	else
	{
		if(pRetinotopicMappingDlg)
		{
			pRetinotopicMappingDlg->blockTrialContentTbWdg->setColumnCount(nrOfParams);
			pRetinotopicMappingDlg->blockTrialContentTbWdg->setRowCount(nrOfEntries);
			pRetinotopicMappingDlg->blockTrialContentTbWdg->setHorizontalHeaderLabels(paramNames);

			pRetinotopicMappingDlg->conditionTypeEd->setText(conditionType);
			pRetinotopicMappingDlg->nrOfParamsSpBx->setValue(nrOfParams);
			pRetinotopicMappingDlg->nameOfParamsCmBx->clear(); 
			pRetinotopicMappingDlg->nameOfParamsCmBx->addItems(paramNames);
			QString strParamTypes;
			for(i=0; i<paramTypes.count(); i++) 
			{ 
				strParamTypes += paramTypes[i]; 
				if(i<(paramTypes.count() - 1)) 
					strParamTypes += " "; 
			}
			pRetinotopicMappingDlg->typeOfParamsEd->setText(strParamTypes);
			pRetinotopicMappingDlg->InfoTxEd->setPlainText(infoText);
		}
	}
	QTableWidgetItem *newItem;
	int r, c;
	for(r=0; r<nrOfEntries; r++)
	{
		for(c=0; c<nrOfParams; c++)
		{
			ar >> str;
			if( readForRendering )
			{
				renderWdg->BlockTrialParamTable[r].append(str);
			}
			else
			{
				newItem = new QTableWidgetItem(str);
				if(pRetinotopicMappingDlg)
					pRetinotopicMappingDlg->blockTrialContentTbWdg->setItem(r, c, newItem);
			}
		}
	}
	fileB.close();
	if( !readForRendering )
	{
		if(pRetinotopicMappingDlg)
		{
			pRetinotopicMappingDlg->blockTrialContentTbWdg->resizeColumnsToContents();
			pRetinotopicMappingDlg->blockTrialContentTbWdg->resizeRowsToContents();
		}
	}
}

bool Retinotopic_Mapping::ClearBlockTrials()
{
	BlockTrialFiles.clear();
	BlockTrialsDurationInTRs.clear();
	return true;
}

bool Retinotopic_Mapping::AppendBlockTrial(const QString qstrFile, const int nDurationInTRs)
{
	BlockTrialFiles.append(qstrFile);
	BlockTrialsDurationInTRs.append(nDurationInTRs);
	return true;
}

QString Retinotopic_Mapping::getExperimentFileName()
{
	QString tmpFilename;

	if(pRetinotopicMappingDlg)//Do we have an Dialog? In case of a script object the dialog is not constructed!
	{		
		if(pRetinotopicMappingDlg->ExperimentCmb->currentText() == "")
			return "";
		tmpFilename = combinePathAndFileName(pRetinotopicMappingDlg->experimentsFolderEd->text(), pRetinotopicMappingDlg->ExperimentCmb->currentText());	
		m_ExpFolder = pRetinotopicMappingDlg->experimentsFolderEd->text();
	}
	else
	{
		if(m_ExpFileName == "")
			return "";
		tmpFilename = m_ExpFileName;
	}
	if(QFile::exists(tmpFilename))
	{
		return tmpFilename;
	}
	else
	{
		m_ExpFolder = "";
		return "";
	}
}

//bool Retinotopic_Mapping::setExperimentFolder(const QString qstrExpFileName)
//{
//	m_ExpFileName = qstrExpFileName;
//	//if(pRetinotopicMappingDlg)
//	//pRetinotopicMappingDlg->ExperimentCmb->text()
//	return true;
//}

bool Retinotopic_Mapping::setExperimentFileName(const QString qstrExpFileName)
{
	m_ExpFileName = qstrExpFileName;
	if(!pRetinotopicMappingDlg)
	{
		QFileInfo fInfo(m_ExpFileName);
		m_ExpFolder = fInfo.absolutePath();
	}
	//pRetinotopicMappingDlg->ExperimentCmb->text()
	return true;
}

bool Retinotopic_Mapping::loadExperiment()
{
	QString filename = getExperimentFileName();
	if(filename == "")
		return false;

	QFile fileEXP(filename);
	if(!fileEXP.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::warning(stimContainerDlg, " Retinotopic Mapping", "Can not open file", QMessageBox::Ok);
		return false;
	}

	QTextStream ar(&fileEXP);

	int TR_N, nrofentries = 0;
	QString TrialOrBlockFile;

	ClearBlockTrials();

	while(1)
	{
		ar >> TrialOrBlockFile;
		if(ar.atEnd())
			break;

		ar >> TR_N;

		// for easier access during runtime, store also in "model" vars
		AppendBlockTrial(TrialOrBlockFile,TR_N);

		// and here for display/edit
		if(pRetinotopicMappingDlg)
		{
			pRetinotopicMappingDlg->ExpContentTableWdg->setRowCount(nrofentries + 1);

			QTableWidgetItem *newBlockTrialItem = new QTableWidgetItem(TrialOrBlockFile);
			pRetinotopicMappingDlg->ExpContentTableWdg->setItem(nrofentries, 0, newBlockTrialItem);

			QTableWidgetItem *newTRItem = new QTableWidgetItem(QString::number(TR_N));
			pRetinotopicMappingDlg->ExpContentTableWdg->setItem(nrofentries, 1, newTRItem);
		}

		nrofentries++;
	}

	fileEXP.close();
	return true;
}

bool Retinotopic_Mapping::saveExperiment()
{
	if(BlockTrialFiles.size() == 0)
		return false;
	updateExperimentFromTable();
	QString filename = getExperimentFileName();
	if(filename == "")
		return false;
	QFile fileEXP(filename);
	if(!fileEXP.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::warning(stimContainerDlg, " Retinotopic Mapping", "Can not open experiment file", QMessageBox::Ok);
		return false;
	}
	QTextStream ar(&fileEXP);
	int i, maxstring = 0;
	for(i=0; i<BlockTrialFiles.size(); i++)
	{
		if(BlockTrialFiles[i].length() > maxstring)
			maxstring = BlockTrialFiles[i].length();
	}
	for(i=0; i<BlockTrialFiles.size(); i++)
	{
		ar.setFieldWidth(maxstring + 2);
		ar.setFieldAlignment(QTextStream::AlignLeft);
		ar << BlockTrialFiles[i];

		ar.setFieldWidth(0);
		ar << BlockTrialsDurationInTRs[i];

		ar << "\n";
	}
	fileEXP.close();
	return true;
}

bool Retinotopic_Mapping::runExperiment()
{
	m_ForceExperimentToStop = false;
	if(BlockTrialFiles.size() == 0)//Do we have some Block Trial Files?
		return false;
	updateExperimentFromTable(); // use most recent table entries, only when using the UI dialog!
	// check that all block/trial files are available:
	for(int i=0; i<BlockTrialFiles.size(); i++)//Do all defined BlockTrialFiles exist?
	{
		QString BlockTrialFile = combinePathAndFileName(m_ExpFolder, BlockTrialFiles[i]);
		if( !QFile::exists(BlockTrialFile) )
		{
			QMessageBox::warning(stimContainerDlg, " Retinotopic Mapping", QString("Can not find file \"%1\" ").arg(BlockTrialFile), QMessageBox::Ok);
			return false;
		}
	}
	if (pRetinotopicMappingDlg)
		m_TrTime = pRetinotopicMappingDlg->TRSpBx->value(); 
	nextTimeThresholdTRs = 0;
	completedTR = 0;
	m_TriggerCount = 0;
#ifdef Q_OS_WIN
	bool ret = SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
#else
	QThread::currentThread()->setPriority(QThread::HighPriority);  
	// QThread::TimeCriticalPriority);
#endif
	cleanupExperiment(true);
	if (pRetinotopicMappingDlg)
		stimContainerDlg = new ContainerDlg(WidgetParent);
	else
		stimContainerDlg = new ContainerDlg();//parent parameter?
	stimContainerDlg->setAttribute(Qt::WA_DeleteOnClose);
	QGLFormat fmt;
	fmt.setSwapInterval(1); // sync with vertical refresh
	fmt.setSampleBuffers(true);
	renderWdg = new GLWidget(fmt, stimContainerDlg, this);
	renderWdg->setObjectName("RenderWidgetGL");
	stimContainerDlg->installEventFilter(this);//re-route all stimContainerDlg events to this->bool Retinotopic_Mapping::eventFilter(QObject *target, QEvent *event)
	// check whether we have double buffering, otherwise cancel
	if(!renderWdg->format().doubleBuffer() )
	{
		// message box
		stimContainerDlg->deleteLater();//Schedules this object for deletion, the object will be deleted when control returns to the event loop
		return false;
	}
	int swap_interval = renderWdg->format().swapInterval(); // support for vertical retrace sync?
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->setAlignment(Qt::AlignCenter);
	mainLayout->setMargin(0);
	mainLayout->addWidget(renderWdg);
	stimContainerDlg->setLayout(mainLayout);
	if(m_RunFullScreen)
		stimContainerDlg->showFullScreen();
	else
		stimContainerDlg->show();
	QDesktopWidget *dt = QApplication::desktop();
	QCursor::setPos(dt->width(), dt->height()/2); // not at bottom because then mouse movement on Mac would show dock
	stimTimer = new QTimer(this);
	renderWdg->ExpType = m_ExpType;
	renderWdg->TR = m_TrTime/1000;
	experimentRunningTime.start();
	renderWdg->trialTime.start();
	renderWdg->frameTime.start();
	currentBlockTrial = -1;
	checkForNextBlockTrial();
	return true;
}

bool Retinotopic_Mapping::setFullScreenMode(const bool bFullScreen)
{
	m_RunFullScreen = bFullScreen;
	if(pRetinotopicMappingDlg)
		pRetinotopicMappingDlg->FullScreenToggle->setChecked(m_RunFullScreen);
	return true;
}

bool Retinotopic_Mapping::setDebugMode(const bool bDebugMode)
{
	m_DebugMode = bDebugMode;
	if(pRetinotopicMappingDlg)
		pRetinotopicMappingDlg->DebugInfoToggle->setChecked(m_DebugMode);
	return true;
}

bool Retinotopic_Mapping::setTrTime(const int nTrTimeInMillSec)
{
	m_TrTime = nTrTimeInMillSec;
	if(pRetinotopicMappingDlg)
		pRetinotopicMappingDlg->TRSpBx->setValue(nTrTimeInMillSec);
	return true;
}

bool Retinotopic_Mapping::setExperimentType(const int nExpType)
{
	m_ExpType = nExpType;
	//if(pRetinotopicMappingDlg)
		//pRetinotopicMappingDlg->etc..
	return true;
}

bool Retinotopic_Mapping::setTriggerMode(const int nTriggerMode)
{
	m_TriggerMode = nTriggerMode;
	//if(pRetinotopicMappingDlg)
	//pRetinotopicMappingDlg->etc..
	return true;
}

ContainerDlg::ContainerDlg(QWidget *parent) : QDialog(parent)
{
	QPalette pal = palette();
	pal.setColor(QPalette::Window, QColor(16, 16, 16));
	setPalette(pal);
}

ContainerDlg::~ContainerDlg()
{

}

void ContainerDlg::closeEvent(QCloseEvent *e)
{
	showNormal();

	e->accept();
}

void ContainerDlg::reject() // called when "Esc" pressed
{
	showNormal(); // to get back dock on Mac, we need to call this here (restore from fullscreen mode)

	QDialog::reject();
}