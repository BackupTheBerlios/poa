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
 * $Id: pathchooserdialog.h,v 1.5 2004/03/03 04:51:04 keulsn Exp $
 *
 *****************************************************************************/


#ifndef POA_PATHCHOOSERDIALOG
#define POA_PATHCHOOSERDIALOG

#include <qcombobox.h>
#include <qdialog.h>
#include <qlistbox.h>

class BlockGraph;
class BlockNode;
#include "pinmodel.h"
#include "path.h"


/**
 * Dialog box in which the user can enter a path through a block graph. The
 * user can specify a start node and a target node. Optionally the output pin
 * of the source node and optionally the input pin of the target node. <br>
 * Then this dialog shows a list of all available paths. The user selects one
 * of the paths from that list and chooses ok.
 */
class PathChooserDialog : public QDialog
{
    Q_OBJECT

        public:

    /**
     * Creates the dialog on a block graph. When the user is done, then
     * a pointer to the chosen {@link Path}-object will be stored in
     * <code>*result</code>. This dialog should be modal.
     */
    PathChooserDialog(BlockGraph *graph, Path **result);

    /**
     * Default destructor.
     */
    virtual ~PathChooserDialog();

    protected slots:
        /** Updates the pin combo box for the source block and the path list. */
        void sourceBlockActivated(int index);
    /** Updates the path list. */
    void sourcePinActivated(int index);
    /** Updates the pin combo box for the target block and the path list */
    void targetBlockActivated(int index);
    /** Updates the path list */
    void targetPinActivated(int index);

    /** Accepts the path selected in the path list if there is one. */
    virtual void accept();

 protected:
    /** Updates the pin combo box for the input pins. */
    void updateInPins();
    /** Updates the pin combo box for the output pins. */
    void updateOutPins();
    /** Updates the path list */
    void updatePaths();

 private:
    /** Creates the list of blocks in the block graph. */
    void loadBlocks();
    /** Deletes all paths */
    void freePaths();
    /**
     * Updates the pin combo box <code>box</code> with all pins of type
     * <code>type</code> that dock onto the block <code>block</code>.
     * Deletes an old array <code>*array</code> if one exists and allocates
     * a new one.
     */
    static void updatePinCombo(QComboBox *box,
                               BlockNode *block,
                               PinModel::PinType type,
                               PinModel ***array);

    /** The block graph this is based on */
    BlockGraph *graph_;
    /**
     * Pointer to an array of blocks shown in the combo boxes. The entry at
     * index 0 is 0 to provide for a none choice in the combo box.
     */
    BlockNode **blocks_;
    /**
     * Pointer to an array of pins shown in the output combo box. The entry
     * at index 0 is 0 to provide for a none choice in the combo box.
     */
    PinModel **outPins_;
    /**
     * Pointer to an array of pins shown in the input combo box. The entry at
     * index 0 is 0 to provide for a none choice in the combo box.
     */
    PinModel **inPins_;
    /** Pointer to an array of paths shown in the list box. */
    Path **paths_;
    /** Length of the array designated by <code>paths_</code>. */
    unsigned int pathsCount_;
    /** Combo box to choose the source block. */
    QComboBox *sourceBlock_;
    /** Combo box to choose the output pin. */
    QComboBox *sourcePin_;
    /** Combo box to choose the target block. */
    QComboBox *targetBlock_;
    /** Combo box to choose the input pin. */
    QComboBox *targetPin_;
    /** List box to choose one of the available paths */
    QListBox *pathChooser_;
    /** 
     * Pointer to a variable where the result of this dialog should be 
     * copied to
     */
    Path **result_;
};

#endif // POA_PATHCHOOSERDIALOG
