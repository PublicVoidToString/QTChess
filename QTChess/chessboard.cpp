#include "chessboard.h"
#include "ui_chessboard.h"
#include "Board.h"
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
    printAllPieces();
}

ChessBoard::~ChessBoard()
{
    free(chessTiles);
    delete ui;
}

void ChessBoard::initBoard()
{
    board = new Board();

    // Główny layout w formie poziomego układu
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // Layout dla szachownicy
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(0); // Usuwamy odstępy między przyciskami

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            int buttonId = (7 - row) * 8 + col;
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
                handleButtonClick(buttonId); // Przekazujemy identyfikator przycisku
            });

            // Dodajemy przycisk do layoutu w odpowiedniej pozycji
            gridLayout->addWidget(chessTiles[buttonId], row, col);
        }
    }

    // Tworzymy przycisk "Cofnij ruch"
    QPushButton *undoButton = new QPushButton("Cofnij ruch", this);
    undoButton->setStyleSheet("background-color: black; color: white;");
    undoButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(undoButton, &QPushButton::clicked, this, &ChessBoard::undoMove);

    // Dodajemy layout szachownicy i przycisk do głównego layoutu
    mainLayout->addLayout(gridLayout);       // Dodajemy szachownicę
    mainLayout->addWidget(undoButton);      // Dodajemy przycisk z prawej strony

    // Ustawiamy główny layout dla widgetu
    this->setLayout(mainLayout);
}

void ChessBoard::undoMove(){
    if(board->prev!=nullptr){
        board=board->prev;
        printAllPieces();
        clearSelectedFromBoard();
    }
}

void ChessBoard::handleButtonClick(int buttonId)
{
    bool moved = board->pressedButton(buttonId);
    if(moved) {
        if (board->next != nullptr) {
            board = board->next;
            printAllPieces();
            clearSelectedFromBoard();
        } else {
            QMessageBox::warning(nullptr, "Warning", "No next board available!");
        }
    } else{
        printSelection();
        printAllPieces();
    }
}

void ChessBoard::resizeEvent(QResizeEvent *event)
{
    // Obliczamy mniejszy z dwóch wymiarów okna
    int size = qMin(event->size().width(), event->size().height());

    // Ustawiamy nowy rozmiar okna, aby było kwadratowe
    this->resize(size, size);
}

void ChessBoard::clearSelectedFromBoard(){
    for (int i = 0; i < 64; ++i) {
        int row = i / 8;
        int col = i % 8;

        // Zmieniamy kolor tła na podstawie naprzemiennych kolorów na planszy szachowej
        if ((row + col) % 2 == 0) {
            chessTiles[i]->setStyleSheet("background-color: #7aad7e;");
        } else {
            chessTiles[i]->setStyleSheet("background-color: #b9efbd;");
        }
    }

}

void ChessBoard::printAllPieces(){
    for (int i = 0; i < 64; ++i) {
        QString pieceName;
        // Sprawdzenie obecności figury białej
        if (board->whitePawns & (1ULL << i)) pieceName = "WhitePawn.png";
        else if (board->whiteRooks & (1ULL << i)) pieceName = "WhiteRook.png";
        else if (board->whiteKnights & (1ULL << i)) pieceName = "WhiteKnight.png";
        else if (board->whiteBishops & (1ULL << i)) pieceName = "WhiteBishop.png";
        else if (board->whiteQueens & (1ULL << i)) pieceName = "WhiteQueen.png";
        else if (board->whiteKings & (1ULL << i)) pieceName = "WhiteKing.png";

        // Sprawdzenie obecności figury czarnej
        else if (board->blackPawns & (1ULL << i)) pieceName = "Pawn.png";
        else if (board->blackRooks & (1ULL << i)) pieceName = "Rook.png";
        else if (board->blackKnights & (1ULL << i)) pieceName = "Knight.png";
        else if (board->blackBishops & (1ULL << i)) pieceName = "Bishop.png";
        else if (board->blackQueens & (1ULL << i)) pieceName = "Queen.png";
        else if (board->blackKings & (1ULL << i)) pieceName = "King.png";

        // Jeżeli znalazł się obrazek do przypisania, ustawiamy ikonę
        if (!pieceName.isEmpty()) {
            QPixmap piecePixmap(":/images/" + pieceName);
            if (!piecePixmap.isNull()) {
                piecePixmap = piecePixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                chessTiles[i]->setIcon(QIcon(piecePixmap));
                chessTiles[i]->setIconSize(QSize(80, 80));
            } else {
                qWarning() << "Nie udało się załadować obrazka figury: " << pieceName;
            }
        } else {
            // Jeśli nie ma figury na danym polu, zostawiamy ikonę pustą
            chessTiles[i]->setIcon(QIcon());
            chessTiles[i]->setIconSize(QSize(80, 80));
        }
    }
}

void ChessBoard::printSelection(){
    if(board->selected!=64) {
        chessTiles[board->selected]->setStyleSheet("background-color: #76b5ff;");
    }
    if(board->clearSelected!=64){
        int row = board->clearSelected / 8;
        int col = board->clearSelected % 8;

        // Zmieniamy kolor tła na podstawie naprzemiennych kolorów na planszy szachowej
        if ((row + col) % 2 == 0) {
            chessTiles[board->clearSelected]->setStyleSheet("background-color: #7aad7e;");
        } else {
            chessTiles[board->clearSelected]->setStyleSheet("background-color: #b9efbd;");
        }
    }
    for (int i = 0; i < 64; ++i) {
        QString pieceName;
        if (board->clearMoves & (1ULL << i)) {
            int row = i / 8;
            int col = i % 8;

            // Zmieniamy kolor tła na podstawie naprzemiennych kolorów na planszy szachowej
            if ((row + col) % 2 == 0) {
                chessTiles[i]->setStyleSheet("background-color: #7aad7e;");
            } else {
                chessTiles[i]->setStyleSheet("background-color: #b9efbd;");
            }
        }

        if(board->moves & (1ULL << i)) {
            chessTiles[i]->setStyleSheet("background-color: #f59e9e;");
        }
    }
}
