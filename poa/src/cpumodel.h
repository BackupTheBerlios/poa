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
 * $Id: cpumodel.h,v 1.28 2004/01/20 17:46:10 vanto Exp $
 *
 *****************************************************************************/
#ifndef POA_CPUMODEL_H
#define POA_CPUMODEL_H

#include "blockmodel.h"
#include "project.h"

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
     * Returns true, if the automatic runtime calculation should be used.
     */
    virtual bool autoRuntime() const;

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
     * Toggles the save source property.
     */
    void setSaveSource(const bool saveSource);

    /**
     * Serializes this instance to a xml subtree @param document the
     * main QDomDocument instance. Needed to create elements.
     */
    QDomElement serialize(QDomDocument *document);

    /**
     * Deserializes an xml subtree and sets this' properties
     */
    void deserialize(QDomElement element);

    /**
     * Sets the automatic runtime calculation.
     */
    virtual void setAutoRuntime(const bool autoRuntime);

    /**
     * Sets the project.
     * Used by CodeManager to save its sources.
     */
    void setProject(Project *project);

    /**
     * Returns the project.
     */
    Project *project() const;

    /**
     * Returns the tooltip text.
     */
    virtual QString tip();

 private:
    bool autoRuntime_;
    int cpuId_;
    Project *project_;
    bool saveSource_;

};

#endif
