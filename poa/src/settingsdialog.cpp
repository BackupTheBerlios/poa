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
 * $Id: settingsdialog.cpp,v 1.12 2004/01/09 14:05:29 squig Exp $
 *
 *****************************************************************************/
#include "settingsdialog.h"

#include "settings.h"

#include <qvariant.h>
#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>


/**
 * Constructs the dialog.
 */
SettingsDialog::SettingsDialog(QWidget* parent, const char* name, bool modal,
                               WFlags fl)
    : QTabDialog(parent, name, modal, fl)
{
    setCaption(tr("Settings"));
    setApplyButton();
    setCancelButton();
    setOkButton();
    setHelpButton();

    this->addTab(createGeneralTab(), tr("General"));
    this->addTab(createPathTab(), tr("Paths"));
    this->addTab(createDownloadTab(), tr("Download"));
    setup();

    resize(sizeHint());//resize(400, 400);

    connect(this, SIGNAL(applyButtonPressed()), this, SLOT(applySettings()));
}

/**
 * Destroys the dialog.
 */
SettingsDialog::~SettingsDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/**
 * Returns the general tab.
 */
QWidget *SettingsDialog::createGeneralTab()
{
    QWidget *tab = new QWidget(this);
    // the number of vertical items should be one more than the number
    // of actual widgets, in order to align all widgets north
    QGridLayout *grid = new QGridLayout(tab, 4, 2, 5, 5);

    grid->addWidget(new QLabel(tr("Grid Size"), tab), 0, 0);
    gridSizeSpinBox_ = new QSpinBox(1, 100, 1, tab);
    grid->addWidget(gridSizeSpinBox_, 0, 1);

    return tab;
}

/**
 * Returns the path tab.
 */
QWidget *SettingsDialog::createPathTab()
{
    QPushButton *button;
    QWidget *tab = new QWidget(this);
    QGridLayout *grid = new QGridLayout(tab, 5, 3, 5, 5);

    grid->addWidget(new QLabel(tr("External Editor"), tab), 0, 0);
    editorLineEdit_ = new QLineEdit(tab);
    grid->addWidget(editorLineEdit_, 0, 1);
    button = new QPushButton("...", tab);
    grid->addWidget(button, 0, 2);
    connect(button, SIGNAL(clicked()), this, SLOT(chooseExternalEditor()));

    grid->addWidget(new QLabel(tr("External Compiler"), tab), 1, 0);
    compilerLineEdit_ = new QLineEdit(tab);
    grid->addWidget(compilerLineEdit_, 1, 1);
    button = new QPushButton("...", tab);
    grid->addWidget(button, 1, 2);
    connect(button, SIGNAL(clicked()), this, SLOT(chooseExternalCompiler()));

    grid->addWidget(new QLabel(tr("C Source Template Path"), tab), 2, 0);
    cTemplateLineEdit_ = new QLineEdit(tab);
    grid->addWidget(cTemplateLineEdit_, 2, 1);
    button = new QPushButton("...", tab);
    grid->addWidget(button, 2, 2);
    connect(button, SIGNAL(clicked()), this, SLOT(chooseTemplatePath()));

    grid->addWidget(new QLabel(tr("External Download Tool"), tab), 3, 0);
    downloadLineEdit_ = new QLineEdit(tab);
    grid->addWidget(downloadLineEdit_, 3, 1);
    button = new QPushButton("...", tab);
    grid->addWidget(button, 3, 2);
    connect(button, SIGNAL(clicked()), this, SLOT(chooseDownloadTool()));

    return tab;
}

/**
 * Returns the download tab.
 */
QWidget *SettingsDialog::createDownloadTab()
{
  QWidget *tab = new QWidget(this);
  QGridLayout *grid =new QGridLayout(tab, 4, 3, 5, 5);

  grid->addWidget(new QLabel(tr("Serial Port"), tab), 0, 0);
  serialPortComboBox_ = new QComboBox(tab);
  serialPortComboBox_->setEditable(true);
#ifdef _TTY_WIN_
  serialPortComboBox_->insertItem("COM1");
  serialPortComboBox_->insertItem("COM2");
  serialPortComboBox_->insertItem("COM3");
  serialPortComboBox_->insertItem("COM4");
#else
  serialPortComboBox_->insertItem("/dev/ttyS0");
  serialPortComboBox_->insertItem("/dev/ttyS1");
  serialPortComboBox_->insertItem("/dev/ttyS2");
  serialPortComboBox_->insertItem("/dev/ttyS3");
#endif
  grid->addWidget(serialPortComboBox_, 0, 1);

//   QButtonGroup *ports = new QButtonGroup(tr("Serial Port:"), tab, 0);
//   box->addWidget(ports, 0, 0, 0);
//   QGridLayout *grid = new QGridLayout(ports, 2 , 1, 4, -1, 0);
//   grid->addWidget(new QRadioButton(tr("/dev/ttyS0 (POSIX) Com1 (MS)"), ports,0 ), 0, 0);
//   grid->addWidget(new QRadioButton(tr("/dev/ttyS1 (POSIX) Com2 (MS)"), ports,0 ), 1, 0);

  return tab;
}

/**
 * Reads and sets the default settings from Settings.
 */
void SettingsDialog::setup()
{
    Settings* s = Settings::instance();

    // general tab
    gridSizeSpinBox_->setValue(s->gridSize());

    // path tab
    editorLineEdit_->setText(s->get("Editor"));
    compilerLineEdit_->setText(s->compilerCmd());
    cTemplateLineEdit_->setText(s->templatePath());
    downloadLineEdit_->setText(s->get("Download Path"));

    // download tab
    serialPortComboBox_->setCurrentText(s->get("Serial Port"));
}

/**
 * Stores the settings to Settings.
 */
void SettingsDialog::applySettings()
{
    Settings* s = Settings::instance();

    // general tab
    s->setGridSize(gridSizeSpinBox_->value());

    // path tab
    s->set("Editor", editorLineEdit_->text());
    s->set("Compiler", compilerLineEdit_->text());
    s->set("Template Path", cTemplateLineEdit_->text());
    s->set("Download Tool", downloadLineEdit_->text());

    // download tab
    s->set("Serial Port", serialPortComboBox_->currentText());
}

void SettingsDialog::chooseExternalEditor()
{
    QString s = QFileDialog::getOpenFileName(editorLineEdit_->text(),
                                             QString::null,
                                             this, "open file dialog",
                                             tr("Select External Editor"));
    if (s != QString::null) {
        editorLineEdit_->setText(s);
    }
}

void SettingsDialog::chooseExternalCompiler()
{
    QString s = QFileDialog::getOpenFileName(compilerLineEdit_->text(),
                                             QString::null,
                                             this, "open file dialog",
                                             tr("Select External Compiler"));
    if (s != QString::null) {
        compilerLineEdit_->setText(s);
    }
}

void SettingsDialog::chooseTemplatePath()
{
  QString s = QFileDialog::getOpenFileName(cTemplateLineEdit_->text(),
                        QString("Source files (*.c *.h)"),
                        this, "get existingdirectory",
                        tr("Select C Source Template Path"));
  if (s !=QString::null) {
    cTemplateLineEdit_->setText(s);
  }
}

void SettingsDialog::chooseDownloadTool()
{
  QString s= QFileDialog::getOpenFileName(downloadLineEdit_->text(),
                     QString::null,
                     this, "open file dialog",
                     tr("Select External Download Tool"));
  if (s != QString::null) {
    downloadLineEdit_->setText(s);
  }
}
