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
 * $Id: problemmanager.h,v 1.4 2004/02/13 15:57:57 squig Exp $
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

/**
 * Stores a problem report. A problem report can support quick fix
 * actions to solve the problem.
 */
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

    /**
     * Returns true, if the problem has been fixed.
     */
    bool isFixed() const;

    /**
     * Returns a long description that is shown when the report is
     * selected by the user.
     */
    QString longDescription() const;

    /**
     * Sets the long description.
     *
     * @see #longDescription()
     */
    void setLongDescription(const QString longDescription);

    /**
     * Sets the short description.
     *
     * @see #shortDescription()
     */
    void setShortDescription(const QString shortDescription);

    /**
     * Sets the status. This should be either "Warning" or "Critical".
     *
     * @see #status()
     */
    void setStatus(const QString status);

    /**
     * Returns the short description. The short description is
     * displayed in the first column of the list view.
     */
    QString shortDescription() const;

    /**
     * Returns the status
     */
    QString status() const;

signals:

    void updated();

protected:

    /**
     * Updates the fixed status and emits the updated signal.
     */
    void setFixed(const bool fixed);

private:

    bool fixed_;
    QString longDescription_;
};

/**
 * Represents a reports for a pin that is not connected.
 */
class DisconnectedPinReport : public ProblemReportItem
{
    Q_OBJECT

public:

    /**
     * Constructs the report.
     *
     * @param the disconnected pin
     */
    DisconnectedPinReport(QListViewItem *parent, PinModel *pin);

    virtual void addWidgets(QWidget *widget);

public slots:

    void deletePin();

private:

     PinModel *pin_;

};

/**
 * Represents a reports for a two connected pins that have different widths.
 */
class DifferentWidthReport : public ProblemReportItem
{
    Q_OBJECT

public:

    /**
     * Constructs the report.
     *
     * @param source the first pin
     * @param target the second pin
     */
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
 * Represents a reports for a two connected blocks that have different clocks.
 */
class DifferentClockReport : public ProblemReportItem
{
    Q_OBJECT

public:

    /**
     * Constructs the report.
     *
     * @param source the first block
     * @param target the second block
     */
    DifferentClockReport(QListViewItem *parent, BlockModel *source,
                         BlockModel *target);

    virtual void addWidgets(QWidget *widget);

public slots:

    void adjustSource();
    void adjustTarget();

private:

     BlockModel *source_;
     BlockModel *target_;

};

/**
 * Checks a project for consistency.
 */
class ProblemManager
{
public:

    /**
     * Constructs a problem manager.
     *
     * @param project the project to check
     * @param listView problem reports are appended to listView
     */
    ProblemManager(Project *project, QListView *listView);
    ~ProblemManager();

    /**
     * Check the project and append the reports to the list
     * view. Problem reports are grouped into categories and their
     * status is represented by an icon.
     *
     * @see ProblemReportItem
     */
    void report();

protected:

    /**
     * Checks block for problems.
     */
    void checkBlock(BlockModel *block);

    /**
     * Updates the comment field of the item.
     */
    void updateRoot(QListViewItem *item);

private:

    Project *project_;
    QListViewItem *connectionRoot_;
    QListViewItem *blockRoot_;
};


#endif // POA_PROBLEMMANAGER_H
