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
 * $Id: librarywindow.h,v 1.8 2004/01/19 13:56:18 squig Exp $
 *
 *****************************************************************************/
#ifndef LIBRARYWINDOW_H
#define LIBRARYWINDOW_H

#include <qvariant.h>
#include <qdockwindow.h>
#include <qlistview.h>
class AbstractModel;
class QListView;
class QListViewItem;
class QSplitter;
class QTextBrowser;

/**
 * Provides the library module list view.
 *
 * The list view has drag support.
 */
class LibraryListView : public QListView
{
public:

    /**
     * Default constructor
     */
    LibraryListView(QWidget *parent = 0, const char *name = 0, WFlags f = 0);

    /**
     * Serializes the currently selected item to a drag object
     */
    QDragObject *dragObject();
};

/**
 * Provides the library module viewitems
 */
class LibraryListViewItem : QListViewItem
{
public:

    /**
     * Creates a library list view item for the given abstract model
     */
    LibraryListViewItem(QListViewItem *parent, AbstractModel *item);

    /**
     * Default destructor
     */
    ~LibraryListViewItem();

    /**
     * Returns the AbstractModel, represented by this view item
     */
    AbstractModel &data() const;

private:
    AbstractModel *item_;
};

/**
 * Provides the library window.
 *
 * The library is a dock window with a tree and an information area
 * that show a short description of the selected item.
 *
 */
class LibraryWindow : public QDockWindow
{
    Q_OBJECT

public:

    /**
     * Creates a library window an docks it to the given place on the
     * given widget
     */
    LibraryWindow(Place p = InDock, QWidget * parent = 0,
                  const char* name = 0, WFlags f = 0 );

    /**
     * Does nothing.
     */
    ~LibraryWindow();

private :

    QListView* moduleListView;
    QTextBrowser* descriptionTextBrowser;
    QListViewItem* coreListViewItem;
    QListViewItem* cpuListViewItem;
    QListViewItem* ioListViewItem;
    QListViewItem* muxListViewItem;
    QSplitter* splitter;

    void initializeLibrary();

private slots:

    /**
     * Sets the description
     */
    void setDescription(QListViewItem* item);

    /**
     * Sets the orientation of the splitter to the reverse value of orientation.
     */
    void setOrientation(Qt::Orientation orientation);

};

#endif // LIBRARYWINDOW_H
