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
 * $Id: downloadmanager.h,v 1.12 2004/06/04 13:55:35 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_DOWNLOADMANAGER_H
#define POA_DOWNLOADMANAGER_H

#include "qextserialport/qextserialport.h"
#include "srecord.h"

#include <qdir.h>
#include <qfile.h>
#include <qstring.h>
class QProgressDialog;

/**
 * This class can download compiled cpu source code to the CPLD. The
 * download is done through the serial port.
 *
 * The download manager also has support to trigger the downloaded
 * code by sending a load command through the serial port.
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
    bool download(const char* portName, QProgressDialog *monitor = 0);

    unsigned long filesize() const;

    /**
     * @return true, if the run command was sent; false, if the
     * operation was cancelled
     */
    static bool run(const char* portName);

    static void sendChar(QextSerialPort &port, const unsigned char data);

	static QString *lastString_;

private:
    static void initializeAndOpen(QextSerialPort &port);

    void readFile(const QString &filename);

	QPtrList<QString> strings_;
    QPtrList<SRecord> records_;
    unsigned long filesize_;
};

#endif // POA_DOWNLOADMANAGER_H
