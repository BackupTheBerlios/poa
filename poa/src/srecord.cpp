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
 * $Id: srecord.cpp,v 1.4 2004/02/01 20:28:57 squig Exp $
 *
 *****************************************************************************/

#include "poaexception.h"
#include "srecord.h"

SRecord::SRecord(const char *line, const unsigned int length)
{
    if (length < minLength()) {
        throw PoaException(tr("Record is too short"));
    }

    if (line[0] == 'S' && line[1] == '1') {
        type_ = S1;
    }
    else {
        throw PoaException(tr("Invalid type"));
    }

    bool ok;
    count_ = QString::fromLatin1(&line[2], 2).toUInt(&ok, 16);
    if (!ok || count_ < 3 || count_ * 2 > length - 4) {
        throw PoaException(tr("Invalid count"));
    }

    address_ = QString::fromLatin1(&line[4], 4).toUInt(&ok, 16);
    if (!ok) {
        throw PoaException(tr("Invalid address"));
    }

    data_ = new unsigned char[count_ - 3];
    for (unsigned int i = 0; i < count_ - 3; i++) {
        data_[i] = QString::fromLatin1(&line[8 + i * 2], 2).toUInt(&ok, 16);
        if (!ok) {
            throw PoaException(tr("Invalid data"));
        }
    }

    checksum_
        = QString::fromLatin1(&line[4 + count_ * 2 - 2], 2).toUInt(&ok, 16);
    if (!ok) {
        throw PoaException(tr("Invalid checksum"));
    }
}

SRecord::~SRecord()
{
    delete data_;
}

unsigned int SRecord::address() const
{
    return address_;
}

unsigned int SRecord::checksum() const
{
    return checksum_;
}

const unsigned char *SRecord::data() const
{
    return data_;
}

unsigned int SRecord::dataSize() const
{
    return count_ - 3;
}

unsigned int SRecord::minLength()
{
    return 10;
}

SRecord::Record_Type SRecord::type() const
{
    return type_;
}
