#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

struct Pos
{
  int x, y;
};

struct WeightedPos
{
  Pos p;
  int weight;
};

bool operator==(Pos const & a, Pos const & b)
{
  return a.x == b.x && a.y == b.y;
}

enum Dir { Up, Down, Left, Right }; 

Pos getNextPos(Pos const & c, Dir dir)
{
  switch(dir)
  {
    case Right: return Pos({c.x + 1, c.y});
    case Left: return Pos({c.x + -1, c.y});
    case Up: return Pos({c.x, c.y + 1});
    case Down: return Pos({c.x, c.y - 1});
  }

  throw runtime_error("Unknown dir");
}

int abs(int val)
{
  return val > 0 ? val : -val;
}

int sign(int val)
{
  return val > 0 ? 1 : -1;
}

Pos getInnerPos(Pos const & pos, Dir dir)
{
  switch(dir)
  {
    case Right: return { pos.x, pos.y + 1 };
    case Left: return { pos.x, pos.y - 1 };
    case Up: return { pos.x - 1, pos.y };
    case Down: return { pos.x + 1, pos.y };
  }

  throw runtime_error("Unknown dir");
}

bool shouldTurn(vector<Pos> const & previous, Dir curDir)
{
  if (previous.size() == 1)
    return false;

  auto curPos = previous.back();
  auto innerPos = getInnerPos(curPos, curDir);
  return find(previous.begin(), previous.end(), innerPos) == end(previous);
}

Dir getNextDir(Dir curDir)
{
  switch(curDir)
  {
    case Left: return Down;
    case Right: return Up;
    case Down: return Right;
    case Up: return Left;
  }

  throw runtime_error("Unknown dir");
}

string toString(Dir d)
{
  switch(d)
  {
    case Left: return "Left";
    case Right: return "Right";
    case Up: return "Up";
    case Down: return "Down";
  }

  throw runtime_error("Unknown dir");
}

vector<Pos> makeNeighbors(Pos const & p)
{
  return {
    { p.x + 1, p.y },
    { p.x + 1, p.y + 1 },
    { p.x , p.y + 1},
    { p.x - 1, p.y + 1 },
    { p.x - 1, p.y },
    { p.x - 1, p.y - 1 },
    { p.x, p.y -1 },
    { p.x + 1, p.y - 1 }
  };
}

vector<Pos> extractPos(vector<WeightedPos> const & v)
{
  vector<Pos> res;
  for(auto elt: v)
    res.push_back(elt.p);
  return res;
}

int getWeight(Pos const & p, vector<WeightedPos> const & currentWeights)
{
  auto neighbors = makeNeighbors(p);
  auto weightSum = 0;
  for(auto n: neighbors)
  {
    for(auto weightedPos: currentWeights)
      if (n == weightedPos.p)
	weightSum += weightedPos.weight;
  }

  return weightSum;
}

int getManhattanPos(int limit)
{
  vector<WeightedPos> prevWeights;

  auto curDir = Right;
  prevWeights.push_back({{0, 0}, 1});
  auto steps = 1;

  while(true)
  {
    auto nextPos = getNextPos(prevWeights.back().p, curDir);
    auto weight = getWeight(nextPos, prevWeights);
    if (weight > limit)
      return weight;

    prevWeights.push_back({nextPos, weight});
    auto prevPos = extractPos(prevWeights);

    if(shouldTurn(prevPos, curDir))
      curDir = getNextDir(curDir);

    steps++;
  }

  throw runtime_error(":(");
}

int main(int argc, char** argv)
{
  assert(Pos({1, 1}) == getNextPos({0, 1}, Right));
  assert(Pos({11, 13}) == getNextPos({12, 13}, Left));
  assert(Pos({55, 2}) == getNextPos({55, 1}, Up));
  assert(Pos({-12, -15}) == getNextPos({-12, -14}, Down));

  assert(Pos({12, 13}) == getInnerPos({12, 12}, Right));
  assert(Pos({-8, 5}) == getInnerPos({-9, 5}, Down));
  assert(Pos({18, -13}) == getInnerPos({19, -13}, Up));
  assert(Pos({-7, -77}) == getInnerPos({-7, -76}, Left));

  assert(!shouldTurn({{0, 0}}, Right));
  assert(shouldTurn({ {0, 0}, {1, 0} }, Right));
  assert(shouldTurn({{0, 0}, {1, 0}, {1, 1}}, Up));
  assert(!shouldTurn({{0, 0}, {1, 0}, {1, 1}, {0, 1}}, Left));
  assert(!shouldTurn({{0, 0}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}}, Down));

  /*
  assert(Pos({0, 0}) == getManhattanPos(1).p);
  assert(Pos({1, 0}) == getManhattanPos(2).p);
  assert(Pos({1, 1}) == getManhattanPos(3).p);
  assert(Pos({2, 1}) == getManhattanPos(12).p);
  */

  auto limit = atoi(argv[1]);
  auto res = getManhattanPos(limit);
  cout << " res = " << res << endl;

  return 0;
}
