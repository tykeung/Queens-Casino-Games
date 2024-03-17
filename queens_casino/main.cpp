#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QStackedWidget>
#include <QLineEdit>
#include "blackjack_menu.h"
#include "dice_menu.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Queen's Casino Games");
    QStackedWidget stackedWidget(&window);

    float balance = 1000.00;

    QVBoxLayout *main_menu = new QVBoxLayout(&window);
    QPushButton *blackjack_game_button = new QPushButton("Blackjack", &window);
    QPushButton *dice_game_button = new QPushButton("Dice", &window);
    QPushButton *mines_game_button = new QPushButton("Mines", &window);
    QLabel *label1 = new QLabel("Queen's Casino Games", &window);
    QLineEdit *balance_display = new QLineEdit();
    balance_display->setText(QString::number(balance, 'f', 2));
    balance_display->setReadOnly(true);
    balance_display->setFixedSize(50, balance_display->sizeHint().height());
    balance_display->setAlignment(Qt::AlignRight);




    label1->setAlignment(Qt::AlignCenter); // Center align the label
    label1->setStyleSheet("font-size: 36pt");
    main_menu->addWidget(balance_display);
    main_menu->addWidget(label1);
    main_menu->addWidget(blackjack_game_button);
    main_menu->addWidget(dice_game_button);
    main_menu->addWidget(mines_game_button);

    QObject::connect(blackjack_game_button, &QPushButton::clicked, [&]() {
        stackedWidget.setCurrentIndex(1);
    });

    QObject::connect(dice_game_button, &QPushButton::clicked, [&]() {
        stackedWidget.setCurrentIndex(2);
    });

    int window_width = window.sizeHint().width();

    QObject::connect(mines_game_button, &QPushButton::clicked, [&]() {
        stackedWidget.setCurrentIndex(3);
    });


    QVBoxLayout *blackjack_game = new QVBoxLayout;
    QPushButton *blackjack_backtomenu = new QPushButton("Back to main menu", &window);
    blackjack_menu *blackjack_widget = new blackjack_menu(&window);
    blackjack_game->addWidget(blackjack_widget);
    blackjack_game->addWidget(blackjack_backtomenu);


    QObject::connect(blackjack_backtomenu, &QPushButton::clicked, [&]() {
        stackedWidget.setCurrentIndex(0);
    });

    QVBoxLayout *dice_game = new QVBoxLayout;
    QPushButton *dice_backtomenu = new QPushButton("Back to main menu", &window);
    dice_menu *dice_widget = new dice_menu(window_width, balance, &window);
    dice_game->addWidget(dice_widget);
    dice_game->addWidget(dice_backtomenu);

    auto update_balance = [&](float val) {
        balance += val;
        balance_display->setText(QString::number(balance, 'f', 2));
        dice_widget->emit balance_updated(balance);

    };

    QObject::connect(dice_widget, &dice_menu::roll_clicked, [&](float val) {
        update_balance(val);
    });




    QObject::connect(dice_backtomenu, &QPushButton::clicked, [&]() {

        stackedWidget.setCurrentIndex(0);
    });

    QVBoxLayout *mines_game = new QVBoxLayout;
    QPushButton *mines_backtomenu = new QPushButton("Back to main menu", &window);
    QLabel *mines_label = new QLabel("mines here", &window);
    mines_game->addWidget(mines_label);
    mines_game->addWidget(mines_backtomenu);

    QObject::connect(mines_backtomenu, &QPushButton::clicked, [&]() {
        stackedWidget.setCurrentIndex(0);
    });




    stackedWidget.addWidget(new QWidget);
    stackedWidget.addWidget(new QWidget);
    stackedWidget.addWidget(new QWidget);
    stackedWidget.addWidget(new QWidget);

    stackedWidget.widget(0)->setLayout(main_menu);
    stackedWidget.widget(1)->setLayout(blackjack_game);
    stackedWidget.widget(2)->setLayout(dice_game);
    stackedWidget.widget(3)->setLayout(mines_game);


    QVBoxLayout *mainLayout = new QVBoxLayout(&window);


    mainLayout->addWidget(&stackedWidget);



    window.setLayout(mainLayout);

    window.show();
    window.resize(640, 480);

    return app.exec();
}

