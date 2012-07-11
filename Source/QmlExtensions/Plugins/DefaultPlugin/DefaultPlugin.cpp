//DefaultQMLPlugin
//Copyright (C) 2012  Sven Gijsen
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

#include <QtDeclarative/QDeclarativeExtensionPlugin>
#include <QtDeclarative/qdeclarative.h>
#include <qdebug.h>
#include <qdatetime.h>
#include <qbasictimer.h>
#include <qapplication.h>

// Implements a "TimeModel" class with hour and minute properties
// that change on-the-minute yet efficiently sleep the rest
// of the time.

class MinuteTimer : public QObject
{
    Q_OBJECT
public:
    MinuteTimer(QObject *parent) : QObject(parent)
    {
    }

    void start()
    {
        if (!timer.isActive()) {
            time = QTime::currentTime();
            timer.start(60000-time.second()*1000, this);
        }
    }

    void stop()
    {
        timer.stop();
    }

    int hour() const { return time.hour(); }
    int minute() const { return time.minute(); }

signals:
    void timeChanged();

protected:
    void timerEvent(QTimerEvent *)
    {
        QTime now = QTime::currentTime();
        if (now.second() == 59 && now.minute() == time.minute() && now.hour() == time.hour()) {
            // just missed time tick over, force it, wait extra 0.5 seconds
            time.addSecs(60);
            timer.start(60500, this);
        } else {
            time = now;
            timer.start(60000-time.second()*1000, this);
        }
        emit timeChanged();
    }

private:
    QTime time;
    QBasicTimer timer;
};

class TimeModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int hour READ hour NOTIFY timeChanged)
    Q_PROPERTY(int minute READ minute NOTIFY timeChanged)

public:
    TimeModel(QObject *parent=0) : QObject(parent)
    {
        if (++instances == 1) {
            if (!timer)
                timer = new MinuteTimer(qApp);
            connect(timer, SIGNAL(timeChanged()), this, SIGNAL(timeChanged()));
            timer->start();
        }
    }

    ~TimeModel()
    {
        if (--instances == 0) {
            timer->stop();
        }
    }

    int minute() const { return timer->minute(); }
    int hour() const { return timer->hour(); }

signals:
    void timeChanged();

private:
    QTime t;
    static MinuteTimer *timer;
    static int instances;
};

int TimeModel::instances=0;
MinuteTimer *TimeModel::timer=0;

class QExampleQmlPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(uri == QLatin1String("TimeExample"));

        qmlRegisterType<TimeModel>(uri, 1, 0, "Time");
    }
};

#include "DefaultPlugin.moc"

//![export]
Q_EXPORT_PLUGIN2(qmlqtimeexampleplugin, QExampleQmlPlugin);
//![export]
