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
 * $Id: processdialog.h,v 1.2 2003/09/19 11:47:37 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_PROCESSDIALOG_H
#define POA_PROCESSDIALOG_H

#include <qdialog.h>
#include <qprocess.h>
#include <qpushbutton.h>
#include <qstringlist.h>
#include <qtextview.h>

/**
 * Process dialog singleton.
 */
class ProcessDialog : public QDialog
{
    Q_OBJECT

public:

    /**
     * ProcessDialog singleton instance.
     */
    static ProcessDialog *instance();

    /**
     * Run process.
     * @workDir the directory the process should be executed.
     * @process the QProcess instance.
     */
    int run(QString workDir, QStringList arguments);


protected:
    ProcessDialog();
    ~ProcessDialog();

private:
    QTextView *output_;
    QProcess *process_;
    QPushButton *okPushButton_;
    static ProcessDialog* instance_;

public slots:
    void readFromStdout();
    void readFromStderr();
    void enableOkButton();

};

#endif // POA_PROCESSDIALOG_H
