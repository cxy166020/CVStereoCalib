#include "stereovision.h"

StereoVision::StereoVision()  
{
  cornersX = 0;
  cornersY = 0;
  cornersN = 0;

  imageWidth = 0;
  imageHeight = 0;

  intrinsic_L = NULL;
  intrinsic_R = NULL; 
  distortion_L = NULL;
  distortion_R = NULL;

  rotation = NULL;
  translation = NULL;
}

StereoVision::~StereoVision()
{
  if(intrinsic_L)
    {
      cvReleaseMat(&intrinsic_L);
      intrinsic_L = NULL;
    }

  if(intrinsic_R)
    {
      cvReleaseMat(&intrinsic_R);
      intrinsic_R = NULL;
    }
  
  if(distortion_L)
    {
      cvReleaseMat(&distortion_L);
      distortion_L = NULL;
    }

  if(distortion_R)
    {
      cvReleaseMat(&distortion_R);
      distortion_R = NULL;
    }

  if(rotation)
    {
      cvReleaseMat(&rotation);
      rotation = NULL;
    }
  
  if(translation)
    {
      cvReleaseMat(&translation);
      translation = NULL;
    }
}


void StereoVision::calibrationInit(int imageWidth,int imageHeight, 
				   int cornersX, int cornersY)
{
  this->imageWidth  = imageWidth;
  this->imageHeight = imageHeight;
  this->imageSize = cvSize(imageWidth,imageHeight);
  
  this->cornersX = cornersX;
  this->cornersY = cornersY;
  this->cornersN = cornersX*cornersY;

  if(!intrinsic_L)
    intrinsic_L = cvCreateMat(3,3,CV_32FC1);
  
  if(!intrinsic_R)
    intrinsic_R = cvCreateMat(3,3,CV_32FC1); 
  
  if(!distortion_L)
    distortion_L = cvCreateMat(4,1,CV_32FC1);

  if(!distortion_R)
    distortion_R = cvCreateMat(4,1,CV_32FC1);

  if(!rotation)
    rotation = cvCreateMat(3,3,CV_32FC1);
  
  if(!translation)
    translation = cvCreateMat(3,1,CV_32FC1);
}

// User has to manage the memory of intrinsics and distortions 
int StereoVision::monoCalibrate(int ImSetSize, IplImage** ImSet, int lr)
{
  
  CvMat* intrinsic_matrix = (lr==0) ? intrinsic_L : intrinsic_R;
  CvMat* distortion_coeffs = (lr==0) ? distortion_L : distortion_R;

  CvSize board_sz = cvSize(cornersX, cornersY);
  
  //ALLOCATE STORAGE
  CvMat* image_points   = cvCreateMat(ImSetSize*cornersN,2,CV_32FC1);
  CvMat* object_points  = cvCreateMat(ImSetSize*cornersN,3,CV_32FC1);
  CvMat* point_counts   = cvCreateMat(ImSetSize,1,CV_32SC1);

  // CvMat* intrinsic_matrix  = cvCreateMat(3,3,CV_32FC1);
  // CVMat* distortion_coeffs = cvCreateMat(4,1,CV_32FC1);

  IplImage* image = 0;
  IplImage* gray_image = 0;
  CvPoint2D32f* corners = new CvPoint2D32f[cornersN];
  int corner_count;
  int successes = 0;
  int step;

  for( int frame=0; frame<ImSetSize; frame++ ) 
    {
      image = ImSet[frame];
      gray_image = cvCreateImage(cvGetSize(image),8,1);
      
      int found = cvFindChessboardCorners(image, board_sz, corners, &corner_count, 
					  CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

      printf("corner_count = %d \n", corner_count);
      
      //Get Subpixel accuracy on those corners
      cvCvtColor(image, gray_image, CV_BGR2GRAY);
      cvFindCornerSubPix(gray_image, corners, corner_count, cvSize(11,11),cvSize(-1,-1),
			 cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.001 ));

      // If we got a good board, add it to our data
      //
      if( corner_count == cornersN ) 
	{
	  step = successes*cornersN;
	  printf("Found = %d \n",found);
	  
	  for( int i=step, j=0; j<cornersN; ++i,++j ) 
	    {
	      CV_MAT_ELEM(*image_points,  float, i, 0) = corners[j].x;
	      CV_MAT_ELEM(*image_points,  float, i, 1) = corners[j].y;
	      CV_MAT_ELEM(*object_points, float, i, 0) = j/cornersX;
	      CV_MAT_ELEM(*object_points, float, i, 1) = j%cornersX;
	      CV_MAT_ELEM(*object_points, float, i, 2) = 0.0f; 
	    }

	  CV_MAT_ELEM(*point_counts, int, successes, 0) = cornersN;		
	  successes++;
	}

      // Release memory
      cvReleaseImage(&gray_image);
    }

  printf("successes = %d, ImSetSize=%d\n",successes,ImSetSize);
  
  CvMat* object_points2  = cvCreateMat(successes*cornersN,3,CV_32FC1);
  CvMat* image_points2   = cvCreateMat(successes*cornersN,2,CV_32FC1);
  CvMat* point_counts2   = cvCreateMat(successes,1,CV_32SC1);

  for(int i = 0; i<successes*cornersN; ++i)
    {
      CV_MAT_ELEM(*image_points2,  float, i, 0) = CV_MAT_ELEM(*image_points,  float, i, 0);
      CV_MAT_ELEM(*image_points2,  float, i, 1) = CV_MAT_ELEM(*image_points,  float, i, 1);
      CV_MAT_ELEM(*object_points2, float, i, 0) = CV_MAT_ELEM(*object_points, float, i, 0);
      CV_MAT_ELEM(*object_points2, float, i, 1) = CV_MAT_ELEM(*object_points, float, i, 1);
      CV_MAT_ELEM(*object_points2, float, i, 2) = CV_MAT_ELEM(*object_points, float, i, 2);
    } 

  for(int i=0; i<successes; ++i)
    {
      CV_MAT_ELEM(*point_counts2,int,i, 0) = CV_MAT_ELEM(*point_counts, int,i,0);
    }
  

  // Initialize the intrinsic matrix such that the two focal
  // lengths have a ratio of 1.0
  //
  CV_MAT_ELEM( *intrinsic_matrix, float, 0, 0 ) = 1.0f;
  CV_MAT_ELEM( *intrinsic_matrix, float, 1, 1 ) = 1.0f;

  printf("Calibrating...\n");

  cvCalibrateCamera2(object_points2, image_points2, point_counts2,
		     cvGetSize( image ), intrinsic_matrix,
		     distortion_coeffs, NULL, NULL, 0);

  cvReleaseMat(&object_points);
  cvReleaseMat(&image_points);
  cvReleaseMat(&point_counts);

  // cvReleaseMat(&intrinsic_matrix);
  // cvReleaseMat(&distortion_coeffs);

  cvReleaseMat(&object_points2);
  cvReleaseMat(&image_points2);
  cvReleaseMat(&point_counts2);

  delete[] corners;

  return RESULT_OK;
} 



