#ifndef WINDOW_H
#define WINDOW_H

#include <QDebug>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMainWindow>
#include <QMessageBox>

class Window : public QMainWindow
{
    QPushButton button{"Click me"};

    QVBoxLayout VBLayout;

    QMenuBar menuBar;
    QWidget centerWidget;
    QStatusBar statusBar;

    QMenu fileMenu{"File"};
    QAction exitAction{"Exit"};

    QMenu helpMenu{"Help"};
    QAction aboutAction{"About"};

public:
    Window()
    {
        button.setFixedWidth(200);

        VBLayout.setAlignment(Qt::AlignCenter);
        VBLayout.addWidget(&button);

        centerWidget.setLayout(&VBLayout);
        setCentralWidget(&centerWidget);

        setWindowTitle("Qt6 MainWindow Example");

        connect(&button, &QPushButton::clicked, this, &Window::onButtonClicked);

        connect(&exitAction, &QAction::triggered, this, &Window::close);
        fileMenu.addAction(&exitAction);

        connect(&aboutAction, &QAction::triggered, [this]()
                { QMessageBox::about(this, "About", "Qt6 MainWindow Example"); });
        helpMenu.addAction(&aboutAction);

        menuBar.addMenu(&fileMenu);
        menuBar.addMenu(&helpMenu);
        setMenuBar(&menuBar);

        statusBar.showMessage("Ready");
        setStatusBar(&statusBar);
    }

private:
    void onButtonClicked()
    {
        qDebug() << "Button clicked ...";
    }
};

#endif