#ifndef BLACKJACK_MENU_H
#define BLACKJACK_MENU_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

class blackjack_menu : public QWidget
{
    Q_OBJECT

public:
    blackjack_menu(QWidget *parent = nullptr);
};

#endif // BLACKJACK_MENU_H
