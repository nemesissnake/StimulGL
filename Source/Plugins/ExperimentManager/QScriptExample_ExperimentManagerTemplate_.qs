////Construct a new Plugin object
//ExperimentManagerObj = new ExperimentManager();//StimulGLGraphScene();

////Example usage of the Example property
//ExperimentManagerObj.setExampleProperty(99);
//Log(ExperimentManagerObj.ExampleProperty);
//ExperimentManagerObj.ExampleProperty = 88;
//Log(ExperimentManagerObj.ExampleProperty);

//ExperimentManagerObj.Init();
//ExperimentManagerObj.Show();//Press ctrl+Alt+Escape to close the form
////Pause(5000);//This function does block the remaining!
////ExperimentManagerObj = null;

//var GraphScene = new QGraphicsScene();
//var rect = new QRectF(0, 0, 100, 100);
//GraphScene.addRect(rect);
//var  pixmap = new QPixmap("D:\\Projects\\StimulGL\\Source\\Plugins\\ExperimentManager\\qt-logo.png");
//GraphScene.addPixmap(pixmap);
////var str = new QString();
////str = "Example text";
//var str = new String("Example text");
//var fnt = Qt.QFont
//var fnt = new QFont("Times", 30, 5, true);//type, size, weight, italic?
//GraphScene.addText(str,fnt);
//var GraphItem = new QGraphicsItem();
//GraphItem = GraphScene.itemAt(50, 50);
//var GraphView = new QGraphicsView(GraphScene);
//var WinMod = Qt.WindowModality(1);//=Qt::WindowModal
////GraphView.setWindowModality(1);
//GraphView.showFullScreen();
/////GraphView.close();

var TriggerTimer = new QTimer(this);
var GraphSceneTimer = new QTimer(this);

function cleanup()
{ 
	Log("cleanup called");
	ExperimentManagerObj.UserWantsToClose.disconnect(this, this.on_Scene_Closed);
	TriggerTimer.timeout.disconnect(this, this.myTriggerFunction);
	GraphView.close();
	ExperimentManagerObj = null;
	rect = null;
	pixmap = null;
	str = null;
	fnt = null;
	GraphItem = null;
	GraphView = null;
	mainDiag.close();
	mainDiag = null;
	on_Scene_Closed = null;
	cleanup = null;
	myTriggerFunction = null;
	TriggerTimer = null;
	GraphSceneTimer = null;
}

function on_Scene_Closed() 
{ 
	Log("on_Scene_Closed called");
	cleanup();
}

var triggercounter_max = 180;
var triggercounter = triggercounter_max; //total amount of triggers
var triggerrate = 60; //times per second
function myTriggerFunction() 
{ 
	triggercounter--;
	//Log("myTriggerFunction called");
	
	if (triggercounter==0)
	{
		cleanup();
	}
	else
	{
		GraphItem.setPos((triggercounter_max-triggercounter)*10,10);
		graphpixmap.setRotation(triggercounter_max-triggercounter);
		//graphpixmap.setPixmap("D:\\Projects\\StimulGL\\Source\\Plugins\\ExperimentManager\\qt-logo_turned.png")
		GraphView.update();
	}
}

var ExperimentManagerObj = new ExperimentManager();//is derived from QGraphicsScene
var rect = new QRectF(0, 0, 100, 100);
ExperimentManagerObj.addRect(rect);
var  pixmap = new QPixmap("D:\\Projects\\StimulGL\\Source\\Plugins\\ExperimentManager\\qt-logo.png");
var graphpixmap = QGraphicsPixmapItem;
graphpixmap = ExperimentManagerObj.addPixmap(pixmap);
//var str = new QString();
//str = "Example text";
var str = new String("Example text");
//var fnt = Qt.QFont
var fnt = new QFont("Times", 30, 5, true);//type, size, weight, italic?
ExperimentManagerObj.addText(str,fnt);
var GraphItem = new QGraphicsItem();
GraphItem = ExperimentManagerObj.itemAt(0, 0);

var GraphView = new QGraphicsView(ExperimentManagerObj);
//GraphView.setRenderHint(QPainter.Antialiasing | QPainter.SmoothPixmapTransform);
//GraphView.setCacheMode(QGraphicsView.CacheBackground);// to speed up difficult background refresh speed
//GraphView.setOptimizationFlags(QGraphicsView.DontSavePainterState | QGraphicsView.DontAdjustForAntialiasing);
//GraphView.setSceneRect(x,y,w,h);

