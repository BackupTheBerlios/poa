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
 * $Id: scheduledialog.h,v 1.4 2004/01/09 21:40:38 squig Exp $
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
    QWMatrix zoomMatrix;

    Project *project_;
    QPtrList<BlockTree> inputBlocks;

    void buildBranch(BlockTree *bt);
    void buildTree();
    void fillTimingTable(BlockTree* bt);
    bool drawTimings(BlockTree* bt, int* Y, int* time);

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
     * Synchronizes dialog with model_'s content.
     */
//    void syncModel();

    /**
     * Updates model_'s content by using current dialog's content.
     */
//    void updateModel();

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

    void updateHighlighter(int row, int column);

    /**
     * Changes the zooming
     */
    void zoomChanged(int);
};


class SpinBoxItem : public QTableItem
{
public:
    enum BTField {RUNTIME, CLOCK, OFFSET};
    SpinBoxItem(QTable *t, EditType et, BlockTree *bt, BTField field);
    QWidget *createEditor() const;
    void setContentFromEditor( QWidget *w );

private:
    QSpinBox *spinbox_;
    BlockTree *blocktree_;
    BTField field_;

    void setValue(int value);
    int value() const;
};

#endif // POA_SCHEDULEDIALOG_H
