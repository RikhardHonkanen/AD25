#ifndef MAINWIN_H
#define MAINWIN_H

#include <QDialog>
#include <QBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedLayout>

#include "boxdlg.h"
#include "griddlg.h"
#include "vboxdlg.h"
#include "hboxdlg.h"

class MainWin : public QDialog
{
    QPushButton prevBtn{"Prev"};
    QPushButton nextBtn{"Next"};
    QPushButton closeBtn{"Close"};

    QVBoxLayout mainLayout;
    QStackedLayout stackLayout;
    QBoxLayout navigationLayout{QBoxLayout::Direction::RightToLeft};

    BoxDlg bxdlg;
    GridDlg grddlg;
    VBoxDlg vbxdlg;
    HBoxDlg hbxdlg;

public:
    MainWin()
    {
        stackLayout.addWidget(&bxdlg);
        stackLayout.addWidget(&grddlg);
        stackLayout.addWidget(&vbxdlg);
        stackLayout.addWidget(&hbxdlg);

        navigationLayout.addWidget(&closeBtn);
        navigationLayout.addWidget(&nextBtn);
        navigationLayout.addWidget(&prevBtn);
        navigationLayout.addStretch();

        mainLayout.addLayout(&stackLayout);
        mainLayout.addLayout(&navigationLayout);

        mainLayout.setSizeConstraint(QLayout::SetFixedSize);
        setLayout(&mainLayout);

        setWindowTitle("Stacked Layout");

        connect(&closeBtn, &QPushButton::clicked, this, &MainWin::close);

        connect(&nextBtn, &QPushButton::clicked, this, &MainWin::onNextPage);

        connect(&prevBtn, &QPushButton::clicked, this, &MainWin::onPrevPage);
    }

private:
    void onNextPage()
    {
        int index = stackLayout.currentIndex();

        if (index < 3)
        {
            stackLayout.setCurrentIndex(index + 1);
        }
    }

    void onPrevPage()
    {
        int index = stackLayout.currentIndex();

        if (index > 0)
        {
            stackLayout.setCurrentIndex(index - 1);
        }
    }
};

#endif