#include "mines_menu.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QStackedWidget>
#include <QTimer>
#include <QStyleOptionSlider>
#include <QStylePainter>
#include <QLineEdit>
#include <QMessageBox>

bool minefield[5][5];
int count;
int num_mines;
float multiplier;

mines_menu::mines_menu(int window_width, float &balance, QWidget *parent) : QWidget(parent) {

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLineEdit *balance_display = new QLineEdit();
    balance_display->setText(QString::number(balance));
    balance_display->setReadOnly(true);
    balance_display->setFixedSize(100, balance_display->sizeHint().height());
    balance_display->setAlignment(Qt::AlignRight);

    QLabel *bet_word = new QLabel("Bet");
    QLineEdit *bet = new QLineEdit();
    QHBoxLayout *bet_line = new QHBoxLayout();
    QWidget *bet_line_widget = new QWidget();
    bet_line->addWidget(bet_word);
    bet_line->addWidget(bet);
    bet_line_widget->setLayout(bet_line);

    QLabel *game_result = new QLabel(this);
    QVBoxLayout *text_layout = new QVBoxLayout();
    text_layout->addWidget(game_result);
    QWidget *text_layout_widget = new QWidget();
    text_layout_widget->setLayout(text_layout);

    QPushButton *start_game = new QPushButton("Start Mines");
    QPushButton *end_game = new QPushButton("Cashout");
    QHBoxLayout *button_row = new QHBoxLayout();
    button_row->addWidget(start_game);
    button_row->addWidget(end_game);
    QWidget *button_row_widget = new QWidget();
    button_row_widget->setLayout(button_row);

    QLabel *mine_word = new QLabel("Total Mines");
    QLineEdit *mine = new QLineEdit();
    QHBoxLayout *mine_line = new QHBoxLayout();
    QWidget *mine_line_widget = new QWidget();
    mine_line->addWidget(mine_word);
    mine_line->addWidget(mine);
    mine_line_widget->setLayout(mine_line);

    const QSize btnSize = QSize(120, 50);
    QPushButton *btn[5][5];
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++){
            btn[i][j] = new QPushButton();
            //btn[i][j]->setText(QString::number(i) + " - " + QString::number(j));
            btn[i][j]->setFixedSize(btnSize);
            btn[i][j]->setEnabled(false);
        }
    }

    QGridLayout *btnLayout = new QGridLayout();
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            QObject::connect(btn[i][j], &QPushButton::clicked, [start_game, bet, btn, end_game, mine, game_result, i, j, this](){
                if (minefield[i][j] == true) {
                    mines_menu::roll_clicked(-1 * bet->text().toFloat());
                    game_result->setText("You have hit a mine at " + QString::number(i) + "-" + QString::number(j));
                    end_game->setEnabled(false);
                    start_game->setEnabled(true);
                    mine->setEnabled(true);
                    bet->setEnabled(true);
                    for(int x = 0; x < 5; x++) {
                        for(int y = 0; y < 5; y++){
                            if (btn[x][y]->isEnabled()) {
                                if (minefield[x][y]) {
                                    btn[x][y]->setStyleSheet("background-color: #FF7F7F;");
                                } else {
                                    btn[x][y]->setStyleSheet("background-color: #D1FFBD;");
                                }
                            }
                            btn[x][y]->setEnabled(false);
                        }
                    }
                    btn[i][j]->setStyleSheet("background-color: #400000;");
                } else {
                    minefield[i][j] = true;
                    btn[i][j]->setEnabled(false);
                    multiplier = multiplier * (1/(static_cast<float>(25-num_mines-count)/static_cast<float>(25-count)));
                    count++;
                    game_result->setText("You have cleared " + QString::number(count) + " tiles and have a multiplier of " + QString::number(multiplier*0.99, 'f', 2) + "x");
                    //game_result->setText("You have cleared " +  QString::number(count) + " tiles and have a multiplier of " + multiplier + "x");
                    end_game->setEnabled(true);
                    btn[i][j]->setStyleSheet("background-color: #00FF00;");
                }

                bool finished = true;
                for(int x = 0; x < 5; x++) {
                    for(int y = 0; y < 5; y++){
                        if(minefield[x][y] == false){
                            finished = false;
                        }
                    }
                }

                if(finished){
                    float total_win = bet->text().toFloat() * multiplier*0.99 - bet->text().toFloat();
                    mines_menu::roll_clicked(total_win);
                    game_result->setText("You have cleared the board and won " + QString::number(total_win - bet->text().toFloat()) + " chips");
                    end_game->setEnabled(false);
                    start_game->setEnabled(true);
                    mine->setEnabled(true);
                    bet->setEnabled(true);
                    for(int x = 0; x < 5; x++) {
                        for(int y = 0; y < 5; y++){
                            if (btn[x][y]->isEnabled()) {
                                if (minefield[x][y]) {
                                    btn[x][y]->setStyleSheet("background-color: #FF7F7F;");
                                } else {
                                    btn[x][y]->setStyleSheet("background-color: #D1FFBD;");
                                }
                            }
                            btn[x][y]->setEnabled(false);
                        }
                    }
                }
            });
            btnLayout->addWidget(btn[i][j], i, j);
            btnLayout->setSpacing(0);
        }
    }
    QWidget *mines_row_widget = new QWidget();
    mines_row_widget->setLayout(btnLayout);

    layout->addWidget(balance_display);
    layout->addWidget(mines_row_widget);
    layout->addWidget(text_layout_widget);
    layout->addWidget(button_row_widget);
    layout->addWidget(mine_line_widget);
    layout->addWidget(bet_line_widget);

    setLayout(layout);

    QObject::connect(start_game, &QPushButton::clicked, [start_game, bet, btn, end_game, mine, game_result] () {
        bool okbet, okmines;
        num_mines = mine->text().toInt();
        multiplier = 1.00;
        count = 0;
        QString textb = bet->text();
        textb.toFloat(&okbet);
        QString textm = mine->text();
        textm.toInt(&okmines);

        if (okbet && okmines && textm.toInt() < 25 && textm.toInt() > 0) {
            start_game->setEnabled(false);
            end_game->setEnabled(false);
            mine->setEnabled(false);
            bet->setEnabled(false);

            game_result->setText("Pick any tile you want");
            //makes board and enables buttons
            for(int i = 0; i < 5; i++) {
                for(int j = 0; j < 5; j++) {
                    minefield[i][j] = false;
                    btn[i][j]->setEnabled(true);
                    btn[i][j]->setStyleSheet(""); // blank
                }
            }
            //sets board mine placement
            for (int i = 0; i < mine->text().toInt(); ++i) {
                int rand_row = rand() % 5;
                int rand_col = rand() % 5;
                if(minefield[rand_row][rand_col] == false){
                   minefield[rand_row][rand_col] = true;
                   //cheating for testing
                   //game_result->setText("mine at" + QString::number(rand_row) + "-" + QString::number(rand_col));
                } else {
                    i--;
                }
            }
        } else {
            QMessageBox::critical(nullptr, "Error", "Invalid input! Please enter a valid number of mines and a valid bet.");
            bet->clear();
        }
    });

    QObject::connect(end_game, &QPushButton::clicked, [start_game, bet, btn, end_game, mine, game_result, this] () {
        float total_win = bet->text().toFloat() * multiplier*0.99 - bet->text().toFloat();
        mines_menu::roll_clicked(total_win);
        game_result->setText("You have cashed out early with " + QString::number(total_win));
        end_game->setEnabled(false);
        start_game->setEnabled(true);
        mine->setEnabled(true);
        bet->setEnabled(true);
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 5; j++){
                if (btn[i][j]->isEnabled()) {
                    if (minefield[i][j]) {
                        btn[i][j]->setStyleSheet("background-color: #FF7F7F;");
                    } else {
                        btn[i][j]->setStyleSheet("background-color: #D1FFBD;");
                    }
                }
                btn[i][j]->setEnabled(false);
            }
        }
    });

    QObject::connect(this, &mines_menu::balance_updated, [balance_display](float balance_change) {
        balance_display->setText(QString::number(balance_change, 'f', 2));
    });
}

