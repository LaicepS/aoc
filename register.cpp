#include <fstream>
#include <map>
#include <string>
#include <climits>

#include <gtest/gtest.h>

using namespace std;

enum Op
{
  Inc, Dec
};

enum CondOp
{
  Lt, Le, Gt, Ge, Eq, Neq
};

struct Condition
{
  string reg;
  CondOp op;
  int operand;
};

struct Instruction
{
  string reg;
  Op op;
  int operand;
  Condition cond;
};

Op to_op(string const & s)
{
  if(s == "inc") return Inc;
  if(s == "dec") return Dec;
  throw runtime_error("Unknown op string");
}

TEST(InstructionParsing, OpParsing)
{
  ASSERT_THROW(to_op("foof"), std::exception);
  ASSERT_EQ(Dec, to_op("dec"));
}

string pop_word(stringstream & s)
{
  string word;
  getline(s, word, ' ');
  return word;
}

CondOp to_cond_op(string const & s)
{
  if (s == "<") return Lt;
  if (s == "<=") return Le;
  if (s == ">") return Gt;
  if (s == ">=") return Ge;
  if (s == "==") return Eq;
  if (s == "!=") return Neq;

  throw runtime_error("Unkwown conditional operator string");
}

Instruction parse_instr(string const & line)
{
  auto ss = stringstream(line);
  Instruction instr;
  instr.reg = pop_word(ss);
  instr.op = to_op(pop_word(ss));
  instr.operand = atoi(pop_word(ss).c_str());
  pop_word(ss); // pop the 'if'
  instr.cond.reg = pop_word(ss);
  instr.cond.op = to_cond_op(pop_word(ss));
  instr.cond.operand = atoi(pop_word(ss).c_str());
  return instr;
}

using Memory = map<string, int>;

bool eval(Condition const & cond, Memory & mem)
{
  auto const reg_val = mem[cond.reg];
  switch(cond.op)
  {
    case Lt: return reg_val < cond.operand;
    case Le: return reg_val <= cond.operand;
    case Gt: return reg_val > cond.operand;
    case Ge: return reg_val >= cond.operand;
    case Eq: return reg_val == cond.operand;
    case Neq: return reg_val != cond.operand;
  }

  return true;
}

TEST(Execution, Eval)
{
  Memory mem({ {"a", 1} });
  ASSERT_TRUE(eval(Condition({ "a", Eq, 1}), mem));
}

void execute_instr(Instruction const & inst, Memory & mem)
{
  switch(inst.op)
  {
    case Inc: mem[inst.reg] += inst.operand; break;
    case Dec: mem[inst.reg] -= inst.operand; break;
  }
} 

TEST(Execution, ExeInstr)
{
  Memory mem({{"a", 1}});
  Instruction instru({"a", Inc, 10, {}});
  execute_instr(instru, mem);
  EXPECT_EQ(11, mem["a"]);
}

void execute(Instruction const & instr, Memory & memory)
{
  if(memory.find(instr.reg) == memory.end())
    memory[instr.reg] = 0;

  if(memory.find(instr.cond.reg) == memory.end())
    memory[instr.cond.reg] = 0;

  auto const cond = eval(instr.cond, memory);
  if(cond)
    execute_instr(instr, memory);
}

TEST(Execution, )
{
  {
    Memory memory; 
    execute(parse_instr("b inc 5 if a > 1"), memory);
    EXPECT_EQ(0, memory.at("a"));
    EXPECT_EQ(0, memory.at("b"));
  }

  {
    Memory memory; 
    execute(parse_instr("a inc 1 if b < 5"), memory);
    EXPECT_EQ(1, memory.at("a"));
  }
}

void execute_file(char const * file)
{
  ifstream f(file);
  string line;
  map<string, int> memory;
  int max_val = INT_MIN;
  while(getline(f, line))
  {
    auto instr = parse_instr(line);
    execute(instr, memory);
    if (memory[instr.reg] > max_val)
      max_val = memory[instr.reg];
  }

  cout << max_val << endl;

  int max = -10000;
  for(auto val: memory)
    if(val.second > max)
      max = val.second;

  cout << max << endl;
}

TEST(InstructionParsing, General)
{
  auto const instru = parse_instr("b inc 5 if a > 10");
  EXPECT_EQ("b", instru.reg);
  EXPECT_EQ(Inc, instru.op);
  EXPECT_EQ(5, instru.operand);
  EXPECT_EQ("a", instru.cond.reg);
  EXPECT_EQ(Gt, instru.cond.op);
  EXPECT_EQ(10, instru.cond.operand);
}

int main(int argc, char** argv)
{
  try
  {
    testing::InitGoogleTest(&argc, argv);
    execute_file("register.txt");
    return RUN_ALL_TESTS();
  }
  catch(exception const & e)
  {
    cerr << e.what() << endl;
    return 1;
  }
}