int StereoVision::stereoCalibrate( const float squareSize, const int ImSetSize, 
				   IplImage** ImSetL, IplImage** ImSetR )
{
  printf("Stereo calibration... \n");

  const int maxScale = 1;
  
  int  nframes, n = cornersX*cornersY, N = 0;

  vector<CvPoint3D32f> objectPoints;
  vector<CvPoint2D32f> points[2];
  vector<int> npoints;
  vector<unsigned char> active[2];
  vector<CvPoint2D32f> temp(n);
  
  // ARRAY AND VECTOR STORAGE:
  // double M1[3][3], M2[3][3], D1[5], D2[5];
  // double R[3][3], T[3];
  double E[3][3], F[3][3];
  // CvMat _M1 = cvMat(3, 3, CV_64F, M1 );
  // CvMat _M2 = cvMat(3, 3, CV_64F, M2 );
  // CvMat _D1 = cvMat(1, 5, CV_64F, D1 );
  // CvMat _D2 = cvMat(1, 5, CV_64F, D2 );
  // CvMat _R = cvMat(3, 3, CV_64F, R );
  // CvMat _T = cvMat(3, 1, CV_64F, T );
  CvMat _E = cvMat(3, 3, CV_64F, E );
  CvMat _F = cvMat(3, 3, CV_64F, F );
  
  for(int i=0; i< ImSetSize*2; i++)
    {
      int count = 0, result=0;

      int lr  = i%2;
      int idx = i/2;

      vector<CvPoint2D32f>& pts = points[lr];

      IplImage* img = (lr == 0) ? ImSetL[idx] : ImSetR[idx];

      //FIND CHESSBOARDS AND CORNERS THEREIN:
      for(int s=1; s<=maxScale; s++)
        {
	  IplImage* timg = img;

	  if( s>1 )
            {
	      timg = cvCreateImage( cvSize(img->width*s, img->height*s),
				    img->depth, img->nChannels );
	      
	      cvResize( img, timg, CV_INTER_CUBIC );
            }
	  
	  result = cvFindChessboardCorners( timg, cvSize(cornersX, cornersY),
					    &temp[0], &count,
					    CV_CALIB_CB_ADAPTIVE_THRESH |
					    CV_CALIB_CB_NORMALIZE_IMAGE);

	  printf("corners_count: %d\n", count);
	  
	  if( timg != img )
	    {
	      cvReleaseImage( &timg );
	    }
	  
	  if( result || s == maxScale )
	    {	  
	      for(int j=0; j<count; j++ )
		{
		  temp[j].x /= s;
		  temp[j].y /= s;
		}
	    }

	  if( result )
	    break;
        }

      N = pts.size();
      pts.resize(N + n, cvPoint2D32f(0,0));
      active[lr].push_back((unsigned char)result);

      if( result )
        {
	  IplImage* gray_image = cvCreateImage(cvGetSize(img),8,1);
	  cvCvtColor(img, gray_image, CV_BGR2GRAY);

	  // Calibration will suffer without subpixel interpolation
	  cvFindCornerSubPix(gray_image, &temp[0], count, cvSize(11, 11), cvSize(-1,-1),
			     cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 30, 0.001));

	  copy(temp.begin(), temp.end(), pts.begin() + N);

	  cvReleaseImage(&gray_image);
        }
    }
  
  // HARVEST CHESSBOARD 3D OBJECT POINT LIST:
  nframes = active[0].size();//Number of good chessboads found
  objectPoints.resize(nframes*n);

  for(int i = 0; i < cornersY; i++ )
    {
      for(int j = 0; j < cornersX; j++ )
	{
	  objectPoints[i*cornersX + j] =
	    cvPoint3D32f(i*squareSize, j*squareSize, 0);
	}
    }
  
  for(int i=1; i<nframes; i++ )
    {
      copy( objectPoints.begin(), objectPoints.begin() + n,
	    objectPoints.begin() + i*n );
    }
  
  npoints.resize(nframes,n);
  N = nframes*n;

  CvMat _objectPoints = cvMat(1, N, CV_32FC3, &objectPoints[0] );
  CvMat _imagePoints1 = cvMat(1, N, CV_32FC2, &points[0][0] );
  CvMat _imagePoints2 = cvMat(1, N, CV_32FC2, &points[1][0] );
  CvMat _npoints = cvMat(1, npoints.size(), CV_32S, &npoints[0] );


  // CALIBRATE THE STEREO CAMERAS
  printf("Running stereo calibration ...");
 
  cvStereoCalibrate( &_objectPoints, &_imagePoints1,
		     &_imagePoints2, &_npoints,
		     intrinsic_L, distortion_L,
		     intrinsic_R, distortion_R, 
		     // &_M1, &_D1, &_M2, &_D2,
		     imageSize, rotation, translation,
		     // &_R, &_T, 
		     &_E, &_F,
		     cvTermCriteria(CV_TERMCRIT_ITER+
				    CV_TERMCRIT_EPS, 100, 1e-5),
		     // Fix fx/fy 
		     CV_CALIB_FIX_ASPECT_RATIO +
		     CV_CALIB_FIX_PRINCIPAL_POINT + 
		     // Most of the time, radial distortion dominates
		     // CV_CALIB_ZERO_TANGENT_DIST +
		     // Make sure both cameras have the same focal length
		     // CV_CALIB_SAME_FOCAL_LENGTH + 
		     // Provide an intrinsics initialization
		     CV_CALIB_USE_INTRINSIC_GUESS  );

  printf(" done\n");

  // CALIBRATION QUALITY CHECK
  // because the output fundamental matrix implicitly
  // includes all the output information,
  // we can check the quality of calibration using the
  // epipolar geometry constraint: m2^t*F*m1=0
  vector<CvPoint3D32f> lines[2];
  points[0].resize(N);
  points[1].resize(N);
  _imagePoints1 = cvMat(1, N, CV_32FC2, &points[0][0] );
  _imagePoints2 = cvMat(1, N, CV_32FC2, &points[1][0] );
  lines[0].resize(N);
  lines[1].resize(N);
  CvMat _L1 = cvMat(1, N, CV_32FC3, &lines[0][0]);
  CvMat _L2 = cvMat(1, N, CV_32FC3, &lines[1][0]);

  // Always work in undistorted space
  // During this step, intrinsics are also changed
  cvUndistortPoints( &_imagePoints1, &_imagePoints1,
		     intrinsic_L, distortion_L );
  // &_M1, &_D1, 0, &_M1 );
  cvUndistortPoints( &_imagePoints2, &_imagePoints2,
		     intrinsic_R, distortion_R );
  // &_M2, &_D2, 0, &_M2 );
  cvComputeCorrespondEpilines( &_imagePoints1, 1, &_F, &_L1 );
  cvComputeCorrespondEpilines( &_imagePoints2, 2, &_F, &_L2 );

  // Calculate average error
  double avgErr = 0;
  for(int i = 0; i < N; i++ )
    {
      double err = fabs(points[0][i].x*lines[1][i].x +
			points[0][i].y*lines[1][i].y + lines[1][i].z)
	+ fabs(points[1][i].x*lines[0][i].x +
	       points[1][i].y*lines[0][i].y + lines[0][i].z);
      avgErr += err;
    }
  printf( "avg err = %g\n", avgErr/(nframes*n) );

  return RESULT_OK;
}


