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
 * $Id: deployprojectwizard.h,v 1.14 2004/02/01 20:28:57 squig Exp $
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
#include "cpumodel.h"
class ProblemReportItem;
class Project;

/**
 * Provides the deploy wizard. The deploy wizard enables the
 * verification of a project and the download to the CPLD.
 */
class DeployProjectWizard : public QWizard
{
    Q_OBJECT

public:
    /*
     *  Constructs the DeployProjectWizard.
     *
     *  The wizard will by default be modeless, unless you set 'modal' to
     *  TRUE to construct a modal wizard.
     */
    DeployProjectWizard(Project *project, QWidget* parent = 0,
                        const char* name = 0, bool modal = FALSE, WFlags fl = 0);
    /*
     *  Destroys the object and frees any allocated resources
     */
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

protected slots:

    void compileSelectedCpu();
    void cpuSelected(int index);
    void downloadSelectedCpu();
    void sendRunCommand();
    void setProblemReportItem(QListViewItem* item);
    void updateProblemReport();

private:

    QString checkSerialPort();

    Project *project_;
    QPtrList<CpuModel> cpuModels;

};

#endif // DEPLOYPROJECTWIZARD_H
