var tmpString;
var InstallProcess = new QProcess();
var tmpByteArray = new QByteArray();
var tmpStringList =  new Array();
var tmpString = new String();

var sInstallerAppName = tr("StimulGL");//StimulGL
var sInstallerConfiguration = tr("win32");//win32 or x64
var sInstallerVersion = tr("2.0.0.2");//<Major>.<Minor>.<Build>.<Revision>
var sInstallerPlatform = tr("Windows");

QByteArray.prototype.toString = function()
{
	ts = new QTextStream( this, QIODevice.ReadOnly );
	return ts.readAll();
}

function CleanUpScript()
{
	Log("CleanUpScript started...");
	ConnectDisconnectScriptFunctions(false);
	tr=null;
	tmpByteArray=null;
	tmpStringList=null;
	tmpString=null;
	ExecuteInstallJammer=null;
	processReadyRead=null;
	QByteArray.prototype.toString=null;
	InstallProcess=null;
	ConnectDisconnectScriptFunctions=null;
	processFinished=null;
	CleanUpScript=null;	
	Log("CleanUpScript exited successfully!");
	StimulGL.cleanupScript();
}

function ConnectDisconnectScriptFunctions(Connect)
{
	if(Connect)
	{
		Log("Connecting Signal/Slots...");
		try 
		{	
			InstallProcess.readyRead.connect(this, this.processReadyRead);	
			InstallProcess["finished(int,QProcess::ExitStatus)"].connect(this, this.processFinished);	 
		} 
		catch (e) 
		{
			Log(".*. Something went wrong connecting the Signal/Slots:" + e);
		}		
	}
	else
	{
		Log("Disconnecting Signal/Slots...");
		try 
		{	
			InstallProcess.readyRead.disconnect(this, this.processReadyRead);
			InstallProcess.finished.disconnect(this, this.processFinished);	 
		} 
		catch (e) 
		{
			Log(".*. Something went wrong disconnecting the Signal/Slots:" + e);
		}		
	}	
}

function tr(s) { return s; }

function getString(title,text,defaultValue)
{
	tmpString = QInputDialog.getText(this, tr(title), tr(text), QLineEdit.Normal, tr(defaultValue), Qt.WindowFlags(0));
	if (tmpString != null && tmpString.length != 0)
	{
		return tr(tmpString);
	}
	else
	{	
		return tr("");
	}
}

function processReadyRead()
{
	tmpByteArray = InstallProcess.readAll();
	Log(tmpByteArray.toString());	
}

function processFinished()
{
	Log("processFinished() called!");
	//Log(__qt_sender__.workingDirectory());
	//Log(this.name);
	//Log(arguments.callee);
	//Log("processFinished() proccessed Successfully (" + __qt_sender__.name() + ", " + this.name + ", " + arguments.callee + ").");	
	CleanUpScript();
}

function ExecuteInstallJammer()
{
	//var bKeepReading = true;
	var strInstallJammerPath = StimulGL.getEnvironmentVariabele("PROGRAMFILES(x86)") + "/InstallJammer/installjammer.exe";//"C:/Windows/System32/notepad.exe"
	var strInstallConfigurationFile = StimulGL.getSelectedScriptFileLocation() + "/InstallJammer_(32+64bit).mpi";
	
	tmpStringList = new Array();//Reset list
	tmpStringList[0] = "-DAppName";
	tmpStringList[1] = sInstallerAppName;
	tmpStringList[2] = "-DPlatformName";
	tmpStringList[3] = sInstallerConfiguration;
	tmpStringList[4] = "-DInstallType";
	tmpStringList[5] = sInstallerConfiguration;
	tmpStringList[6] = "-DInstallVersion";
	tmpStringList[7] = sInstallerVersion;
	tmpStringList[8] = "-DVersion"
	tmpStringList[9] = sInstallerVersion;
	tmpStringList[10] = "--platform"
	tmpStringList[11] = sInstallerPlatform;
	//Insert new options here!	
	tmpStringList[12] = "--build-for-release";
	tmpStringList[13] = "--build";
	tmpStringList[14] = strInstallConfigurationFile;
	//Log(tmpStringList.length);
	//return false;

	InstallProcess.start(strInstallJammerPath, tmpStringList);

	if (!InstallProcess.waitForStarted())
		return false;
//		if(!InstallProcess.waitForReadyRead(5000))
	//if (!InstallProcess.waitForFinished())
	return true;
}

ConnectDisconnectScriptFunctions(true);
//Log(getString("This is an Title", "Some Text....","default x"));
if(ExecuteInstallJammer())
{
	Log("InstallScript started Successfully!");
}
else
{
	Log("InstallScript Failed to start!");
}







