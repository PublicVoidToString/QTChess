#include "PromotionWindow.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QPixmap>
#include <QLabel>

PromotionWindow::PromotionWindow(QWidget *parent)
    : QDialog(parent), promotionChoice(0)
{
    setWindowTitle("Promocja Pionka");


    QHBoxLayout *layout = new QHBoxLayout(this);


    QString imageNames[5] = {
        "",
        ":/images/WhiteQueen.png",
        ":/images/WhiteRook.png",
        ":/images/WhiteBishop.png",
        ":/images/WhiteKnight.png"
    };

    for (int i = 1; i <= 4; ++i) {
        QPushButton *button = new QPushButton(this);

        if (i > 0) {
            QPixmap pixmap(imageNames[i]);
            button->setIcon(QIcon(pixmap));
            button->setIconSize(QSize(100, 100));
        }

        button->setText("");
        button->setFixedSize(100, 100);

        layout->addWidget(button);

        connect(button, &QPushButton::clicked, this, [this, i]() {
            onPromotionSelected(i);
        });
    }

    connect(this, &QDialog::finished, this, &PromotionWindow::onDialogClosed);

    setLayout(layout);
}

int PromotionWindow::getPromotionChoice() const {
    return promotionChoice;
}

void PromotionWindow::onPromotionSelected(int choice) {
    promotionChoice = choice;
    accept();
}

void PromotionWindow::onDialogClosed(int result) {
    if (promotionChoice == 0 && result == QDialog::Rejected) {
        promotionChoice = 5;
    }
}
