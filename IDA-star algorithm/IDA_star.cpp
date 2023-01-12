#include <iostream>
#include <vector>
#include <math.h>
#include <chrono>
#include <algorithm>
int FOUND = 0;
int NOT_FOUND = -1;
int MAX_VAL = INT_MAX;
class Board;
int hamming_score(std::vector<int> tiles, int zero_goal, int size);                 ///
int manhattan_score(std::vector<int> tiles, int zero_goal, int zero_pos, int size); ///
int distance(int pos, int num, int size, int zero_goal);                            ///
Board move_left(std::vector<int> tiles, int zero_pos, int zero_goal);
Board move_right(std::vector<int> tiles, int zero_pos, int zero_goal);
Board move_up(std::vector<int> tiles, int zero_pos, int zero_goal);
Board move_down(std::vector<int> tiles, int zero_pos, int zero_goal);
void neighbours(std::vector<Board> &res, const Board &node);
int search(std::vector<Board> &path, int cost, int bound);
int ida_star(Board &start, std::vector<Board> &path);
void print_path(std::vector<Board> &path);
class Board
{
public:
    int size;
    int hamming;
    int zero_pos;
    int zero_goal;
    int manhattan;
    std::vector<int> tiles;
    Board();
    Board(std::vector<int> arr, int zero_pos, int zero_goal);
    Board(const Board &other);
    //~Board();
    inline bool operator==(const Board &rhs);
    friend std::ostream &operator<<(std::ostream &os, Board &brd);
    bool is_goal();
    bool is_solvable();
};

