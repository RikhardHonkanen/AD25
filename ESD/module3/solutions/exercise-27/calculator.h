/**
 * @file calculator.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Using the Qt framework make a simple calculator.
 *        Look at the image(calculator.png)
 *
 * @version 0.1
 * @date 2023-03-11
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QLabel>
#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>

class Calculator : public QDialog
{
    QVBoxLayout mainLayout;
    QFormLayout formLayout;
    QBoxLayout buttonLayout{QBoxLayout::Direction::LeftToRight};

    QLineEdit firstEdit{"0"}, secondEdit{"0"};
    QLabel firstLabel, secondLabel, resultLabel;

    QPushButton addButton{"Add"};
    QPushButton subtractButton{"Subtract"};
    QPushButton multiplyButton{"Multiply"};
    QPushButton divideButton{"Divide"};

public:
    Calculator();

private:
    void onAddButtonClicked();

    void onSubtractButtonClicked();

    void onMultiplyButtonClicked();

    void onDivideButtonClicked();
};

#endif