#include <iostream>
#include <string>
#include <fstream>

#include <opencv/cv.h>
#include <opencv/cxmisc.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>

#include "CommandlineUtils.h"
#include "stereovision.h"

void GetUndistortedImageName(const std::string& OriginalImName, 
			     std::string& UndistortedImName,
			     const std::string& OutputPath);

void InitConfig(std::ofstream& ofm, 
		std::string ConfigName,
		int ImSetSize);

void FillConfig(std::ofstream& ofm, 
		std::string UndistortedImNameL, 
		std::string UndistortedImNameR, 
		CvMat* Intrinsic_L,
		CvMat* Intrinsic_R,
		CvMat* rotation,
		CvMat* translation);

void CloseConfig(std::ofstream& ofm);

int main(int argc, char** argv)
{
  enum LEFT_RIGHT { LEFT=0, RIGHT=1 };
  
  CommandlineUtils ArgList(argc, argv);

  // Get left image set
  int ImSetSizeL = 0;
  char** ImNameL = ArgList.GetArgsByOption("-l", ImSetSizeL);
  
  // Get right image set
  int ImSetSizeR = 0;
  char** ImNameR = ArgList.GetArgsByOption("-r", ImSetSizeR);

  // Get output path
  int PathSize = 0;
  char** pOutputPath = ArgList.GetArgsByOption("-o", PathSize);
  
  std::string OutputPath = pOutputPath[0];
  
  if(OutputPath.at(OutputPath.length()-1) != '/')
    OutputPath += '/';

  // Get calibration file name
  int CalibSize = 0;
  char** pCalibFile = ArgList.GetArgsByOption("-c", CalibSize);

  std::string CalibFile = pCalibFile[0];


  // Get output config file name
  int ConfigSize = 0;
  char** pConfigName = ArgList.GetArgsByOption("-g", ConfigSize);

  std::string ConfigName = pConfigName[0];


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


  StereoVision sv;

  std::cout << "Calibration file ------> " << CalibFile  << std::endl;

  sv.calibrationLoad(CalibFile.c_str());

  std::ofstream ofm;

  InitConfig(ofm, ConfigName, ImSetSizeL);

  // --------------------------------------------
  CvMat* Intrinsic_L = cvCreateMat(3,3,CV_32FC1);
  CvMat* Intrinsic_R = cvCreateMat(3,3,CV_32FC1);
  CvMat* rotation    = cvCreateMat(3,3,CV_32FC1);
  CvMat* translation = cvCreateMat(3,1,CV_32FC1);

  // Undistort and save images
  for(int i=0; i<ImSetSizeL; i++)
    {
      std::string UndistortedImNameL; 
      std::string UndistortedImNameR;

      GetUndistortedImageName(std::string(ImNameL[i]),
			      UndistortedImNameL,
			      OutputPath);

      GetUndistortedImageName(std::string(ImNameR[i]),
			      UndistortedImNameR,
			      OutputPath);

      std::cout << "Undistorting -----> " << ImNameL[i] << std::endl
		<< "------------------> " << ImNameR[i] << std::endl;

      std::cout << "Output -----------> " << UndistortedImNameL << std::endl
		<< "------------------> " << UndistortedImNameR << std::endl;

      sv.undistortImage(ImNameL[i], ImNameR[i], 
      			UndistortedImNameL.c_str(), 
      			UndistortedImNameR.c_str(),
      			Intrinsic_L, Intrinsic_R,
      			rotation, translation);

      // sv.undistortImage(ImNameL[i], ImNameR[i], 
      // 			UndistortedImNameL.c_str(), 
      // 			UndistortedImNameR.c_str(),
      // 			NULL, NULL, rotation, translation);

      FillConfig(ofm, UndistortedImNameL, UndistortedImNameR, 
      		 Intrinsic_L, Intrinsic_R, rotation, translation);
    }

  CloseConfig(ofm);


  cvReleaseMat(&Intrinsic_L);
  cvReleaseMat(&Intrinsic_R);
  cvReleaseMat(&rotation);
  cvReleaseMat(&translation);
  
  return 0;
}

void GetUndistortedImageName(const std::string& OriginalImName, 
			     std::string& UndistortedImName,
			     const std::string& OutputPath)
{
  size_t pos = OriginalImName.find_last_of('/');

  if(pos == std::string::npos)
    {
      UndistortedImName = OutputPath + "undistorted_" + OriginalImName;
    }
  else
    {
      UndistortedImName = OutputPath + "undistorted_" + OriginalImName.substr(pos+1);
    }
}


void InitConfig(std::ofstream& ofm, 
		std::string ConfigName,
		int ImSetSize)
{
  ofm.open(ConfigName.c_str(), std::ios::trunc);
  
  // Number of images
  ofm << ImSetSize  << std::endl;

  // Scale
  ofm << 1.0 << std::endl;
  
  // Use fixed precision
  ofm.setf (ios::fixed, ios::basefield); 

  // Use 5 digits
  ofm.precision(4);

}

void FillConfig(std::ofstream& ofm, 
		std::string UndistortedImNameL, 
		std::string UndistortedImNameR, 
		CvMat* Intrinsic_L,
		CvMat* Intrinsic_R,
		CvMat* rotation,
		CvMat* translation)
{
  const int K_SIZE = 9;
  const int R_SIZE = 9;
  const int T_SIZE = 3;

  size_t pos;
  
  // Left image name
  pos = UndistortedImNameL.find_last_of('/');
  if(pos == std::string::npos)
    {
      ofm << UndistortedImNameL << ' ';
    }
  else
    {
      ofm << UndistortedImNameL.substr(pos+1) << ' ';
    }

  
  // K for left image
  for(int i=0; i<K_SIZE; i++)
    {
      ofm << Intrinsic_L->data.fl[i] << ' ';
    }
  ofm << std::endl;

  // R for left image, which is identity matrix
  ofm << 1.0 << ' ' << 0.0 << ' ' << 0.0 << ' '
      << 0.0 << ' ' << 1.0 << ' ' << 0.0 << ' '
      << 0.0 << ' ' << 0.0 << ' ' << 1.0 << std::endl;

  // T for left image, which is a 0 vector
  ofm << 0.0 << ' ' << 0.0 << ' ' << 0.0 << std::endl;

  // Right image name
  pos = UndistortedImNameR.find_last_of('/');
  if(pos == std::string::npos)
    {
      ofm << UndistortedImNameR << ' ';
    }
  else
    {
      ofm << UndistortedImNameR.substr(pos+1) << ' ';
    }


  // K for right image
  for(int i=0; i<K_SIZE; i++)
    {
      ofm << Intrinsic_R->data.fl[i] << ' ';
    }
  ofm << std::endl;

  // R for right image
  for(int i=0; i<R_SIZE; i++)
    {
      ofm << rotation->data.fl[i] << ' ';
    }
  ofm << std::endl;

  // T for right image
  for(int i=0; i<T_SIZE; i++)
    {
      ofm << translation->data.fl[i] << ' ';
    }
  ofm << std::endl;
}


void CloseConfig(std::ofstream& ofm)
{
  ofm.close();
}
