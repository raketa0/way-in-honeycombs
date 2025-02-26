/*
Пчелы живут в ульях, где есть соты. Соты представляют собой поле правильных шестиугольников, соприкасающихся друг с другом. Правильное поле строится следующим образом:
сначала имеется всего одна сота (рис. 1) – это правильное поле первого уровня;
затем вокруг соты появляются соседние (рис. 2) – это правильное поле второго уровня;
 затем строится еще один «ободок» (рис. 3) – это правильное поле третьего уровня, и т. д.
Пчела Майя возвращается в улей. Она живет в одной из сот правильного поля уровня N (2 ≤ N ≤ 20). Для того, чтобы добраться до своей соты (если она не расположена с краю поля), Майе нужно переместиться через другие соты, в которых могут жить как друзья (перемещаться можно), так и враги (перемещаться нельзя). Майя очень устала и хочет добраться до своей соты, пройдя через минимальное число других сот. Свой путь она может начинать с любой дружественной соты, находящейся с краю поля (то есть такой соты, которая не окружена со всех сторон соседними сотами).

Ввод. В первой строке файла INPUT.TXT записано одно число N – уровень правильного поля. В следующих 2N-1 строках содержатся последовательности из символов ‘V’, ‘D’, ‘M’ (‘V’ – сота врага, ‘D’ – сота друга, ‘M’ – сота Майи).
Вывод. В единственной строке файла OUTPUT.TXT выводится минимальное число сот, через которые придется пройти Майе, чтобы попасть в свою соту (своя сота тоже считается), или ноль, если Майе не удастся попасть в свою соту.
Примеры
Ввод 1     Ввод 2     Ввод 3
2          3          2
VV         VVV        VV
VMV        VDDV       VMV
VD         VMVDV      VV
           VVDV
           VVD
Вывод 1    Вывод 2    Вывод 3
2          6          0


Рычков Александр Александрович
*/


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
using namespace std;

const int INF = 1000000000;

struct Cell
{
    int row, col;
};

vector<Cell> getNeighbors(int row, int col, const vector<vector<char>>& grid)
{
    vector<Cell> neighbors;
    int currentRowSize = grid[row].size();

    if (col - 1 >= 0)
    {
        neighbors.push_back({ row, col - 1 });
    }
    if (col + 1 < currentRowSize)
    {
        neighbors.push_back({ row, col + 1 });
    }

    if (row - 1 >= 0)
    {
        int upperRowSize = grid[row - 1].size();
        if (upperRowSize < currentRowSize)
        {
            if (col - 1 >= 0)
            {
                neighbors.push_back({ row - 1, col - 1 });
            }
            if (col < upperRowSize)
            {
                neighbors.push_back({ row - 1, col });
            }
        }
        else if (upperRowSize > currentRowSize)
        {
            if (col < upperRowSize)
            {
                neighbors.push_back({ row - 1, col });
            }
            if (col + 1 < upperRowSize)
            {
                neighbors.push_back({ row - 1, col + 1 });
            }
        }
        else
        {
            if (col < upperRowSize)
            {
                neighbors.push_back({ row - 1, col });
            }
            if (col + 1 < upperRowSize)
            {
                neighbors.push_back({ row - 1, col + 1 });
            }
        }
    }

    if (row + 1 < grid.size())
    {
        int lowerRowSize = grid[row + 1].size();
        if (lowerRowSize < currentRowSize)
        {
            if (col - 1 >= 0)
            {
                neighbors.push_back({ row + 1, col - 1 });
            }
            if (col < lowerRowSize)
            {
                neighbors.push_back({ row + 1, col });
            }
        }
        else if (lowerRowSize > currentRowSize)
        {
            if (col < lowerRowSize)
            {
                neighbors.push_back({ row + 1, col });
            }
            if (col + 1 < lowerRowSize)
            {
                neighbors.push_back({ row + 1, col + 1 });
            }
        }
        else
        {
            if (col < lowerRowSize)
            {
                neighbors.push_back({ row + 1, col });
            }
            if (col + 1 < lowerRowSize)
            {
                neighbors.push_back({ row + 1, col + 1 });
            }
        }
    }

    return neighbors;
}

void readInput(const string& filename, int& numRows, vector<vector<char>>& grid, Cell& mayaPosition)
{
    ifstream inputFile(filename);
    inputFile >> numRows;
    string line;
    getline(inputFile, line);

    int currentRow = 0;
    while (getline(inputFile, line))
    {
        if (line.empty()) continue;
        vector<char> currentRowCells;
        for (int i = 0; i < line.size(); i++) {
            char cell = line[i];
            if (cell == ' ')
                continue;
            currentRowCells.push_back(cell);
            if (cell == 'M')
            {
                mayaPosition = { currentRow, static_cast<int>(currentRowCells.size()) - 1 };
            }
        }
        grid.push_back(currentRowCells);
        currentRow++;
    }
    inputFile.close();
}

vector<Cell> findStartCells(const vector<vector<char>>& grid, const Cell& mayaPosition)
{
    vector<Cell> startCells;
    for (int row = 0; row < grid.size(); row++)
    {
        for (int col = 0; col < grid[row].size(); col++)
        {
            if (grid[row][col] == 'D' || grid[row][col] == 'M')
            {
                vector<Cell> neighbors = getNeighbors(row, col, grid);
                if (neighbors.size() < 6)
                {
                    startCells.push_back({ row, col });
                }
            }
        }
    }

    vector<Cell> mayaNeighbors = getNeighbors(mayaPosition.row, mayaPosition.col, grid);
    if (mayaNeighbors.size() < 6)
    {
        startCells.push_back(mayaPosition);
    }

    return startCells;
}

int bfs(const vector<vector<char>>& grid, const vector<Cell>& startCells, const Cell& mayaPosition)
{
    int numRows = grid.size();
    vector<vector<int>> distance(numRows);
    for (int row = 0; row < numRows; row++)
    {
        distance[row].assign(grid[row].size(), INF);
    }

    queue<Cell> queue;
    for (auto start : startCells)
    {
        distance[start.row][start.col] = 1;
        queue.push(start);
    }

    while (!queue.empty())
    {
        Cell current = queue.front();
        queue.pop();
        if (current.row == mayaPosition.row && current.col == mayaPosition.col)
        {
            return distance[current.row][current.col];
        }

        vector<Cell> neighbors = getNeighbors(current.row, current.col, grid);
        for (auto neighbor : neighbors)
        {
            if (distance[neighbor.row][neighbor.col] != INF) continue;
            if (grid[neighbor.row][neighbor.col] == 'D' || grid[neighbor.row][neighbor.col] == 'M') {
                distance[neighbor.row][neighbor.col] = distance[current.row][current.col] + 1;
                queue.push(neighbor);
            }
        }
    }
    return INF;
}

int main()
{
    int numRows;
    vector<vector<char>> grid;
    Cell mayaPosition = { -1, -1 };

    readInput("INPUT.TXT", numRows, grid, mayaPosition);

    if (mayaPosition.row == -1)
    {
        ofstream("OUTPUT.TXT") << 0;
        return 0;
    }

    vector<Cell> startCells = findStartCells(grid, mayaPosition);
    if (startCells.empty())
    {
        ofstream("OUTPUT.TXT") << 0;
        return 0;
    }

    int minSteps = bfs(grid, startCells, mayaPosition);

    ofstream output("OUTPUT.TXT");
    if (minSteps == INF)
    {
        output << 0;
    }
    else
    {
        output << minSteps;
    }

    return 0;
}