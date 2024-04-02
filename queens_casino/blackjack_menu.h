#ifndef BLACKJACK_MENU_H
#define BLACKJACK_MENU_H

#include <QWidget>
#include <QTimer>
#include <QSlider>
#include <QLabel>


class blackjack_menu : public QWidget
{
    Q_OBJECT

public:
    blackjack_menu(int window_width, float &balance, QWidget *parent = nullptr);
    void updateTextDisplay(int window_width, QSlider* invisible_slider, QLabel* text_display_label);

signals:
    void roll_clicked(float val);
    void balance_updated(float new_balance);

};


#endif
