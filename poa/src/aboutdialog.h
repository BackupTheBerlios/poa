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
 * $Id: aboutdialog.h,v 1.1 2003/08/20 10:33:16 squig Exp $
 *
 *****************************************************************************/

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <qvariant.h>
#include <qtabdialog.h>

/*****************************************************************************
 * Provides the about dialog.
 * 
 * The about dialog has mutliple tabs. The first tab contains a short
 * description of the software, the second a list of authors and the
 * third tab shows the GPL.
 *
 * @author Steffen Pingel
 */
class AboutDialog : public QTabDialog
{
    Q_OBJECT
	
public:
	AboutDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE,
				WFlags flags = 0);
	~AboutDialog();

	void addFileTab(const QString &filename, const QString &label);
};

#endif // ABOUTDIALOG_H
