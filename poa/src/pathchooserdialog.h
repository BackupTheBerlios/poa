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
 * $Id: pathchooserdialog.h,v 1.3 2004/02/16 10:40:26 keulsn Exp $
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
#include "scheduler.h"


class PathChooserDialog : public QDialog
{
    Q_OBJECT

public:

    PathChooserDialog(BlockGraph *graph);
    virtual ~PathChooserDialog();

protected slots:
    void sourceBlockActivated(int index);
    void sourcePinActivated(int index);
    void targetBlockActivated(int index);
    void targetPinActivated(int index);

    virtual void accept();

protected:
    void updateInPins();
    void updateOutPins();
    void updatePaths();

private:
    void loadBlocks();
    void freePaths();
    static void updatePinCombo(QComboBox *box,
			       BlockNode *block,
			       PinModel::PinType type,
			       PinModel ***array);

    BlockGraph *graph_;
    BlockNode **blocks_;
    PinModel **outPins_;
    PinModel **inPins_;
    Path **paths_;
    unsigned int pathsCount_;
    QComboBox *sourceBlock_;
    QComboBox *sourcePin_;
    QComboBox *targetBlock_;
    QComboBox *targetPin_;
    QListBox *pathChooser_;
    Scheduler scheduler_;
};

#endif // POA_PATHCHOOSERDIALOG
