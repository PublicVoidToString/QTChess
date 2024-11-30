/********************************************************************************
** Form generated from reading UI file 'chessboard.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHESSBOARD_H
#define UI_CHESSBOARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChessBoard
{
public:
    QPushButton *pushButton;

    void setupUi(QWidget *ChessBoard)
    {
        if (ChessBoard->objectName().isEmpty())
            ChessBoard->setObjectName("ChessBoard");
        ChessBoard->resize(990, 725);
        pushButton = new QPushButton(ChessBoard);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(0, 0, 71, 71));

        retranslateUi(ChessBoard);

        QMetaObject::connectSlotsByName(ChessBoard);
    } // setupUi

    void retranslateUi(QWidget *ChessBoard)
    {
        ChessBoard->setWindowTitle(QCoreApplication::translate("ChessBoard", "Form", nullptr));
        pushButton->setText(QCoreApplication::translate("ChessBoard", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChessBoard: public Ui_ChessBoard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHESSBOARD_H