ExperimentManagerObj.changeToOpenGLView(GraphView);

//var frmt = Qt.QGLFormat;
//frmt = new QGLFormat(Qt.QGL.SampleBuffers);//QGL::SampleBuffers
//var GLWidget = new QGLWidget(frmt);
//view.setViewport(GLWidget);

var col = new QColor(Qt.blue);
ExperimentManagerObj.setBackgroundColor(col);

ExperimentManagerObj.UserWantsToClose.connect(this, this.on_Scene_Closed);

TriggerTimer.timeout.connect(this, this.myTriggerFunction); 
TriggerTimer.start(1000/triggerrate);

//GraphSceneTimer.timeout.connect(this, ExperimentManagerObj.advance());
//GraphSceneTimer.start(1000 / 33);

GraphView.showFullScreen();
//GraphView.show();

//QList<QGraphicsItem*> items = scene->items();
//	for (int i = 0; i < items.size(); i++) {
//		scene->removeItem(items[i]);
//		delete items[i];
//	}

//mainDiag.scene = ExperimentManagerObj;
////self.scene.setSceneRect(0, 0, 500, 500)
//mainDiag.view = GraphView;
////self.view.setRenderHint(QPainter.Antialiasing)
//mainDiag.view.setScene(mainDiag.scene)
////self.view.setFocusPolicy(Qt.NoFocus)

//var mainDiag = new QDialog();
//var layout = new QVBoxLayout();
//layout.addWidget(GraphView,0,0);
////layout.setMargin(0);
////layout.setSpacing(0);
//mainDiag.setLayout(layout);
//mainDiag.setWindowState(mainDiag.windowState());// | Qt::WindowMaximized);
//mainDiag.showFullScreen();

//var WinMod = Qt.WindowModality(1);//=Qt::WindowModal
//mainDiag.setWindowModality(1);
//showMaximized
//mainDiag.exec();

Log("Ended main loop");

//view.setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
//view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
//view.setScene(new OpenGLScene);
//view.show();
//view.resize(1024, 768);
//return app.exec();


//function tr(s) { return s; }
//
//
//
//function RenderArea(parent)
//{
//    QWidget.call(this, parent);
//
//    this.shape = RenderArea.Polygon;
//    this.antialiased = false;
//    this.transformed = false;
//    this.pen = new QPen();
//    this.brush = new QBrush();
//    this.pixmap = new QPixmap("images/qt-logo.png");
//
//    this.setBackgroundRole(QPalette.Base);
//    this.autoFillBackground = true;
//}
//
//RenderArea.prototype = new QWidget();
//
//RenderArea.Line = 0;
//RenderArea.Points = 1;
//RenderArea.Polyline = 2;
//RenderArea.Polygon = 3;
//RenderArea.Rect = 4;
//RenderArea.RoundedRect = 5;
//RenderArea.Ellipse = 6;
//RenderArea.Arc = 7;
//RenderArea.Chord = 8;
//RenderArea.Pie = 9;
//RenderArea.Path = 10;
//RenderArea.Text = 11;
//RenderArea.Pixmap = 12;
//
//RenderArea.prototype.getMinimumSizeHint = function()
//{
//    return new QSize(100, 100);
//}
//
//RenderArea.prototype.getSizeHint = function()
//{
//    return new QSize(400, 200);
//}
//
//RenderArea.prototype.setShape = function(shape)
//{
//    this.shape = shape;
//    this.update();
//}
//
//RenderArea.prototype.setPen = function(pen)
//{
//    this.pen = pen;
//    this.update();
//}
//
//RenderArea.prototype.setBrush = function(brush)
//{
//    this.brush = brush;
//    this.update();
//}
//
//RenderArea.prototype.setAntialiased = function(antialiased)
//{
//    this.antialiased = antialiased;
//    this.update();
//}
//
//RenderArea.prototype.setTransformed = function(transformed)
//{
//    this.transformed = transformed;
//    this.update();
//}
//
//RenderArea.prototype.paintEvent = function(/* event */)
//{
//    if (RenderArea.points == undefined) {
//        RenderArea.points = new QPolygon();
//        RenderArea.points.append(new QPoint(10, 80));
//        RenderArea.points.append(new QPoint(20, 10));
//        RenderArea.points.append(new QPoint(80, 30));
//        RenderArea.points.append(new QPoint(90, 70));
//    }
//
//    var rect = new QRect(10, 20, 80, 60);
//
//    var path = new QPainterPath();
//    path.moveTo(20, 80);
//    path.lineTo(20, 30);
//    path.cubicTo(80, 0, 50, 50, 80, 80);
//
//    var startAngle = 20 * 16;
//    var arcLength = 120 * 16;
//
//    var painter = new QPainter();
//    painter.begin(this);
//    painter.setPen(this.pen);
//    painter.setBrush(this.brush);
//    if (this.antialiased) {
//        painter.setRenderHint(QPainter.Antialiasing, true);
//        painter.translate(+0.5, +0.5);
//    }
//
//    for (var x = 0; x < this.width; x += 100) {
//        for (var y = 0; y < this.height; y += 100) {
//            painter.save();
//            painter.translate(x, y);
//            if (this.transformed) {
//                painter.translate(50, 50);
//                painter.rotate(60.0);
//                painter.scale(0.6, 0.9);
//                painter.translate(-50, -50);
//            }
//
//            switch (this.shape) {
//            case RenderArea.Line:
//                painter.drawLine(rect.bottomLeft(), rect.topRight());
//                break;
//            case RenderArea.Points:
//                painter.drawPoints(RenderArea.points);
//                break;
//            case RenderArea.Polyline:
//                painter.drawPolyline(RenderArea.points);
//                break;
//            case RenderArea.Polygon:
//                painter.drawPolygon(RenderArea.points);
//                break;
//            case RenderArea.Rect:
//                painter.drawRect(rect);
//                break;
//            case RenderArea.RoundedRect:
//                painter.drawRoundedRect(rect, 25, 25, Qt.RelativeSize);
//                break;
//            case RenderArea.Ellipse:
//                painter.drawEllipse(rect);
//                break;
//            case RenderArea.Arc:
//                painter.drawArc(rect, startAngle, arcLength);
//                break;
//            case RenderArea.Chord:
//                painter.drawChord(rect, startAngle, arcLength);
//                break;
//            case RenderArea.Pie:
//                painter.drawPie(rect, startAngle, arcLength);
//                break;
//            case RenderArea.Path:
//                painter.drawPath(path);
//                break;
//            case RenderArea.Text:
                // ### overload issue
                //painter.drawText(rect, Qt.AlignCenter, tr("Qt by\nTrolltech"));
