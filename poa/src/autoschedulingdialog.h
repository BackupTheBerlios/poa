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
 * $Id: autoschedulingdialog.h,v 1.1 2004/02/18 03:41:31 keulsn Exp $
 *
 *****************************************************************************/


#ifndef POA_AUTOSCHEDULINGDIALOG_H
#define POA_AUTOSCHEDULINGDIALOG_H

#include "blockgraph.h"
#include "path.h"

#include <qdialog.h>
#include <qlistbox.h>

class AutoSchedulingDialog : public QDialog
{
    Q_OBJECT

public:
    AutoSchedulingDialog(BlockGraph *graph);
    virtual ~AutoSchedulingDialog();

protected:
    void updateBlockList();

protected slots:
    virtual void accept();
    virtual void addPath();
    virtual void removePath();
    virtual void raisePrio();
    virtual void lowerPrio();

private:
    QListBox *blockBox_;
    QListBox *pathBox_;

    BlockGraph *graph_;
    QPtrList<Path> paths_;
};


#endif // POA_AUTOSCHEDULINGDIALOG_H