int StereoVision::undistortImage(const char* lImName, const char* rImName, 
				 const char* lUndistortedName, const char* rUndistortedName,
				 CvMat* newIntrinsic_L, CvMat* newIntrinsic_R,
				 CvMat* newRotation, CvMat* newTranslation)
{
  IplImage* lIm = cvLoadImage(lImName);
  IplImage* rIm = cvLoadImage(rImName);
  
  IplImage* lUndistorted = cvCreateImage(cvSize(lIm->width, lIm->height), lIm->depth, lIm->nChannels);
  IplImage* rUndistorted = cvCreateImage(cvSize(rIm->width, rIm->height), rIm->depth, rIm->nChannels);

  cvUndistort2(lIm, lUndistorted, intrinsic_L, distortion_L);
  cvUndistort2(rIm, rUndistorted, intrinsic_R, distortion_R);

  cvSaveImage(lUndistortedName, lUndistorted);
  cvSaveImage(rUndistortedName, rUndistorted);

  if(newIntrinsic_L)
    {
      cvCopy(intrinsic_L, newIntrinsic_L);
    }

  if(newIntrinsic_R)
    {
      cvCopy(intrinsic_R, newIntrinsic_R);
    }

  if(newRotation)
    {
      cvCopy(rotation, newRotation);
    }

  if(newTranslation)
    {
      cvCopy(translation, newTranslation);
    }

  return RESULT_OK;
}

