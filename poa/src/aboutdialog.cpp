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
 * $Id: aboutdialog.cpp,v 1.6 2003/10/01 15:55:17 squig Exp $
 *
 *****************************************************************************/
#include "aboutdialog.h"

#include "poa.h"

#include <qvariant.h>
#include <qfile.h>
#include <qimage.h>
#include <qtextedit.h>

/**
 * Constructs the dialog.
 */
AboutDialog::AboutDialog(QWidget* parent, const char* name, bool modal,
                         WFlags flags)
    : QTabDialog(parent, name, modal, flags)
{
    this->setCaption(tr("About POA"));
    this->setOkButton();
    this->resize(400, 400);

    // create the mime source for the img tag in about.html
    QMimeSourceFactory::defaultFactory()->setImage
        ("poalogo", QImage(ICON_PATH + "poa-logo.png"));

    addFileTab("about.html", tr("About"));
    addFileTab("AUTHORS", tr("Authors"));
    addFileTab("COPYING", tr("License Agreement"));
}

/**
 * Destroys the dialog.
 */
AboutDialog::~AboutDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/**
 * Adds a tab to the dialog that contains the content of file.
 */
void AboutDialog::addFileTab(const QString &filename, const QString &label)
{
    QTextEdit* page = new QTextEdit(this);
    page->setReadOnly(TRUE);
    page->setWordWrap(QTextEdit::NoWrap);

    QFile file(filename);
    if (file.open(IO_ReadOnly)) {
        QTextStream ts(&file);
        page->setText(ts.read());
        file.close();
    }
    this->addTab(page, label);
}
