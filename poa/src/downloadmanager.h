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
 * $Id: downloadmanager.h,v 1.7 2004/02/01 17:18:48 squig Exp $
 *
 *****************************************************************************/

#ifndef POA_DOWNLOADMANAGER_H
#define POA_DOWNLOADMANAGER_H

#include "qextserialport/qextserialport.h"
class SRecord;

#include <qdir.h>
#include <qfile.h>
#include <qstring.h>
/**
 * Manages a portion of binary code.
 */
class DownloadManager : public QObject
{

    Q_OBJECT

public:

    DownloadManager(const QString &filename);
    ~DownloadManager();

    /**
     * @return true, if download was successful; false, if the
     * download was cancelled
     */
    bool download(const char* portName);

    unsigned long filesize() const;

    /**
     * @return true, if the run command was sent; false, if the
     * operation was cancelled
     */
    static bool run(const char* portName);

private:
    static void initializeAndOpen(QextSerialPort &port);

    void readFile(const QString &filename);

    QPtrList<SRecord> records_;
    unsigned long filesize_;
};

#endif // POA_DOWNLOADMANAGER_H
