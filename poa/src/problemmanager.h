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
 * $Id: problemmanager.h,v 1.1 2004/01/19 11:23:07 squig Exp $
 *
 *****************************************************************************/

#ifndef POA_PROBLEMMANAGER_H
#define POA_PROBLEMMANAGER_H

#include <qlistview.h>
#include <qvaluelist.h>

class ProblemReportItem : public QListViewItem
{
public:

    ProblemReportItem(QListViewItem *parent, QString shortDescription);

    QString longDescription();
    void setLongDescription(QString longDescription);
    QString shortDescription();

    //QValueList<QAction*> actions();

private:

    QString longDescription_;
};

/**
 * Checks a project for consistency
 */
class ProblemManager
{
public:

    ProblemManager(Project *project, QListView *listView);
    ~ProblemManager();

    void report();

protected:

    void checkConnected(PinModel *pin);
    void checkConnectionBits(PinModel *pin);
    void updateRoot(QListViewItem *item);

private:

    Project *project_;
    QListViewItem *connectionRoot_;
    QListViewItem *blockRoot_;
};


#endif // POA_PROBLEMMANAGER_H
