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
 * $Id: problemmanager.h,v 1.2 2004/01/19 13:56:18 squig Exp $
 *
 *****************************************************************************/

#ifndef POA_PROBLEMMANAGER_H
#define POA_PROBLEMMANAGER_H

#include <qlistview.h>
#include <qobject.h>
#include <qvaluelist.h>
class QWidget;

#include "pinmodel.h"
#include "project.h"

class ProblemReportItem : public QObject, public QListViewItem
{
    Q_OBJECT

public:

    ProblemReportItem(QListViewItem *parent, QString status = "Error",
                      QString shortDescription = QString::null);

    /**
     * Invoked when the report is displayed on screen. Does
     * nothing. Sub classes should overwrite this method.
     */
    virtual void addWidgets(QWidget *widget);

    bool isFixed() const;
    QString longDescription() const;
    void setLongDescription(const QString longDescription);
    void setShortDescription(const QString shortDescription);
    void setStatus(const QString status);
    QString shortDescription() const;
    QString status() const;

signals:

    void updated();

protected:

    void setFixed(const bool fixed);

private:

    bool fixed_;
    QString longDescription_;
};

class DisconnectedPinReport : public ProblemReportItem
{
    Q_OBJECT

public:

    DisconnectedPinReport(QListViewItem *parent, PinModel *pin);

    virtual void addWidgets(QWidget *widget);

public slots:

    void deletePin();

private:

     PinModel *pin_;

};

class DifferentWidthReport : public ProblemReportItem
{
    Q_OBJECT

public:

    DifferentWidthReport(QListViewItem *parent, PinModel *source,
                         PinModel *target);

    virtual void addWidgets(QWidget *widget);

public slots:

    void adjustSource();
    void adjustTarget();

private:

     PinModel *source_;
     PinModel *target_;

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

    void checkBlock(BlockModel *block);
    void checkConnected(PinModel *pin);
    void checkConnectionBits(PinModel *pin);
    void updateRoot(QListViewItem *item);

private:

    Project *project_;
    QListViewItem *connectionRoot_;
    QListViewItem *blockRoot_;
};


#endif // POA_PROBLEMMANAGER_H
