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
 * $Id: muxconfdialog.h,v 1.4 2003/09/24 15:44:28 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_MUXCONFDIALOG_H
#define POA_MUXCONFDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
#include <qlistview.h>
#include <qptrlist.h>

class PinModel;
class MuxMapping;
class MuxModel;

class QBoxLayout;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;

/**
 * Provides the MuxMapping view items.
 */
class MuxMappingListViewItem : public QListViewItem
{
public:

    /**
     * Creates a MuxMapping list view item for the given abstract model
     */
    MuxMappingListViewItem(QListViewItem *parent, MuxMapping*clone = 0,
                           MuxMapping *origin = 0);

    /**
     * Default destructor
     */
    ~MuxMappingListViewItem();

    /**
     * Returns the PinModel, represented by this view item
     */
    MuxMapping *data() const;

    /**
     * Returns the origin PinModel, represented by this view item
     */
    MuxMapping *origData() const;

    /**
     * Updates the view with current contents of the MuxMapping.
     */
    void update();

private:
    MuxMapping *clone_;
    MuxMapping *origin_;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Provides the mux view items.
 */
class MuxListViewItem : public QListViewItem
{
public:

    /**
     * Creates a IO list view item for the given abstract model
     */
    MuxListViewItem(QListView *parent, QListViewItem *after,
                    PinModel *clone = 0, PinModel *origin = 0);

    /**
     * Default destructor
     */
    ~MuxListViewItem();

    /**
     * Returns the PinModel, represented by this view item
     */
    PinModel *data() const;

    /**
     * Returns the origin PinModel, represented by this view item
     */
    PinModel *origData() const;

    /**
     * Updates the view with current contents of the pin model.
     */
    void update();

private:
    PinModel *clone_;
    PinModel *origin_;
};

///////////////////////////////////////////////////////////////////////////////

class MuxConfDialog : public QDialog
{
    Q_OBJECT

public:
    MuxConfDialog(MuxModel *model, QWidget* parent = 0,
                  const char* name = 0, bool modal = FALSE, WFlags fl = 0);
    ~MuxConfDialog();

private:
    /** Initializes all Layouts */
    void initLayout();

    /** Initializes top widget. */
    void initTopWidget();

    /** Initializes mapping widget. */
    void initMappingWidget();

    /** Initializes bottom buttons. */
    void initBottomWidget();

    /** Initializes connections. */
    void initConnections();

    /** Syncs this dialog with <code>model_</code>. */
    void syncModel();

    /** Syncs the <code>model_</code> with this dialog. */
    void updateModel();

    QLineEdit* nameLineEdit;
    QPushButton* helpPushButton;
    QPushButton* okPushButton;
    QPushButton* cancelPushButton;
    QPushButton* applyPushButton;
    QPushButton* addPushButton;
    QPushButton* updatePushButton;
    QPushButton* removePushButton;
    QComboBox* ioComboBox_;
    QSpinBox* endSpinBox;
    QSpinBox* beginSpinBox;
    QListView* mappingListView;
    QBoxLayout *dialogLayout_;

    MuxModel *model_;

    QPtrList<MuxMapping> deletedMappings_;
    QPtrList<PinModel> deletedPins_;

private slots:

    /** Magic selection, button title changing. */
    void mappingSelectionChanged();


    /** 
     * Adds an IO if no I/O is selected or no mapping was defined.
     * Otherwise adds a MuxMapping to the selected IO.
     */
    void addIoOrMapping();
};

#endif // POA_MUXCONFDIALOG_H
