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
    printPieces();
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
                chessTiles[buttonId]->setStyleSheet("background-color: #b9efbd;");
            } else {
                chessTiles[buttonId]->setStyleSheet("background-color: #7aad7e;");
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
    Click();
    printPieces();
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
        // Zmienna przechowująca nazwę figury do przypisania
        QString pieceName;

        // Sprawdzenie obecności figury białej
        if (board.whitePawns & (1ULL << i)) pieceName = "WhitePawn.png";
        else if (board.whiteRooks & (1ULL << i)) pieceName = "WhiteRook.png";
        else if (board.whiteKnights & (1ULL << i)) pieceName = "WhiteKnight.png";
        else if (board.whiteBishops & (1ULL << i)) pieceName = "WhiteBishop.png";
        else if (board.whiteQueens & (1ULL << i)) pieceName = "WhiteQueen.png";
        else if (board.whiteKings & (1ULL << i)) pieceName = "WhiteKing.png";

        // Sprawdzenie obecności figury czarnej
        else if (board.blackPawns & (1ULL << i)) pieceName = "Pawn.png";
        else if (board.blackRooks & (1ULL << i)) pieceName = "Rook.png";
        else if (board.blackKnights & (1ULL << i)) pieceName = "Knight.png";
        else if (board.blackBishops & (1ULL << i)) pieceName = "Bishop.png";
        else if (board.blackQueens & (1ULL << i)) pieceName = "Queen.png";
        else if (board.blackKings & (1ULL << i)) pieceName = "King.png";

        // Jeżeli znalazł się obrazek do przypisania, ustawiamy ikonę
        if (!pieceName.isEmpty()) {
            QPixmap piecePixmap(":/images/" + pieceName);
            if (!piecePixmap.isNull()) {
                piecePixmap = piecePixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                chessTiles[i]->setIcon(QIcon(piecePixmap));
                chessTiles[i]->setIconSize(QSize(100, 100));
            } else {
                qWarning() << "Nie udało się załadować obrazka figury: " << pieceName;
            }
        } else {
            // Jeśli nie ma figury na danym polu, zostawiamy ikonę pustą
            chessTiles[i]->setIcon(QIcon());
            chessTiles[i]->setIconSize(QSize(100, 100));
        }
    }
}
