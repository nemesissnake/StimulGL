//ExperimentManagerplugin
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

#ifndef QUICK2CONTAINERWIDGET_H
#define QUICK2CONTAINERWIDGET_H

#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <QQuickView>
#include <QScopedPointer>
#include <QWidget>

class Quick2ContainerWidget : public QWidget
{
  Q_OBJECT

public:
  Quick2ContainerWidget(QWidget* = 0);

  Quick2ContainerWidget(QQuickWindow*, QWidget* = 0);

  QQuickWindow* window() const;
  void setWindow(QQuickWindow*);

protected:
  void init();

  bool event(QEvent*);

  bool eventFilter(QObject*, QEvent*);

private:
  void paintEvent(QPaintEvent*);

private:
  QQuickWindow* window_;

  QScopedPointer<QOpenGLFramebufferObject> fbo_;

private slots:
  void afterRendering();
  void beforeRendering();
};

//////////////////////////////////////////////////////////////////////////////
inline QQuickWindow* Quick2ContainerWidget::window() const
{
  return window_;
}

//////////////////////////////////////////////////////////////////////////////
inline void Quick2ContainerWidget::paintEvent(QPaintEvent*)
{
}

#endif // QUICK2CONTAINERWIDGET_H
