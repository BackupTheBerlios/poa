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
 * $Id: settingsdialog.cpp,v 1.2 2003/08/21 13:07:43 papier Exp $
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
    resize(400, 400); 
    setCaption(tr("Settingss"));
	setApplyButton();
	setCancelButton();
	setOkButton();
	setHelpButton();

	this->addTab(createPathTab(), tr("Paths"));

	connect(this, SIGNAL(applyButtonPressed()), this, SLOT(applySettings()));
//     compilerPushButton = new QPushButton( this, "compilerPushButton" );
//     compilerPushButton->setGeometry( QRect( 420, 60, 20, 22 ) ); 
//     compilerPushButton->setText( trUtf8( "..." ) );

//     editorPushButton = new QPushButton( this, "editorPushButton" );
//     editorPushButton->setGeometry( QRect( 420, 20, 20, 22 ) ); 
//     editorPushButton->setText( trUtf8( "..." ) );

//     compilerLineEdit = new QLineEdit( this, "compilerLineEdit" );
//     compilerLineEdit->setGeometry( QRect( 160, 60, 250, 22 ) ); 

//     cTemplateLineEdit = new QLineEdit( this, "cTemplateLineEdit" );
//     cTemplateLineEdit->setGeometry( QRect( 160, 100, 250, 22 ) ); 

//     downloadLineEdit = new QLineEdit( this, "downloadLineEdit" );
//     downloadLineEdit->setGeometry( QRect( 160, 140, 250, 22 ) ); 

//     editorLineEdit = new QLineEdit( this, "editorLineEdit" );
//     editorLineEdit->setGeometry( QRect( 160, 20, 250, 22 ) ); 

//     editorTextLabel = new QLabel( this, "editorTextLabel" );
//     editorTextLabel->setGeometry( QRect( 20, 20, 90, 20 ) ); 
//     editorTextLabel->setText( trUtf8( "external editor" ) );

//     externalCompilerTextLabel = new QLabel( this, "externalCompilerTextLabel" );
//     externalCompilerTextLabel->setGeometry( QRect( 20, 60, 100, 20 ) ); 
//     externalCompilerTextLabel->setText( trUtf8( "external compiler" ) );

//     externalDownloadTextLabel = new QLabel( this, "externalDownloadTextLabel" );
//     externalDownloadTextLabel->setGeometry( QRect( 20, 140, 130, 20 ) ); 
//     externalDownloadTextLabel->setText( trUtf8( "external download tool" ) );

//     cTemplateTextLabel = new QLabel( this, "cTemplateTextLabel" );
//     cTemplateTextLabel->setGeometry( QRect( 20, 100, 133, 20 ) ); 
//     cTemplateTextLabel->setText( trUtf8( "C source template path" ) );

//     okPushButton = new QPushButton( this, "okPushButton" );
//     okPushButton->setGeometry( QRect( 250, 182, 91, 30 ) ); 
//     okPushButton->setText( trUtf8( "&OK" ) );
//     okPushButton->setDefault( TRUE );

//     cancelPushButton = new QPushButton( this, "cancelPushButton" );
//     cancelPushButton->setGeometry( QRect( 350, 182, 91, 30 ) ); 
//     cancelPushButton->setText( trUtf8( "&Cancel" ) );

//     helpPushButton = new QPushButton( this, "helpPushButton" );
//     helpPushButton->setGeometry( QRect( 20, 182, 91, 30 ) ); 
//     helpPushButton->setText( trUtf8( "&Help" ) );
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
	s->write();
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
