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
 * $Id: printmanager.h,v 1.4 2004/02/17 01:08:51 kilgus Exp $
 *
 *****************************************************************************/

#ifndef POA_PRINTMANAGER_H
#define POA_PRINTMANAGER_H

#include <qstring.h> 
#include <qprinter.h>

class QCanvas;

/**
 * Prints a canvas.
 */
class PrintManager
{
public:
	/**
	 * Create PrintManager object.
     * @param title String to be printed in the header.
	 */
    PrintManager(QString title);

    /**
     * Default destructor.
     */
    ~PrintManager();

    /**
     * Shows the print dialog.
     */
	bool setup();

    /**
     * Get device metrics of printer.
	 *
	 * Returns NULL if printer has not been setup.
     */
	QPaintDeviceMetrics *getMetrics();

    /**
     * Shows the print dialog if not already done and prints the canvas.
     */
    void print(QCanvas *canvas);

private:
	QString title_;
    QPrinter printer_;
	bool printerSetup_;
};

#endif // POA_PRINTMANAGER_H
