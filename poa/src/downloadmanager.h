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
 * $Id: downloadmanager.h,v 1.3 2004/01/09 18:08:39 papier Exp $
 *
 *****************************************************************************/

#ifndef POA_DOWNLOADMANAGER_H
#define POA_DOWNLOADMANAGER_H

#include "cpumodel.h"

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

    /**
     * DownloadManager singleton instance.
     */
    static DownloadManager *instance();
    
    /**
     * returns <code>true</code> if the download of the srec file was
     * successful.
     */
    bool download(QString filename, const char* portName);

protected:
    DownloadManager();
    ~DownloadManager();


private:
    static DownloadManager* instance_;

public slots:

signals:
    void increaseProgressBar();
    void setProgressBarLength(int totalSteps); 


};

#endif // POA_DOWNLOADMANAGER_H
