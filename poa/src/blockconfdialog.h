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
 * $Id: blockconfdialog.h,v 1.26 2004/01/28 02:20:40 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_BLOCKCONFDIALOG_H
#define POA_BLOCKCONFDIALOG_H

#include <qdialog.h>

#include "blockmodel.h"
#include "pinmodel.h"

class BlockConfWidget;
class QBoxLayout;
class QLineEdit;
class QPushButton;
class QSpinBox;

/**
 * Block Configuration Dialog.
 */
class BlockConfDialog : public QDialog
{
    Q_OBJECT

public:
    BlockConfDialog(BlockModel *model, QWidget* parent = 0,
                    const char* name = 0, bool modal = FALSE, WFlags fl = 0);
    ~BlockConfDialog();

private:
    QSpinBox *clockSpinBox;
    QSpinBox *offsetSpinBox;
    QSpinBox *runtimeSpinBox;
    QSpinBox *cpuIdSpinBox;

    QLineEdit *blockNameLineEdit;
    QLineEdit *blockDescrLineEdit;

    QBoxLayout *dialogLayout;
    QBoxLayout *topLayout;
    QBoxLayout *rightLayout;
    QWidget *topWidget;
    QWidget *rightWidget;
    QWidget *bottomWidget;

    BlockModel *model_;
    BlockConfWidget *blockConfWidget_;

    /**
     * Initializes layout.
     */
    void initLayout();

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
    void sync();

    /**
     * Commits all current dialog's content to model.
     */
    void commit();

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
     * Calculates the runtime of a cpu.
     */
    void calcRuntime();

    /**
     * Invokes compiler on the current cpuModel.
     */
    void compile();

    /**
     * Opens the editor of the current cpuModel.
     */
    void edit();

};

#endif // POA_BLOCKCONFDIALOG_H
