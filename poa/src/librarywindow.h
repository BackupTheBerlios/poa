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
 * $Id: librarywindow.h,v 1.11 2004/01/21 16:07:51 squig Exp $
 *
 *****************************************************************************/
#ifndef LIBRARYWINDOW_H
#define LIBRARYWINDOW_H

class AbstractModel;
class LibraryWindow;

#include <qvariant.h>
#include <qdict.h>
#include <qfile.h>
#include <qdockwindow.h>
#include <qlistview.h>
class QListView;
class QListViewItem;
class QPopupMenu;
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
 * Provides a QListViewItem that stores an AbstractModel object.
 */
class LibraryListViewItem : public QListViewItem
{
public:

    /**
     * Creates a library list view item for the given abstract model
     */
    LibraryListViewItem(QListViewItem *parent, AbstractModel *item);

    /**
     * Deletes item_.
     */
    ~LibraryListViewItem();

    /**
     * Returns the AbstractModel, represented by this view item
     */
    AbstractModel &data() const;

    /**
     * Changes the name of item_.
     */
    virtual void okRename(int col);

private:
    AbstractModel *item_;
};

/**
 * Provides a QListViewItem that acts as a root item for
 * LibraryListViewItem objects.
 */
class TypeListViewItem : public QListViewItem
{
public:

    TypeListViewItem(LibraryWindow *library, QListView *parent, QString text);

    /**
     * Changes the type of all child items.
     */
    virtual void okRename(int col);

private:

    LibraryWindow *library_;

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

    /**
     * Adds model to library.
     */
    void add(AbstractModel *model);

    /**
     * Reads models from file and adds them to the library.
     */
    void open(QFile *file);

    /**
     * Saves all library items to file.
     */
    void save(QFile *file);

    /**
     * Returns a list of item types that are currently in the library.
     */
    QStringList types();

private :

    QListView* modelListView_;
    QTextBrowser* descriptionTextBrowser_;
    QSplitter* splitter_;
    QDict<QListViewItem> typeItemByType;
    bool modified_;
    QPopupMenu *popupMenu_;

    /**
     * Adds a few default items to the library. Invoked when the
     * library could not be restored from disk.
     */
    void addDefaultItems();

    /**
     * Returns the root item for type. If no item does exist for type,
     * a new item is created.
     */
    QListViewItem *getTypeItem(const QString &type);

    /**
     * Adds the modules to the tree.
     */
    void initialize();

    /**
     * Renames item and changes the type of all child nodes to newName.
     */
    void renameTypeItem(QListViewItem *item, const QString &oldName,
                        const QString &newName);

private slots:

    /**
     * Popups up an input dialog that allows the user to change the
     * type of the currently selected item.
     */
    void changeTypeOfSelected();

    /**
     * Removes currently selected item from library.
     */
    void removeSelected();

    /**
     * Popups up an input dialog that allows the user to rename
     * the currently selected item.
     */
    void renameSelected();

    /**
     * Sets the description
     */
    void setDescription(QListViewItem* item);

    /**
     * Sets the orientation of the splitter to the reverse value of orientation.
     */
    void setOrientation(Qt::Orientation orientation);

    /**
     * Shows the popup menu at pos.
     */
    void showPopup(QListViewItem *item, const QPoint &pos, int col);

    friend class TypeListViewItem;

};

#endif // LIBRARYWINDOW_H
