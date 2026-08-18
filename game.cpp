#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#endif
using namespace std;

void setupConsole() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
    SetConsoleOutputCP(CP_UTF8);
#endif
}

char getchNonBlocking()
{
#ifdef _WIN32
    if (_kbhit()) {
        return _getch();
    }
    return 0;
#else
    char ch = 0;
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fd_set set;
    struct timeval tv{0, 0};
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    if (select(STDIN_FILENO + 1, &set, NULL, NULL, &tv) > 0) {
        read(STDIN_FILENO, &ch, 1);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

struct node
{
    int mi, mj;
    char data;
    node *next;
};

class Snake
{
public:
    node *head;
    int m1, m2;
    int dx, dy;
    char hch, bch;
    int cs, hs;

    Snake()
    {
        cs = 0;
        hs = 0;
        m1 = 1;
        m2 = 1;
        dx = 0;
        dy = 1;
        hch = '>';
        bch = '+';
        head = NULL;
        initSnake();
    }

    void insert(char data)
    {
        node *n = new node;
        n->data = data;
        n->next = NULL;
        if (head == NULL) {
            n->mi = m1;
            n->mj = m2;
            head = n;
        }
        else
        {
            node *tn = head;
            while (tn->next != NULL)
            {
                tn = tn->next;
            }
            n->mi = tn->mi;
            n->mj = tn->mj;
            tn->next = n;
        }
    }

    void initSnake()
    {
        head = new node{m1, m2, hch, NULL};
        insert('+');
        insert('+');
    }

    bool onSnake(int x, int y)
    {
        if (x == m1 && y == m2)
            return true;
        node *tn = head;
        while (tn)
        {
            if (tn->mi == x && tn->mj == y)
                return true;
            tn = tn->next;
        }
        return false;
    }

    bool hitSnake(int x, int y)
    {
        node *tn = head;
        if (tn) tn = tn->next;
        while (tn)
        {
            if (tn->mi == x && tn->mj == y)
                return true;
            tn = tn->next;
        }
        return false;
    }

    void changeIndex()
    {
        if (!head)
            return;
        node *tn = head;
        int prev_i = m1, prev_j = m2;
        while (tn)
        {
            int temp_i = tn->mi;
            int temp_j = tn->mj;
            tn->mi = prev_i;
            tn->mj = prev_j;
            prev_i = temp_i;
            prev_j = temp_j;
            tn = tn->next;
        }
    }

    void reset()
    {
        node* current = head;
        while (current != NULL) {
            node* next = current->next;
            delete current;
            current = next;
        }
        cs = 0;
        m1 = 1;
        m2 = 1;
        dx = 0;
        dy = 1;
        hch = '>';
        head = NULL;
        initSnake();
    }
};

class Food
{
public:
    int fi, fj;
    Food()
    {
        srand(time(0));
        fi = (rand() % 18) + 1;
        fj = (rand() % 48) + 1;
    }

    void newFood(int r, int c, vector<Snake> &snakes)
    {
        bool occupied;
        do
        {
            fi = (rand() % (r - 2)) + 1;
            fj = (rand() % (c - 2)) + 1;
            occupied = false;
            for (auto &s : snakes)
            {
                if (s.onSnake(fi, fj))
                {
                    occupied = true;
                    break;
                }
            }
        } while (occupied);
    }
};

class GameBoard
{
    static const int r = 20;
    static const int c = 50;
    char arr[r][c];

public:
    vector<Snake> snakes;
    Food food;

    GameBoard()
    {
        Snake p1;
        Snake p2;
        p2.m1 = r - 2;
        p2.m2 = c - 2;
        p2.dx = 0;
        p2.dy = -1;
        p2.hch = '<';
        node* tn = p2.head;
        while (tn) {
            tn->mi = p2.m1;
            tn->mj = p2.m2;
            tn = tn->next;
        }
        snakes.push_back(p1);
        snakes.push_back(p2);
    }

    void display()
    {
        cout << "P1 Score:" << snakes[0].cs << "  P2 Score:" << snakes[1].cs << endl;
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                if (arr[i][j] == ' ')
                    cout << "\u2B1C";
                else if (arr[i][j] == '#')
                    cout << "\U0001F9F1";
                else if (arr[i][j] == 'o')
                    cout << "\U0001F34E";
                else if (arr[i][j] == '>' || arr[i][j] == '<' || arr[i][j] == '^' || arr[i][j] == 'v')
                    cout << "\U0001F438";
                else if (arr[i][j] == '+')
                    cout << "\U0001F7E9";
                else
                    cout << arr[i][j];
            }
            cout << endl;
        }
    }

    void createGrid()
    {
        cout << "\033[H\033[J";
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                if ((i == 0 || i == r - 1) || (j == 0 || j == c - 1))
                    arr[i][j] = '#';
                else if (i == food.fi && j == food.fj)
                {
                    bool foodEaten = false;
                    for (auto &s : snakes)
                    {
                        if (s.m1 == food.fi && s.m2 == food.fj)
                        {
                            s.insert(s.bch);
                            s.cs += 10;
                            foodEaten = true;
                        }
                    }
                    if (foodEaten)
                        food.newFood(r, c, snakes);
                    else
                        arr[i][j] = 'o';
                }
                else
                    arr[i][j] = ' ';

                for (auto &s : snakes)
                {
                    node *tn = s.head;
                    while (tn != NULL)
                    {
                        arr[tn->mi][tn->mj] = tn->data;
                        tn = tn->next;
                    }
                }
            }
        }

        bool s1Hit = (snakes[0].m1 == 0 || snakes[0].m1 == r - 1 || snakes[0].m2 == 0 || snakes[0].m2 == c - 1) || snakes[0].hitSnake(snakes[0].m1, snakes[0].m2) || snakes[1].onSnake(snakes[0].m1, snakes[0].m2);
        bool s2Hit = (snakes[1].m1 == 0 || snakes[1].m1 == r - 1 || snakes[1].m2 == 0 || snakes[1].m2 == c - 1) || snakes[1].hitSnake(snakes[1].m1, snakes[1].m2) || snakes[0].onSnake(snakes[1].m1, snakes[1].m2);

        if (s1Hit || s2Hit)
        {
            if (s1Hit) { snakes[0].m1 -= snakes[0].dx; snakes[0].m2 -= snakes[0].dy; arr[snakes[0].m1][snakes[0].m2] = snakes[0].hch; }
            if (s2Hit) { snakes[1].m1 -= snakes[1].dx; snakes[1].m2 -= snakes[1].dy; arr[snakes[1].m1][snakes[1].m2] = snakes[1].hch; }
            display();
            if (s1Hit && s2Hit) cout << "Game Over! Both players lost!\n" << endl;
            else if (s1Hit) cout << "Game Over! Player 1 lost!\n" << endl;
            else cout << "Game Over! Player 2 lost!\n" << endl;
            cout << "Press any key-Restart\nX-Exit\n";
            char c = 0;
            while (c == 0) c = getchNonBlocking();

            switch (c)
            {
                case 'x':
                case 'X': exit(0);
                default:
                    snakes[0].reset();
                    snakes[1].reset();
                    snakes[1].m1 = r - 2;
                    snakes[1].m2 = c - 2;
                    snakes[1].dx = 0;
                    snakes[1].dy = -1;
                    snakes[1].hch = '<';
                    node* tn = snakes[1].head;
                    while (tn) {
                        tn->mi = snakes[1].m1;
                        tn->mj = snakes[1].m2;
                        tn = tn->next;
                    }
                    food.newFood(r, c, snakes);
            }
        }
        else
        {
            arr[snakes[0].m1][snakes[0].m2] = snakes[0].hch;
            arr[snakes[1].m1][snakes[1].m2] = snakes[1].hch;
        }
    }

    void handleInput()
    {
        char ch = getchNonBlocking();
        if (ch == '\033')
        {
            getchNonBlocking();
            switch (getchNonBlocking())
            {
                case 'A': snakes[0].dx = -1; snakes[0].dy = 0; snakes[0].hch = '^'; break;
                case 'B': snakes[0].dx = 1;  snakes[0].dy = 0; snakes[0].hch = 'v'; break;
                case 'C': snakes[0].dx = 0;  snakes[0].dy = 1; snakes[0].hch = '>'; break;
                case 'D': snakes[0].dx = 0;  snakes[0].dy = -1; snakes[0].hch = '<'; break;
            }
        }
#ifdef _WIN32
        else if (ch == -32 || ch == 224) {
            switch (getchNonBlocking()) {
                case 72: snakes[0].dx = -1; snakes[0].dy = 0; snakes[0].hch = '^'; break;
                case 80: snakes[0].dx = 1;  snakes[0].dy = 0; snakes[0].hch = 'v'; break;
                case 77: snakes[0].dx = 0;  snakes[0].dy = 1; snakes[0].hch = '>'; break;
                case 75: snakes[0].dx = 0;  snakes[0].dy = -1; snakes[0].hch = '<'; break;
            }
        }
#endif
        else
        {
            switch (ch)
            {
                case 'w': case 'W': snakes[1].dx = -1; snakes[1].dy = 0; snakes[1].hch = '^'; break;
                case 'a': case 'A': snakes[1].dx = 0; snakes[1].dy = -1; snakes[1].hch = '<'; break;
                case 's': case 'S': snakes[1].dx = 1; snakes[1].dy = 0; snakes[1].hch = 'v'; break;
                case 'd': case 'D': snakes[1].dx = 0; snakes[1].dy = 1; snakes[1].hch = '>'; break;
            }
        }
    }

    void run() {
        while (true)
        {
            handleInput();
            for (auto &s : snakes)
            {
                s.m1 += s.dx;
                s.m2 += s.dy;
                s.changeIndex();
            }
            createGrid();
            display();
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }
};

int main()
{
    setupConsole();
    GameBoard game;
    game.run();
    return 0;
}