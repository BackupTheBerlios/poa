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
 * $Id: deployprojectwizard.h,v 1.1 2003/09/25 17:25:19 papier Exp $
 *
 *****************************************************************************/

#ifndef DEPLOYPROJECTWIZARD_H
#define DEPLOYPROJECTWIZARD_H

#include <qvariant.h>
#include <qwizard.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QGroupBox;
class QLabel;
class QListView;
class QListViewItem;
class QProgressBar;
class QTextEdit;
class QWidget;

class DeployProjectWizard : public QWizard
{ 
    Q_OBJECT

public:
    DeployProjectWizard( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~DeployProjectWizard();

    QWidget* page;
    QGroupBox* ErrorsGroupBox;
    QTextEdit* ErrorsTextEdit;
    QListView* CheckupListView;
    QWidget* page_2;
    QListView* CompileListView;
    QWidget* page_3;
    QListView* SchedulingListView;
    QWidget* page_4;
    QLabel* CompileTextLabel;
    QProgressBar* DownloadProgressBar;
    QLabel* DownloadTextLabel;
    QProgressBar* CompileProgressBar;


};

#endif // DEPLOYPROJECTWIZARD_H
