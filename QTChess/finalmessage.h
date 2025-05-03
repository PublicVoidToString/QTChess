#ifndef FINALMESSAGE_H
#define FINALMESSAGE_H

#include <QWidget>

namespace Ui {
class FinalMessage;
}

class FinalMessage : public QWidget
{
    Q_OBJECT

public:
    explicit FinalMessage(const QString &imageName, int length, QWidget *parent = nullptr);
    ~FinalMessage();

private:
    Ui::FinalMessage *ui;
};

#endif // FINALMESSAGE_H
