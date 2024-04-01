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
#include <QStylePainter>
#include <QLineEdit>
#include <QMessageBox>
#include "blackjack_game.cpp"

blackjack_menu::blackjack_menu(int window_width, float &balance, QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);



    QLabel *dealer_text = new QLabel("Dealer's Hand", this);
    QLabel *dealer_hand = new QLabel(this);
    QLabel *player_text = new QLabel("Player's Hand", this);
    QLabel *player_hand = new QLabel(this);
    QLabel *game_result = new QLabel(this);

    QVBoxLayout *text_layout = new QVBoxLayout();
    text_layout->addWidget(dealer_text);
    text_layout->addWidget(dealer_hand);
    text_layout->addWidget(player_text);
    text_layout->addWidget(player_hand);
    text_layout->addWidget(game_result);
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

    QObject::connect(start_game, &QPushButton::clicked, [double_down, game_result, bet, deck, playerHand, dealerHand, start_game, hit, stand, dealer_hand, player_hand, this] () {
        bool ok;
        QString text = bet->text();
        text.toFloat(&ok);

        if (ok) {
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


            std::string player_cards;
            for (const Card& card : *playerHand) {
                player_cards += getCardRank(card) + " ";
            }
            player_hand->setText(QString::fromStdString(std::to_string(playerTotal) + ": " + player_cards));

            dealer_hand->setText(QString::fromStdString(std::to_string(dealerTotal) + ": " + getCardRank(dealerFaceUp)));

            dealerHand->push_back(dealerFaceDown);
            if (calculatePoints(dealerHand) == 21 || calculatePoints(playerHand) == 21) {
                dealerTotal = calculatePoints(dealerHand);
                std::string dealer_cards;
                for (const Card& card : *dealerHand) {
                    // Append card rank to the label text
                    dealer_cards += getCardRank(card) + " ";
                }
                dealer_hand->setText(QString::fromStdString(std::to_string(dealerTotal) + ": " + dealer_cards));

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

    QObject::connect(hit, &QPushButton::clicked, [double_down, game_result, hit, stand, start_game, bet, deck, playerHand, dealerHand, player_hand, this] () {
        playerHand->push_back(deck->dealCard());
        double_down->setEnabled(false);
        std::string player_cards;
        for (const Card& card : *playerHand) {
            player_cards += getCardRank(card) + " ";
        }

        int playerTotal = calculatePoints(playerHand);
        player_hand->setText(QString::fromStdString(std::to_string(playerTotal) + ": " + player_cards));

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

    QObject::connect(stand, &QPushButton::clicked, [double_down, hit, stand, start_game, bet, deck, playerHand, dealerHand, player_hand, dealer_hand, game_result, this] () {

        while (calculatePoints(dealerHand) < 17) {
            dealerHand->push_back(deck->dealCard());
        }
        int dealerTotal = calculatePoints(dealerHand);
        int playerTotal = calculatePoints(playerHand);
        std::string dealer_cards;
        for (const Card& card : *dealerHand) {
            dealer_cards += getCardRank(card) + " ";
        }
        dealer_hand->setText(QString::fromStdString(std::to_string(dealerTotal) + ": " + dealer_cards));



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

    QObject::connect(double_down, &QPushButton::clicked, [double_down, hit, stand, start_game, bet, deck, playerHand, dealerHand, player_hand, dealer_hand, game_result, this] () {
        playerHand->push_back(deck->dealCard());


        while (calculatePoints(dealerHand) < 17) {
            dealerHand->push_back(deck->dealCard());
        }
        int dealerTotal = calculatePoints(dealerHand);
        int playerTotal = calculatePoints(playerHand);
        std::string dealer_cards;
        for (const Card& card : *dealerHand) {
            dealer_cards += getCardRank(card) + " ";
        }
        dealer_hand->setText(QString::fromStdString(std::to_string(dealerTotal) + ": " + dealer_cards));
        std::string player_cards;
        for (const Card& card : *playerHand) {
            player_cards += getCardRank(card) + " ";
        }
        player_hand->setText(QString::fromStdString(std::to_string(playerTotal) + ": " + player_cards));

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
