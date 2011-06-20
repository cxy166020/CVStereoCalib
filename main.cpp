#include <iostream>

#include <opencv/cv.h>

#include "CommandlineUtils.h"

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
  
  // Load images 
  IplImage** ImSetL = LoadImages(ImNameL, ImSetSizeL);
  IplImage** ImSetR = LoadImages(ImNameR, ImSetSizeR);


  
  

  // Release images
  ReleaseImages(ImSetL, ImSetSizeL);
  ReleaseImages(ImSetR, ImSetSizeR);
  
  return 0;
}



IplImage** LoadImages(const char** ImName, int NumOfIm)
{
  IplImage** ImSet = new IplImage*[NumOfIm];

  for(int i=0; i<NumOfIm; i++)
    {
      ImSet[i] = cvLoadImage(ImName[i], CV_LOAD_IMAGE_UNCHANGED);
    }
}

void ReleaseImages(IplImage** ImSet, int NumOfIm)
{
  for(int i=0; i<NumOfIm; i++)
    {
      cvReleaseImage(&(ImSet[i]));
    }

  delete[] ImageSet;
}
