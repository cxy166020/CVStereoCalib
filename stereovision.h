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

public:
    StereoVision();
    ~StereoVision();

    void calibrationInit(int imageWidth,int imageHeight, 
			 int cornersX, int cornersY);

    int monoCalibrate(int ImSetSize, CvMat* intrinsic_matrix, 
		      CvMat* distortion_coeffs, IplImage** ImSet);
};

#endif // STEREOVISION_H
