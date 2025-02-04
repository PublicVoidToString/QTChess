#include "finalmessage.h"
#include "ui_finalmessage.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>

FinalMessage::FinalMessage(const QString &imageName, int length, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FinalMessage)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);

    // Oblicz wysokość okna na podstawie szerokości i proporcji 600:2550
    int height = length * 60 / 255;

    // Utworzenie kontenera layoutu
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Dodanie obrazu
    QPixmap piecePixmap(":/images/" + imageName);
    piecePixmap = piecePixmap.scaled(length, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QLabel *imageLabel = new QLabel(this);
    imageLabel->setPixmap(piecePixmap);
    layout->addWidget(imageLabel);


    // Ustawienie layoutu
    setLayout(layout);

    resize(length, height);

}

FinalMessage::~FinalMessage()
{
    delete ui;
}
