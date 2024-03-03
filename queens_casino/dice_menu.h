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
    dice_menu(int window_width, QWidget *parent = nullptr);
    void updateTextDisplay(int window_width, QSlider* invisible_slider, QLabel* text_display_label);

};

#endif // DICE_MENU_H
