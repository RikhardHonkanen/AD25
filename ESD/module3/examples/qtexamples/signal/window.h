#ifndef WINDOW_H
#define WINDOW_H

#include <QDebug>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>

class Window : public QDialog
{
    QPushButton btn{"Click me"};
    QVBoxLayout layout;

public:
    Window()
    {
        btn.setFixedWidth(200);

        layout.addWidget(&btn);

        layout.setSizeConstraint(QLayout::SetFixedSize);
        setLayout(&layout);

        setWindowTitle("Signal & Slot");

        connect(&btn, &QPushButton::clicked, this, &Window::onButtonClicked);
    }

private:
    void onButtonClicked()
    {
        qDebug() << "Button clicked ...";
    }
};

#endif