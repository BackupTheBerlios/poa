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
 * $Id: deployprojectwizard.cpp,v 1.14 2004/01/27 06:26:26 kilgus Exp $
 *
 *****************************************************************************/

#include "abstractmodel.h"
#include "blockmodel.h"
#include "codemanager.h"
#include "cpumodel.h"
#include "deployprojectwizard.h"
#include "downloadmanager.h"
#include "pinmodel.h"
#include "problemmanager.h"
#include "project.h"

#include <qpushbutton.h>
#include <qvariant.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qprogressbar.h>
#include <qtextedit.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qptrlist.h>
#include <qfileinfo.h>
#include <qdatetime.h>
#include <qsplitter.h>
#include <qtextbrowser.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qvgroupbox.h>

/*
 *  Constructs the DeployProjectWizard.
 *
 *  The wizard will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal wizard.
 */
DeployProjectWizard::DeployProjectWizard(Project *project, QWidget* parent,
                                         const char* name, bool modal,
                                         WFlags fl)
    : QWizard(parent, name, modal, fl), project_(project)
{
    currentCpu_ = 0;
    currentProblemReport_ = 0;

    resize(600, 400);
    setCaption(tr("Deploy Project"));

    for (QPtrListIterator<AbstractModel> it(*project->blocks()); it != 0; ++it) {
        CpuModel *cpu = dynamic_cast<CpuModel *>(*it);
        if (cpu != 0) {
            cpuModels.append(cpu);
        }
    }

    setupCheckPage();
    setupDownloadPage();
}

/*
 *  Destroys the object and frees any allocated resources
 */
DeployProjectWizard::~DeployProjectWizard()
{
    // no need to delete child widgets, Qt does it all for us
}

void DeployProjectWizard::setupCheckPage()
{
    QSplitter* splitter = new QSplitter(QSplitter::Vertical, this);

    QListView *listView = new QListView(splitter);
    listView->addColumn(trUtf8("Description"));
    listView->addColumn(trUtf8("Status"));

    problemReportDetailsGroupBox_ = new QVGroupBox(splitter);
    problemReportDetailsGroupBox_->setTitle(trUtf8("Details"));

    problemReportDescriptionTextBrowser_
        = new QTextBrowser(problemReportDetailsGroupBox_);
    problemReportButtonBox_ = 0;

    ProblemManager manager(project_, listView);
    manager.report();

    connect(listView, SIGNAL(selectionChanged(QListViewItem *)),
            this, SLOT(setProblemReportItem(QListViewItem *)));

    addPage(splitter, trUtf8("Check Project"));
}

void DeployProjectWizard::setupDownloadPage()
{
    QWidget *page = new QWidget(this);

    QLabel *infoLabel = new QLabel
        ("Currently the download of a single CPU only is supported.", page);

    QWidget *cpuWidget = new QWidget(page);
    QLabel *cpuLabel = new QLabel("CPU", cpuWidget);
    QComboBox *cpuComboBox = new QComboBox(cpuWidget);
    for (QPtrListIterator<CpuModel> it(cpuModels); it != 0; ++it) {
        cpuComboBox->insertItem((*it)->name());
    }
    QBoxLayout *cpuLayout = new QHBoxLayout(cpuWidget, WIDGET_SPACING);
    cpuLayout->addWidget(cpuLabel);
    cpuLayout->addWidget(cpuComboBox);
    cpuLayout->addStretch(1);

    QGroupBox *detailsGroupBox = new QVGroupBox("Details", page);
    cpuDetailsLabel_ = new QLabel(detailsGroupBox);

    if (cpuModels.count() > 0) {
        cpuSelected(0);
    }
    connect(cpuComboBox, SIGNAL(activated(int)), this, SLOT(cpuSelected(int)));

    QWidget *buttonWidget = new QWidget(page);
    QPushButton *compileButton = new QPushButton("Compile", buttonWidget);
    QPushButton *downloadButton = new QPushButton("Download", buttonWidget);
    QPushButton *runButton = new QPushButton("Run", buttonWidget);

    QBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget, WIDGET_SPACING);
    buttonLayout->addWidget(compileButton);
    buttonLayout->addWidget(downloadButton);
    buttonLayout->addWidget(runButton);
    buttonLayout->addStretch(1);
    /*
    QWidget *downloadWidget = new QWidget(page);
    QProgressBar *downloadProgressBar = new QProgressBar(downloadWidget, "DownloadProgressBar" );
    
    QBoxLayout *downloadLayout = new QHBoxLayout(downloadWidget, WIDGET_SPACING);
    downloadLayout->addWidget(downloadProgressBar);
    */
    QBoxLayout *pageLayout = new QVBoxLayout(page, WIDGET_SPACING);
    pageLayout->addWidget(infoLabel);
    pageLayout->addSpacing(2 * WIDGET_SPACING);
    pageLayout->addWidget(cpuWidget);
    pageLayout->addWidget(detailsGroupBox);
    pageLayout->addWidget(buttonWidget);
    //    pageLayout->addWidget(downloadWidget);
    pageLayout->addStretch(1);

    addPage(page, trUtf8("Compile and Download Project"));
    setFinishEnabled(page, true);
}

