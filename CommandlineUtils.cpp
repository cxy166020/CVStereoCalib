#include "CommandlineUtils.h"


CommandlineUtils::CommandlineUtils(int argc, char** argv)
{
  mArgc = argc;
  mArgv = argv;

  // Count how many options are available
  mOptionCount = 0;
  for(int i=0; i<mArgc; i++)
    {
      if(IsOption(mArgv[i]))
	mOptionCount++;
    }

  mOptions = new char*[mOptionCount];
  for(int i=0, j=0; i<mArgc; i++)
    {
      if(IsOption(mArgv[i]))
	{
	  mOptions[j++] = mArgv[i];
	}
    }
  
}

CommandlineUtils::~CommandlineUtils()
{
  if(mOptions)
    delete[] mOptions;
}

char** CommandlineUtils::GetArgsByOption(const char* opt, int& num)
{

  for(int i=0; i<mArgc; i++)
    {
      if(IsOption(mArgv[i]) && strcmp(mArgv[i], opt) == 0)
	{
	  int j = 0;

	  for( ; j<mArgc-i-1; j++)
	    {
	      if(IsOption(mArgv[i+j+1]))
		break;
	    }
	  
	  num = j;  
	  
	  if(num == 0)
	    return NULL;
	  else
	    return &(mArgv[i+1]);
	}
    }

  
  std::cerr << "Unknown option " << opt << std::endl;
  return NULL;   
}
