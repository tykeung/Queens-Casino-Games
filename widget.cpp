#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    gifLabel1 = new QLabel(this);
    gifLabel2 = new QLabel(this);

    // Initialize the movies here but don't start them yet
    movie1 = new QMovie(":/images/manji.gif");
    movie2 = new QMovie(":/images/tomdean.gif");


    gifLabel1->setMovie(movie1);
    gifLabel2->setMovie(movie2);

    gifLabel1->setVisible(true);
    gifLabel2->setVisible(true);
    // Set fixed size if needed, or use size policies
    gifLabel1->setFixedSize(100, 100);
    gifLabel2->setFixedSize(100, 100);

    // The movies will be started in the showEvent, which you must override

    positionGifs();
}

void Widget::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    movie1->start(); // Start playing the first GIF
    movie2->start(); // Start playing the second GIF
    qDebug() << "Show event called.";
}

void Widget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    positionGifs(); // Adjust GIF positions when the widget is resized
}

void Widget::positionGifs() {
    int widgetWidth = this->width();
    int widgetHeight = this->height();

    // Position gifLabel1 in the bottom-right corner
    int xPositionRight = widgetWidth - gifLabel1->width();
    int yPositionBottom = widgetHeight - gifLabel1->height();
    gifLabel1->move(xPositionRight, yPositionBottom);

    // Position gifLabel2 in the bottom-left corner
    int xPositionLeft = 0; // for left alignment
    gifLabel2->move(xPositionLeft, yPositionBottom);
    qDebug() << "Position GIFs called.";
}

Widget::~Widget() {
    delete ui;
}
