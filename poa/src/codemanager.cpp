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
 * $Id: codemanager.cpp,v 1.5 2003/09/17 15:38:52 garbeam Exp $
 *
 *****************************************************************************/


#include "codemanager.h"
#include "cpumodel.h"
#include "settings.h"

#include <qfileinfo.h>
#include <qprocess.h>

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

QString CodeManager::sourcePath(CpuModel *model)
{
    return QString("%1/CPU_%2_sdk")
        .arg(*(model->projectPath()))
        .arg(model->cpuId());
}

QString CodeManager::sourceFilePath(CpuModel *model)
{
    return QString("%1/src/cpu_%2.c")
        .arg(sourcePath(model))
        .arg(model->cpuId());
}

int CodeManager::compile(CpuModel *model)
{
    Settings* s = Settings::instance();

    QProcess *proc = new QProcess(this);
    proc->addArgument(s->terminalCmd());
    proc->addArgument(s->compilerCmd());
    proc->addArgument(sourcePath(model));
    proc->setWorkingDirectory(QDir(*(model->projectPath())));

    proc->launch("");

    return proc->exitStatus();
}

void CodeManager::edit(CpuModel *model)
{
    Settings* s = Settings::instance();
    QFile source(sourceFilePath(model));

    if (!source.exists()) {
        save(model);
    }

    QProcess *proc = new QProcess(this);
    proc->addArgument(s->editorCmd());
    proc->addArgument(source.name());
    proc->setWorkingDirectory(QDir(sourcePath(model)));

    proc->launch("");

}

bool CodeManager::copyFile(QFile *source, QFile *target)
{
    QStringList lines;
    if (source->open(IO_ReadOnly) && target->open(IO_WriteOnly)) {

        QTextStream istream(source);

        QString line;
        while (!istream.eof()) {
            line = istream.readLine();
            lines += line;
        }
        source->close();

        QTextStream ostream(target);
        for (QStringList::Iterator it = lines.begin(); it != lines.end(); ++it) {
            ostream << *it << "\n";
        }
        target->close();

        return true;
    }

    return false;
}

void CodeManager::save(CpuModel *model)
{
    Settings* s = Settings::instance();

    // check directory structure
    QDir cpuDir(sourcePath(model));
    QDir srcDir(sourcePath(model) + QString("/src"));
    QDir libDir(sourcePath(model) + QString("/lib"));
    QDir incDir(sourcePath(model) + QString("/inc"));

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
            copyFile(&cpuTemplate, &source);
        }
        else {
            // TODO: Pop up error dialog.
        }
        // TODO: Copy template.
    }
    // Else: Don't care, the user has to save all changes
    // with his editor.
}

bool CodeManager::removeDir(QDir *subDir)
{
    const QFileInfoList *filist = subDir->entryInfoList();
    QFileInfoListIterator it(*filist);
    QFileInfo *fi;
    while ((fi = it.current()) != 0) {
        ++it;
        if (fi->isDir()) {
            QDir *dir = new QDir(fi->filePath());
            if (!removeDir(dir))
            {
                delete dir; // free
                // error, directory is not empty!
                return false;
            }
            delete dir; // else
        }
        else {
            if (!QFile(fi->filePath()).remove())
            {
                return false;
            }
        }
    }

    return subDir->rmdir(subDir->absPath());
}

void CodeManager::remove(CpuModel *model)
{
    // check directory structure
    QDir cpuDir(sourcePath(model));

    if (!removeDir(&cpuDir)) {
        // TODO: pop up error dialog.
    }
}
