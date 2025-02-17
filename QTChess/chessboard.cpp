#include "chessboard.h"
#include "ui_chessboard.h"
#include "engine.h"
#include "promotionwindow.h"
#include <QResizeEvent>
#include <QMessageBox>
#include <QIcon>

#include "finalmessage.h"



void ChessBoard::handleButtonClick(int buttonId)
{
    if(gameEnded) return;
    short promotion = 0;
    bool isPromotion = Engine::isPromotion(board,buttonId,selected,moves);
    if (isPromotion) {
        PromotionWindow promoWindow;
        promoWindow.exec();
        promotion = promoWindow.getPromotionChoice();
        qWarning() << "promotion: " << promotion;
    }

    bool madePlayerMove = Engine::madePlayerMove(&board,buttonId,&selected,&moves,botDepth, promotion);

    reprintBoard();

    if (madePlayerMove && botDepth>1) {
        Engine::engineMove(&board, botDepth);
        reprintBoard();
    }

}

void ChessBoard::reprintBoard() {
    clearSelectedFromBoard();
    printAllPieces();
    switch(Engine::isGameEnded(board)){
    case 1:
        printOutcome("BlackWin.png");
        return;
    case 2:
        printOutcome("WhiteWin.png");
        return;
    case 3:
        printOutcome("Draw.png");
        return;
    }
    printSelection();
    QApplication::processEvents();
}

ChessBoard::ChessBoard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChessBoard)
{
    setAttribute(Qt::WA_DeleteOnClose);
    selected = 64;
    clearSelected = 64;
    windowSize=640;
    moves        = 0b0000000000000000000000000000000000000000000000000000000000000000;
    clearMoves   = 0b0000000000000000000000000000000000000000000000000000000000000000;
    botDepth     = 1;
    chessTiles = (QPushButton**)malloc(sizeof(QPushButton*)*64);
    ui->setupUi(this);
    initBoard();
    printAllPieces();
}

void ChessBoard::setDepth(char level){
    botDepth = level*2;
}

ChessBoard::~ChessBoard()
{
    free(chessTiles);
    for(;board->prev!=nullptr;board=board->prev);
    board->cutAllBranches();
    delete board;
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

    // Adding chessbouard and additional buttons to the background
    mainLayout->addLayout(gridLayout);

    // Setting main layout
    this->setLayout(mainLayout);
}


void ChessBoard::resizeEvent(QResizeEvent *event)
{
    if(!gameEnded){
        // Obliczamy mniejszy z dwóch wymiarów okna
        int size = qMin(event->size().width(), event->size().height());
        windowSize=size;
        // Ustawiamy nowy rozmiar okna, aby było kwadratowe
        printAllPieces();
    }
    this->resize(windowSize, windowSize);
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
    int size = windowSize/13;
    for (int i = 0; i < 64; ++i) {
        QString pieceName;
        // Sprawdzenie obecności figury białej
        if (board->getWhitePawns() & (1ULL << i)) pieceName = "WhitePawn.png";
        else if (board->getWhiteRooks() & (1ULL << i)) pieceName = "WhiteRook.png";
        else if (board->getWhiteKnights() & (1ULL << i)) pieceName = "WhiteKnight.png";
        else if (board->getWhiteBishops() & (1ULL << i)) pieceName = "WhiteBishop.png";
        else if (board->getWhiteQueens() & (1ULL << i)) pieceName = "WhiteQueen.png";
        else if (board->getWhiteKings() & (1ULL << i)) pieceName = "WhiteKing.png";

        // Sprawdzenie obecności figury czarnej
        else if (board->getBlackPawns() & (1ULL << i)) pieceName = "Pawn.png";
        else if (board->getBlackRooks() & (1ULL << i)) pieceName = "Rook.png";
        else if (board->getBlackKnights() & (1ULL << i)) pieceName = "Knight.png";
        else if (board->getBlackBishops() & (1ULL << i)) pieceName = "Bishop.png";
        else if (board->getBlackQueens() & (1ULL << i)) pieceName = "Queen.png";
        else if (board->getBlackKings() & (1ULL << i)) pieceName = "King.png";
        else pieceName = "empty.png";

        // Jeżeli znalazł się obrazek do przypisania, ustawiamy ikonę
        QPixmap piecePixmap(":/images/" + pieceName);
        if (!piecePixmap.isNull()) {
            piecePixmap = piecePixmap.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            chessTiles[i]->setIcon(QIcon(piecePixmap));
            chessTiles[i]->setIconSize(QSize(size, size));
        } else {
            qWarning() << "Nie udało się załadować obrazka figury: " << pieceName;
        }
    }
}

void ChessBoard::printSelection(){
    if(selected!=64) {
        chessTiles[selected]->setStyleSheet("background-color: #76b5ff;");
    }
    if(clearSelected!=64){
        int row = clearSelected / 8;
        int col = clearSelected % 8;

        // Zmieniamy kolor tła na podstawie naprzemiennych kolorów na planszy szachowej
        if ((row + col) % 2 == 0) {
            chessTiles[clearSelected]->setStyleSheet("background-color: #7aad7e;");
        } else {
            chessTiles[clearSelected]->setStyleSheet("background-color: #b9efbd;");
        }
    }
    for (int i = 0; i < 64; ++i) {
        QString pieceName;
        if (clearMoves & (1ULL << i)) {
            int row = i / 8;
            int col = i % 8;

            // Zmieniamy kolor tła na podstawie naprzemiennych kolorów na planszy szachowej
            if ((row + col) % 2 == 0) {
                chessTiles[i]->setStyleSheet("background-color: #7aad7e;");
            } else {
                chessTiles[i]->setStyleSheet("background-color: #b9efbd;");
            }
        }

        if(moves & (1ULL << i)) {
            chessTiles[i]->setStyleSheet("background-color: #f59e9e;");
        }
    }
}

void ChessBoard::printOutcome(QString outcome) {
    FinalMessage *popup = new FinalMessage(outcome, windowSize, this);
    popup->move(0, windowSize/3);
    gameEnded=true;
    popup->show();
}
