#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>

class Window : public QDialog
{
    QPushButton btn1{"Button 1"};
    QPushButton btn2{"Button 2"};
    QPushButton btn3{"Button 3"};
    QPushButton btn4{"Button 4"};
    QPushButton btn5{"Button 5"};
    QPushButton btn6{"Button 6"};
    QPushButton btn7{"Button 7"};
    QPushButton btn8{"Button 8"};

    QGridLayout layout;

public:
    Window()
    {
        btn3.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        layout.addWidget(&btn1, 0, 0, 1, 1);
        layout.addWidget(&btn2, 0, 1, 1, 2);
        layout.addWidget(&btn3, 0, 3, 2, 1);
        layout.addWidget(&btn4, 1, 0, 1, 3);

        layout.addWidget(&btn5, 2, 0);
        layout.addWidget(&btn6, 2, 1);
        layout.addWidget(&btn7, 2, 2);
        layout.addWidget(&btn8, 2, 3);

        setLayout(&layout);

        setWindowTitle("Grid Layout");
    }
};

#endif