#include "dice_menu.h"
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

class CustomSlider : public QSlider {
public:
    CustomSlider(QWidget *parent = nullptr, QWidget *labelParent = nullptr) : QSlider(Qt::Horizontal, parent), roll_display(new QLabel("", labelParent)) {
        roll_display->setFixedWidth(50);
        roll_display->setAlignment(Qt::AlignCenter);

        int label_x = this->rect().center().x() - (roll_display->width() / 2);
        int label_y = this->rect().center().y() - (roll_display->height() / 2);
        roll_display->move(label_x, label_y - 30);
    }

    QLabel* getTextDisplayLabel() const {
        return roll_display;
    }

protected:
    void paintEvent(QPaintEvent *ev) override {
        QSlider::paintEvent(ev);
        QStyleOptionSlider opt;
        initStyleOption(&opt);

        QRect groove_rect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
        QRect handle_rect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

        int dot_radius = 5;
        int dot_x = handle_rect.center().x();
        int dot_y = groove_rect.center().y();

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(Qt::red);
        painter.drawEllipse(dot_x - dot_radius, dot_y - dot_radius, dot_radius * 2, dot_radius * 2);

        int label_x = handle_rect.center().x() - (roll_display->width() / 2);
        int label_y = groove_rect.center().y() - (roll_display->height() / 2);

        QPoint label_pos = mapToParent(QPoint(label_x, label_y - 30));
        roll_display->move(label_pos);
    }

private:
    QLabel *roll_display;
};

dice_menu::dice_menu(int window_width, float &balance, QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *win_label = new QLabel("Win chance 50.0%, to win 1.980x");
    QLabel *game_result = new QLabel("");
    QLabel *history = new QLabel("");
    CustomSlider *invisible_slider = new CustomSlider(this, this);
    invisible_slider->setRange(0, 1000);
    invisible_slider->setValue(500);
    invisible_slider->setEnabled(false);

    QSlider *slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 1000);
    slider->setValue(500);

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




    QPushButton *roll_button = new QPushButton("Roll");
    layout->addWidget(balance_display);
    layout->addWidget(invisible_slider->getTextDisplayLabel());
    layout->addWidget(invisible_slider);
    layout->addWidget(slider);
    layout->addWidget(win_label);
    layout->addWidget(game_result);
    layout->addWidget(history);
    layout->addWidget(roll_button);
    layout->addWidget(bet_line_widget);
    setLayout(layout);

    QObject::connect(invisible_slider, &QSlider::valueChanged, [invisible_slider](int value) {
        invisible_slider->getTextDisplayLabel()->setText(QString::number(value));
    });

    QObject::connect(slider, &QSlider::valueChanged, [win_label, slider](int value) {
        if (value == 0) {
            slider->setValue(1);
            value = 1;
        } else if (value > 989) {
            slider->setValue(989);
            value = 989;
        }

        float reciprocal = (1.0 / (static_cast<float>(value) / 1000.00)) * 0.990;
        win_label->setText("Win chance " + QString::number(value / 10.0, 'f', 1) + "%, to win " + QString::number(reciprocal, 'f', 3) + "x");
    });

    QObject::connect(roll_button, &QPushButton::clicked, [game_result, bet, invisible_slider, slider, this]() {
        bool ok;
        QString text = bet->text();
        float bet_value = text.toFloat(&ok);

        if (ok) {
            int random_number = (rand() % 1000) + 1;
            invisible_slider->setValue(random_number);
            invisible_slider->getTextDisplayLabel()->setText(QString::number(random_number));
            if (slider->value() > random_number) {
                float win = bet_value * 0.990 * ((1.0 / (static_cast<float>(slider->value()) / 1000.00))-1);
                emit roll_clicked(win);
                game_result->setText("Roll: " + QString::number(random_number) + ". You win " + QString::number(win));
            } else {
                emit roll_clicked(-1 * bet_value);
                game_result->setText("Roll: " + QString::number(random_number) + ". You lose " + QString::number(bet_value));
            }

        } else {
            QMessageBox::critical(nullptr, "Error", "Invalid input! Please enter a valid number.");
            bet->clear();
        }


    });

    QObject::connect(this, &dice_menu::balance_updated, [balance_display](float balance_change) {
        balance_display->setText(QString::number(balance_change, 'f', 2));
    });


}
