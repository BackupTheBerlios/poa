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
 * $Id: settings.h,v 1.19 2004/01/12 17:06:24 squig Exp $
 *
 *****************************************************************************/
#ifndef SETTINGS_H
#define SETTINGS_H

#include <qapplication.h>
#include <qstringlist.h>
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
    static Settings *instance();

    QString get(const QString &key, QString defaultValue = QString::null);
    bool getBool(const QString &key, bool defaultValue = FALSE);
    QFont getFont(const QString &key, QFont defaultValue = QApplication::font());
    int getNum(const QString &key, int defaultValue = 0);
    QStringList getStrings(const QString &key, bool *ok = 0);

    bool set(const QString &key, const char *value);
    bool set(const QString &key, const QString &value);
    bool set(const QString &key, bool value);
    bool set(const QString &key, int value);
    bool set(const QString &key, const QStringList &value);

    QColor activatedColor();
    void addToRecentProjects(QString filename);
    QColor defaultBrushColor();
    QColor defaultColor();
    int gridSize();
    QColor selectedColor();
    bool snapToGrid();
    bool showGrid();

    QString templatePath();
    QString terminalCmd();
    QString compilerCmd();
    QString editorCmd();

public slots:
    void setGridSize(int gridSize);
    void setSnapToGrid(bool snapToGrid);
    void setShowGrid(bool showGrid);

signals:
    void settingChanged(const QString &key);

    // the following settings emit an additional signal
    void gridSizeChanged(int gridSize);
    void recentProjectsChanged();
    void showGridChanged(bool showGrid);

protected:
    Settings(QString prefix);
    Settings(const Settings &);
    ~Settings();

private :
    static Settings* instance_;

    /**
     * A common prefix used for all setting keys.
     */
    QString prefix;

    /**
     * Methods of {@link SettingsTest} need access the protected constructor.
     */
    friend class SettingsTest;
};

#endif // SETTINGS_H