//function Dialog(parent)
//{
//    QDialog.call(this, parent);
//
//    this.errorMessageDialog = new QErrorMessage(this);
//
//    var frameStyle = QFrame.Sunken | QFrame.Panel;
//
//    this.integerLabel = new QLabel;
//    this.integerLabel.setFrameStyle(frameStyle);
//    var integerButton =
//            new QPushButton(tr("QInputDialog::get&Integer()"));
//
//    this.doubleLabel = new QLabel;
//    this.doubleLabel.setFrameStyle(frameStyle);
//    var doubleButton =
//            new QPushButton(tr("QInputDialog::get&Double()"));
//
//    this.itemLabel = new QLabel;
//    this.itemLabel.setFrameStyle(frameStyle);
//    var itemButton = new QPushButton(tr("QInputDialog::getIte&m()"));
//
//    this.textLabel = new QLabel;
//    this.textLabel.setFrameStyle(frameStyle);
//    var textButton = new QPushButton(tr("QInputDialog::get&Text()"));
//
//    this.colorLabel = new QLabel;
//    this.colorLabel.setFrameStyle(frameStyle);
//    var colorButton = new QPushButton(tr("QColorDialog::get&Color()"));
//
//    this.fontLabel = new QLabel;
//    this.fontLabel.setFrameStyle(frameStyle);
//    var fontButton = new QPushButton(tr("QFontDialog::get&Font()"));
//
//    this.directoryLabel = new QLabel;
//    this.directoryLabel.setFrameStyle(frameStyle);
//    var directoryButton =
//            new QPushButton(tr("QFileDialog::getE&xistingDirectory()"));
//
//    this.openFilePaths = ".";
//    this.openFileNameLabel = new QLabel;
//    this.openFileNameLabel.setFrameStyle(frameStyle);
//    var openFileNameButton =
//            new QPushButton(tr("QFileDialog::get&OpenFileName()"));
//
//    this.openFileNamesLabel = new QLabel;
//    this.openFileNamesLabel.setFrameStyle(frameStyle);
//    var openFileNamesButton =
//            new QPushButton(tr("QFileDialog::&getOpenFileNames()"));
//
//    this.saveFileNameLabel = new QLabel;
//    this.saveFileNameLabel.setFrameStyle(frameStyle);
//    var saveFileNameButton =
//            new QPushButton(tr("QFileDialog::get&SaveFileName()"));
//
//    this.criticalLabel = new QLabel;
//    this.criticalLabel.setFrameStyle(frameStyle);
//    var criticalButton =
//            new QPushButton(tr("QMessageBox::critica&l()"));
//
//    this.informationLabel = new QLabel;
//    this.informationLabel.setFrameStyle(frameStyle);
//    var informationButton =
//            new QPushButton(tr("QMessageBox::i&nformation()"));
//
//    this.questionLabel = new QLabel;
//    this.questionLabel.setFrameStyle(frameStyle);
//    var questionButton =
//            new QPushButton(tr("QMessageBox::&question()"));
//
//    this.warningLabel = new QLabel;
//    this.warningLabel.setFrameStyle(frameStyle);
//    var warningButton = new QPushButton(tr("QMessageBox::&warning()"));
//
//    this.errorLabel = new QLabel;
//    this.errorLabel.setFrameStyle(frameStyle);
//    var errorButton =
//            new QPushButton(tr("QErrorMessage::show&M&essage()"));
//
//    integerButton["clicked()"].connect(this, this.setInteger);
//    doubleButton["clicked()"].connect(this, this.setDouble);
//    itemButton["clicked()"].connect(this, this.setItem);
//    textButton["clicked()"].connect(this, this.setText);
//    colorButton["clicked()"].connect(this, this.setColor);
//    fontButton["clicked()"].connect(this, this.setFont);
//    directoryButton["clicked()"].connect(
//            this, this.setExistingDirectory);
//    openFileNameButton["clicked()"].connect(
//            this, this.setOpenFileName);
//    openFileNamesButton["clicked()"].connect(
//            this, this.setOpenFileNames);
//    saveFileNameButton["clicked()"].connect(
//            this, this.setSaveFileName);
//    criticalButton["clicked()"].connect(this, this.criticalMessage);
//    informationButton["clicked()"].connect(
//            this, this.informationMessage);
//    questionButton["clicked()"].connect(this, this.questionMessage);
//    warningButton["clicked()"].connect(this, this.warningMessage);
//    errorButton["clicked()"].connect(this, this.errorMessage);
//
//    this.useNative = new QCheckBox(this);
//    this.useNative.text = tr("Use native file dialog");
//    this.useNative.checked = true;
//
//    var layout = new QGridLayout;
//    layout.setColumnStretch(1, 1);
//    layout.setColumnMinimumWidth(1, 250);
//    layout.addWidget(integerButton, 0, 0);
//    layout.addWidget(this.integerLabel, 0, 1);
//    layout.addWidget(doubleButton, 1, 0);
//    layout.addWidget(this.doubleLabel, 1, 1);
//    layout.addWidget(itemButton, 2, 0);
//    layout.addWidget(this.itemLabel, 2, 1);
//    layout.addWidget(textButton, 3, 0);
//    layout.addWidget(this.textLabel, 3, 1);
//    layout.addWidget(colorButton, 4, 0);
//    layout.addWidget(this.colorLabel, 4, 1);
//    layout.addWidget(fontButton, 5, 0);
//    layout.addWidget(this.fontLabel, 5, 1);
//    layout.addWidget(directoryButton, 6, 0);
//    layout.addWidget(this.directoryLabel, 6, 1);
//    layout.addWidget(openFileNameButton, 7, 0);
//    layout.addWidget(this.openFileNameLabel, 7, 1);
//    layout.addWidget(openFileNamesButton, 8, 0);
//    layout.addWidget(this.openFileNamesLabel, 8, 1);
//    layout.addWidget(saveFileNameButton, 9, 0);
//    layout.addWidget(this.saveFileNameLabel, 9, 1);
//    layout.addWidget(criticalButton, 10, 0);
//    layout.addWidget(this.criticalLabel, 10, 1);
//    layout.addWidget(informationButton, 11, 0);
//    layout.addWidget(this.informationLabel, 11, 1);
//    layout.addWidget(questionButton, 12, 0);
//    layout.addWidget(this.questionLabel, 12, 1);
//    layout.addWidget(warningButton, 13, 0);
//    layout.addWidget(this.warningLabel, 13, 1);
//    layout.addWidget(errorButton, 14, 0);
//    layout.addWidget(this.errorLabel, 14, 1);
//    layout.addWidget(this.useNative, 15, 0);
//    this.setLayout(layout);
//
//    this.windowTitle = tr("Standard Dialogs");
//}
//
//Dialog.MESSAGE = tr("<p>Message boxes have a caption, a text, "
//               + "and any number of buttons, each with standard or custom texts."
//               + "<p>Click a button to close the message box. Pressing the Esc button "
//               + "will activate the detected escape button (if any).");
//
//Dialog.prototype = new QDialog();
//
//Dialog.prototype.setInteger = function()
//{
//    var i = QInputDialog.getInteger(this, tr("QInputDialog::getInteger()"),
//        tr("Percentage:"), 25, 0, 100, 1, Qt.WindowFlags(0));
//    if (i != null)
//        this.integerLabel.text = i;
//}
//
//Dialog.prototype.setDouble = function()
//{
//    var d = QInputDialog.getDouble(this, tr("QInputDialog::getDouble()"),
//        tr("Amount:"), 37.56, -10000, 10000, 2, Qt.WindowFlags(0));
//    if (d != null)
//        this.doubleLabel.text = d;
//}
//
//Dialog.prototype.setItem = function()
//{
//    var items = new Array(tr("Spring"), tr("Summer"), tr("Fall"), tr("Winter"));
//
//    var item = QInputDialog.getItem(this, tr("QInputDialog::getItem()"),
//        tr("Season:"), items, 0, false, Qt.WindowFlags(0));
//    if (item != null & item.length != 0)
//        this.itemLabel.text = item;
//}
//
//Dialog.prototype.setText = function()
//{
//    var text = QInputDialog.getText(this, tr("QInputDialog::getText()"),
//                                         tr("User name:"), QLineEdit.Normal,
//        QDir.home().dirName(), Qt.WindowFlags(0));
//    if (text != null && text.length != 0)
//        this.textLabel.text = text;
//}
//
//Dialog.prototype.setColor = function()
//{
//    var color = QColorDialog.getColor(new QColor(Qt.green), this);
//    if (color.isValid()) {
//        this.colorLabel.text = color.name();
//        this.colorLabel.palette = new QPalette(color);
//        this.colorLabel.autoFillBackground = true;
//    }
//}
//
//Dialog.prototype.setFont = function()
//{
//    var font = QFontDialog.getFont(new QFont(this.fontLabel.text), this);
//    if (font != null) {
//        this.fontLabel.text = font.key();
//        this.fontLabel.font = font;
//    }
//}
//
//Dialog.prototype.setExistingDirectory = function()
//{
//    var options = QFileDialog.DontResolveSymlinks | QFileDialog.ShowDirsOnly;
//    if (!this.useNative.checked)
//        options |= QFileDialog.DontUseNativeDialog;
//    var directory = QFileDialog.getExistingDirectory(this,
//                                tr("QFileDialog::getExistingDirectory()"),
//                                this.directoryLabel.text,
//                                QFileDialog.Options(options));
//    if (directory.length != 0)
//        this.directoryLabel.text = directory;
//}
//
//Dialog.prototype.setOpenFileName = function()
//{
//    var options = 0;
//    if (!this.useNative.checked)
//        options |= QFileDialog.DontUseNativeDialog;
//    var selectedFilter;
//    var fileName = QFileDialog.getOpenFileName(this,
//                                tr("QFileDialog::getOpenFileName()"),
//                                this.openFileNameLabel.text,
//                                tr("All Files (*);;Text Files (*.txt)"),
//                                selectedFilter,
//                                QFileDialog.Options(options));
//    if (fileName.length != 0)
//        this.openFileNameLabel.text = fileName;
//}
//
//Dialog.prototype.setOpenFileNames = function()
//{
//    var options = 0;
//    if (!this.useNative.checked)
//        options |= QFileDialog.DontUseNativeDialog;
//    var selectedFilter;
//    var files = QFileDialog.getOpenFileNames(
//                                this, tr("QFileDialog::getOpenFileNames()"),
//                                this.openFilesPath,
//                                tr("All Files (*);;Text Files (*.txt)"),
//                                selectedFilter,
//                                options);
//    if (files.length != 0) {
//        this.openFilesPath = files[0];
//        this.openFileNamesLabel.text = "[" + files.join(", ") + "]";
//    }
//}
//
//Dialog.prototype.setSaveFileName = function()
//{
//    var options = 0;
//    if (!this.useNative.checked)
//        options |= QFileDialog.DontUseNativeDialog;
//    var selectedFilter;
//    var fileName = QFileDialog.getSaveFileName(this,
//                                tr("QFileDialog::getSaveFileName()"),
//                                this.saveFileNameLabel.text,
//                                tr("All Files (*);;Text Files (*.txt)"),
//                                selectedFilter,
//                                options);
//    if (fileName.length != 0)
//        this.saveFileNameLabel.text = fileName;
//}
//
//Dialog.prototype.criticalMessage = function()
//{
//    var reply = QMessageBox.critical(this, tr("QMessageBox::critical()"),
//        Dialog.MESSAGE, QMessageBox.StandardButtons(QMessageBox.Abort, QMessageBox.Retry, QMessageBox.Ignore));
//    if (reply == QMessageBox.Abort)
//        this.criticalLabel.text = tr("Abort");
//    else if (reply == QMessageBox.Retry)
//        this.criticalLabel.text = tr("Retry");
//    else
//        this.criticalLabel.text = tr("Ignore");
//}
//
//Dialog.prototype.informationMessage = function()
//{
//    var reply = QMessageBox.information(this, tr("QMessageBox::information()"), Dialog.MESSAGE);
//    if (reply == QMessageBox.Ok)
//        this.informationLabel.text = tr("OK");
//    else
//        this.informationLabel.text = tr("Escape");
//}
//
//Dialog.prototype.questionMessage = function()
//{
//    var reply = QMessageBox.question(this, tr("QMessageBox::question()"),
//        Dialog.MESSAGE, QMessageBox.StandardButtons(QMessageBox.Yes, QMessageBox.No, QMessageBox.Cancel));
//    if (reply == QMessageBox.Yes)
//        this.questionLabel.text = tr("Yes");
//    else if (reply == QMessageBox.No)
//        this.questionLabel.text = tr("No");
//    else
//        this.questionLabel.text = tr("Cancel");
//}
//
//Dialog.prototype.warningMessage = function()
//{
//    var msgBox = new QMessageBox(QMessageBox.Warning, tr("QMessageBox::warning()"),
//                       Dialog.MESSAGE, 0, this);
//    msgBox.addButton(tr("Save &Again"), QMessageBox.AcceptRole);
//    msgBox.addButton(tr("&Continue"), QMessageBox.RejectRole);
//    if (msgBox.exec() == QMessageBox.AcceptRole)
//        this.warningLabel.text = tr("Save Again");
//    else
//        this.warningLabel.text = tr("Continue");
//}
//
//Dialog.prototype.errorMessage = function()
//{
//    this.errorMessageDialog.showMessage(
//            tr("This dialog shows and remembers error messages. "
//               + "If the checkbox is checked (as it is by default), "
//               + "the shown message will be shown again, "
//               + "but if the user unchecks the box the message "
//               + "will not appear again if QErrorMessage::showMessage() "
//               + "is called with the same message."));
//    this.errorLabel.text = tr("If the box is unchecked, the message "
//                           + "won't appear again.");
//};
//
//(function() {
//    var translatorFileName = "qt_";
//    translatorFileName += QLocale.system().name();
//    var translator = new QTranslator(App);
//    if (translator.load(translatorFileName, QLibraryInfo.location(QLibraryInfo.TranslationsPath)))
//        qApp.installTranslator(translator);
//
//    var dialog = new Dialog();
//    return dialog.exec();
//})();
