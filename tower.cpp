#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "helpers.h"

using namespace std;

vector<string> read_lines(const char * path)
{
  ifstream file(path);
  string line;
  vector<string> res;
  while(getline(file, line))
      res.push_back(line);
  return res;
}

struct Node
{
  string name;
  int weight;
  vector<Node*> children;
};

string pop_word(string & line)
{
  auto next_space = line.find(' ');
  auto word = line.substr(0, next_space);

  if (next_space == string::npos)
    line = "";

  line = string(line, next_space + 1);
    
  return word;
}

int parse_weight(string & line)
{
  auto weight = pop_word(line);
  weight = string(weight.begin()+1, weight.end()-1);
  return atoi(weight.c_str());
}

vector<string> parse_neighbors(string & line)
{
  auto neighbors = split(line, ',');

  for(auto& n: neighbors)
  {
    auto first_char_idx = 0;
    while(is_white(n[first_char_idx]))
      first_char_idx++;

    auto last_char_idx = n.size() - 1;
    while(is_white(n[last_char_idx]))
      last_char_idx--;

    n = n.substr(first_char_idx, last_char_idx+1);
  }

  return neighbors;
}

pair<Node, vector<string>> parse_node(string const & line_)
{
  string line(line_); 
  auto name = pop_word(line);
  auto weight = parse_weight(line);

  if (line == "") // no neighbors
    return { {name, weight, {}}, {}};

  pop_word(line); // remove the arrow

  auto neighbors = parse_neighbors(line);

  return { {name, weight, {}}, neighbors };
}

int get_weight(Node* n)
{
  auto weight = n->weight;
  for(auto child: n->children)
    weight += get_weight(child);
  return weight;
}

pair<Node*, int> get_unbalanced(vector<Node*> v)
{
  int previous_weight = get_weight(v[0]);
  for(size_t i = 1; i < v.size(); i++)
  {
    if (get_weight(v[i]) != previous_weight)
    {
      if(get_weight(v[i+1]) == get_weight(v[i]))
	return {v[i-1], get_weight(v[i])};
      else
      {
	assert(get_weight(v[i+1]) == previous_weight);
	return {v[i], get_weight(v[i-1])};
      }
    }
    previous_weight = get_weight(v[i]);
  }

  return {nullptr, -1};
} 

int balance(Node * n, int neighbor_weight)
{
  auto unbalanced_child = get_unbalanced(n->children);
  if(unbalanced_child.first == nullptr)
  {
    auto children_weight = 0;
    for(auto c: n->children)
      children_weight += get_weight(c);

    return neighbor_weight - children_weight;
  }
  else
    return balance(unbalanced_child.first, unbalanced_child.second);
}

void parse_tree(vector<string> const & lines)
{
  cout << "Parsing file...";

  vector<pair<Node, vector<string>>> parsed_file;
  for(auto line: lines)
    parsed_file.push_back(parse_node(line));

  cout << " File parsed." << endl;

  cout << "Building tree..." << endl;
  for(auto& kv: parsed_file)
  {
    for(auto name: kv.second)
    {
      auto neighbor = find_if(begin(parsed_file), end(parsed_file), 
	  [=](auto a) { return a.first.name == name; });
      assert(neighbor != end(parsed_file));
      kv.first.children.push_back(&neighbor->first);
    }
  }

  cout << "Tree built." << endl;

  auto root = find_if(begin(parsed_file), end(parsed_file), 
	[](auto a) { return a.first.name == "mkxke" ; })->first;
  

  auto b = balance(&root, 0);
  cout << b << endl;
}


void unittests()
{
  test_split();
}

int main()
{
  unittests();
  auto lines = read_lines("tower.txt");
  parse_tree(lines);
  return 0;
}
