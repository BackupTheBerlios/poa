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
 * $Id: printmanager.cpp,v 1.6 2004/02/17 01:08:51 kilgus Exp $
 *
 *****************************************************************************/

#include "printmanager.h"
#include "settings.h"

#include <qcanvas.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qprinter.h>
#include <qdatetime.h>

PrintManager::PrintManager(QString title)
{
    title_ = title;
    title_.append(" - ");
    title_.append(QDateTime::currentDateTime().toString());
}

PrintManager::~PrintManager()
{
}

bool PrintManager::setup()
{
    // doesnt work in GS-Pool
    //printer.setOptionEnabled(QPrinter::PrintSelection, false);
    //printer.setOptionEnabled(QPrinter::PrintPageRange, false);
    printerSetup_ = printer_.setup();
    return printerSetup_;
}

QPaintDeviceMetrics *PrintManager::getMetrics()
{
    if (!printerSetup_) return NULL;
    return new QPaintDeviceMetrics(&printer_);
}

void PrintManager::print(QCanvas *canvas)
{

    if (!printerSetup_)
        setup();
    if (printerSetup_) {
        // disable grid for printing
        bool oldGridState = Settings::instance()->showGrid();
        Settings::instance()->setShowGrid(false);

        QPainter painter(&printer_);
        QFont f = QApplication::font();
        QFontMetrics fm(f);

        int fontHeight = fm.height();
        int headerSpace = 2 * fontHeight;   // Y space to reserve for header

        QPaintDeviceMetrics metrics(&printer_);
        int pageWidth = metrics.width();
        int pageHeight = metrics.height() - headerSpace;

        for (int copies = 0; copies < printer_.numCopies(); copies++) {
            int page = 1;
            int y = 0;
            while (y < canvas->height()) {
                int x = 0;
                while (x < canvas->width()) {
                    QRect rect(x, y, pageWidth, pageHeight);

                    QCanvasItemList items = canvas->collisions(rect);
                    if (items.empty()) {
                        x += pageWidth;
                        painter.translate(-pageWidth, 0);
                        continue;
                    }

                    if (!(x == 0 && y == 0)) {
                        printer_.newPage();
                        page++;
                    }

                    // Draw header
                    QString pageString("Page ");
                    pageString.append(QString::number(page));
                    painter.setFont(f);
                    painter.setPen(QPen(Qt::black));
                    painter.drawText(x, y + fontHeight, title_);
                    painter.drawText(x + pageWidth - fm.width(pageString),
                        y + fontHeight, pageString);

                    // Draw graphics (clip to leave header space out)
                    painter.setClipRect(0, headerSpace, pageWidth, pageHeight);
                    painter.translate(0, headerSpace);
                    canvas->drawArea(rect, &painter, false);
                    painter.translate(0, -headerSpace);
                    painter.setClipping(false);

                    x += pageWidth;
                    painter.translate(-pageWidth, 0);
                }
                y += pageHeight;
                // reverse x-translation
                painter.translate(x, -pageHeight);
            }
        }
        painter.end();

        // restore grid setting
        Settings::instance()->setShowGrid(oldGridState);
    }
}
