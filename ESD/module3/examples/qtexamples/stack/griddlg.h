#ifndef GRIDDLG_H
#define GRIDDLG_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>

class GridDlg : public QDialog
{
    QPushButton btn1{"Button 1"};
    QPushButton btn2{"Button 2"};
    QPushButton btn3{"Button 3"};
    QPushButton btn4{"Button 4"};
    QPushButton btn5{"Button 5"};

    QPushButton btn6{"Button 6"};
    QPushButton btn7{"Button 7"};
    QPushButton btn8{"Button 8"};
    QPushButton btn9{"Button 9"};
    QPushButton btn10{"Button 10"};

    QGridLayout layout;

public:
    GridDlg()
    {
        layout.addWidget(&btn1, 0, 0);
        layout.addWidget(&btn2, 0, 1);
        layout.addWidget(&btn3, 0, 2);
        layout.addWidget(&btn4, 0, 3);
        layout.addWidget(&btn5, 0, 4);

        layout.addWidget(&btn6, 1, 0);
        layout.addWidget(&btn7, 1, 1);
        layout.addWidget(&btn8, 1, 2);
        layout.addWidget(&btn9, 1, 3);
        layout.addWidget(&btn10, 1, 4);

        setLayout(&layout);
    }
};

#endif