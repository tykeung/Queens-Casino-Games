#include "blackjack_menu.h"
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
#include <QCoreApplication>
#include <QStylePainter>
#include <QLineEdit>
#include <QPixmap>
#include <QMessageBox>
#include <QDir>
#include "blackjack_game.cpp"




blackjack_menu::blackjack_menu(int window_width, float &balance, QWidget *parent) : QWidget(parent) {
    QString appDirPath = QCoreApplication::applicationDirPath();
    QDir app_dir(appDirPath);
    app_dir.cdUp();
    app_dir.cdUp();
    QString real_path = app_dir.absolutePath() + "/queens_casino/cards/";

    QVBoxLayout *layout = new QVBoxLayout(this);




    QHBoxLayout *dealer_cards_images = new QHBoxLayout();
    QLabel *dealer_hand = new QLabel(this);
    dealer_hand->setAlignment(Qt::AlignCenter);
    dealer_hand->setStyleSheet("font-size: 28pt");

    QLabel *dealer_text = new QLabel("Dealer", this);
    dealer_text->setStyleSheet("font-size: 12pt");
    dealer_text->setAlignment(Qt::AlignCenter);

    dealer_cards_images->setAlignment(Qt::AlignCenter);


    QHBoxLayout *player_cards_images = new QHBoxLayout();
    QLabel *player_hand = new QLabel(this);
    player_hand->setAlignment(Qt::AlignCenter);
    player_hand->setStyleSheet("font-size: 28pt");

    QLabel *player_text = new QLabel("You", this);
    player_text->setStyleSheet("font-size: 12pt");
    player_text->setAlignment(Qt::AlignCenter);

    player_cards_images->setAlignment(Qt::AlignCenter);


    QLabel *game_result = new QLabel(this);
    /*
    QString path = real_path + "/cards/2_of_spades.png";
    QPixmap img(path);

    QLabel *temp_card = new QLabel(this);
    temp_card->setPixmap(img);
    temp_card->setPixmap(img.scaled(40, 40, Qt::KeepAspectRatio));

    dealer_cards_images->addWidget(temp_card);
    dealer_cards_images->update();
    */

    QWidget *dealer_cards_images_widget = new QWidget();
    dealer_cards_images_widget->setLayout(dealer_cards_images);
    QWidget *player_cards_images_widget = new QWidget();
    player_cards_images_widget->setLayout(player_cards_images);


    QVBoxLayout *text_layout = new QVBoxLayout();

    text_layout->addWidget(dealer_cards_images_widget);
    text_layout->addWidget(dealer_hand);
    text_layout->addWidget(dealer_text);


    text_layout->addWidget(player_cards_images_widget);
    text_layout->addWidget(player_hand);
    text_layout->addWidget(player_text);


    text_layout->addWidget(game_result);
    text_layout->setSpacing(0);

    QWidget *text_layout_widget = new QWidget();
    text_layout_widget->setLayout(text_layout);


    QPushButton *hit = new QPushButton("Hit");
    hit->setEnabled(false);
    QPushButton *stand = new QPushButton("Stand");
    stand->setEnabled(false);
    QPushButton *double_down = new QPushButton("Double");
    double_down->setEnabled(false);
    QPushButton *start_game = new QPushButton("Deal Cards");


    QHBoxLayout *button_row1 = new QHBoxLayout();
    button_row1->addWidget(hit);
    button_row1->addWidget(stand);
    QWidget *button_row1_widget = new QWidget();
    button_row1_widget->setLayout(button_row1);
    QHBoxLayout *button_row2 = new QHBoxLayout();
    button_row2->addWidget(double_down);
    button_row2->addWidget(start_game);
    QWidget *button_row2_widget = new QWidget();
    button_row2_widget->setLayout(button_row2);
    QVBoxLayout *button_row_qv = new QVBoxLayout();
    button_row_qv->addWidget(button_row1_widget);
    button_row_qv->addWidget(button_row2_widget);
    QWidget *button_row_qv_widget = new QWidget();
    button_row_qv_widget->setLayout(button_row_qv);


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



    layout->addWidget(balance_display);
    layout->addWidget(text_layout_widget);
    layout->addWidget(button_row_qv_widget);
    layout->addWidget(bet_line_widget);

    setLayout(layout);

    Deck *deck = new Deck();


    vector<Card> *playerHand = new vector<Card>;
    vector<Card> *dealerHand = new vector<Card>;


    QObject::connect(this, &blackjack_menu::balance_updated, [balance_display](float balance_change) {
        balance_display->setText(QString::number(balance_change, 'f', 2));
    });

    QObject::connect(start_game, &QPushButton::clicked, [double_down, game_result, bet, deck, playerHand, dealerHand, start_game, hit, stand, dealer_hand, player_hand, dealer_cards_images, player_cards_images, real_path, this] () {
        bool ok;
        QString text = bet->text();
        text.toFloat(&ok);

        if (ok) {
            QLayoutItem* item;
            while ((item = dealer_cards_images->takeAt(0)) != nullptr) {
                if (QWidget* widget = item->widget()) {
                    // If the item is a widget, remove it from layout and delete it
                    dealer_cards_images->removeWidget(widget);
                    delete widget;
                }
                // Delete the layout item
                delete item;
            }
            while ((item = player_cards_images->takeAt(0)) != nullptr) {
                if (QWidget* widget = item->widget()) {
                    // If the item is a widget, remove it from layout and delete it
                    player_cards_images->removeWidget(widget);
                    delete widget;
                }
                // Delete the layout item
                delete item;
            }

            game_result->setText("");
            bet->setEnabled(false);
            deck->shuffle();


            hit->setEnabled(true);
            stand->setEnabled(true);
            start_game->setEnabled(false);

            int playerTotal = 0;
            int dealerTotal = 0;


            playerHand->push_back(deck->dealCard());

            Card dealerFaceUp = deck->dealCard();
            dealerHand->push_back(dealerFaceUp);

            playerHand->push_back(deck->dealCard());
            Card dealerFaceDown = deck->dealCard();

            playerTotal = calculatePoints(playerHand);
            dealerTotal = calculatePoints(dealerHand);

            QString path = real_path + QString::fromStdString(getCardFile(dealerFaceUp));
            QPixmap* img = new QPixmap(path);
            QLabel* temp_card = new QLabel;
            temp_card->setPixmap(img->scaled(60, 60, Qt::KeepAspectRatio));

            dealer_cards_images->addWidget(temp_card);
            for (const Card& card : *playerHand) {


                QString path = real_path + QString::fromStdString(getCardFile(card));
                QPixmap* img = new QPixmap(path);
                QLabel* temp_card = new QLabel;
                temp_card->setPixmap(img->scaled(60, 60, Qt::KeepAspectRatio));

                player_cards_images->addWidget(temp_card);

            }

            player_hand->setText(QString::fromStdString(std::to_string(playerTotal)));

            dealer_hand->setText(QString::fromStdString(std::to_string(dealerTotal)));

            dealerHand->push_back(dealerFaceDown);

            if (calculatePoints(dealerHand) == 21 || calculatePoints(playerHand) == 21) {
                dealerTotal = calculatePoints(dealerHand);

                QString path = real_path + QString::fromStdString(getCardFile(dealerFaceDown));
                QPixmap* img = new QPixmap(path);
                QLabel* temp_card = new QLabel;
                temp_card->setPixmap(img->scaled(60, 60, Qt::KeepAspectRatio));

                dealer_cards_images->addWidget(temp_card);
                dealer_hand->setText(QString::fromStdString(std::to_string(dealerTotal)));

                if (calculatePoints(dealerHand) == 21 && calculatePoints(playerHand) == 21) {
                    game_result->setText("Game resulted in a push.");
                    hit->setEnabled(false);
                    stand->setEnabled(false);
                    start_game->setEnabled(true);
                    playerHand->clear();
                    dealerHand->clear();
                } else if (calculatePoints(dealerHand) == 21) {
                    roll_clicked(-1 * bet->text().toFloat());
                    hit->setEnabled(false);
                    stand->setEnabled(false);
                    start_game->setEnabled(true);
                    playerHand->clear();
                    dealerHand->clear();

                    game_result->setText("Dealer had 21. You lose " + bet->text());
                } else if (calculatePoints(playerHand) == 21) {
                    float winnings = 1.5 * bet->text().toFloat();
                    roll_clicked(1.5 * bet->text().toFloat());
                    game_result->setText("You have blackjack. You win " + QString::number(winnings));
                    hit->setEnabled(false);
                    stand->setEnabled(false);
                    start_game->setEnabled(true);
                    playerHand->clear();
                    dealerHand->clear();
                }
            } else {
                double_down->setEnabled(true);
            }

        } else {
            QMessageBox::critical(nullptr, "Error", "Invalid input! Please enter a valid number.");
            bet->clear();
        }

    });

    QObject::connect(hit, &QPushButton::clicked, [double_down, game_result, hit, stand, start_game, bet, deck, playerHand, dealerHand, player_hand, dealer_cards_images, player_cards_images, real_path, this] () {
        Card new_card = deck->dealCard();
        playerHand->push_back(new_card);
        double_down->setEnabled(false);

        QString path = real_path + QString::fromStdString(getCardFile(new_card));
        QPixmap* img = new QPixmap(path);
        QLabel* temp_card = new QLabel;
        temp_card->setPixmap(img->scaled(60, 60, Qt::KeepAspectRatio));

        player_cards_images->addWidget(temp_card);

        std::string player_cards;

        int playerTotal = calculatePoints(playerHand);
        player_hand->setText(QString::fromStdString(std::to_string(playerTotal)));

        if (playerTotal > 21) {
            roll_clicked(-1 * bet->text().toFloat());
            hit->setEnabled(false);
            stand->setEnabled(false);
            start_game->setEnabled(true);
            bet->setEnabled(true);

            playerHand->clear();
            dealerHand->clear();
            game_result->setText("You bust. You lose " + bet->text());
        }
    });

    QObject::connect(stand, &QPushButton::clicked, [double_down, hit, stand, start_game, bet, deck, playerHand, dealerHand, player_hand, dealer_hand, game_result, dealer_cards_images, player_cards_images, real_path, this] () {

        QLayoutItem *item;
        while ((item = dealer_cards_images->takeAt(0)) != nullptr) {
            if (QWidget* widget = item->widget()) {
                // If the item is a widget, remove it from layout and delete it
                dealer_cards_images->removeWidget(widget);
                delete widget;
            }
            // Delete the layout item
            delete item;
        }

        while (calculatePoints(dealerHand) < 17) {
            Card new_card = deck->dealCard();
            dealerHand->push_back(new_card);


        }
        int dealerTotal = calculatePoints(dealerHand);
        int playerTotal = calculatePoints(playerHand);


        for (const Card& card : *dealerHand) {

            QString path = real_path + QString::fromStdString(getCardFile(card));
            QPixmap* img = new QPixmap(path);
            QLabel* temp_card = new QLabel;
            temp_card->setPixmap(img->scaled(60, 60, Qt::KeepAspectRatio));

            dealer_cards_images->addWidget(temp_card);
        }
        dealer_hand->setText(QString::fromStdString(std::to_string(dealerTotal)));



        if (dealerTotal > 21) {
            roll_clicked(1 * bet->text().toFloat());
            game_result->setText("Dealer bust. You win " + bet->text());
        } else {
            if (dealerTotal > playerTotal) {
                roll_clicked(-1 * bet->text().toFloat());
                game_result->setText("Dealer's total was higher. You lose " + bet->text());
            } else if (dealerTotal < playerTotal) {
                roll_clicked(1 * bet->text().toFloat());
                game_result->setText("Your total was higher. You win " + bet->text());
            } else {
                game_result->setText("Game resulted in a push.");
                //push
            }
        }

        hit->setEnabled(false);
        stand->setEnabled(false);
        start_game->setEnabled(true);
        double_down->setEnabled(false);
        playerHand->clear();
        dealerHand->clear();
        bet->setEnabled(true);
    });

    QObject::connect(double_down, &QPushButton::clicked, [double_down, hit, stand, start_game, bet, deck, playerHand, dealerHand, player_hand, dealer_hand, game_result, dealer_cards_images, player_cards_images, real_path, this] () {
        Card new_card = deck->dealCard();
        playerHand->push_back(new_card);

        QString path = real_path + QString::fromStdString(getCardFile(new_card));
        QPixmap* img = new QPixmap(path);
        QLabel* temp_card = new QLabel;
        temp_card->setPixmap(img->scaled(60, 60, Qt::KeepAspectRatio));

        player_cards_images->addWidget(temp_card);

        QLayoutItem *item;
        while ((item = dealer_cards_images->takeAt(0)) != nullptr) {
            if (QWidget* widget = item->widget()) {
                // If the item is a widget, remove it from layout and delete it
                dealer_cards_images->removeWidget(widget);
                delete widget;
            }
            // Delete the layout item
            delete item;
        }

        while (calculatePoints(dealerHand) < 17) {
            dealerHand->push_back(deck->dealCard());
        }
        int dealerTotal = calculatePoints(dealerHand);
        int playerTotal = calculatePoints(playerHand);
        for (const Card& card : *dealerHand) {

            QString path = real_path + QString::fromStdString(getCardFile(card));
            QPixmap* img = new QPixmap(path);
            QLabel* temp_card = new QLabel;
            temp_card->setPixmap(img->scaled(60, 60, Qt::KeepAspectRatio));

            dealer_cards_images->addWidget(temp_card);

        }
        dealer_hand->setText(QString::fromStdString(std::to_string(dealerTotal)));
        player_hand->setText(QString::fromStdString(std::to_string(playerTotal)));

        float winnings = 2.0 * bet->text().toFloat();

        if (dealerTotal > 21) {
            roll_clicked(2 * bet->text().toFloat());
            game_result->setText("Dealer bust. You win " + QString::number(winnings));
        } else {
            if (dealerTotal > playerTotal) {
                roll_clicked(-2 * bet->text().toFloat());
                game_result->setText("Dealer's total was higher. You lose " + QString::number(winnings));
            } else if (dealerTotal < playerTotal) {
                roll_clicked(2 * bet->text().toFloat());
                game_result->setText("Your total was higher. You win " + QString::number(winnings));
            } else {
                game_result->setText("Game resulted in a push.");
            }
        }

        hit->setEnabled(false);
        stand->setEnabled(false);
        start_game->setEnabled(true);
        double_down->setEnabled(false);
        playerHand->clear();
        dealerHand->clear();
        bet->setEnabled(true);
    });

}

/*
QLabel* getCardPixmap(std::string card) {
    QString appDirPath = QCoreApplication::applicationDirPath();
    QDir app_dir(appDirPath);
    app_dir.cdUp();
    app_dir.cdUp();
    QString real_path = app_dir.absolutePath() + "/queens_casino/cards/";


    QString path = real_path + QString::fromStdString(card);
    QPixmap* img = new QPixmap(path);
    QLabel* temp_card = new QLabel;
    temp_card->setPixmap(img->scaled(60, 60, Qt::KeepAspectRatio));

    return temp_card;

}
*/
