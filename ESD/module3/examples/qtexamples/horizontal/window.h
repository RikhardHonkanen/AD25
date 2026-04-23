#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include <QHBoxLayout>
#include <QPushButton>

class Window : public QDialog
{
    QPushButton btn1{"Button 1"};
    QPushButton btn2{"Button 2"};
    QPushButton btn3{"Button 3"};

    QHBoxLayout layout;

public:
    Window()
    {
        btn1.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        btn2.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        btn3.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        layout.addWidget(&btn1);
        layout.addWidget(&btn2);
        layout.addWidget(&btn3);

        setLayout(&layout);

        setWindowTitle("Horizontal Box Layout");
    }
};

#endif