void DeployProjectWizard::compileSelectedCpu()
{
  CodeManager *cm = CodeManager::instance();
  cm->compile(currentCpu_);
}

void DeployProjectWizard::cpuSelected(int index)
{
    currentCpu_ = cpuModels.at(index);

    CodeManager *cm = CodeManager::instance();

    QString sourceFilename = cm->sourceFilePath(currentCpu_);
    QFileInfo sourceFileInfo(sourceFilename);

    QString srecFilename =
      (sourceFilename.endsWith(".c"))
        ? sourceFilename.left(sourceFilename.length() - 2) + ".srec"
        : sourceFilename + ".srec";
    QFileInfo srecFileInfo(srecFilename);

    cpuDetailsLabel_->setText
        (QString("Soure Filename: %1\nSource Last Modified:%2\n"
                 "SRec Filename: %2\nSRec Last Modified:%3\n")
         .arg(sourceFilename).arg(sourceFileInfo.lastModified().toString())
         .arg(srecFilename).arg(srecFileInfo.lastModified().toString()));
}

void DeployProjectWizard::downloadSelectedCpu()
{
  CodeManager *cm = CodeManager::instance();
  DownloadManager *dm = DownloadManager::instance();
  QString sourceFilename = cm->sourceFilePath(currentCpu_);
  QString srecFilename =
      (sourceFilename.endsWith(".c"))
        ? sourceFilename.left(sourceFilename.length() - 2) + ".srec"
        : sourceFilename + ".srec";
  dm->download(srecFilename, (const char*)Settings::instance()->serialPort());
}

/*void DeployProjectWizard::setDownloadProgressBarLength(int totalSteps)
{
  downloadProgressBar->setTotalSteps(totalSteps);
}

void DeployProjectWizard::increaseDownloadProgressBar()
{
  int progress = downloadProgressBar->progress();
  downloadProgressBar->setProgress(progress +1);
}
*/

void DeployProjectWizard::setProblemReportItem(QListViewItem* item)
{
    if (problemReportButtonBox_ != 0) {
        delete problemReportButtonBox_;
        problemReportButtonBox_ = 0;
    }

    ProblemReportItem *report = dynamic_cast<ProblemReportItem*>(item);
    if (report != 0) {
        if (currentProblemReport_ != 0) {
            currentProblemReport_->disconnect(this);
        }
        currentProblemReport_ = report;

        connect(report, SIGNAL(updated()), this, SLOT(updateProblemReport()));

        problemReportDescriptionTextBrowser_->setText(report->longDescription());

        if (!report->isFixed()) {
            problemReportButtonBox_ = new QWidget(problemReportDetailsGroupBox_);
            QHBoxLayout *layout = new QHBoxLayout(problemReportButtonBox_);
            layout->setAutoAdd(true);
            report->addWidgets(problemReportButtonBox_);
            layout->addStretch(1);
            problemReportButtonBox_->show();
        }
    }
    else {
        problemReportDescriptionTextBrowser_->setText("");
    }
}

void DeployProjectWizard::updateProblemReport()
{
    setProblemReportItem(currentProblemReport_);
}
