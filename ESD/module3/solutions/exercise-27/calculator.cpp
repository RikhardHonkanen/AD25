#include "calculator.h"

Calculator::Calculator()
{
    buttonLayout.addWidget(&divideButton);
    buttonLayout.addWidget(&multiplyButton);
    buttonLayout.addWidget(&subtractButton);
    buttonLayout.addWidget(&addButton);
    buttonLayout.addStretch();

    formLayout.addRow("First number", &firstEdit);
    formLayout.addRow("Second number", &secondEdit);

    resultLabel.setFixedHeight(30);
    formLayout.addRow("Result", &resultLabel);

    mainLayout.addLayout(&formLayout);
    mainLayout.addLayout(&buttonLayout);

    mainLayout.setSizeConstraint(QLayout::SetFixedSize);
    setLayout(&mainLayout);

    setWindowTitle("Calculator");

    connect(&addButton, &QPushButton::clicked, this, &Calculator::onAddButtonClicked);

    connect(&subtractButton, &QPushButton::clicked, this, &Calculator::onSubtractButtonClicked);

    connect(&multiplyButton, &QPushButton::clicked, this, &Calculator::onMultiplyButtonClicked);

    connect(&divideButton, &QPushButton::clicked, this, &Calculator::onDivideButtonClicked);

    connect(&firstEdit, &QLineEdit::textChanged, this, [this](const QString &text)
            {
                    bool number;
                    (void)text.toDouble(&number);
                    this->addButton.setDisabled(!number);
                    this->subtractButton.setDisabled(!number);
                    this->multiplyButton.setDisabled(!number);
                    this->divideButton.setDisabled(!number);
                    this->firstEdit.setStyleSheet(number ? "color: black" : "color: red"); });

    connect(&secondEdit, &QLineEdit::textChanged, this, [this](const QString &text)
            {
                    bool number;
                    (void)text.toDouble(&number);
                    this->addButton.setDisabled(!number);
                    this->subtractButton.setDisabled(!number);
                    this->multiplyButton.setDisabled(!number);
                    this->divideButton.setDisabled(!number);
                    this->secondEdit.setStyleSheet(number ? "color: black" : "color: red"); });
}

void Calculator::onAddButtonClicked()
{
    QString result;
    double firstNumber = firstEdit.text().toDouble();
    double secondNumber = secondEdit.text().toDouble();

    result.setNum(firstNumber + secondNumber, 'g');
    resultLabel.setText(result);
}

void Calculator::onSubtractButtonClicked()
{
    QString result;
    double firstNumber = firstEdit.text().toDouble();
    double secondNumber = secondEdit.text().toDouble();

    result.setNum(firstNumber - secondNumber, 'g');
    resultLabel.setText(result);
}

void Calculator::onMultiplyButtonClicked()
{
    QString result;
    double firstNumber = firstEdit.text().toDouble();
    double secondNumber = secondEdit.text().toDouble();

    result.setNum(firstNumber * secondNumber, 'g');

    resultLabel.setText(result);
}

void Calculator::onDivideButtonClicked()
{
    QString result{"Error"};
    double secondNumber = secondEdit.text().toDouble();

    if (secondNumber != 0)
    {
        double firstNumber = firstEdit.text().toDouble();
        result.setNum(firstNumber / secondNumber, 'g');
    }

    resultLabel.setText(result);
}