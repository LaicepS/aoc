#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

using Maze = vector<int>;

int jump(int head, Maze const & maze)
{
  return head + maze[head];
}

void update_maze(int idx, Maze & maze)
{
  if(maze[idx] >= 3)
    maze[idx]--;
  else
    maze[idx]++;
}

int exit_maze(Maze const & maze_)
{
  int steps = 0;
  int head = 0;
  auto maze(maze_);

  while(true)
  {
    if (head >= (int) maze.size())
      break;

    auto previous_head = head;
    head = jump(head, maze);
    update_maze(previous_head, maze);

    steps++;
  }

  return steps;
}

Maze read_maze(char const * path)
{
  ifstream maze_file(path);
  string line;
  Maze maze;
  while(getline(maze_file, line))
    maze.push_back(atoi(line.c_str()));

  return maze;
}

int main()
{
  assert(exit_maze({1}) == 1);
  assert(exit_maze({0, 3, 0, 1, -3}) == 10);
  
  auto maze = read_maze("maze.txt");

  auto res = exit_maze(maze);
  cout << res << endl;

  return 0;
}
