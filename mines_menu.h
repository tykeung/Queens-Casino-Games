#ifndef MINES_MENU_H
#define MINES_MENU_H

#include <QWidget>
#include <QTimer>
#include <QSlider>
#include <QLabel>


class mines_menu : public QWidget
{
    Q_OBJECT

public:
    mines_menu(int window_width, float &balance, QWidget *parent = nullptr);
    void updateTextDisplay(int window_width, QSlider* invisible_slider, QLabel* text_display_label);

signals:
    void roll_clicked(float val);
    void balance_updated(float new_balance);

};


#endif // MINES_MENU_H
