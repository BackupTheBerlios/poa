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
 * $Id: mdiwindow.cpp,v 1.23 2004/01/21 23:38:21 squig Exp $
 *
 *****************************************************************************/

#include "mdiwindow.h"

#include "canvasview.h"
#include "gridcanvas.h"
#include "poa.h"
#include "project.h"
#include "mainwindow.h"
#include "util.h"

#include <qvariant.h>
#include <qcanvas.h>
#include <qlayout.h>
#include <qaction.h>
#include <qframe.h>
#include <qimage.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qpoint.h>
#include <qsize.h>
#include <qwmatrix.h>

MdiWindow::MdiWindow(CanvasView *view, QWidget* parent,
                     const char* name, WFlags f)
    : QMainWindow(parent, name, f), view_(view)
{
    zoomLevel_ = 1.0;

    view_->reparent(this, 0, QPoint());
    view->setFrameStyle(QFrame::NoFrame);

    setCentralWidget(view_);
    setCaption(view->project()->name()+" - "+view->canvas()->name());
    setIcon(Util::findIcon("document.xpm"));
    resize(sizeHint());
}

MdiWindow::~MdiWindow()
{
    delete view_->project();
}

CanvasView *MdiWindow::view() const
{
    return view_;
}

void MdiWindow::closeEvent(QCloseEvent *e)
{
    if (view()->project()->isModified()) {
        int ans = QMessageBox::information
            (this, "POA", "The document contains unsaved changes\n"
             "Do you want to save the changes before exiting?",
             "&Save", "&Discard", "Cancel",
             0, // Enter == button 0
             2); // Escape == button 2
        switch(ans) {
        case 0: // Save clicked or Alt+S pressed or Enter pressed.
            MainWindow::instance()->fileSave();
            e->accept();
            break;
        case 1: // Discard clicked or Alt+D pressed
            e->accept();
            break;
        case 2: // Cancel clicked or Alt+C pressed or Escape pressed
            e->ignore();
            break;
        }
    }
    else {
        e->accept();
    }
}

/**
 * Supplies resizing the canvas of this view.
 * Used also from MainWindow.
 */
void MdiWindow::resizeCanvas()
{

    QSize viewSize = view()->size();
    viewSize /= zoomLevel_;
    QSize canvasSize = view()->canvas()->size();

    // Resize the canvas only if its smaller than the current
    // canvas view.
    if ((canvasSize.width() < viewSize.width()) ||
            (canvasSize.height() < viewSize.height()))
    {
        view()->canvas()->resize(viewSize.width(), viewSize.height());
    }

}

void MdiWindow::resizeEvent(QResizeEvent *e)
{
    e = 0; // don't care
    resizeCanvas();
}

void MdiWindow::setZoomLevel(double zoomLevel)
{
    if (zoomLevel != 0 && zoomLevel != zoomLevel_) {
        QWMatrix m = view()->worldMatrix();
        double diff = zoomLevel / zoomLevel_;
        zoomLevel_ = zoomLevel;
        m.scale(diff, diff);
        view()->setWorldMatrix(m);
        view()->update();
    }
}

double MdiWindow::zoomLevel()
{
    return zoomLevel_;
}

void MdiWindow::setModified(bool modified)
{
    setCaption(view_->project()->name() + " - "
               + view_->canvas()->name() + QString((modified) ? "*" : ""));

}