//                painter.drawText(rect.x, rect.y, rect.width, rect.height, Qt.AlignCenter, tr("Qt by\nTrolltech"), undefined);
//                break;
//            case RenderArea.Pixmap:
//                painter.drawPixmap(10, 10, this.pixmap);
//            }
//            painter.restore();
//        }
//    }
//
//    painter.setPen(this.palette.dark().color());
//    painter.setBrush(Qt.NoBrush);
//    painter.drawRect(0, 0, this.width - 1, this.height - 1);
//    painter.end();
//}
//
//
//
//function Window(parent)
//{
//    QWidget.call(this, parent);
//
//    this.renderArea = new RenderArea();
//
//    this.shapeComboBox = new QComboBox();
//    var icon = new QIcon(); // ### working around addItem() overload issue
//    this.shapeComboBox.addItem(icon, tr("Polygon"), RenderArea.Polygon);
//    this.shapeComboBox.addItem(icon, tr("Rectangle"), RenderArea.Rect);
//    this.shapeComboBox.addItem(icon, tr("Rounded Rectangle"), RenderArea.RoundedRect);
//    this.shapeComboBox.addItem(icon, tr("Ellipse"), RenderArea.Ellipse);
//    this.shapeComboBox.addItem(icon, tr("Pie"), RenderArea.Pie);
//    this.shapeComboBox.addItem(icon, tr("Chord"), RenderArea.Chord);
//    this.shapeComboBox.addItem(icon, tr("Path"), RenderArea.Path);
//    this.shapeComboBox.addItem(icon, tr("Line"), RenderArea.Line);
//    this.shapeComboBox.addItem(icon, tr("Polyline"), RenderArea.Polyline);
//    this.shapeComboBox.addItem(icon, tr("Arc"), RenderArea.Arc);
//    this.shapeComboBox.addItem(icon, tr("Points"), RenderArea.Points);
//    this.shapeComboBox.addItem(icon, tr("Text"), RenderArea.Text);
//    this.shapeComboBox.addItem(icon, tr("Pixmap"), RenderArea.Pixmap);
//
//    this.shapeLabel = new QLabel(tr("&Shape:"));
//    this.shapeLabel.setBuddy(this.shapeComboBox);
//
//    this.penWidthSpinBox = new QSpinBox();
//    this.penWidthSpinBox.setRange(0, 20);
//    this.penWidthSpinBox.specialValueText = tr("0 (cosmetic pen)");
//
//    this.penWidthLabel = new QLabel(tr("Pen &Width:"));
//    this.penWidthLabel.setBuddy(this.penWidthSpinBox);
//
//    this.penStyleComboBox = new QComboBox();
//    this.penStyleComboBox.addItem(tr("Solid"), Qt.SolidLine);
//    this.penStyleComboBox.addItem(tr("Dash"), Qt.DashLine);
//    this.penStyleComboBox.addItem(tr("Dot"), Qt.DotLine);
//    this.penStyleComboBox.addItem(tr("Dash Dot"), Qt.DashDotLine);
//    this.penStyleComboBox.addItem(tr("Dash Dot Dot"), Qt.DashDotDotLine);
//    this.penStyleComboBox.addItem(tr("None"), Qt.NoPen);
//
//    this.penStyleLabel = new QLabel(tr("&Pen Style:"));
//    this.penStyleLabel.setBuddy(this.penStyleComboBox);
//
//    this.penCapComboBox = new QComboBox();
//    this.penCapComboBox.addItem(tr("Flat"), Qt.FlatCap);
//    this.penCapComboBox.addItem(tr("Square"), Qt.SquareCap);
//    this.penCapComboBox.addItem(tr("Round"), Qt.RoundCap);
//
//    this.penCapLabel = new QLabel(tr("Pen &Cap:"));
//    this.penCapLabel.setBuddy(this.penCapComboBox);
//
//    this.penJoinComboBox = new QComboBox();
//    this.penJoinComboBox.addItem(tr("Miter"), Qt.MiterJoin);
//    this.penJoinComboBox.addItem(tr("Bevel"), Qt.BevelJoin);
//    this.penJoinComboBox.addItem(tr("Round"), Qt.RoundJoin);
//
//    this.penJoinLabel = new QLabel(tr("Pen &Join:"));
//    this.penJoinLabel.setBuddy(this.penJoinComboBox);
//
//    this.brushStyleComboBox = new QComboBox();
//    this.brushStyleComboBox.addItem(tr("Linear Gradient"),
//            Qt.LinearGradientPattern);
//    this.brushStyleComboBox.addItem(tr("Radial Gradient"),
//            Qt.RadialGradientPattern);
//    this.brushStyleComboBox.addItem(tr("Conical Gradient"),
//            Qt.ConicalGradientPattern);
//    this.brushStyleComboBox.addItem(tr("Texture"), Qt.TexturePattern);
//    this.brushStyleComboBox.addItem(tr("Solid"), Qt.SolidPattern);
//    this.brushStyleComboBox.addItem(tr("Horizontal"), Qt.HorPattern);
//    this.brushStyleComboBox.addItem(tr("Vertical"), Qt.VerPattern);
//    this.brushStyleComboBox.addItem(tr("Cross"), Qt.CrossPattern);
//    this.brushStyleComboBox.addItem(tr("Backward Diagonal"), Qt.BDiagPattern);
//    this.brushStyleComboBox.addItem(tr("Forward Diagonal"), Qt.FDiagPattern);
//    this.brushStyleComboBox.addItem(tr("Diagonal Cross"), Qt.DiagCrossPattern);
//    this.brushStyleComboBox.addItem(tr("Dense 1"), Qt.Dense1Pattern);
//    this.brushStyleComboBox.addItem(tr("Dense 2"), Qt.Dense2Pattern);
//    this.brushStyleComboBox.addItem(tr("Dense 3"), Qt.Dense3Pattern);
//    this.brushStyleComboBox.addItem(tr("Dense 4"), Qt.Dense4Pattern);
//    this.brushStyleComboBox.addItem(tr("Dense 5"), Qt.Dense5Pattern);
//    this.brushStyleComboBox.addItem(tr("Dense 6"), Qt.Dense6Pattern);
//    this.brushStyleComboBox.addItem(tr("Dense 7"), Qt.Dense7Pattern);
//    this.brushStyleComboBox.addItem(tr("None"), Qt.NoBrush);
//
//    this.brushStyleLabel = new QLabel(tr("&Brush Style:"));
//    this.brushStyleLabel.setBuddy(this.brushStyleComboBox);
//
//    this.otherOptionsLabel = new QLabel(tr("Other Options:"));
//    this.antialiasingCheckBox = new QCheckBox(tr("&Antialiasing"));
//    this.transformationsCheckBox = new QCheckBox(tr("&Transformations"));
//
//    this.shapeComboBox["activated(int)"].connect(
//            this, this.shapeChanged);
//    this.penWidthSpinBox["valueChanged(int)"].connect(
//            this, this.penChanged);
//    this.penStyleComboBox["activated(int)"].connect(
//            this, this.penChanged);
//    this.penCapComboBox["activated(int)"].connect(
//            this, this.penChanged);
//    this.penJoinComboBox["activated(int)"].connect(
//            this, this.penChanged);
//    this.brushStyleComboBox["activated(int)"].connect(
//            this, this.brushChanged);
//    this.antialiasingCheckBox["toggled(bool)"].connect(
//            this.renderArea, this.renderArea.setAntialiased);
//    this.transformationsCheckBox["toggled(bool)"].connect(
//            this.renderArea, this.renderArea.setTransformed);
//
//    var mainLayout = new QGridLayout();
//    mainLayout.setColumnStretch(0, 1);
//    mainLayout.setColumnStretch(3, 1);
//    mainLayout.addWidget(this.renderArea, 0, 0, 1, 4);
//    mainLayout.setRowMinimumHeight(1, 6);
//    mainLayout.addWidget(this.shapeLabel, 2, 1, Qt.AlignRight);
//    mainLayout.addWidget(this.shapeComboBox, 2, 2);
//    mainLayout.addWidget(this.penWidthLabel, 3, 1, Qt.AlignRight);
//    mainLayout.addWidget(this.penWidthSpinBox, 3, 2);
//    mainLayout.addWidget(this.penStyleLabel, 4, 1, Qt.AlignRight);
//    mainLayout.addWidget(this.penStyleComboBox, 4, 2);
//    mainLayout.addWidget(this.penCapLabel, 5, 1, Qt.AlignRight);
//    mainLayout.addWidget(this.penCapComboBox, 5, 2);
//    mainLayout.addWidget(this.penJoinLabel, 6, 1, Qt.AlignRight);
//    mainLayout.addWidget(this.penJoinComboBox, 6, 2);
//    mainLayout.addWidget(this.brushStyleLabel, 7, 1, Qt.AlignRight);
//    mainLayout.addWidget(this.brushStyleComboBox, 7, 2);
//    mainLayout.setRowMinimumHeight(8, 6);
//    mainLayout.addWidget(this.otherOptionsLabel, 9, 1, Qt.AlignRight);
//    mainLayout.addWidget(this.antialiasingCheckBox, 9, 2);
//    mainLayout.addWidget(this.transformationsCheckBox, 10, 2);
//    this.setLayout(mainLayout);
//
//    this.shapeChanged();
//    this.penChanged();
//    this.brushChanged();
//    this.antialiasingCheckBox.checked = true;
//
//    this.windowTitle = tr("Basic Drawing");
//}
//
//Window.prototype = new QWidget();
//
//Window.prototype.shapeChanged = function()
//{
//    var shape = this.shapeComboBox.itemData(this.shapeComboBox.currentIndex);
//    this.renderArea.setShape(shape);
//}
//
//Window.prototype.penChanged = function()
//{
//    var width = this.penWidthSpinBox.value;
//    var style = this.penStyleComboBox.itemData(
//            this.penStyleComboBox.currentIndex);
//    var cap = this.penCapComboBox.itemData(
//            this.penCapComboBox.currentIndex);
//    var join = this.penJoinComboBox.itemData(
//            this.penJoinComboBox.currentIndex);
//
//    this.renderArea.setPen(new QPen(new QColor(Qt.blue), width, style, cap, join));
//}
//
//Window.prototype.brushChanged = function()
//{
//    var style = Qt.BrushStyle(this.brushStyleComboBox.itemData(
//            this.brushStyleComboBox.currentIndex));
//
//    if (style == Qt.LinearGradientPattern) {
//        var linearGradient = new QLinearGradient(0, 0, 100, 100);
        // ### working around issue with Qt.GlobalColor
