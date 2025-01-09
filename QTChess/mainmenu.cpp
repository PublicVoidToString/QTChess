#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "chessboard.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)
{
    level = 1;  // Ustawienie początkowego poziomu trudności na 1
    ui->setupUi(this);

    // Tworzenie przycisków
    QPushButton *playAgainstPlayerButton = new QPushButton("Play Against Player", this);
    QPushButton *playAgainstBotButton = new QPushButton("Play Against Bot", this);

    // Tworzenie slidera do ustawiania poziomu trudności
    QLabel *levelLabel = new QLabel("Difficulty Level: 1", this);
    QSlider *levelSlider = new QSlider(Qt::Horizontal, this);
    levelSlider->setRange(1, 5);  // Zakres poziomu od 1 do 5
    levelSlider->setValue(level); // Ustawienie początkowego poziomu

    // Funkcja zmieniająca kolor suwaka na podstawie wartości
    auto updateSliderColor = [levelSlider](int value) {
        QString color;
        if (value == 1) {
            color = "lime";
        } else if (value == 2) {
            color = "green";
        } else if (value == 3) {
            color = "yellow";
        } else if (value == 4) {
            color = "red";
        } else if (value == 5) {
            color = "black";
        }

        levelSlider->setStyleSheet(QString(
                                       "QSlider::groove:horizontal {"
                                       "    background: #ddd;"
                                       "    height: 8px;"
                                       "    border-radius: 4px;"
                                       "}"
                                       "QSlider::handle:horizontal {"
                                       "    background: %1;"
                                       "    border: 1px solid #777;"
                                       "    width: 150%;"
                                       "    height: 20px;"
                                       "    border-radius: 10px;"
                                       "    margin: -6px 0;"
                                       "}"
                                       ).arg(color));
    };

    // Aktualizacja etykiety i koloru suwaka, gdy poziom zmienia się
    connect(levelSlider, &QSlider::valueChanged, [levelLabel, updateSliderColor](int value) {
        levelLabel->setText("Difficulty Level: " + QString::number(value));
        updateSliderColor(value); // Zmiana koloru suwaka
    });

    // Ustawienie większej czcionki dla przycisków i etykiety
    playAgainstPlayerButton->setStyleSheet("font-size: 18px;");
    playAgainstBotButton->setStyleSheet("font-size: 18px;");
    levelLabel->setStyleSheet("font-size: 18px;");

    // Layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(playAgainstPlayerButton);
    layout->addWidget(playAgainstBotButton);
    layout->addWidget(levelLabel);  // Dodanie etykiety
    layout->addWidget(levelSlider); // Dodanie slidera

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    playAgainstPlayerButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    playAgainstBotButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Połączenia przycisków
    connect(playAgainstPlayerButton, &QPushButton::clicked, this, &MainMenu::openChessBoard);
    connect(playAgainstBotButton, &QPushButton::clicked, this, &MainMenu::openChessBoardBot);

    // Połączenie, które aktualizuje poziom w obiekcie chessBoard
    connect(levelSlider, &QSlider::valueChanged, this, [this](int value) {
        level = value; // Zmieniamy poziom trudności
    });

    // Początkowa zmiana koloru suwaka
    updateSliderColor(level);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::openChessBoard()
{
    // Tworzymy obiekt ChessBoard i go pokazujemy
    chessBoard = new ChessBoard();
    chessBoard->show();
}

void MainMenu::openChessBoardBot()
{
    // Tworzymy obiekt ChessBoard z poziomem trudności
    chessBoard = new ChessBoard();
    chessBoard->setDepth(level);
    chessBoard->show();
}
