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
 * $Id: pathtablemodel.h,v 1.1 2004/01/26 00:40:58 keulsn Exp $
 *
 *****************************************************************************/


#ifndef POA_PATH_TABLE_MODEL
#define POA_PATH_TABLE_MODEL

#include <qobject.h>
#include <qtable.h>


struct TableEntry
{
    QComboTableItem *before;
    QComboTableItem *item;
    QComboTableItem *after;
    BlockNode *node;
};


/**
 * Provides a model to a QTable-instance. A QTable is provided to allow the
 * user to choose a path through the blocks of a {@link BlockGraph}. The user
 * can choose blocks and incident pins to specify a set of paths starting
 * from one block and ending in another.
 *
 * The BlockGraph is required to be acyclic.
 *
 * A QListBox should be provided to allow the user to quickly choose one
 * specific path after having specified at least the first and the last block.
 * The {@link #update} slot helps update that list box.
 */
class PathTableModel : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates the model on top of <code>graph</code>. If <code>graph</code>
     * contains cycles then it will be ignored and the empty block graph will
     * be used instead. If <code>graph == 0</code> then this will also be
     * based on the empty graph.
     */
    PathTableModel(BlockGraph *graph);

    /**
     * Destroys this model.
     */
    virtual ~PathTableModel();

    /**
     * Creates a <code>QTable</code> instance that can be inserted into a
     * dialog.
     */
    QTable *createTable(QWidget *parent = 0, const char *name);

protected:

    /**
     * Returns the number of rows the table should contain.
     */
    int tableHeight() const;

public signals:
    /**
     * Emitted at least everytime the set of paths that match the rules
     * specified in this model has changed. The <code>paths</code> parameter
     * contains an ordered list of string representations for each path.
     * This list might be shown inside a <code>QListBox</code> so the user
     * can change one of these quickly.
     */
    void updated(const StringList &paths);

private:
    static const int SIZE_STEP; 

    /** The graph this is based on, or 0 if there is none. */
    BlockGraph *graph_;
    /** The view for this model, or 0 if there is none. */
    QTable *table_;
    /** The initial part of all paths */
    TableEntry *beginning_;
    /** Size of array pointed to by beginning */
    int beginningSize_;
    /** Number of meaningful entries in array */
    int beginningCount_;
    /** The last part of all paths */
    TableEndtry *ending_;
    /** Size of array pointed to by ending */
    int endingSize_;
    /** Number of meaningful entries in array */
    int endingCount_;
};


#endif // POA_PATH_TABLE_MODEL


