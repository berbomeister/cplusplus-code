#include <iostream>
#include <vector>
#include <tuple>
enum Mark
{
    Nothing = 0,
    Cross,
    Nought
};
#define Board std::vector<Mark>
Mark default_move_computer = Nought;
Mark default_move_player = Cross;

std::ostream &operator<<(std::ostream &os, const Mark m)
{
    switch (m)
    {
    case Nothing:
        os << "_ ";
        break;
    case Cross:
        os << "X ";
        break;
    case Nought:
        os << "O ";
        break;
    }
    return os;
}
std::ostream &operator<<(std::ostream &os, Board brd)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            os << brd[i * 3 + j];
        }
        os << std::endl;
    }
    return os;
}
Mark operator!(Mark &a)
{
    if (a == Nothing)
        return Nothing;
    if (a == Cross)
        return Nought;
    return Cross;
}
std::pair<int, int> maximize(Board &brd, int &alpha, int &beta, int depth, Mark move);
std::pair<int, int> minimize(Board &brd, int &alpha, int &beta, int depth, Mark move);

bool play_move(Board &board, unsigned i, Mark move)
{
    if (i >= 9 || board[i] != Nothing)
    {
        return false;
    }
    board[i] = move;

    // std::cout << board << "\n-----------\n";
    return true;
}

bool terminal_state(Board &brd, Mark &winning_mark)
{
    bool finished = true;
    for (int i = 0; i < 3; i++)
    {
        if (brd[i * 3] != Nothing && brd[i * 3] == brd[i * 3 + 1] && brd[i * 3] == brd[i * 3 + 2])
        {
            winning_mark = brd[i * 3];
            // std::cout << "row\n";
            return true;
        }
        if (brd[i] != Nothing && brd[i] == brd[i + 3] && brd[i] == brd[i + 6])
        {
            winning_mark = brd[i];
            // std::cout << "column\n";
            return true;
        }
        if (brd[i * 3] == Nothing || brd[i * 3 + 1] == Nothing || brd[i * 3 + 2] == Nothing)
        {
            finished = false;
        }
    }
    if (brd[0] != Nothing && brd[0] == brd[4] && brd[0] == brd[8])
    {
        winning_mark = brd[0];
        // std::cout << "main diag\n";
        return true;
    }
    if (brd[2] != Nothing && brd[2] == brd[4] && brd[2] == brd[6])
    {
        winning_mark = brd[2];
        // std::cout << "sec diag\n";
        return true;
    }
    winning_mark = Nothing;
    return finished;
}

// int utility(unsigned depth, Mark move)
// {
//     if (move == default_move_computer)
//     {
//         return 20 - depth;
//     }
//     else if (move == default_move_player)
//     {
//         return -20 + depth;
//     }
//     return 0;
// }

