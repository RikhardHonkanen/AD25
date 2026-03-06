#include <iostream>

constexpr int GUESSES{10};
constexpr int MAX_NUM{100};

void greet(void);
void play_game(void);

void greet(void)
{
    std::cout << "******************************************\n\n************** Game Started **************\n\n******************************************\n";
}

void play_game(void)
{
    int secret = rand() % MAX_NUM;
    int guesses = GUESSES;
    bool win = false;

    int guess;
    while (!win && guesses > 0)
    {
        std::cout << guesses << " guesses left.\n";
        do
        {
            std::cout << "Guess a number [0, 99]: ";
            std::cin >> guess;
        } while (0 > guess || guess >= MAX_NUM);

        if (guess == secret)
        {
            win = true;
        }
        else if (guess < secret)
        {
            std::cout << "Sorry, too small. Try again!\n";
        }
        else
        {
            std::cout << "Sorry, too big. Try again!\n";
        }

        --guesses;
    }

    if (win)
    {
        std::cout << "Congratulations, you guessed the secret!\n";
    }
    else
    {
        std::cout << "Unfortunately, you did not guess the secret.\n";
    }
}

int main(void)
{
    srand(time(nullptr));
    greet();
    char again;
    do
    {
        play_game();
        std::cout << "Play again [Y/N]? ";
        std::cin >> again;
    } while (again == 'Y' || again == 'y');

    std::cout << "\nThanks for playing!\n";

    return 0;
}