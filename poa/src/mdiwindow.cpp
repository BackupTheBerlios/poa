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
 * $Id: mdiwindow.cpp,v 1.2 2003/08/20 16:09:42 squig Exp $
 *
 *****************************************************************************/

#include "mdiwindow.h"

#include "layoutcanvas.h"

#include <qvariant.h>
#include <qcanvas.h>
#include <qlayout.h>
#include <qaction.h>
#include <qimage.h>
#include <qpixmap.h>


MdiWindow::MdiWindow(LayoutCanvas* canvas, QWidget* parent, const char* name, WFlags f)
	: QMainWindow(parent, name, f)
{
	view_ = new QCanvasView(canvas, this);
    setCentralWidget(view_);
}

MdiWindow::~MdiWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

void MdiWindow::closeEvent( QCloseEvent *e )
{
	e->accept();
}

LayoutCanvas *MdiWindow::canvas()
{
	return (LayoutCanvas *)view_->canvas();
}

void MdiWindow::load( const QString& fn )
{
    QString bla = fn;
}

void MdiWindow::save()
{
}

void MdiWindow::saveAs()
{
}
