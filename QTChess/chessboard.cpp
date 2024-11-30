#include "chessboard.h"
#include "ui_chessboard.h"

ChessBoard::ChessBoard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChessBoard)
{
    ui->setupUi(this);
}

ChessBoard::~ChessBoard()
{
    delete ui;
}
