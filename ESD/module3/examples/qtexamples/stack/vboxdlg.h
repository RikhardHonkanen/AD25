#ifndef VBOXDLG_H
#define VBOXDLG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

class VBoxDlg : public QDialog
{
    QPushButton btn1{"Button 1"};
    QPushButton btn2{"Button 2"};
    QPushButton btn3{"Button 3"};
    QPushButton btn4{"Button 4"};
    QPushButton btn5{"Button 5"};

    QVBoxLayout layout;

public:
    VBoxDlg()
    {
        layout.addWidget(&btn1);
        layout.addWidget(&btn2);
        layout.addWidget(&btn3);
        layout.addWidget(&btn4);
        layout.addWidget(&btn5);

        setLayout(&layout);
    }
};

#endif