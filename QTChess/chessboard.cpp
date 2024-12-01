#include "chessboard.h"
#include "ui_chessboard.h"
#include <QResizeEvent>
#include <QMessageBox>
#include <QIcon>

ChessBoard::ChessBoard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChessBoard)
{
    chessTiles = (QPushButton**)malloc(sizeof(QPushButton*)*64);
    ui->setupUi(this);

    initBoard();
}

ChessBoard::~ChessBoard()
{
    free(chessTiles);
    delete ui;
}

void ChessBoard::initBoard()
{
    // Tworzymy layout w formie siatki
    QGridLayout *gridLayout = new QGridLayout(this);

    // Zmieniamy politykę rozmiarową dla layoutu, aby automatycznie dostosowywał rozmiar widgetów
    gridLayout->setSpacing(0); // Usuwamy odstępy między przyciskami

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            int buttonId = (7-row)*8 + col;
            chessTiles[buttonId] = new QPushButton(this);

            // Ustawiamy politykę rozmiaru, aby przyciski rozciągały się w ramach dostępnej przestrzeni
            chessTiles[buttonId]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            // Kolorowanie przycisków (pola szachownicy)
            if ((row + col) % 2 == 0) {
                chessTiles[buttonId]->setStyleSheet("background-color: white;");
            } else {
                chessTiles[buttonId]->setStyleSheet("background-color: gray;");
                chessTiles[buttonId]->setStyleSheet(
                    chessTiles[buttonId]->styleSheet() +
                    "QPushButton:hover { background-color: white; }"
                );
            }
            connect(chessTiles[buttonId], &QPushButton::clicked, [this, buttonId]() {
                handleButtonClick(buttonId);  // Przekazujemy identyfikator przycisku
            });
            // Dodajemy przycisk do layoutu w odpowiedniej pozycji
            gridLayout->addWidget(chessTiles[buttonId], row, col);
        }
    }

    // Ustawiamy layout dla widgetu
    this->setLayout(gridLayout);
}

void ChessBoard::handleButtonClick(int buttonId)
{
    QMessageBox::information(this, "Button Clicked", QString("Button ID: %1 clicked").arg(buttonId));
}

void ChessBoard::resizeEvent(QResizeEvent *event)
{
    // Obliczamy mniejszy z dwóch wymiarów okna
    int size = qMin(event->size().width(), event->size().height());

    // Ustawiamy nowy rozmiar okna, aby było kwadratowe
    this->resize(size, size);
}

void ChessBoard::printPieces(){

    for (int i = 0; i < 64; ++i) {
        // Ładujemy obrazek "figura.png" jako QPixmap
        QPixmap piecePixmap(":/images/figura.png");  // Zakładając, że obrazek jest w folderze zasobów

        // Sprawdzamy, czy obrazek został poprawnie załadowany
        if (!piecePixmap.isNull()) {
            // Ustawiamy obrazek jako ikonę przycisku
            chessTiles[i]->setIcon(QIcon(piecePixmap));

            // Opcjonalnie, dostosowujemy rozmiar ikony, aby pasowała do rozmiaru przycisku
            chessTiles[i]->setIconSize(chessTiles[i]->size());  // Ustawiamy ikonę na rozmiar przycisku
        } else {
            // Obsługuje sytuację, gdy obrazek nie zostanie załadowany
            qWarning() << "Nie udało się załadować obrazka figury.";
        }
    }
}
