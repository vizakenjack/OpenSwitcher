/********************************************************************************
** Form generated from reading UI file 'adminaccessinstructions.ui'
**
** Created by: Qt User Interface Compiler version 5.12.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINACCESSINSTRUCTIONS_H
#define UI_ADMINACCESSINSTRUCTIONS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_adminAccessInstructions
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;

    void setupUi(QDialog *adminAccessInstructions)
    {
        if (adminAccessInstructions->objectName().isEmpty())
            adminAccessInstructions->setObjectName(QString::fromUtf8("adminAccessInstructions"));
        adminAccessInstructions->resize(550, 634);
        buttonBox = new QDialogButtonBox(adminAccessInstructions);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(80, 600, 461, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(adminAccessInstructions);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 120, 521, 471));
        label->setPixmap(QPixmap(QString::fromUtf8(":/imgAddAdmin.png")));
        label->setScaledContents(true);
        label_2 = new QLabel(adminAccessInstructions);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(40, 20, 331, 16));
        label_3 = new QLabel(adminAccessInstructions);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(40, 50, 481, 21));
        label_4 = new QLabel(adminAccessInstructions);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(40, 70, 441, 16));
        label_5 = new QLabel(adminAccessInstructions);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(40, 90, 401, 16));

        retranslateUi(adminAccessInstructions);
        QObject::connect(buttonBox, SIGNAL(accepted()), adminAccessInstructions, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), adminAccessInstructions, SLOT(reject()));

        QMetaObject::connectSlotsByName(adminAccessInstructions);
    } // setupUi

    void retranslateUi(QDialog *adminAccessInstructions)
    {
        adminAccessInstructions->setWindowTitle(QApplication::translate("adminAccessInstructions", "Dialog", nullptr));
        label->setText(QString());
        label_2->setText(QApplication::translate("adminAccessInstructions", "Error - insufficient permissions to read hotkey state", nullptr));
        label_3->setText(QApplication::translate("adminAccessInstructions", "1. click the lock icon and enter you details in the box that appears.", nullptr));
        label_4->setText(QApplication::translate("adminAccessInstructions", "2. click the plus icon and then navigate to this app and click open.", nullptr));
        label_5->setText(QApplication::translate("adminAccessInstructions", "3. click the cheak mark next to the app.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class adminAccessInstructions: public Ui_adminAccessInstructions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINACCESSINSTRUCTIONS_H
