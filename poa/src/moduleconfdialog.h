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
 * $Id: moduleconfdialog.h,v 1.3 2003/09/01 19:51:10 garbeam Exp $
 *
 *****************************************************************************/

#ifndef MODULECONFDIALOG_H
#define MODULECONFDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QGroupBox;
class QLabel;
class QLineEdit;
class QListView;
class QListViewItem;
class QPushButton;
class QRadioButton;
class QSpinBox;

class ModuleConfDialog : public QDialog
{ 
    Q_OBJECT

public:
    ModuleConfDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ModuleConfDialog();

    QPushButton* compilePushButton;
    QPushButton* editCodePushButton;
    QPushButton* helpPushButton;
    QPushButton* cancelPushButton;
    QPushButton* addIoPushButton;
    QPushButton* removeIoPushButton;
    QPushButton* okPushButton;
    QListView* ioListView;
    QGroupBox* cpuGroupBox;
    QLabel* cpuNameTextLabel;
    QLineEdit* cpuNameLineEdit;
    QLabel* cpuClockTextLabel;
    QSpinBox* cpuClockSpinBox;
    QLabel* clockMsTextLabel;
    QButtonGroup* offsetButtonGroup;
    QLabel* offsetMsTextLabel;
    QRadioButton* offsetAutoCalcRadioButton;
    QRadioButton* offsetRadioButton;
    QSpinBox* offsetSpinBox;
    QButtonGroup* runtimeButtonGroup;
    QLabel* runtimeMsTextLabel;
    QRadioButton* runtimeAutoCalcRadioButton;
    QSpinBox* runtimeSpinBox;
    QRadioButton* runtimeRadioButton;

    QLabel *ioNumberTextLabel;
    QLineEdit *ioNumberLineEdit;
    QLabel *dataTextLabel;
    QLineEdit *dataLineEdit;
    QLabel *bitsTextLabel;
    QLineEdit *bitsLineEdit;

private:
    QListViewItem *inputRoot;
    QListViewItem *outputRoot;
    QListViewItem *periodicalRoot;

private slots:

    void addIo();
    void removeIo();
};

#endif // MODULECONFDIALOG_H
