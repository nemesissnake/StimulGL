#include "ExperimentManager_dialog.h"

ExperimentManager_Dialog::ExperimentManager_Dialog(QWidget *parent)	: QDialog(parent)
{
	//Gets constructed only once during the load of the plugin
	ui.setupUi(this);
	connect(ui.btnExampleButton, SIGNAL(clicked()), this, SLOT(exampleButton_Pressed()));
}

ExperimentManager_Dialog::~ExperimentManager_Dialog()
{

}

void ExperimentManager_Dialog::on_okButton_clicked()
{
	cleanUp();
	accept();
}

void ExperimentManager_Dialog::on_cancelButton_clicked()
{
	cleanUp();
	reject();
}

void ExperimentManager_Dialog::cleanUp()
{
	return;
}

void ExperimentManager_Dialog::exampleButton_Pressed()
{
	QMessageBox msgBox;
	msgBox.setText("This Gui has not been implemented jet.");
	msgBox.exec();

	//StimGLScene = new StimulGLGraphScene(this);
	//if(StimGLScene->Init())
	//{
	//	GraphView = new QGraphicsView(StimGLScene);
	//	GraphView->setWindowModality(Qt::WindowModal);
	//	//connect(StimGLScene, SIGNAL(UserWantsToClose(void)), this, SLOT(on_GraphScene_Closed(void)));
	//	GraphView->showFullScreen();
	//}
}

//void ExperimentManager_Dialog::on_GraphScene_Closed()
//{
//	//disconnect(StimGLScene, SIGNAL(UserWantsToClose(void)), this, SLOT(on_GraphScene_Closed(void)));
//	GraphView->close();
//	delete GraphView;
//	delete StimGLScene;
//	GraphView = NULL;
//	StimGLScene = NULL;
//}
