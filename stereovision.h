#ifndef STEREOVISION_H
#define STEREOVISION_H


#include <opencv/cv.h>
#include <opencv/cxmisc.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>

#include <vector>
#include <iostream>

using namespace std;


#define RESULT_OK   0
#define RESULT_FAIL 1

class StereoVision
{
private:
  int cornersX,cornersY,cornersN;

  CvSize imageSize;
  int imageWidth;
  int imageHeight;

  CvMat *intrinsic_L,  *intrinsic_R; 
  CvMat *distortion_L, *distortion_R;

public:
  StereoVision();
  ~StereoVision();

  void calibrationInit(int imageWidth,int imageHeight, 
		       int cornersX, int cornersY);

  int monoCalibrate(int ImSetSize, IplImage** ImSet, int lr);

  int stereoCalibrate( const float squareSize, const int ImSetSize, 
		       IplImage** ImSetL, IplImage** ImSetR );  

  int calibrationSave(const char* filename);

  int calibrationLoad(const char* filename);

  int undistortImage(const char* lImName, const char* rImName, 
		     const char* lUndistortedName, const char* rUndistortedName,
		     const CvMat* newIntrinsic_L=0, const CvMat* newIntrinsic_R=0);

};

#endif // STEREOVISION_H
