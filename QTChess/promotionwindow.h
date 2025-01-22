#ifndef PROMOTIONWINDOW_H
#define PROMOTIONWINDOW_H

#include <QDialog>

class PromotionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PromotionWindow(QWidget *parent = nullptr);

    // Zwraca numer wyboru promocji
    int getPromotionChoice() const;

private slots:
    // Slot wywoływany po kliknięciu przycisku w oknie promocji
    void onPromotionSelected(int choice);

    // Slot wywoływany po zamknięciu okna
    void onDialogClosed(int result);

private:
    int promotionChoice;  // Przechowuje wybór użytkownika (0 - brak wyboru, 1-4 - wybrana promocja)
};

#endif // PROMOTIONWINDOW_H
