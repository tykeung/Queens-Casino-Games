#ifndef DICE_MENU_H
#define DICE_MENU_H

#include <QWidget>
#include <QTimer>
#include <QSlider>
#include <QLabel>


class dice_menu : public QWidget
{
    Q_OBJECT

public:
    dice_menu(int window_width, float &balance, QWidget *parent = nullptr);
    void updateTextDisplay(int window_width, QSlider* invisible_slider, QLabel* text_display_label);

signals:
    void roll_clicked(float val);
    void balance_updated(float new_balance);

};


#endif // DICE_MENU_H
