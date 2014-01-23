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

#ifndef QGVSCENE_H
#define QGVSCENE_H

#include "QGVCore.h"
#include <QGraphicsScene>
#include "QGVNode.h"
#include "QGVEdge.h"
#include "QGVSubGraph.h"

/**
 * @brief GraphViz interactive scene
 *
 */
class QGVScene : public QGraphicsScene
{
    Q_OBJECT

signals:
	void nodeContextMenu(QGVNode* node);
	void nodeDoubleClick(QGVNode* node);

	void edgeContextMenu(QGVEdge* edge);
	void edgeDoubleClick(QGVEdge* edge);

	void subGraphContextMenu(QGVSubGraph* graph);
	void subGraphDoubleClick(QGVSubGraph* graph);

	void graphContextMenuEvent();

public:

    explicit QGVScene(const QString &name, QObject *parent = 0);
    ~QGVScene();

    void setGraphAttribute(const QString &name, const QString &value);
    void setNodeAttribute(const QString &name, const QString &value);
    void setEdgeAttribute(const QString &name, const QString &value);

    QGVNode* addNode(const QString& label);
    QGVEdge* addEdge(QGVNode* source, QGVNode* target, const QString& label=QString());
	//QGVNode* getNode(const QString& label);
    QGVSubGraph* addSubGraph(const QString& name, bool cluster=true);

    void setRootNode(QGVNode *node);

    void loadLayout(const QString &text);
	bool saveLayout(const QString &sFilename, const int &nType);
    void applyLayout();
    void clear();
	    
public slots:

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * contextMenuEvent);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void drawBackground(QPainter * painter, const QRectF & rect);
private:
    friend class QGVNode;
    friend class QGVEdge;
    friend class QGVSubGraph;

    GVC_t *_context;
    Agraph_t *_graph;
    //QFont _font;

    QList<QGVNode*> _nodes;
    QList<QGVEdge*> _edges;
    QList<QGVSubGraph*> _subGraphs;
};

#endif // QGVSCENE_H
