#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

using Banks = vector<int>;

int select_bank(Banks const & banks)
{
  int max = -1;

  for(size_t i = 0; i < banks.size(); i++)
    if(banks[i] > banks[max])
      max = i;

  return max;
}

void update_banks(Banks & banks)
{
  auto bank = select_bank(banks);
  auto mem_count = banks[bank];
  banks[bank] = 0;
  while(mem_count--)
    banks[++bank % banks.size()]++;
}

int communism(Banks const & banks_)
{
  int steps = 0;
  vector<Banks> previous_banks{banks_};
  vector<int> banks(banks_);

  while(true)
  {
    steps++;
    update_banks(banks);
    if(find(previous_banks.begin(), previous_banks.end(), banks) != end(previous_banks))
      return steps;

    previous_banks.push_back(banks);
  }
}

int main()
{
  assert(select_bank({0, 2, 7, 0}) == 2);
  assert(select_bank({2, 4, 1, 2}) == 1);
  assert(select_bank({3, 1, 2, 3}) == 0);

  {
    Banks b { 0, 2, 7, 0 };
    update_banks(b);
    assert(b == Banks({ 2, 4, 1, 2}));
  }

  {
    Banks b { 2, 4, 1, 2 };
    update_banks(b);
    assert(b == Banks({ 3, 1, 2, 3}));
  }

  {
    Banks b { 3, 1, 2, 3};
    update_banks(b);
    assert(b == Banks({ 0, 2, 3, 4}));
  }

  assert(communism({0, 2, 7, 0}) == 5);
  cout << communism({ 0, 5, 10, 0, 11, 14, 13, 4, 11, 8, 8, 7, 1, 4, 12, 11}) << endl;
  return 0;
}
