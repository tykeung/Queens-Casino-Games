#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QResizeEvent>
#include <QMovie>


namespace Ui { class Widget; }

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void positionGifs();

private:
    Ui::Widget *ui;
    QLabel *gifLabel1;
    QLabel *gifLabel2;
    QMovie *movie1;
    QMovie *movie2;

};

#endif // WIDGET_H
