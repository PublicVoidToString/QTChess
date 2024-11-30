#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "chessboard.h"

#include <QPushButton>
#include <QVBoxLayout>  // Dodajemy layout

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent) // Inicjalizacja klasy bazowej QMainWindow
    , ui(new Ui::MainMenu) // Inicjalizacja obiektu UI
{
    ui->setupUi(this); // Konfiguracja UI

    // Tworzymy przycisk
    pushButton = new QPushButton("Rozpocznij", this);

    // Ustawiamy layout na centralnym widgetcie
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(pushButton);  // Dodajemy przycisk do layoutu

    // Ustawiamy layout na centralnym widgetcie
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);  // Przypisujemy layout
    setCentralWidget(centralWidget);   // Ustawiamy centralny widget

    // Ustawiamy rozmiar, który będzie dynamicznie dostosowywał się do okna
    pushButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(pushButton, &QPushButton::clicked, this, &MainMenu::openChessBoard);
}

MainMenu::~MainMenu()
{
    delete ui;  // Usuwamy obiekt ui
}


void MainMenu::openChessBoard()
{
    // Tworzymy obiekt ChessBoard i go pokazujemy
    chessBoard = new ChessBoard();
    chessBoard->show();
}