std::pair<int, int> maximize(Board &brd, int &alpha, int &beta, int depth, Mark move)
{
    Mark t;
    if (terminal_state(brd, t))
    {
        if (t == move)
        {
            return std::make_pair(20 - depth, -1);
        }
        else if (t == !move)
        {
            return std::make_pair(-20 + depth, -1);
        }
        return std::make_pair(0, -1);
        // return std::make_pair(utility(depth, move), -1);
        // return std::make_pair(20 - depth, -1);
    }
    int best = INT_MIN;
    int cell_id = -1;
    for (int i = 0; i < 9; i++)
    {
        if (play_move(brd, i, move))
        {
            int _i = 0;
            int value;
            std::tie(value, _i) = minimize(brd, alpha, beta, depth + 1, !move);
            brd[i] = Nothing;
            if (value > best)
            {
                best = value;
                cell_id = i;
                if (best > alpha)
                {
                    // return std::make_pair(best, cell_id);
                    alpha = value;
                }
                // alpha = std::max(alpha, best);
            }
            if (alpha < beta)
            {
                // printf("index=%d alpha=%d beta=%d\n", i, alpha, beta);
                break;
            }
        }
    }
    return std::make_pair(best, cell_id);
}
std::pair<int, int> minimize(Board &brd, int &alpha, int &beta, int depth, Mark move)
{
    Mark t;
    // std::cout << "mark here is " << move << "\n";
    if (terminal_state(brd, t))
    {
        if (t == move)
        {
            return std::make_pair(-20 + depth, -1);
        }
        else if (t == !move)
        {
            return std::make_pair(20 - depth, -1);
        }
        return std::make_pair(0, -1);
        // std::cout << t << std::endl;
        // return std::make_pair(utility(depth, move), -1);
        // return std::make_pair(-20 + depth, -1);
    }
    int best = INT_MAX;
    int cell_id = -1;
    // std::cout << "mark here is " << move << "\n";
    // std::cout << move << "\n";
    for (int i = 0; i < 9; i++)
    {
        bool valid_play = play_move(brd, i, move);
        // std::cout << brd << "\n-----------\n";
        if (valid_play)
        {
            int _i = 0;
            int value;
            std::tie(value, _i) = maximize(brd, alpha, beta, depth + 1, !move);
            brd[i] = Nothing;
            if (value < best)
            {
                best = value;
                cell_id = i;
                if (best < alpha)
                {
                    // return std::make_pair(best, cell_id);
                    beta = value;
                }
                beta = std::min(best, beta);
            }
            if (alpha < beta)
            {
                // printf("index=%d alpha=%d beta=%d\n", i, alpha, beta);
                break;
            }
        }
    }
    return std::make_pair(best, cell_id);
}
int minimax_decision(Board &brd, Mark &computer_move)
{
    // std::cout << "ASd\n";
    // std::cout << "mark here is " << computer_move << "\n";
    int alpha = INT_MIN;
    int beta = INT_MAX;
    int value, index;
    std::tie(value, index) = minimize(brd, alpha, beta, 0, computer_move);

    // printf("took decision with %d value\n alpha= %d, beta= %d\n", value, alpha, beta);
    return index;
}

void play()
{
    Board brd(9);
    Mark winning_move;
    char first;
    bool player = false;
    while (true)
    {
        printf("Who goes first, computer or player?[c/p]\n");
        scanf("%c", &first);
        if (first == 'c')
        {
            // computer goes first
            player = false;
            break;
        }
        if (first == 'p')
        {
            // player goes first
            player = true;
            break;
        }
    }
    if (player)
    {
        while (!terminal_state(brd, winning_move))
        {
            int x, y;
            do
            {
                printf("whats your move ?\n");
                scanf("%d %d", &x, &y);
            } while (!play_move(brd, x * 3 + y, default_move_player));

            std::cout << brd << "\n-------\n";

            if (terminal_state(brd, winning_move))
                break;

            int pc_move = minimax_decision(brd, default_move_computer);
            play_move(brd, pc_move, default_move_computer);

            std::cout << brd << "\n-------\n";
        }
    }
    else
    {
        while (!terminal_state(brd, winning_move))
        {
            int pc_move = minimax_decision(brd, default_move_computer);
            play_move(brd, pc_move, default_move_computer);

            std::cout << brd << "\n-------\n";

            if (terminal_state(brd, winning_move))
                break;

            int x, y;
            do
            {
                printf("whats your move ?\n");
                scanf("%d %d", &x, &y);
            } while (!play_move(brd, x * 3 + y, default_move_player));

            std::cout << brd << "\n-------\n";
        }
    }
    if (winning_move == Nothing)
    {
        printf("It's a draw!\n");
    }
    else if (winning_move == default_move_player)
    {
        printf("You win!\n");
    }
    else
    {
        printf("I win!\n");
    }
}
int main()
{
    play();
    // Board test(9);
    // std::cout << test << "\n--------\n";
    // play_move(test, 4, !default_move_computer);
    // std::cout << test << "\n--------\n";
    return 0;
}