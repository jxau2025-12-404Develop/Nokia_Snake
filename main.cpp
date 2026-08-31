#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype>

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 20;

struct Point
{
    int x, y;
};

vector<Point> snake;
Point food;
int score = 0;
int dirX = 1, dirY = 0;

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor()
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void generateFood()
{
    bool onSnake;
    do
    {
        onSnake = false;
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;
        for (const auto& s : snake)
        {
            if (s.x == food.x && s.y == food.y)
            {
                onSnake = true;
                break;
            }
        }
    } while (onSnake);
}

void draw()
{
    gotoxy(0, 0);
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;
    for (int y = 0; y < HEIGHT; y++)
    {
        cout << "#";
        for (int x = 0; x < WIDTH; x++)
        {
            bool printed = false;
            for (size_t i = 0; i < snake.size(); i++)
            {
                if (snake[i].x == x && snake[i].y == y)
                {
                    cout << (i == 0 ? "O" : "o");
                    printed = true;
                    break;
                }
            }
            if (!printed && food.x == x && food.y == y)
            {
                cout << "@";
                printed = true;
            }
            if (!printed)
                cout << " ";
        }
        cout << "#" << endl;
    }
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;
    cout << "Score: " << score << "   Press WASD/Arrows to move, Q to quit" << endl;
}

void input()
{
    if (_kbhit())
    {
        int key = _getch();
        if (key == 224)
        {
            key = _getch();
            switch (key)
            {
            case 72:
                if (dirY == 0)
                {
                    dirX = 0;
                    dirY = -1;
                }
                break;
            case 80:
                if (dirY == 0)
                {
                    dirX = 0;
                    dirY = 1;
                }
                break;
            case 75:
                if (dirX == 0)
                {
                    dirX = -1;
                    dirY = 0;
                }
                break;
            case 77:
                if (dirX == 0)
                {
                    dirX = 1;
                    dirY = 0;
                }
                break;
            }
        }
        else
        {
            switch (tolower(key))
            {
            case 'w':
                if (dirY == 0)
                {
                    dirX = 0;
                    dirY = -1;
                }
                break;
            case 's':
                if (dirY == 0)
                {
                    dirX = 0;
                    dirY = 1;
                }
                break;
            case 'a':
                if (dirX == 0)
                {
                    dirX = -1;
                    dirY = 0;
                }
                break;
            case 'd':
                if (dirX == 0)
                {
                    dirX = 1;
                    dirY = 0;
                }
                break;
            case 'q':
                exit(0);
            }
        }
    }
}

bool moveSnake()
{
    Point newHead = {snake[0].x + dirX, snake[0].y + dirY};

    if (newHead.x < 0 || newHead.x >= WIDTH || newHead.y < 0 || newHead.y >= HEIGHT)
        return false;

    bool eating = (newHead.x == food.x && newHead.y == food.y);
    size_t end = snake.size() - (eating ? 0 : 1);
    for (size_t i = 0; i < end; i++)
    {
        if (snake[i].x == newHead.x && snake[i].y == newHead.y)
            return false;
    }

    snake.insert(snake.begin(), newHead);
    if (eating)
    {
        score++;
        generateFood();
    }
    else
    {
        snake.pop_back();
    }
    return true;
}

int main()
{
    srand(static_cast<unsigned>(time(NULL)));
    hideCursor();
    snake.push_back({WIDTH / 2, HEIGHT / 2});
    generateFood();

    while (true)
    {
        draw();
        input();
        if (!moveSnake())
            break;
        Sleep(100);
    }

    system("cls");
    cout << "Game Over! Final Score: " << score << endl;
    system("pause");
    return 0;
}