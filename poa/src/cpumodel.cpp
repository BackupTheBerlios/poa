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
 * $Id: cpumodel.cpp,v 1.13 2003/08/29 14:34:41 vanto Exp $
 *
 *****************************************************************************/
#include "cpumodel.h"
#include "abstractmodel.h"

#include <qdom.h> // used to provide serialization
#include <qtextstream.h>

#include "pinmodel.h"

CpuModel::CpuModel(QString type, QString description)
    : BlockModel(type, description)
{
    cpuId_ = 0;
    autoExecTime_ = 0;

    // FIX: remove
    PinModel *firstPin = new PinModel(QString("Input1"));
    addInputPin(firstPin);
    addInputPin(new PinModel("another Input"), firstPin);
    addOutputPin(new PinModel("Output"));
    // FIX: end remove
}

CpuModel::CpuModel(QDomElement cpuElem)
    : BlockModel(QString::null, QString::null)
{
    if (!cpuElem.isNull()) {
        deserialize(cpuElem);
    }
}

QString CpuModel::code() const
{
  return code_;
}

void CpuModel::setCode(const QString &code)
{
  code_ = code;
}

unsigned int CpuModel::cpuId()
{
    return cpuId_;
}

void CpuModel::setCpuId(const unsigned int cpuId)
{
    cpuId_ = cpuId;
}

bool CpuModel::autoExecTime()
{
    return autoExecTime_;
}

void CpuModel::setAutoExecTime(const bool autoExecTime)
{
    autoExecTime_ = autoExecTime;
}

/**
 * Produces the XML representation of this instance
 */
QDomElement CpuModel::serialize(QDomDocument *document)
{
    QDomElement root = BlockModel::serialize(document);
    root.setAttribute("block-type", "cpu");
    root.setAttribute("srcfile", code_);
    root.setAttribute("cpuid", (unsigned int) cpuId_);
    root.setAttribute("autotime", autoExecTime_ ? "true" : "false");
    return root;
}

void CpuModel::deserialize(QDomElement element) {
    BlockModel::deserialize(element);
    cpuId_ = (unsigned short) element.attribute("cpuid", "0").toUInt();
    // TRUE if value of autotime contains "TRUE" (case insensitive),
    // FALSE otherwise.
    autoExecTime_ =
        element.attribute("autotime", "true").contains("true", false);
    setCode(element.attribute("srcfile",""));
}
