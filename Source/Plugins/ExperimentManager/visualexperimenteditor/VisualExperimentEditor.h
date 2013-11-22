//ExperimentManagerplugin
//Copyright (C) 2013  Sven Gijsen
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

#ifndef VISUALEXPERIMENTEDITOR_H
#define VISUALEXPERIMENTEDITOR_H

#include <QWindow>
#include <QWidget>
#include <QToolBar>
#include <QMenuBar>
#include "ExperimentStructures.h"
#include "DiagramItem.h"

class DiagramScene;

QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QToolButton;
class QAbstractButton;
class ExperimentGraphConnection;
QT_END_NAMESPACE

class VisualExperimentEditor : public QWidget
{
	Q_OBJECT

signals:
	void destroyed(QWidget*);

public:
	VisualExperimentEditor(QWidget *parent = NULL);
	~VisualExperimentEditor();

	bool parseExperimentStructure(cExperimentStructure *ExpStruct);
	bool eventFilter(QObject *, QEvent *);
	void load(QDataStream &ds);
	void save(QDataStream &ds);

	private slots:
		void backgroundButtonGroupClicked(QAbstractButton *button);
		void buttonGroupClicked(int id);
		void deleteItem();
		void pointerGroupClicked(int id);
		void bringToFront();
		void sendToBack();
		void itemInserted(DiagramItem *item);
		void textInserted(QGraphicsTextItem *item);
		void currentFontChanged(const QFont &font);
		void fontSizeChanged(const QString &size);
		void sceneScaleChanged(const QString &scale);
		void textColorChanged();
		void itemColorChanged();
		void lineColorChanged();
		void textButtonTriggered();
		void fillButtonTriggered();
		void lineButtonTriggered();
		void handleFontChange();
		void itemSelected(QGraphicsItem *item);
		void about();

private:
	void createToolBox();
	void createActions();
	void createMenus();
	void createToolbars();
	QWidget *createBackgroundCellWidget(const QString &text, const QString &image);
	QWidget *createCellWidget(const QString &text, DiagramItem::DiagramType type);
	QMenu *createColorMenu(const char *slot, QColor defaultColor);
	QIcon createColorToolButtonIcon(const QString &image, QColor color);
	QIcon createColorIcon(QColor color);

	QGraphicsItem *itemAt(const QPointF &pos);
	bool createConnection(QGraphicsItem *from, QGraphicsItem *to, int nRepeats = 1);

	ExperimentGraphConnection *conn;
	bool bAllowSelfRecurrentConnection;

	DiagramScene *gScene;
	QGraphicsView *gView;

	QAction *exitAction;
	QAction *addAction;
	QAction *deleteAction;

	QAction *toFrontAction;
	QAction *sendBackAction;
	QAction *aboutAction;

	QMenuBar *mainMenuBar;
	QToolBar *mainToolBar;

	QMenu *fileMenu;
	QMenu *itemMenu;
	QMenu *aboutMenu;

	QToolBar *textToolBar;
	QToolBar *editToolBar;
	QToolBar *colorToolBar;
	QToolBar *pointerToolbar;

	QComboBox *sceneScaleCombo;
	QComboBox *itemColorCombo;
	QComboBox *textColorCombo;
	QComboBox *fontSizeCombo;
	QFontComboBox *fontCombo;

	QToolBox *toolBox;
	QButtonGroup *buttonGroup;
	QButtonGroup *pointerTypeGroup;
	QButtonGroup *backgroundButtonGroup;
	QToolButton *fontColorToolButton;
	QToolButton *fillColorToolButton;
	QToolButton *lineColorToolButton;
	QAction *boldAction;
	QAction *underlineAction;
	QAction *italicAction;
	QAction *textAction;
	QAction *fillAction;
	QAction *lineAction;
};

#endif // VISUALEXPERIMENTEDITOR_H