int main()
{
    // std::vector<int> tiles;
    // int n, i;
    // std::cin >> n >> i;
    // if (i == -1)
    // {
    //     i = n;
    // }
    // for (int pos = 0; pos <= n; pos++)
    // {
    //     int temp;
    //     std::cin >> temp;
    //     tiles.push_back(temp);
    // }
    // for (auto x : tiles)
    // {
    //     std::cout << x << " ";
    // }
    // std::cout << std::endl;
    // Board game_board(tiles, i);
    // std::cout << game_board << std::endl;
    // std::cout << game_board << std::endl;

    std::vector<int> test{1, 2, 3, 4,
                          12, 0, 6, 13,
                          9, 14, 8, 10,
                          5, 15, 7, 11};
    int zero_pos = 5;
    int zero_goal = 15;
    Board test_board = Board(test, zero_pos, zero_goal);
    std::cout << test_board << std::endl;
    // std::cout << test_board.manhattan << " " << test_board.hamming << std::endl;
    printf("---------------------------\n");

    std::vector<Board> path;
    auto start = std::chrono::steady_clock::now();
    int result = ida_star(test_board, path);
    auto end = std::chrono::steady_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    if (result == NOT_FOUND)
    {
        std::cout << "Board wasnt solved in " << time / 1000.0 << "seconds\n";
    }
    else
    {
        std::cout << path.size() - 1 << std::endl;
        print_path(path);
        std::cout << "Elapsed time : " << time / 1000.0 << "s\n";
    }

    return 0;
}
int distance(int pos, int num, int size, int zero_goal)
{
    // if num<=zero_goal
    // then num_pos = num - 1
    // else num_pos = num
    int n = sqrt(size);
    int x_cur = pos / n;
    int y_cur = pos % n;
    int goal_pos = (num <= zero_goal) ? num : num + 1;
    int x_goal = ceil(float(goal_pos) / n) - 1;
    int y_goal = (goal_pos % n == 0) ? n - 1 : (goal_pos % n - 1);
    return abs(x_cur - x_goal) + abs(y_cur - y_goal);
}
int hamming_score(std::vector<int> tiles, int zero_goal, int size)
{

    int score = 0;
    int pos = 0;
    while (pos < zero_goal)
    {
        // printf("first %d %d\n", pos + 1, tiles[pos]);
        score += (tiles[pos] != pos + 1);
        pos++;
    }
    // printf("%d \n", pos);
    pos += 1;
    while (pos < size)
    {
        // printf("second %d %d\n", pos, tiles[pos]);
        score += (tiles[pos] != pos);
        pos++;
    }

    return score;
}
int manhattan_score(std::vector<int> tiles, int zero_goal, int zero_pos, int size)
{
    int dist = 0;
    int pos = 0;
    while (pos < zero_pos)
    {
        // printf("the number %d is on pos %d and has manhattan distance %d\n",
        //        tiles[pos], pos, distance(pos, tiles[pos], size, zero_goal));
        dist += distance(pos, tiles[pos], size, zero_goal);
        pos++;
    }
    pos++;
    while (pos < size)
    {
        // printf("the number %d is on pos %d and has manhattan distance %d\n",
        //        tiles[pos], pos, distance(pos, tiles[pos], size, zero_goal));
        dist += distance(pos, tiles[pos], size, zero_goal);
        pos++;
    }

    return dist;
}
Board::Board()
{
    this->tiles = std::vector<int>(0);
    this->size = 0;
    this->hamming = 0;
    this->manhattan = 0;
    this->zero_pos = 0;
    this->zero_goal = 0;
}
Board::Board(std::vector<int> arr, int zero_pos, int zero_goal)
{
    this->tiles = arr;
    this->size = arr.size();
    this->zero_pos = zero_pos;
    this->zero_goal = zero_goal;
    this->hamming = hamming_score(arr, zero_goal, this->size);
    this->manhattan = manhattan_score(arr, zero_goal, zero_pos, this->size);
}
Board::Board(const Board &other)
{
    this->tiles = other.tiles;
    this->size = other.size;
    this->zero_pos = other.zero_pos;
    this->zero_goal = other.zero_goal;
    this->hamming = other.hamming;
    this->manhattan = other.manhattan;
    // std::cout << "Copied\n";
}
inline bool Board::is_goal()
{
    return this->hamming == 0;
}
bool Board::is_solvable() // not working
{
    // this may not be true
    int inversions_count = 0;
    int size = this->size;
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (this->tiles[i] > this->tiles[j] && this->tiles[j] != 0)
                inversions_count++;
        }
    }
    if (size % 2 == 1)
        return inversions_count % 2 == 0;
    else
    {
        int n = sqrt(size);
        int zero_y = this->zero_pos % n;
        // std::cout << zero_y << " " << inversions_count << std::endl;

        return (inversions_count + zero_y) % 2 == 1;
    }
}
inline bool Board::operator==(const Board &rhs)
{
    return this->tiles == rhs.tiles;
}
std::ostream &operator<<(std::ostream &os, Board &brd)
{
    int n = sqrt(brd.size);
    int i = 0;
    for (auto x : brd.tiles)
    {
        if (i % n == 0 && i != 0)
            os << std::endl;
        os << x << " ";
        i++;
    }
    return os;
}
void neighbours(std::vector<Board> &res, const Board &node)
{
    int n = sqrt(node.size);
    int zero_pos = node.zero_pos;
    int zero_goal = node.zero_goal;
    int x_zero = zero_pos / n;
    int y_zero = zero_pos % n;
    // printf("n=%d x=%d y=%d\n", n, x_zero, y_zero);
    // Board up = move_up(node.tiles, zero_pos);
    // Board down = move_down(node.tiles, zero_pos);
    // Board left = move_left(node.tiles, zero_pos);
    // Board right = move_right(node.tiles, zero_pos);
    // std::cout << &up << " " << &down << " " << &left << " " << &right << std::endl;
    if (x_zero == 0 && y_zero == 0)
    {
        // empty cell is at top left corner
        // ->we can move it down or right

        Board down = move_down(node.tiles, zero_pos, zero_goal);
        Board right = move_right(node.tiles, zero_pos, zero_goal);

        res.push_back(down);
        res.push_back(right);
    }
    else if (x_zero == 0 && y_zero == n - 1)
    {
        // empty cell is at top right corner
        //  -> we can move it down or left

        Board down = move_down(node.tiles, zero_pos, zero_goal);
        Board left = move_left(node.tiles, zero_pos, zero_goal);

        res.push_back(down);
        res.push_back(left);
    }
    else if (x_zero == n - 1 && y_zero == 0)
    {
        // empty cell is at bottom left corner
        //  -> we can move it up or right

        Board up = move_up(node.tiles, zero_pos, zero_goal);
        Board right = move_right(node.tiles, zero_pos, zero_goal);

        res.push_back(up);
        res.push_back(right);
    }
    else if (x_zero == n - 1 && y_zero == n - 1)
    {
        // empty cell is at bottom right corner
        //  -> we can move it up or left

        Board up = move_up(node.tiles, zero_pos, zero_goal);
        Board left = move_left(node.tiles, zero_pos, zero_goal);

        res.push_back(up);
        res.push_back(left);
    }
    else if (y_zero == 0)
    {
        // empty cell is on the left edge
        //  -> we can move it up, down or right

        Board up = move_up(node.tiles, zero_pos, zero_goal);
        Board down = move_down(node.tiles, zero_pos, zero_goal);
        Board right = move_right(node.tiles, zero_pos, zero_goal);

        res.push_back(up);
        res.push_back(down);
        res.push_back(right);
    }
    else if (y_zero == n - 1)
    {
        // empty cell is on the right edge
        //  -> we can move it up, down or left

        Board up = move_up(node.tiles, zero_pos, zero_goal);
        Board down = move_down(node.tiles, zero_pos, zero_goal);
        Board left = move_left(node.tiles, zero_pos, zero_goal);

        res.push_back(up);
        res.push_back(down);
        res.push_back(left);
    }
    else if (x_zero == 0)
    {
        // empty cell is on the top edge
        //  -> we can move it left,right or down

        Board left = move_left(node.tiles, zero_pos, zero_goal);
        Board right = move_right(node.tiles, zero_pos, zero_goal);
        Board down = move_down(node.tiles, zero_pos, zero_goal);

        res.push_back(left);
        res.push_back(right);
        res.push_back(down);
    }
    else if (x_zero == n - 1)
    {
        // empty cell is on the bottom edge
        //  -> we can move it left,right or up

        Board left = move_left(node.tiles, zero_pos, zero_goal);
        Board right = move_right(node.tiles, zero_pos, zero_goal);
        Board up = move_up(node.tiles, zero_pos, zero_goal);

        res.push_back(left);
        res.push_back(right);
        res.push_back(up);
    }
    else
    {
        // empty cell is inside of the board
        //  -> we can move it anywhere

        Board up = move_up(node.tiles, zero_pos, zero_goal);
        Board down = move_down(node.tiles, zero_pos, zero_goal);
        Board left = move_left(node.tiles, zero_pos, zero_goal);
        Board right = move_right(node.tiles, zero_pos, zero_goal);

        // printf("i get inside\n");
        res.push_back(up);
        // printf("i insert1\n");
        res.push_back(down);
        // printf("i insert2\n");
        res.push_back(left);
        // printf("i insert3\n");
        res.push_back(right);
        // printf("i insert4\n");
    }
    // std::cout << &res << " " << res.size() << " " << sizeof(res) << std::endl;
    // for (auto board = res.begin(); board != res.end(); board++)
    // {
    //     // std::cout << board << std::endl;
    //     std::cout << &(*board) << " ";
    //     for (auto x : (*board).tiles)
    //     {
    //         std::cout << x << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // printf("i get here\n");
}
Board move_left(std::vector<int> tiles, int zero_pos, int zero_goal)
{
    // std::vector<int> new_tiles(tiles);

    std::swap(tiles[zero_pos], tiles[zero_pos - 1]);

    Board res = Board(tiles, zero_pos - 1, zero_goal);
    return res;
}
Board move_right(std::vector<int> tiles, int zero_pos, int zero_goal)
{
    // std::vector<int> new_tiles(tiles);

    std::swap(tiles[zero_pos], tiles[zero_pos + 1]);

    Board res = Board(tiles, zero_pos + 1, zero_goal);
    return res;
}
Board move_down(std::vector<int> tiles, int zero_pos, int zero_goal)
{
    // std::vector<int> new_tiles(tiles);
    int n = sqrt(tiles.size());
    std::swap(tiles[zero_pos], tiles[zero_pos + n]);

    Board res = Board(tiles, zero_pos + n, zero_goal);
    return res;
}
Board move_up(std::vector<int> tiles, int zero_pos, int zero_goal)
{
    // std::vector<int> new_tiles(tiles);
    int n = sqrt(tiles.size());
    std::swap(tiles[zero_pos], tiles[zero_pos - n]);

    Board res = Board(tiles, zero_pos - n, zero_goal);
    return res;
}
int ida_star(Board &start, std::vector<Board> &path)
{
    // if (!start.is_solvable())
    // {
    //     printf("Board is not solvable\n");
    //     return NOT_FOUND;
    // }
    int bound = start.manhattan;
    // std::vector<Board> path;
    // std::cout << &path << " " << path.size() << " " << sizeof(path) << std::endl;
    // path.resize(40);
    for (int i = 0; i < 10; i++)
    {
        path.push_back(start);
        // std::cout << &path << " " << path.size() << " " << sizeof(path) << std::endl;
        int t = search(path, 0, bound);
        if (t == FOUND)
        {
            return FOUND;
        }
        else if (t == MAX_VAL)
        {
            return NOT_FOUND;
        }
        bound = t;
        path.pop_back();
    }
    return NOT_FOUND;
}
int search(std::vector<Board> &path, int cost, int bound)
{
    // std::cout << &path << " " << path.size() << " " << sizeof(path) << std::endl;
    Board node = path.back();
    int f_score = cost + node.manhattan;
    if (f_score > bound)
    {
        return f_score;
    }
    else if (node.is_goal())
    {
        return FOUND;
    }
    int min_val = MAX_VAL;
    std::vector<Board> nbs;
    neighbours(nbs, node);

    for (auto succ : nbs)
    {
        if (std::count(path.begin(), path.end(), succ))
        {
            continue;
        }
        path.push_back(succ);
        int t = search(path, cost + 1, bound);
        if (t == FOUND)
        {
            // print_path(path);
            return FOUND;
        }
        else if (t < min_val)
        {
            min_val = t;
        }
        path.pop_back();
    }
    return min_val;
}
void print_path(std::vector<Board> &path)
{
    for (auto x = path.begin(); x != path.end(); x++)
    {
        if (std::next(x, 1) != path.end())
        {
            int val = (*x).zero_pos - (*std::next(x, 1)).zero_pos;
            if (val == -1)
            {
                std::cout << "left\n";
            }
            else if (val == 1)
            {
                std::cout << "right\n";
            }
            else if (val < -1)
            {
                std::cout << "up\n";
            }
            else
            {
                std::cout << "down\n";
            }
        }
    }
}