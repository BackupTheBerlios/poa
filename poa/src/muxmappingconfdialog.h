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
 * $Id: muxmappingconfdialog.h,v 1.4 2004/01/29 16:30:05 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_MUXMAPPINGCONFDIALOG_H
#define POA_MUXMAPPINGCONFDIALOG_H

#include <qcombobox.h>
#include <qdialog.h>
#include <qlistview.h>
#include <qptrlist.h>

class QSpinBox;
class QString;

class MuxModel;
class MuxMappingListViewItem;
class PinListViewItem;

/**
 * PinItemComboBox
 */

class PinItemComboBox : public QComboBox
{

public:
    PinItemComboBox(QWidget *parent);
    ~PinItemComboBox();

    void insertItem(PinListViewItem *item);

    PinListViewItem *selectedItem();

private:

    PinListViewItem *selected_;
    QPtrList<PinListViewItem> items_;
};

//////////////////////////////////////////////////////////////////////////////

/**
 * MuxMapping Configuration Dialog.
 */
class MuxMappingConfDialog : public QDialog
{
    Q_OBJECT

public:
    MuxMappingConfDialog(QListViewItemIterator iter,
                         MuxMappingListViewItem *item,
                         QWidget* parent = 0,
                         const char* name = 0,
                         bool modal = FALSE, WFlags fl = 0);
    ~MuxMappingConfDialog();

private:

    QComboBox *inputComboBox_;
    QComboBox *outputComboBox_;
    QSpinBox *firstInputBitSpinBox_;
    QSpinBox *lastInputBitSpinBox_;
    QSpinBox *firstOutputBitSpinBox_;
    QSpinBox *lastOutputBitSpinBox_;

    QListViewItemIterator itemIter_;
    MuxMappingListViewItem *muxMappingItem_;

    /**
     * Initializes layout.
     */
    void initLayout();

    /**
     * Synchronizes dialog with model_'s content.
     */
    void sync();

    /**
     * Commits all current dialog's content to model.
     */
    void commit();

    /**
     * Workaround since QComboBox doesn't supports something else
     * than QString's.
     * A tricky alternative would be: take address of an object as
     * its index... :)
     */
    PinListViewItem *pinListViewItemForString(QString name);

private slots:

    /**
     * Applies all changes, deletes all cloned data and closes
     * this dialog.
     */
    void ok();

};

#endif // POA_MUXMAPPINGCONFDIALOG_H
