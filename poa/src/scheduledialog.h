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
 * $Id: scheduledialog.h,v 1.16 2004/01/18 17:20:02 vanto Exp $
 *
 *****************************************************************************/

#ifndef POA_SCHEDULEDIALOG_H
#define POA_SCHEDULEDIALOG_H

#include <qdialog.h>
#include <qlistview.h>
#include <qptrlist.h>
#include <qvariant.h>
#include <qtable.h>
#include <qcanvas.h>
#include <qwmatrix.h>
#include <qspinbox.h>

#include "blockgraph.h"
#include "blocktree.h"
#include "blockmodel.h"
#include "pinmodel.h"
#include "project.h"

class QBoxLayout;
class QPushButton;
class QTable;
class QSlider;
class QSplitter;

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
    QSplitter *splitterWidget;
    QWidget *topWidget;
    QWidget *middleWidget;
    QWidget *bottomWidget;
    QCanvasRectangle *highlightCanvasRectangle;

    QTable *timingTable;

    QCanvas *canvas;
    QCanvasView *canvasView;

    QCanvas *labelCanvas;
    QCanvasView *labelCanvasView;

    QPushButton *applyPushButton;
    QPushButton *helpPushButton;
    QPushButton *cancelPushButton;
    QPushButton *okPushButton;

    QSlider *zoomSlider;
    double zoom_;

    double pixPerNs_;

    Project *project_;
    bool modified_;

    QPtrList<BlockTree> inputBlocks;
    QPtrList<BlockTree> blocks_;
    QMap<BlockModel*, BlockTree*> blocksToTree_;

    QPtrList<BlockNode> graph_;

    void buildBranch(BlockTree *bt);
    void buildTree();
    void fillTimingTable(BlockTree* bt);
    void drawTimings(BlockTree* bt);

    /**
     * Draws the ruler.
     */
    void drawRuler();

    /**
     * Calculates the position of the given block at the given time
     */
    QRect calcBlockPosition(BlockTree *bt, int time);

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

    void initCanvas();

    /**
     * Updates model_'s content by using current dialog's content.
     */
    void updateModel();

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
     * A row has been swapped.
     */
    void rowMoved(int section, int fromIndex, int toIndex);
};

/**
 * This class provides a spin box editor for a BlockTree object in a
 * QTable.
 */
class SpinBoxItem : public QTableItem
{

 public:
    /**
     * Type of the blocktree field.
     */
    enum BTField {RUNTIME, CLOCK, OFFSET};
    SpinBoxItem(QTable *t, EditType et, BlockTree *bt, BTField field);

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
    BlockTree *blocktree_;
    BTField field_;

    /**
     * Sets the value to the corresponding property in the blocktree
     * object.
     */
    void setValue(int value);
    /**
     * Returns a value from the blocktree object, depending on the field.
     */
    int value() const;

};

/**
 * This class provides a arrowheaded line.
 */
class ArrowLine : public QCanvasLine
{

 public:
    ArrowLine(QCanvas *canvas);
    virtual void drawShape(QPainter &p);

 private:
    static const double RAD2DEG;
    double computeAngle(int sx, int sy, int ex, int ey);

};

#endif // POA_SCHEDULEDIALOG_H
