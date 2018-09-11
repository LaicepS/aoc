#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

bool is_anagram(string const & a, string const & b)
{
  if(a.size() != b.size())
    return false;

  vector<char> a_letters(a.begin(), a.end());
  vector<char> b_letters(b.begin(), b.end());

  sort(a_letters.begin(), a_letters.end());
  sort(b_letters.begin(), b_letters.end());

  for(size_t i = 0; i < a_letters.size(); i++)
    if(a_letters[i] != b_letters[i])
      return false;
  
  return true;
}

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

bool contains_anagrams(vector<string> const & words)
{
  for(auto w = words.begin(); w != words.end(); w++)
    for(auto w2 = w+1; w2 != words.end(); w2++)
      if(is_anagram(*w, *w2))
	return true;

  return false;
}

bool is_valid_passphrase(string const & s)
{
  auto words = split(s);
  if(words.size() < 2)
    return false;

  if (contains_anagrams(words))
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
      valid_passphrase++;

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

  assert(is_anagram("toto", "toto"));
  assert(!is_anagram("toto", ""));
  assert(is_anagram("toto", "otot"));
  assert(!is_anagram("toto", "ototo"));
  assert(is_anagram("tota", "atot"));
  assert(!is_anagram("tota", "atat"));

  cout << valid_passphrase << endl;

  return 0;
}