//        linearGradient.setColorAt(0.0, new QColor(Qt.white));
//        linearGradient.setColorAt(0.2, new QColor(Qt.green));
//        linearGradient.setColorAt(1.0, new QColor(Qt.black));
//        this.renderArea.setBrush(new QBrush(linearGradient));
//    } else if (style == Qt.RadialGradientPattern) {
//        var radialGradient = new QRadialGradient(50, 50, 50, 70, 70);
//        radialGradient.setColorAt(0.0, new QColor(Qt.white));
//        radialGradient.setColorAt(0.2, new QColor(Qt.green));
//        radialGradient.setColorAt(1.0, new QColor(Qt.black));
//        this.renderArea.setBrush(new QBrush(radialGradient));
//    } else if (style == Qt.ConicalGradientPattern) {
//        var conicalGradient = new QConicalGradient(50, 50, 150);
//        conicalGradient.setColorAt(0.0, new QColor(Qt.white));
//        conicalGradient.setColorAt(0.2, new QColor(Qt.green));
//        conicalGradient.setColorAt(1.0, new QColor(Qt.black));
//        this.renderArea.setBrush(new QBrush(conicalGradient));
//    } else if (style == Qt.TexturePattern) {
//        this.renderArea.setBrush(new QBrush(new QPixmap("images/brick.png")));
//    } else {
//        this.renderArea.setBrush(new QBrush(new QColor(Qt.green), style));
//    }
//}
//
//var win = new Window();
//win.show();
//QCoreApplication.exec();
//
//
//
//Construct a new Plugin object
//ExampleExtPluginObj = new ExperimentManager();
//
//Example usage of the Example property
//ExampleExtPluginObj.setExampleProperty(99);
//Log(ExampleExtPluginObj.ExampleProperty);
//ExampleExtPluginObj.ExampleProperty = 88;
//Log(ExampleExtPluginObj.ExampleProperty);
//
//There are some global standard Script Functions which can be used like Log(), Beep(), ThrowError(), Include(), Pause(), ExitScript() and StimQT()
//Beep();
//
//Almost all ECMA(Java)-script compatible code can be used here, see below String
//var txt = new String("ECMA string");
//
//Almost all QT SDK libraries can be used trough automatically loaded script bindings
//We can even tell the main program to open an QT script binding example
//var strPath = new String(StimQT.getSelectedScriptFileLocation());
//strPath = strPath + "/" + "StandardDialogs.qs"
//Log(strPath);
//StimQT.openFiles(strPath);
//StimQT.executeScript();
//Log(StimQT.getSelectedScriptFileName());
//StimQT.closeSelectedScriptFile();
//
//Define a custom function
//function myBeeps(n) 
//{ 
//	for(var i=0 ;i<n;i++)
//	{
//		Beep();
//		txt = txt + i;
//		Log(txt);
//	}
//} 
//Call the newly created function
//myBeeps(5);
//Make sure to cleanup your functions before exiting!
//myBeeps = null;
//
//Signal/Slot functionality
//var timer = new QTimer(this);
//var nCounter = 0;
//function cleanup() //First we define a function for the necessary cleanup before exiting the script
//{
//	timer.stop(); //Stop the timer
//	timer.timeout.disconnect(this, this.myFunction1); //Disconnect again
//	myFunction1 = null; //cleanup the function
//	ExampleExtPluginObj.ExampleSignalTriggered.disconnect(this, this.myFunction2); //Disconnect again
//	myFunction2 = null; //Cleanup the function
//	cleanup = null; //Cleanup the function
//}
//function myFunction1() //The first function demonstrates how a QTimer constructed from within the script can be connected to a custom defined script function
//{ 
//	nCounter++;
//	Log("myFunction1() has been called " + nCounter + " times" ) 
//	ExampleExtPluginObj.setExampleProperty(nCounter);
//	if (nCounter==5)
//	{
//		cleanup();
//	}
//}
//function myFunction2() //The second function demonstrates how a Plugin constructed from within the script can be connected to a custom defined script function
//{ 
//	Log("myFunction2() has been called" ) 
//}
//function PauseMills(millis)
//{
//	var date = new Date();
//	var curDate = null;
//     
//	do { curDate = new Date(); }
//	while(curDate-date < millis)
//}
//
//timer.timeout.connect(this, this.myFunction1); //The same as: connect(timer, SIGNAL(timeout()), this, SLOT(myFunction()));
//ExampleExtPluginObj.ExampleSignalTriggered.connect(this, this.myFunction2); //Connect a Signal from the Plugin object dynamically to a custom script Function
//timer.start(1000);
//PauseMills(8000);//This function doesn't block the Timer thread, but has a high CPU load, it's better to use Timers/Signal/Slots
//Pause(5000);//This function does block the remaining!
//PauseMills = null;
//Log("Ready");
//ThrowError("Fake Error");//Interrupts the script at this line and opens the script parser
//
//
//The ExampleExtPluginObj object gets automatically destructed
