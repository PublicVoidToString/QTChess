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

    // Main layout (where later the grid is added)
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // Chessboard grid
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(0); // Usuwamy odstępy między przyciskami

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            int buttonId = (7 - row) * 8 + col;
            chessTiles[buttonId] = new QPushButton(this);

            // Expanding policy - allow only for much shrinking as content allows for
            chessTiles[buttonId]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            // Painting the chessboard
            if ((row + col) % 2 == 0) {
                chessTiles[buttonId]->setStyleSheet("background-color: #b9efbd;");
            } else {
                chessTiles[buttonId]->setStyleSheet("background-color: #7aad7e;");
            }

            // QT slot action - event based
            connect(chessTiles[buttonId], &QPushButton::clicked, [this, buttonId]() {
                handleButtonClick(buttonId);
            });

            // Adding buttons to the grid layout
            gridLayout->addWidget(chessTiles[buttonId], row, col);
        }
    }

    // Button "Cofnij ruch"
    QPushButton *undoButton = new QPushButton("Cofnij ruch", this);
    undoButton->setStyleSheet("background-color: black; color: white;");
    undoButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(undoButton, &QPushButton::clicked, this, &ChessBoard::undoMove);

    // Adding chessbouard and additional buttons to the background
    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(undoButton);

    // Setting main layout
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
    // calculate size
    int size = qMin(event->size().width(), event->size().height());

    // resizing
    this->resize(size, size);
}

void ChessBoard::clearSelectedFromBoard(){
    for (int i = 0; i < 64; ++i) {
        int row = i / 8;
        int col = i % 8;

        // changing colors - board TODO - changing colors multiple times, should be a function
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
        // checking white piece presence
        if (board->getWhitePawns() & (1ULL << i)) pieceName = "WhitePawn.png";
        else if (board->getWhiteRooks() & (1ULL << i)) pieceName = "WhiteRook.png";
        else if (board->getWhiteKnights() & (1ULL << i)) pieceName = "WhiteKnight.png";
        else if (board->getWhiteBishops() & (1ULL << i)) pieceName = "WhiteBishop.png";
        else if (board->getWhiteQueens() & (1ULL << i)) pieceName = "WhiteQueen.png";
        else if (board->getWhiteKings() & (1ULL << i)) pieceName = "WhiteKing.png";
        // checking black piece presence
        else if (board->getBlackPawns() & (1ULL << i)) pieceName = "Pawn.png";
        else if (board->getBlackRooks() & (1ULL << i)) pieceName = "Rook.png";
        else if (board->getBlackKnights() & (1ULL << i)) pieceName = "Knight.png";
        else if (board->getBlackBishops() & (1ULL << i)) pieceName = "Bishop.png";
        else if (board->getBlackQueens() & (1ULL << i)) pieceName = "Queen.png";
        else if (board->getBlackKings() & (1ULL << i)) pieceName = "King.png";

        // Setting correct icon
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
            // In case of empty
            chessTiles[i]->setIcon(QIcon());
            chessTiles[i]->setIconSize(QSize(80, 80));
        }
    }
}

void ChessBoard::printSelection(){
    if(board->getSelected()!=64) {
        chessTiles[board->getSelected()]->setStyleSheet("background-color: #76b5ff;");
    }
    if(board->getClearSelected()!=64){
        int row = board->getClearSelected() / 8;
        int col = board->getClearSelected() % 8;

        if ((row + col) % 2 == 0) {
            chessTiles[board->getClearSelected()]->setStyleSheet("background-color: #7aad7e;");
        } else {
            chessTiles[board->getClearSelected()]->setStyleSheet("background-color: #b9efbd;");
        }
    }
    for (int i = 0; i < 64; ++i) {
        QString pieceName;
        if (board->getClearMoves() & (1ULL << i)) {
            int row = i / 8;
            int col = i % 8;

            if ((row + col) % 2 == 0) {
                chessTiles[i]->setStyleSheet("background-color: #7aad7e;");
            } else {
                chessTiles[i]->setStyleSheet("background-color: #b9efbd;");
            }
        }

        if(board->getMoves() & (1ULL << i)) {
            chessTiles[i]->setStyleSheet("background-color: #f59e9e;");
        }
    }
}
