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
 * $Id: autoschedulingdialog.h,v 1.3 2004/03/05 14:48:24 keulsn Exp $
 *
 *****************************************************************************/


#ifndef POA_AUTOSCHEDULINGDIALOG_H
#define POA_AUTOSCHEDULINGDIALOG_H

#include "blockgraph.h"
#include "path.h"

#include <qdialog.h>
#include <qlistbox.h>


/**
 * Instances of this class provide a dialog in which the user can initiate the
 * auto-scheduling features of POA. The user sees a list of all blocks that
 * are not yet included into the scheduling in a list box. Then the user
 * sees a list box of paths that are already included into the scheduling
 * order. When the user presses the add button, the a {@link PathChooserDialog}
 * is shown, and he can add another path to the list of paths.
 */
class AutoSchedulingDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * Creates a new dialog based on a block graph. This dialog should be
     * made modal.
     */
    AutoSchedulingDialog(BlockGraph *graph);

    /**
     * Default destructor.
     */
    virtual ~AutoSchedulingDialog();

protected:
    /**
     * Updates the list box of remaining blocks.
     */
    void updateBlockList();

protected slots:
    /**
     * Initiates the auto scheduling on all paths in their specified order.
     */
    virtual void accept();

    /**
     * Executes a {@link PathChooserDialog} and adds the resulting path to
     * the path list. Updates the list of remaining blocks.
     */
    virtual void addPath();

    /**
     * Removes the selected path from the path list. Updates the list of
     * remaining blocks.
     */
    virtual void removePath();

    /**
     * Exchanges the selected path with the path directly above it, unless
     * the selected path is already at the top or there is no path selected.
     */
    virtual void raisePrio();

    /**
     * Exchanges the selected path with the path directly below it, unless
     * the selected path is already at the bottom or there is no path selected.
     */
    virtual void lowerPrio();

private:
    /** The list box of remaining blocks */
    QListBox *blockBox_;
    /** The list box of paths for scheduling */
    QListBox *pathBox_;

    /** The block graph this is based on */
    BlockGraph *graph_;
    /** The list of paths for scheduling */
    QPtrList<Path> paths_;
};


#endif // POA_AUTOSCHEDULINGDIALOG_H
