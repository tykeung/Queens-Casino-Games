#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Queen's Casino Games");
    QStackedWidget stackedWidget(&window);

    QVBoxLayout *main_menu = new QVBoxLayout;
    QPushButton *blackjack_menu = new QPushButton("Blackjack", &window);
    QPushButton *dice_menu = new QPushButton("Dice", &window);
    QPushButton *crash_menu = new QPushButton("Crash", &window);
    QLabel *label1 = new QLabel("Queen's Casino Games", &window);
    main_menu->addWidget(label1);
    main_menu->addWidget(blackjack_menu);
    main_menu->addWidget(dice_menu);
    main_menu->addWidget(crash_menu);

    QObject::connect(blackjack_menu, &QPushButton::clicked, [&]() {
        stackedWidget.setCurrentIndex(1);
    });

    QObject::connect(dice_menu, &QPushButton::clicked, [&]() {
        stackedWidget.setCurrentIndex(2);
    });

    QObject::connect(crash_menu, &QPushButton::clicked, [&]() {
        stackedWidget.setCurrentIndex(3);
    });


    QVBoxLayout *blackjack_game = new QVBoxLayout;
    QPushButton *blackjack_backtomenu = new QPushButton("Back to main menu", &window);
    QLabel *blackjack_label = new QLabel("blackjack here", &window);
    blackjack_game->addWidget(blackjack_label);
    blackjack_game->addWidget(blackjack_backtomenu);


    QObject::connect(blackjack_backtomenu, &QPushButton::clicked, [&]() {
        stackedWidget.setCurrentIndex(0);
    });

    QVBoxLayout *dice_game = new QVBoxLayout;
    QPushButton *dice_backtomenu = new QPushButton("Back to main menu", &window);
    QLabel *dice_label = new QLabel("dice here", &window);
    dice_game->addWidget(dice_label);
    dice_game->addWidget(dice_backtomenu);

    QObject::connect(dice_backtomenu, &QPushButton::clicked, [&]() {
        stackedWidget.setCurrentIndex(0);
    });

    QVBoxLayout *crash_game = new QVBoxLayout;
    QPushButton *crash_backtomenu = new QPushButton("Back to main menu", &window);
    QLabel *crash_label = new QLabel("crash here", &window);
    crash_game->addWidget(crash_label);
    crash_game->addWidget(crash_backtomenu);

    QObject::connect(crash_backtomenu, &QPushButton::clicked, [&]() {
        stackedWidget.setCurrentIndex(0);
    });


    stackedWidget.addWidget(new QWidget);
    stackedWidget.addWidget(new QWidget);
    stackedWidget.addWidget(new QWidget);
    stackedWidget.addWidget(new QWidget);


    stackedWidget.widget(0)->setLayout(main_menu);
    stackedWidget.widget(1)->setLayout(blackjack_game);
    stackedWidget.widget(2)->setLayout(dice_game);
    stackedWidget.widget(3)->setLayout(crash_game);


    QVBoxLayout *mainLayout = new QVBoxLayout(&window);


    mainLayout->addWidget(&stackedWidget);


    window.setLayout(mainLayout);

    window.show();

    return app.exec();
}
