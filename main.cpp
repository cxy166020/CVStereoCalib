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

  // Get output name
  // int NameSize;
  // char** OutputName = ArgList.GetArgsByOption("-o", NameSize);

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
  
  if(ImSetSizeL != ImSetSizeR)
    {
      std::cerr << "Unmatched number of images in left and right set" << std::endl; 
      return 0;
    }

  // int NumOfIm = ImSetSizeL;

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

  StereoVision sv;

  CvMat* intrinsic_L = cvCreateMat(3,3,CV_32FC1);
  CvMat* intrinsic_R = cvCreateMat(3,3,CV_32FC1); 

  CvMat* distortion_L = cvCreateMat(4,1,CV_32FC1);
  CvMat* distortion_R = cvCreateMat(4,1,CV_32FC1);
  
  // Initialize calibration
  sv.calibrationInit(ImWidth, ImHeight, CornersX, CornersY);
  
  // Calibrate left image set
  sv.monoCalibrate(ImSetSizeL, intrinsic_L, distortion_L, ImSetL);
  sv.monoCalibrate(ImSEtSizeR, intrinsic_R, distortion_R, ImSetR);

 
  // Release images
  ReleaseImages(ImSetL, ImSetSizeL);
  ReleaseImages(ImSetR, ImSetSizeR);

  
  cvReleaseMat(&intrinsic_L);
  cvReleaseMat(&intrinsic_R);

  cvReleaseMat(&distortion_L);
  cvReleaseMat(&distortion_R);
  
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
