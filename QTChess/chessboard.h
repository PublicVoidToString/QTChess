#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>

namespace Ui {
class ChessBoard;
}

class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoard(QWidget *parent = nullptr);
    ~ChessBoard();

private:
    Ui::ChessBoard *ui;
};

#endif // CHESSBOARD_H
