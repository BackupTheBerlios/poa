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
 * $Id: settingsdialog.h,v 1.14 2004/03/19 15:41:51 squig Exp $
 *
 *****************************************************************************/
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <qvariant.h>
#include <qtabdialog.h>

class QComboBox;
class QLineEdit;
class QPushButton;
class QSpinBox;

/**
 * Provides the settings dialog.
 *
 * The settings dialog has mutliple tabs.
 */
class SettingsDialog : public QTabDialog
{
    Q_OBJECT

public:
    SettingsDialog(int tabIndex = 0, QWidget* parent = 0, const char* name = 0,
                   bool modal = FALSE, WFlags fl = 0);
    ~SettingsDialog();

    QLineEdit* compilerLineEdit_;
    QLineEdit* cTemplateLineEdit_;
    QLineEdit* editorLineEdit_;
    QSpinBox* gridSizeSpinBox_;
    QComboBox* serialPortComboBox_;
    QComboBox* languageComboBox_;

public slots:
    void applySettings();
    void chooseExternalEditor();
    void chooseExternalCompiler();
    void chooseTemplatePath();

private :
    QWidget *createGeneralTab();
    QWidget *createPathTab();
    QWidget *createDownloadTab();
    void setup();
};

#endif // SETTINGSDIALOG_H
