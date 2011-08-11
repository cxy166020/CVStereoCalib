#include <iostream>

#include <opencv/cv.h>

#include "CommandlineUtils.h"
#include "stereovision.h"


int main(int argc, char** argv)
{
  
  CommandlineUtils ArgList(argc, argv);

  // Get input image names
  int InputImPairSize = 0;
  char** InputImName = ArgList.GetArgsByOption("-i", InputImPairSize);
  
  // Get output image names
  int OutputImPairSize = 0;
  char** OutputImName = ArgList.GetArgsByOption("-o", OutputImPairSize);

  // Get calibration file name
  int CalibSize = 0;
  char** CalibFile = ArgList.GetArgsByOption("-c", CalibSize);

  // Get input image size
  // int SizeDim = 0;
  // char** ImSize = ArgList.GetArgsByOption("-s", SizeDim);
  
  // int ImWidth  = atoi(ImSize[0]);
  // int ImHeight = atoi(ImSize[1]);
  
  char* lInput  = InputImName[0];
  char* rInput  = InputImName[1];
  char* lOutput = OutputImName[0];
  char* rOutput = OutputImName[1];

 
  if( lInput==NULL || rInput==NULL )
    {
      std::cerr << "Empty input images!" << std::endl;
      return 0;
    }
  
  if( lOutput==NULL || rOutput==NULL )
    {   
      std::cerr << "Empty output images!" << std::endl;
      return 0;
    }

  StereoVision sv;

  sv.calibrationLoad(CalibFile[0]);

  sv.undistortImage(lInput, rInput, lOutput, rOutput);
  
  return 0;
}



