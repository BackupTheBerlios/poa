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
 * $Id: cpumodel.h,v 1.24 2004/01/12 19:13:57 squig Exp $
 *
 *****************************************************************************/
#ifndef POA_CPUMODEL_H
#define POA_CPUMODEL_H

#include "blockmodel.h"

#include <qdom.h>


class QCanvas;
class QCanvasItemList;

/**
 * A block that executes some source code everytime it is triggered.
 * A CpuModel-object can calculate its execution time based on its
 * source code.
 */
class CpuModel: public BlockModel
{
    Q_OBJECT

 public:
    /**
     * Creates a CpuModel instance for the library
     * <code>isProducer_</code> will be <code>true</code>.
     */
    CpuModel(QString type, QString description);

    /**
     * Creates a CpuModel instance for the project out of an xml subtree
     * <code>isProducer_</code> will be <code>false</code>.
     */
    CpuModel(QDomElement cpuElement);

    /**
     * Basic destructor.
     */
    ~CpuModel();

    /**
     * Returns <code>true</code> if this model is a library item
     * instance, <code>false</code> otherwise.
     */
    bool isProducer();

    /**
     * Returns the cpu id. This id is used to upload <code>code()</code> to
     * corresponding cpu on the cpld board
     */
    int cpuId();

    /**
     * Sets the cpu id
     */
    void setCpuId(const int cpuId);

    /**
     * Indicates if the automatic execution time calculator should be used
     */
    bool autoExecTime();

    /**
     * Toggles the usage of the automatic execution time calculator
     */
    void setAutoExecTime(const bool autoExecTime);

    /**
     * Toggles the automatic offset calculation.
     */
    void setAutoOffset(bool autoOffset);

    /**
     * Indicates if the automatic offset calculation should be used.
     */
    bool autoOffset();

    /**
     * Serializes this instance to a xml subtree
     * @param document the main QDomDocument instance. Needed to create elements
     */
    QDomElement serialize(QDomDocument *document);

    /**
     * Deserializes an xml subtree and sets this' properties
     */
    void deserialize(QDomElement element);

    /**
     * Sets the project path.
     * Used by CodeManager to save its sources.
     */
    void setProjectPath(QString path);

    /**
     * Returns the project path.
     */
    QString projectPath() const;

    /**
     * Returns the tooltip text.
     */
    virtual QString tip();

 private:
    QString path_;
    int cpuId_;
    bool autoExecTime_;
    bool autoOffset_;
    bool isProducer_;

 signals:
    void serialized(CpuModel *);

};

#endif
