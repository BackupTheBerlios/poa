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
 * $Id: settingsdialog.cpp,v 1.3 2003/08/21 15:59:57 squig Exp $
 *
 *****************************************************************************/
#include "settingsdialog.h"

#include "settings.h"

#include <qvariant.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*****************************************************************************
 * Constructs the dialog.
 */
SettingsDialog::SettingsDialog(QWidget* parent, const char* name, bool modal,
							   WFlags fl)
	: QTabDialog(parent, name, modal, fl)
{
    setCaption(tr("Settingss"));
	setApplyButton();
	setCancelButton();
	setOkButton();
	setHelpButton();

	this->addTab(createPathTab(), tr("Paths"));

    resize(sizeHint());//resize(400, 400); 

	connect(this, SIGNAL(applyButtonPressed()), this, SLOT(applySettings()));
}

/*****************************************************************************
 * Destroys the dialog.
 */
SettingsDialog::~SettingsDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*****************************************************************************
 * Returns the path tab.
 */
QWidget *SettingsDialog::createPathTab()
{
	QWidget *tab = new QWidget(this);
	QGridLayout *grid = new QGridLayout(tab, 4, 3, 5, 5);

	grid->addWidget(new QLabel(tr("External Editor"), tab), 0, 0);
	editorLineEdit = new QLineEdit(Settings::instance()->get("Editor"), tab);
	grid->addWidget(editorLineEdit, 0, 1);
    QPushButton *button = new QPushButton("...", tab);
	grid->addWidget(button, 0, 2);
	connect(button, SIGNAL(clicked()), this, SLOT(chooseExternalEditor()));

	grid->addWidget(new QLabel(tr("External Compiler"), tab), 1, 0);
	compilerLineEdit = new QLineEdit(Settings::instance()->get("Compiler"), tab);
	grid->addWidget(compilerLineEdit, 1, 1);
    button = new QPushButton("...", tab);
	grid->addWidget(button, 1, 2);
	connect(button, SIGNAL(clicked()), this, SLOT(chooseExternalCompiler()));

	grid->addWidget(new QLabel(tr("C Source Template Path"), tab), 2, 0);
	cTemplateLineEdit = new QLineEdit(Settings::instance()->
					     get("Template Path"), tab);
	grid->addWidget(cTemplateLineEdit, 2, 1);
	button = new QPushButton("...", tab);
	grid->addWidget(button, 2, 2);
	connect(button, SIGNAL(clicked()), this, SLOT(chooseTemplatePath()));

	grid->addWidget(new QLabel(tr("External Download Tool"), tab), 3, 0);
	downloadLineEdit = new QLineEdit(Settings::instance()->
					     get("Download Tool"), tab);
	grid->addWidget(downloadLineEdit, 3, 1);
	button = new QPushButton("...", tab);
	grid->addWidget(button, 3, 2);
	connect(button, SIGNAL(clicked()), this, SLOT(chooseDownloadTool()));

	return tab;
}

void SettingsDialog::applySettings()
{
	Settings *s = Settings::instance();
	s->set("Editor", editorLineEdit->text());
	s->set("Compiler", compilerLineEdit->text());
	s->set("Template Path", cTemplateLineEdit->text());
	s->set("Download Tool", downloadLineEdit->text());
}

void SettingsDialog::chooseExternalEditor()
{
	QString s = QFileDialog::getOpenFileName(editorLineEdit->text(),
											 QString::null,
											 this, "open file dialog",
											 tr("Select External Editor"));
	if (s != QString::null) {
		editorLineEdit->setText(s);
	}
}

void SettingsDialog::chooseExternalCompiler()
{
	QString s = QFileDialog::getOpenFileName(compilerLineEdit->text(),
											 QString::null,
											 this, "open file dialog",
											 tr("Select External Compiler"));
	if (s != QString::null) {
		compilerLineEdit->setText(s);
	}
}

void SettingsDialog::chooseTemplatePath()
{
  QString s = QFileDialog::getExistingDirectory(cTemplateLineEdit->text(),
						this, "get existingdirectory",
						tr("Select C Source Template Path"),
						TRUE );
  if (s !=QString::null) {
    cTemplateLineEdit->setText(s);
  }
}

void SettingsDialog::chooseDownloadTool()
{
  QString s= QFileDialog::getOpenFileName(downloadLineEdit->text(),
					 QString::null,
					 this, "open file dialog",
					 tr("Select External Download Tool"));
  if (s != QString::null) {
    downloadLineEdit->setText(s);
  }
}
