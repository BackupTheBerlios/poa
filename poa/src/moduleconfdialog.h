/****************************************************************************
** Form interface generated from reading ui file 'moduleconfdialog.ui'
**
** Created: Tue Aug 19 15:36:18 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef MODULECONFDIALOG_H
#define MODULECONFDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QGroupBox;
class QLabel;
class QLineEdit;
class QListView;
class QListViewItem;
class QPushButton;
class QRadioButton;
class QSpinBox;

class ModuleConfDialog : public QDialog
{ 
    Q_OBJECT

public:
    ModuleConfDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ModuleConfDialog();

    QPushButton* compilePushButton;
    QPushButton* editCodePushButton;
    QPushButton* helpPushButton;
    QPushButton* cancelPushButton;
    QPushButton* addIoPushButton;
    QPushButton* removeIoPushButton;
    QPushButton* okPushButton;
    QListView* ioListView;
    QGroupBox* cpuGroupBox;
    QLabel* cpuNameTextLabel;
    QLineEdit* cpuNameLineEdit;
    QLabel* cpuClockTextLabel;
    QSpinBox* cpuClockSpinBox;
    QLabel* clockMsTextLabel;
    QButtonGroup* offsetButtonGroup;
    QLabel* offsetMsTextLabel;
    QRadioButton* offsetAutoCalcRadioButton;
    QRadioButton* offsetRadioButton;
    QSpinBox* offsetSpinBox;
    QButtonGroup* runtimeButtonGroup;
    QLabel* runtimeMsTextLabel;
    QRadioButton* runtimeAutoCalcRadioButton;
    QSpinBox* runtimeSpinBox;
    QRadioButton* runtimeRadioButton;


};

#endif // MODULECONFDIALOG_H
