#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<string> split(string const & sentence)
{
  istringstream ss(sentence);
  string word;
  vector<string> words;
  while(getline(ss, word, ' '))
    words.push_back(word);

  return words;
} 

bool contains_duplicates(vector<string> const & words)
{
  for(auto it = words.begin(); it != words.end(); it++)
    if(find(it + 1, words.end(), *it) != words.end())
      return true;

  return false;
}

bool is_valid_passphrase(string const & s)
{
  auto words = split(s);
  if(words.size() < 2)
    return false;

  if (contains_duplicates(words))
    return false;

  return true;
}

int main()
{
  string line;
  ifstream mdp_file("mdp.txt");

  int valid_passphrase = 0;
  while(getline(mdp_file, line))
    if(is_valid_passphrase(line))
    {
      valid_passphrase++;
      cout << "phrase: " << line << endl;
    }

  cout << valid_passphrase << endl;

  assert(!is_valid_passphrase(""));
  assert(!is_valid_passphrase("toto"));
  assert(is_valid_passphrase("toto tata"));
  assert(!is_valid_passphrase("toto toto"));
  assert(is_valid_passphrase("aa bb cc dd ee"));
  assert(!is_valid_passphrase("aa bb cc dd aa"));
  assert(is_valid_passphrase("aa bb cc dd aaa"));

  assert(contains_duplicates({"tata", "tata"}));
  assert(!contains_duplicates({"tata", "toto"}));
  assert(contains_duplicates({"tata", "toto", "tata"}));
  assert(contains_duplicates({"toto", "tata", "tata"}));
  assert(!contains_duplicates({"aa", "bb", "cc", "aaa"}));

  return 0;
}
