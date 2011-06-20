#ifndef COMMAND_LINE_UTILS
#define COMMAND_LINE_UTILS

#include <iostream>
#include <cstring>

class CommandlineUtils
{
public:
  CommandlineUtils(int argc, char** argv);
  ~CommandlineUtils();

  char** GetArgsByOption(const char* opt, int& num);

  inline bool IsOption(const char* str)
  {
    if(str[0] == '-')
      return true;
    else
      return false;
  }

  inline char** GetOptions()
  {
    return mOptions;
  }

  inline int GetOptionCount()
  {
    return mOptionCount;
  }


private:
  int mArgc;
  char** mArgv;

  int mOptionCount;
  char** mOptions;
};

#endif
