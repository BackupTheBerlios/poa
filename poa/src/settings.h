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
 * $Id: settings.h,v 1.5 2003/08/22 10:08:54 garbeam Exp $
 *
 *****************************************************************************/
#ifndef SETTINGS_H
#define SETTINGS_H

#include <qvariant.h>
#include <qobject.h>
#include <qsettings.h>

/*****************************************************************************
 * Provides the user settings.
 * 
 * Makes use of the singleton pattern.
 *
 * @author Steffen Pingel
 */
class Settings : QObject
{ 
    Q_OBJECT
    
public:
    static const char* prefix;

    static Settings *instance();

    QString get(const QString &key);
    bool getBool(const QString &key);
    int getNum(const QString &key);

    bool set(const QString &key, const QString &value);
    bool set(const QString &key, bool value);
    bool set(const QString &key, int value);

    // the following settings emit an additional signal
    int gridSize();
    bool snapToGrid();
    void setGridSize(int gridSize);
    void setSnapToGrid(bool snapToGrid);

signals:
    void settingChanged(const QString &key);
    void snapToGridChanged(bool snapToGrid);
    void gridSizeChanged(int gridSize);
   
private :
    Settings();
    Settings(const Settings &);
    ~Settings();

    static Settings* instance_;

    void setDefault(QSettings* settings, const QString &key, 
                    const QString &value);
};

#endif // SETTINGS_H
