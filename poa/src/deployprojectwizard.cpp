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
 * $Id: deployprojectwizard.cpp,v 1.11 2004/01/19 13:56:18 squig Exp $
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
    resize(600, 400);
    setCaption(tr("Deploy Project"));

    currentProblemReport_ = 0;

    setupCheckPage();
    setupCompilePage();
    setupSchedulingPage();
    setupDownloadPage();

    DownloadManager *dm = DownloadManager::instance();

    connect(dm, SIGNAL(increaseProgressBar()),     this, SLOT(increaseDownloadProgressBar()));
    connect(dm, SIGNAL(setProgressBarLength(int)), this, SLOT(setDownloadProgressBarLength(int)));
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

    addPage(splitter, trUtf8("Plausibility checkup"));
}

void DeployProjectWizard::setupCompilePage()
{
    CompilePage = new QWidget( this, "CompilePage" );

    CompileListView = new QListView( CompilePage, "CompileListView" );
    CompileListView->addColumn( trUtf8( "CPU" ) );
    CompileListView->addColumn( trUtf8( "Compile" ) );
    QListViewItem * CompileListItem = new QListViewItem( CompileListView, 0 );
    CompileListItem->setText( 0, trUtf8( "CPU 1" ) );
    CompileListItem->setText( 1, trUtf8( "done" ) );

    CompileListItem = new QListViewItem( CompileListView, CompileListItem );
    CompileListItem->setText( 0, trUtf8( "CPU 2" ) );
    CompileListItem->setText( 1, trUtf8( "in progress" ) );

    CompileListItem = new QListViewItem( CompileListView, CompileListItem );
    CompileListItem->setText( 0, trUtf8( "CPU 3" ) );

    CompileListItem = new QListViewItem( CompileListView, CompileListItem );
    CompileListItem->setText( 0, trUtf8( "CPU 4" ) );

    CompileListItem = new QListViewItem( CompileListView, CompileListItem );
    CompileListItem->setText( 0, trUtf8( "CPU 5" ) );
    CompileListItem->setText( 1, trUtf8( "done" ) );

    CompileListItem = new QListViewItem( CompileListView, CompileListItem );
    CompileListItem->setText( 0, trUtf8( "CPU 6" ) );
    CompileListItem->setText( 1, trUtf8( "done" ) );

    CompileListItem = new QListViewItem( CompileListView,CompileListItem );
    CompileListItem->setText( 0, trUtf8( "CPU x" ) );

    CompileListView->setGeometry( QRect( 0, 0, 580, 410 ) );
    addPage( CompilePage, trUtf8( "Compile" ) );
}

void DeployProjectWizard::setupSchedulingPage()
{
    SchedulingPage = new QWidget( this, "SchedulingPage" );

    SchedulingListView = new QListView( SchedulingPage, "SchedulingListView" );
    SchedulingListView->addColumn( trUtf8( "Block" ) );
    SchedulingListView->addColumn( trUtf8( "Runtime" ) );
    SchedulingListView->addColumn( trUtf8( "Offset" ) );
    QListViewItem * SchedulingListItem =
      new QListViewItem( SchedulingListView, 0 );
    SchedulingListItem->setText( 0, trUtf8( "New Item" ) );

    SchedulingListView->setGeometry( QRect( 0, 0, 580, 410 ) );
    addPage( SchedulingPage, trUtf8( "Scheduling" ) );
}

void DeployProjectWizard::setupDownloadPage()
{
    DownloadPage = new QWidget( this, "DownloadPage" );

    CompileTextLabel = new QLabel( DownloadPage, "CompileTextLabel" );
    CompileTextLabel->setGeometry( QRect( 140, 70, 161, 31 ) );
    CompileTextLabel->setText( trUtf8( "Compiling..." ) );

    DownloadProgressBar = new QProgressBar( DownloadPage, "DownloadProgressBar" );
    DownloadProgressBar->setGeometry( QRect( 140, 280, 280, 31 ) );

    DownloadTextLabel = new QLabel( DownloadPage, "DownloadTextLabel" );
    DownloadTextLabel->setGeometry( QRect( 140, 230, 161, 31 ) );
    DownloadTextLabel->setText( trUtf8( "Downloading..." ) );

    CompileProgressBar = new QProgressBar( DownloadPage, "CompileProgressBar" );
    CompileProgressBar->setGeometry( QRect( 140, 120, 280, 31 ) );
    addPage( DownloadPage, trUtf8( "Compile and download Project" ) );

}

void DeployProjectWizard::setDownloadProgressBarLength(int totalSteps)
{
  CompileProgressBar->setTotalSteps(totalSteps);
}

void DeployProjectWizard::increaseDownloadProgressBar()
{
  int progress = CompileProgressBar->progress();
  CompileProgressBar->setProgress(progress + 1);
}


void DeployProjectWizard::showPage(QWidget* page)
{
  QWizard::showPage(page);

  if (page == CompilePage){

  }
  else if (page == SchedulingPage){

  }
  else if (page == DownloadPage){

  }

}

bool DeployProjectWizard::allPinsConnected(/*QPtrList<AbstractModel>* blocks*/){
    // @papier: Prefer using iterators instæad of at(N)

  /*  for (uint i=0; i < blocks.count(); ++i){
    if blocks.at(i).hasInputPins() {
    DON'T USE PINVECTORS ANYMORE!
           j != blocks.at(i).inputPins().end();
           ++j){
        if (*j).*connected() == null {
          // pin is not connected!!
        }
      }
    if blocks.take(i).hasOutputPins() {
    }
    //nessary?? Or are Episodic Pins always connected??
    if block.take(i).hasEpisodicPins() {
    }
  }
  */
  return true;
}

/*
 * Checks for every block if it is a cpu or not.
 * For every cpu it is cheked if it has already a binary file and if the
 * binary is actual, if not it will be compiled
 */

bool DeployProjectWizard::compileAll(QPtrList<AbstractModel> /*blocks*/){
  /*
  CodeManager *cm = CodeManager::instance();

  QPtrListIterator<AbstractModel> it(blocks);
  AbstractModel *block;
  while ( (block = it.current()) != 0) {
    ++it;

    if (INSTANCEOF(block, CpuModel)) {
      if (QFileInfo binary(cm.cpuPath(block)).exists() and
          QFileInfo source(cm.cpuPath(block) +
                           cm.sourceFilePath(block) +
                           cm.fileName(block)).exists() )  {
        if ( binary(cm.cpuPath(block)).lastModified() <
             source(cm.cpuPath(block) + cm.sourceFilePath(block)).lastModified()) {
          //        if compile(block) != alles ok returncode compile error
          //        }
        }
      }
      else {
        if source(cm.sourceFilePath(block).exists() {
        //        if compile(block) != alles ok returncode compile error
        }
        //      else error cpu without source
      }
    }

  }
  */
  return true;
}

bool DeployProjectWizard::download() {

  return true;
}


/*
 *  Destroys the object and frees any allocated resources
 */
DeployProjectWizard::~DeployProjectWizard()
{
    // no need to delete child widgets, Qt does it all for us
}

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
