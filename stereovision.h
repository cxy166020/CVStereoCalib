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
    //chesboard board corners X,Y, N = X*Y ,  number of corners = (number of cells - 1)
    int cornersX,cornersY,cornersN;
    int sampleCount;
    bool calibrationStarted;
    bool calibrationDone;

    CvSize imageSize;
    int imageWidth;
    int imageHeight;

    vector<CvPoint2D32f> ponintsTemp[2];
    vector<CvPoint3D32f> objectPoints;
    vector<CvPoint2D32f> points[2];
    vector<int> npoints;

public:
    StereoVision(int imageWidth,int imageHeight);
    ~StereoVision();

    //matrices resulting from calibration (used for cvRemap to rectify images)
    CvMat *mx1,*my1,*mx2,*my2;

    CvMat* imagesRectified[2];
    CvMat  *imageDepth,*imageDepthNormalized;

    void calibrationStart(int cornersX,int cornersY);
    int calibrationAddSample(IplImage* imageLeft,IplImage* imageRight);
    int calibrationEnd();

    int calibrationSave(const char* filename);
    int calibrationLoad(const char* filename);

    int stereoProcess(CvArr* imageSrcLeft,CvArr* imageSrcRight);

    int rectifyImage(const char* lImName, const char* rImName, 
		     const char* lRectifiedName, const char* rRectifiedName);

    CvSize getImageSize(){return imageSize;}
    bool getCalibrationStarted(){return calibrationStarted;}
    bool getCalibrationDone(){return calibrationDone;}
    int getSampleCount(){return sampleCount;}

};

#endif // STEREOVISION_H
