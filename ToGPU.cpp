#include <iostream>
#include <string>
#include <fstream>

#include <opencv/cv.h>

#include "CommandlineUtils.h"
#include "stereovision.h"


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

  std::string pCalibFile = pCalibFile[0];


  // Get output config file name
  int ConfigSize = 0;
  char** pConfig = ArgList.GetArgsByOption("-g", ConfigSize);

  std::string config = pConfig[0];


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

  sv.calibrationLoad(CalibFile[0]);

  // Undistort and save images
  for(int i=0; i<ImSetSizeL; i++)
    {
      
    }

  
  return 0;
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
  ofm.precision(5);

}

void FillConfig(std::ofstream& ofm, 
		std::string UndistortedImNameL, 
		std::string UndistortedImNameR, 
		CvMat* Intrinsic_L,
		CvMat* Intrinsic_R,
		CvMat* rotation,
		CvMat* translation)
{
  ofm << UndistortedImNameL << '\t';
  
}


void CloseConfig(std::ofstream& ofm)
{
  ofm.close();
}
