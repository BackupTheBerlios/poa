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
 * $Id: deployprojectwizard.h,v 1.9 2004/01/22 15:52:32 squig Exp $
 *
 *****************************************************************************/

#ifndef DEPLOYPROJECTWIZARD_H
#define DEPLOYPROJECTWIZARD_H

#include <qvariant.h>
#include <qptrlist.h>
#include <qwizard.h>
class QGroupBox;
class QLabel;
class QListViewItem;
class QTextBrowser;
class QVBox;
class QWidget;

#include "abstractmodel.h"
#include "blockmodel.h"
#include "pinmodel.h"
class CpuModel;
class ProblemReportItem;
class Project;

class DeployProjectWizard : public QWizard
{
    Q_OBJECT

public:
    DeployProjectWizard(Project *project, QWidget* parent = 0,
                        const char* name = 0, bool modal = FALSE, WFlags fl = 0);
    virtual ~DeployProjectWizard();

protected:
    QGroupBox* problemReportDetailsGroupBox_;
    QTextBrowser* problemReportDescriptionTextBrowser_;
    QWidget* problemReportButtonBox_;
    ProblemReportItem *currentProblemReport_;
    CpuModel *currentCpu_;
    QLabel *cpuDetailsLabel_;

    void setupCheckPage();
    void setupDownloadPage();

    void compileSelectedCpu();
    void downloadSelectedCpu();

protected slots:

    void cpuSelected(int index);
    void setProblemReportItem(QListViewItem* item);
    void updateProblemReport();

private:

    Project *project_;
    QPtrList<CpuModel> cpuModels;

};

#endif // DEPLOYPROJECTWIZARD_H
