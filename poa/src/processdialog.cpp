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
 * $Id: processdialog.cpp,v 1.2 2003/09/19 11:47:37 garbeam Exp $
 *
 *****************************************************************************/

#include "processdialog.h"

#include <qmessagebox.h>
#include <qlayout.h>
#include <qstringlist.h>

/**
 * The singleton instance.
 */
ProcessDialog* ProcessDialog::instance_ = 0;

ProcessDialog::ProcessDialog()
{
    // Layout
    setCaption("POA Terminal");
    QBoxLayout *vBox = new QVBoxLayout(this);
    output_ = new QTextView(this);
    vBox->addWidget(output_);
    okPushButton_ = new QPushButton(tr("&OK"), this);
    connect(okPushButton_, SIGNAL(clicked()), this, SLOT(reject()) );
    vBox->addWidget(okPushButton_);
    resize( 500, 500 );

    process_ = new QProcess();
    connect(process_, SIGNAL(readyReadStdout()), this, SLOT(readFromStdout()));
    connect(process_, SIGNAL(readyReadStderr()), this, SLOT(readFromStderr()));
    connect(process_, SIGNAL(processExited()), this, SLOT(enableOkButton()));
}

ProcessDialog::~ProcessDialog()
{
}

ProcessDialog *ProcessDialog::instance()
{
    if (instance_ == 0) {
        instance_ = new ProcessDialog();
    }

    return instance_;
}

int ProcessDialog::run(QString workDir, QStringList arguments) 
{

    output_->clear();
    okPushButton_->setEnabled(false);
    process_->clearArguments();
    process_->setWorkingDirectory(workDir);
    process_->setArguments(arguments);

    if ( !process_->start() ) {
        // error handling
        QMessageBox::critical( 0,
                tr("Fatal error"),
                tr("Could not start the command."),
                tr("Quit"));
    }

    exec();
    return process_->exitStatus();
}

void ProcessDialog::readFromStdout()
{
    // Read and process the data.
    // Bear in mind that the data might be output in chunks.
    output_->append(process_->readStdout());
}

void ProcessDialog::readFromStderr()
{
    // Read and process the data.
    // Bear in mind that the data might be output in chunks.
    output_->append(process_->readStderr());
}

void ProcessDialog::enableOkButton()
{
    okPushButton_->setEnabled(true);
}
