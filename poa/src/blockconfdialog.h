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
 * $Id: blockconfdialog.h,v 1.11 2003/09/17 15:38:52 garbeam Exp $
 *
 *****************************************************************************/

#ifndef BLOCKCONFDIALOG_H
#define BLOCKCONFDIALOG_H

#include "blockmodel.h"
#include "pinmodel.h"
#include "pinvector.h"

#include <qlistview.h>
#include <qvariant.h>
#include <qdialog.h>
class QBoxLayout;
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QGroupBox;
class QLabel;
class QLineEdit;
class QListView;
class QPushButton;
class QRadioButton;
class QSpinBox;

/**
 * Provides the I/O view items.
 */
class PinListViewItem : public QListViewItem
{
public:

    /**
     * Creates a IO list view item for the given abstract model
     */
    PinListViewItem(QListView *parent, QListViewItem *after = 0,
            PinModel::PinType type = PinModel::INPUT);

    /**
     * Creates a IO list view item for the given abstract model
     */
    PinListViewItem(QListViewItem *parent, PinModel *clone = 0,
            PinModel *origin = 0);

    /**
     * Default destructor
     */
    ~PinListViewItem();

    /**
     * Returns the PinModel, represented by this view item
     */
    PinModel *data() const;

    /**
     * Returns the origin PinModel, represented by this view item
     */
    PinModel *origData() const;

    /**
     * Returns type of this view item (used for items without
     * models like root items.
     */
    PinModel::PinType type();

    /**
     * Returns <code>true</code>, if this is a root item,
     * <code>false</code> otherwise.
     */
    bool isRoot();

    /**
     * Updates the view with current contents of the pin model.
     */
    void update();

private:
    PinModel *clone_;
    PinModel *origin_;
    PinModel::PinType type_;
    bool root_;
};


class BlockConfDialog : public QDialog
{ 
    Q_OBJECT

public:
    BlockConfDialog(BlockModel *model, QWidget* parent = 0,
                    const char* name = 0, bool modal = FALSE, WFlags fl = 0);
    ~BlockConfDialog();

    QListView *ioListView;

    QRadioButton *offsetAutoCalcRadioButton;
    QRadioButton *offsetRadioButton;
    QRadioButton *runtimeAutoCalcRadioButton;
    QRadioButton *runtimeRadioButton;

    QSpinBox *blockClockSpinBox;
    QSpinBox *offsetSpinBox;
    QSpinBox *runtimeSpinBox;
    QSpinBox *cpuIdSpinBox;

    QLineEdit *blockNameLineEdit;
    QLineEdit *blockDescrLineEdit;
    QLineEdit *ioNumberLineEdit;
    QLineEdit *ioNameLineEdit;
    QLineEdit *addressLineEdit;
    QLineEdit *bitsLineEdit;

private:
    QBoxLayout *dialogLayout;
    QBoxLayout *topLayout;
    QBoxLayout *leftLayout;
    QBoxLayout *rightLayout;
    QWidget *topWidget;
    QWidget *leftWidget;
    QWidget *rightWidget;
    QWidget *bottomWidget;

    QPushButton *applyPushButton;
    QPushButton *compilePushButton;
    QPushButton *editCodePushButton;
    QPushButton *helpPushButton;
    QPushButton *cancelPushButton;
    QPushButton *newIoPushButton;
    QPushButton *updateIoPushButton;
    QPushButton *removeIoPushButton;
    QPushButton *okPushButton;

    PinListViewItem *inputRoot_;
    PinListViewItem *outputRoot_;
    PinListViewItem *episodicRoot_;

    BlockModel *model_;
    PinVector *deletedPins_;
    PinVector *deletedConnections_;

    /**
     * Initializes layout.
     */
    void initLayout();

    /**
     * Initializes list view widget.
     */
    void initListView();

    /**
     * Initializes block widget.
     */
    void initBlockWidget();

    /**
     * Initializes offset widget.
     */
    void initOffsetWidget();

    /**
     * Initializes runtime widget.
     */
    void initRuntimeWidget();

    /**
     * Initializes compile/edit buttons widget.
     */
    void initCompileEditButtonWidget();


    /**
     * Initializes bottom widget.
     */
    void initBottomWidget();

    /**
     * Synchronizes dialog with model_'s content.
     */
    void syncModel();

    /**
     * Updates model_'s content by using current dialog's content.
     */
    void updateModel();

    /**
     * Clones all pins of the model to the list view.
     */
    void addPins(PinVector pins, PinListViewItem *root);

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
     * Adds a new I/O item to the dedicated I/O group.
     */
    void newIo();

    /**
     * Updates the currently selected I/O item with 
     * modified values, BUT without any semantical check, yet.
     */
    void updateIo();

    /**
     * Removes the selected I/O item from the list view.
     */
    void removeIo();

    /**
     * Disables/enables I/O manipulation buttons, if
     * a list view item was (de)selected.
     */
    void ioSelectionChanged();

    /**
     * Disables/enables offset spin box, if the manual
     * offset setting is enabled.
     */
    void toggleManualOffset();

    /**
     * Invokes compiler on the current cpuModel.
     */
    void compile();

    /**
     * Opens the editor of the current cpuModel.
     */
    void edit();

    /**
     * Disables/enables runtime spin box, if the manual
     * runtime setting is enabled.
     */
    void toggleManualRuntime();
};

#endif // BLOCKCONFDIALOG_H
