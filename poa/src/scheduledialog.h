/*****************************************************************************
 * POA - Programmierumgebung Offener Antrieb
 *
 * Copyright (C) 2003 Necati Aydin, Anselm Garbe, Stefan Hauser,
 * Steffen Keul, Marcel Kilgus, Steffen Pingel, Tammo van Lessen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * $Id: scheduledialog.h,v 1.23 2004/01/22 13:11:10 vanto Exp $
 *
 *****************************************************************************/

#ifndef POA_SCHEDULEDIALOG_H
#define POA_SCHEDULEDIALOG_H

#include <qcanvas.h>
#include <qdialog.h>
#include <qevent.h>
#include <qlistview.h>
#include <qptrlist.h>
#include <qspinbox.h>
#include <qtable.h>
#include <qvariant.h>
#include <qwmatrix.h>

#include "blockgraph.h"
#include "blockmodel.h"
#include "pinmodel.h"
#include "project.h"

class QBoxLayout;
class QPushButton;
class QTable;
class QSlider;
class QSplitter;

/**
 * Scheduling Dialog.
 * Shows a table with all block properties and the scheduling graph
 */
class ScheduleDialog : public QDialog
{
    Q_OBJECT

public:
    ScheduleDialog(Project* Pro, QWidget* parent = 0, const char* name = 0,
                   bool modal = FALSE, WFlags fl = 0);
    ~ScheduleDialog();

private:
    QBoxLayout *dialogLayout;
    QBoxLayout *topLayout;
    QBoxLayout *middleLayout;
    QBoxLayout *rightLayout_;
    QSplitter *splitterWidget;
    QWidget *topWidget;
    QWidget *middleWidget;
    QWidget *bottomWidget;
    QWidget *rightWidget_;
    QCanvasRectangle *highlightCanvasRectangle;

    QTable *timingTable;

    QCanvas *canvas;
    QCanvasView *canvasView;

    QCanvas *labelCanvas;
    QCanvasView *labelCanvasView;

    QSlider *zoomSlider;
    double zoom_;

    double pixPerNs_;

    Project *project_;
    bool modified_;

    BlockGraph *graph_;
    QValueList<BlockNode*> blocks_;

    /**
     * Fills the table with timing information.
     */
    void fillTimingTable(BlockNode* node);

    /**
     * Draws one row in the graph.
     */
    void drawTimings(BlockNode* node);

    /**
     * Draws the ruler.
     */
    void drawRuler();

    /**
     * Calculates the position of the given block at the given time.
     */
    QRect calcBlockPosition(BlockNode *node, int time);

    /**
     * Clears all graph widget canvases.
     */
    void clearCanvas();

    /**
     * Initializes layout.
     */
    void initLayout();

    /**
     * Initializes block timing table.
     */
    void initTimingWidget();

    /**
     * Initializes graph widget.
     */
    void initGraphWidget();

    /**
     * Initializes bottom widget.
     */
    void initBottomWidget();

    /**
     * Initializes the graph canvas and calculates the dynamic stretching.
     */
    void initCanvas();

    /**
     * Updates model_'s content by using current dialog's content.
     */
    void updateModel();

    /**
     * Swaps two rows and updates the table as well as the graph.
     */
    void swapRows(int index1, int index2);

private slots:

    /**
     * Cancels all changes, deletes all cloned data and closes
     * this dialog.
     */
    void cancel();

    /**
     * Applies all changes to the model.
     */
    void apply();

    /**
     * Applies all changes, deletes all cloned data and closes
     * this dialog.
     */
    void ok();

    /**
     * Highlightes the same row in the graph widget as in the table.
     */
    void updateHighlighter(int row, int column);

    /**
     * Changes the zooming level.
     */
    void zoomChanged(int);

    /**
     * Some values in the table has been changed.
     */
    void modelChanged(int row, int col);

    /**
     * Move selected row up.
     */
    void moveRowUp();

    /**
     * Move selected row down.
     */
    void moveRowDown();
};

/**
 * Provides a spin box editor for a BlockNode object in a
 * QTable.
 */
class SpinBoxItem : public QTableItem
{

 public:

    /**
     * Type of the blocknode field.
     */
    enum NodeField {RUNTIME, CLOCK, OFFSET};
    SpinBoxItem(QTable *t, EditType et, BlockNode *node, NodeField field);

    /**
     * Creates this editor on demand.
     */
    QWidget *createEditor() const;

    /**
     * Updates the model if the editor gets closed.
     */
    void setContentFromEditor( QWidget *w );

 private:
    QSpinBox *spinbox_;
    BlockNode *node_;
    NodeField field_;

    /**
     * Sets the value to the corresponding property in the blocknode
     * object.
     */
    void setValue(int value);

    /**
     * Returns a value from the blocknode object, depending on the field.
     */
    QString value() const;

};

/**
 * Provides an arrow-headed line.
 */
class ArrowLine : public QCanvasLine
{

 public:
    ArrowLine(QCanvas *canvas);
    virtual void drawShape(QPainter &p);

 private:
    double computeAngle(int sx, int sy, int ex, int ey);

};

/**
 * Provides a customized QCanvasView.
 * Resizes the Canvas to fit into the view if canvas content is smaller
 *.
 */
class FitCanvasView : public QCanvasView
{

 public:
    FitCanvasView(QCanvas *canvas, QWidget *parent, int minHeight);
    virtual void resizeEvent(QResizeEvent *e);

 private:
    int height_;

};



#endif // POA_SCHEDULEDIALOG_H
