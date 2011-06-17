/****************************************************************************
****************************************************************************/

function tr(s) { return s; }


function DigitalClock(parent) 
{ 
    QLCDNumber.call(this, parent);

    this.segmentStyle = QLCDNumber.Filled;

    var timer = new QTimer(this);
    timer.timeout.connect(this, this.showTime);
    timer.start(1000);

    this.showTime();

    this.windowTitle = tr("Digital Clock");
    this.resize(150, 60);
}

DigitalClock.prototype = new QLCDNumber();

DigitalClock.prototype.showTime = function()
{
    var time = QTime.currentTime();
    var format = "hh";
    format += ((time.second() % 2) == 0) ? " " : ":";
    format += "mm";
    var text = time.toString(format);
    this.display(text);
}


var clock = new DigitalClock(5); // ### fixme
clock.show();
QCoreApplication.exec();