int StereoVision::calibrationSave(const char* filename)
{
  FILE* f =  fopen(filename,"wb");
  if(!f) return RESULT_FAIL;
  if(!fwrite(intrinsic_L->data.fl,sizeof(float),intrinsic_L->rows*intrinsic_L->cols,f)) return RESULT_FAIL;
  if(!fwrite(intrinsic_R->data.fl,sizeof(float),intrinsic_R->rows*intrinsic_R->cols,f)) return RESULT_FAIL;
  if(!fwrite(distortion_L->data.fl,sizeof(float),distortion_L->rows*distortion_L->cols,f)) return RESULT_FAIL;
  if(!fwrite(distortion_R->data.fl,sizeof(float),distortion_R->rows*distortion_R->cols,f)) return RESULT_FAIL;
  if(!fwrite(rotation->data.fl,sizeof(float),rotation->rows*rotation->cols,f)) return RESULT_FAIL;
  if(!fwrite(translation->data.fl,sizeof(float),translation->rows*translation->cols,f)) return RESULT_FAIL;
  fclose(f);
  return RESULT_OK;
}


int StereoVision::calibrationLoad(const char* filename)
{
  if(!intrinsic_L)
    intrinsic_L = cvCreateMat(3,3,CV_32FC1);
  
  if(!intrinsic_R)
    intrinsic_R = cvCreateMat(3,3,CV_32FC1); 
  
  if(!distortion_L)
    distortion_L = cvCreateMat(4,1,CV_32FC1);

  if(!distortion_R)
    distortion_R = cvCreateMat(4,1,CV_32FC1);

  if(!rotation)
    rotation = cvCreateMat(3,3,CV_32FC1);

  if(!translation)
    translation = cvCreateMat(3,1,CV_32FC1);


  FILE* f =  fopen(filename,"rb");
  if(!f) return RESULT_FAIL;
  if(!fread(intrinsic_L->data.fl,sizeof(float),intrinsic_L->rows*intrinsic_L->cols,f)) return RESULT_FAIL;
  if(!fread(intrinsic_R->data.fl,sizeof(float),intrinsic_R->rows*intrinsic_R->cols,f)) return RESULT_FAIL;
  if(!fread(distortion_L->data.fl,sizeof(float),distortion_L->rows*distortion_L->cols,f)) return RESULT_FAIL;
  if(!fread(distortion_R->data.fl,sizeof(float),distortion_R->rows*distortion_R->cols,f)) return RESULT_FAIL;
  if(!fread(rotation->data.fl,sizeof(float),rotation->rows*rotation->cols,f)) return RESULT_FAIL;
  if(!fread(translation->data.fl,sizeof(float),translation->rows*translation->cols,f)) return RESULT_FAIL;
  fclose(f);

  return RESULT_OK;
}
