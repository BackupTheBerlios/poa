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
 * $Id: codemanager.cpp,v 1.12 2004/01/12 21:59:48 squig Exp $
 *
 *****************************************************************************/


#include "codemanager.h"
#include "cpumodel.h"
#include "settings.h"
#include "processdialog.h"
#include "util.h"

#include <qfileinfo.h>
#include <qprocess.h>
#include <qstringlist.h>

/**
 * The singleton instance.
 */
CodeManager* CodeManager::instance_ = 0;

CodeManager::CodeManager()
{
}

CodeManager::~CodeManager()
{
}

CodeManager *CodeManager::instance()
{
    if (instance_ == 0) {
        instance_ = new CodeManager();
    }

    return instance_;
}

QString CodeManager::cpuPath(CpuModel *model)
{
    return QString("%1/CPU_%2_sdk")
        .arg(model->projectPath())
        .arg(model->cpuId());
}

QString CodeManager::sourcePath(CpuModel *model)
{
    return QString("%1/src").arg(cpuPath(model));
}

QString CodeManager::libPath(CpuModel *model)
{
    return QString("%1/lib").arg(cpuPath(model));
}

QString CodeManager::incPath(CpuModel *model)
{
    return QString("%1/inc").arg(cpuPath(model));
}

QString CodeManager::fileName(CpuModel *model)
{
    return QString("cpu_%1.c").arg(model->cpuId());
}

QString CodeManager::sourceFilePath(CpuModel *model)
{
    return QString("%1/%2")
        .arg(sourcePath(model))
        .arg(fileName(model));
}

int CodeManager::compile(CpuModel *model)
{
    Settings* s = Settings::instance();
    QFile source(sourceFilePath(model));

    if (!source.exists()) {
        save(model);
    }

    // create arguments
    QStringList args = QStringList::split(QChar(' '), s->compilerCmd());
    args.append(fileName(model));

    ProcessDialog *dialog = ProcessDialog::instance();
    return dialog->run(sourcePath(model), args);
}

bool CodeManager::edit(CpuModel *model)
{
    Settings* s = Settings::instance();
    QFile source(sourceFilePath(model));

    if (!source.exists()) {
        save(model);
    }

    QProcess *proc = new QProcess(this);
    QStringList args = QStringList::split(QChar(' '), s->editorCmd());
    args.append(source.name());

    proc->setArguments(args);
    proc->setWorkingDirectory(QDir(sourcePath(model)));

    return proc->launch("");
}

void CodeManager::save(CpuModel *model)
{
    Settings* s = Settings::instance();

    // check directory structure
    QDir cpuDir(cpuPath(model));
    QDir srcDir(sourcePath(model));
    QDir libDir(libPath(model));
    QDir incDir(incPath(model));

    if (!cpuDir.exists()) {
        cpuDir.mkdir(cpuDir.absPath());
    }
    if (!srcDir.exists()) {
        srcDir.mkdir(srcDir.absPath());
    }
    if (!libDir.exists()) {
        libDir.mkdir(libDir.absPath());
    }
    if (!incDir.exists()) {
        incDir.mkdir(incDir.absPath());
    }

    // check source file
    QFile source(sourceFilePath(model));
    if (!source.exists()) {
        // copy template
        QFile cpuTemplate(s->templatePath());
        if (cpuTemplate.exists())
        {
            Util::copyFile(&cpuTemplate, &source);
        }
        else {
            // TODO: Pop up error dialog.
        }
        // TODO: Copy template.
    }
    // else: don't care, the user has to save all changes
    // with his editor.
}

void CodeManager::remove(CpuModel *model)
{
    // check directory structure
    QDir cpuDir(cpuPath(model));

    if (!Util::removeDir(&cpuDir)) {
        // TODO: pop up error dialog.
    }
}
