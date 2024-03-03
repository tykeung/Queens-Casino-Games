#include "blackjack_menu.h"
#include <QLabel>
#include <QWidget>

blackjack_menu::blackjack_menu(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("Blackjack Menu", this);
    layout->addWidget(label);
    setLayout(layout);
}
