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
 * $Id: settings.h,v 1.13 2003/09/08 13:35:04 squig Exp $
 *
 *****************************************************************************/
#ifndef SETTINGS_H
#define SETTINGS_H

#include <qapplication.h>
#include <qvariant.h>
#include <qobject.h>
#include <qsettings.h>

/**
 * Provides the user settings.
 *
 * Makes use of the singleton pattern.
 */
class Settings : public QObject
{
    Q_OBJECT

public:
    static const char* PREFIX;

    static Settings *instance();

    QString get(const QString &key, QString defaultValue = QString::null);
    bool getBool(const QString &key, bool defaultValue = FALSE);
    QFont getFont(const QString &key, QFont defaultValue = QApplication::font());
    int getNum(const QString &key, int defaultValue = 0);

    bool set(const QString &key, const QString &value);
    bool set(const QString &key, bool value);
    bool set(const QString &key, int value);

    QColor activatedColor();
    QColor defaultBrushColor();
    QColor defaultColor();
    int gridSize();
    void setGridSize(int gridSize);
    QColor selectedColor();
    bool snapToGrid();
    void setSnapToGrid(bool snapToGrid);

signals:
    void settingChanged(const QString &key);

    // the following settings emit an additional signal
    void gridSizeChanged(int gridSize);

protected:
    Settings();
    Settings(const Settings &);
    ~Settings();

private :
    static Settings* instance_;
};

#endif // SETTINGS_H
