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
 * $Id: deployprojectwizard.h,v 1.8 2004/01/19 13:56:18 squig Exp $
 *
 *****************************************************************************/

#ifndef DEPLOYPROJECTWIZARD_H
#define DEPLOYPROJECTWIZARD_H

#include <qvariant.h>
#include <qwizard.h>

#include "abstractmodel.h"
#include "blockmodel.h"
#include "pinmodel.h"

class ProblemReportItem;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QGroupBox;
class QLabel;
class QListView;
class QListViewItem;
class QProgressBar;
class QTextBrowser;
class QWidget;
class Project;
class QHBox;

class DeployProjectWizard : public QWizard
{
    Q_OBJECT

public:
    DeployProjectWizard(Project *project, QWidget* parent = 0,
                        const char* name = 0, bool modal = FALSE, WFlags fl = 0);
    virtual ~DeployProjectWizard();

    void showPage(QWidget* page);

public slots:
    void setDownloadProgressBarLength(int totalSteps);
    void increaseDownloadProgressBar();
    void setProblemReportItem(QListViewItem* item);
    void updateProblemReport();

protected:
    QGroupBox* problemReportDetailsGroupBox_;
    QTextBrowser* problemReportDescriptionTextBrowser_;
    QWidget* problemReportButtonBox_;
    ProblemReportItem *currentProblemReport_;

    QWidget*       CompilePage;
    QListView*     CompileListView;
    QListViewItem* CompileListItem;
    QWidget*       SchedulingPage;
    QListView*     SchedulingListView;
    QListViewItem* SchedulingListItem;
    QWidget*       DownloadPage;
    QLabel*        CompileTextLabel;
    QProgressBar*  DownloadProgressBar;
    QLabel*        DownloadTextLabel;
    QProgressBar*  CompileProgressBar;

    void setupCheckPage();
    void setupCompilePage();
    void setupSchedulingPage();
    void setupDownloadPage();


    /**
     * a consitency check
     */
    bool allPinsConnected(/*QPtrList<AbstractModel> blocks*/);

    /**
     * comiles the source for every cpu, if not already done
     */
    bool compileAll(QPtrList<AbstractModel> blocks);

    bool download();

private:

    Project *project_;

};

#endif // DEPLOYPROJECTWIZARD_H
