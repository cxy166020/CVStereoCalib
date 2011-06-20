#include <iostream>

#include <opencv/cv.h>

#include "CommandlineUtils.h"
#include "stereovision.h"

IplImage** LoadImages(char** const ImName, int NumOfIm);
void ReleaseImages(IplImage** ImSet, int NumOfIm);

int main(int argc, char** argv)
{
  
  CommandlineUtils ArgList(argc, argv);

  // Get left image set
  int ImSetSizeL = 0;
  char** ImNameL = ArgList.GetArgsByOption("-l", ImSetSizeL);
  
  // Get right image set
  int ImSetSizeR = 0;
  char** ImNameR = ArgList.GetArgsByOption("-r", ImSetSizeR);

  if(ImNameL == NULL)
    {
      std::cerr << "Empty left image set!" << std::endl;
      return 0;
    }
  
  if(ImNameR == NULL)
    {   
      std::cerr << "Empty right image set!" << std::endl;
      return 0;
    }
  
  if(ImNameL != ImNameR)
    {
      std::cerr << "Unmatched number of images in left and right set" << std::endl; 
      return 0;
    }

  // Get board size
  int BoardDimension = 0;
  char** BoardSize = ArgList.GetArgsByOption("-s", BoardDimension);
  
  if(BoardDimension != 2)
    {
      std::cerr << "Only two dimensional checkerboard is supported!" << std::endl;
      return 0;
    }

  int CornersX = atoi(BoardSize[0]);
  int CornersY = atoi(BoardSize[1]);
  
  // Load images 
  IplImage** ImSetL = LoadImages(ImNameL, ImSetSizeL);
  IplImage** ImSetR = LoadImages(ImNameR, ImSetSizeR);
  
  int ImWidth  = ImSetL[0]->width;
  int ImHeight = ImSetL[1]->height;

  StereoVision sv(ImWidth, ImHeight);
  
  // Begin calibration
  sv.calibrationStart(CornersX, CornersY);
  
  
  // Add images to calibation data
  for(int i=0; i<ImSetSizeL; i++)
    {
      sv.calibrationAddSample(ImSetL[i], ImSetR[i]);
    }

  // Finish calibration
  sv.calibrationEnd();


  // Release images
  ReleaseImages(ImSetL, ImSetSizeL);
  ReleaseImages(ImSetR, ImSetSizeR);
  
  return 0;
}



IplImage** LoadImages(char** const ImName, int NumOfIm)
{
  IplImage** ImSet = new IplImage*[NumOfIm];

  for(int i=0; i<NumOfIm; i++)
    {
      ImSet[i] = cvLoadImage(ImName[i], CV_LOAD_IMAGE_UNCHANGED);
    }

  return ImSet;
}

void ReleaseImages(IplImage** ImSet, int NumOfIm)
{
  for(int i=0; i<NumOfIm; i++)
    {
      cvReleaseImage(&(ImSet[i]));
    }

  delete[] ImSet;
}
