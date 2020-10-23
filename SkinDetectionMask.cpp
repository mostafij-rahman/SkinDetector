/**
 * @file objectDetection.cpp
 * @author Md Mostafijur Rahman
 */
#include "stdafx.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"   


//#include "rgb.h"

#include <iostream>
#include <stdio.h>
#include "dirent.h"
#include <sstream>
#include <iomanip>
#include <string.h>
#include <math.h>
#include <fstream> 

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );

/** Global variables */
//-- Note, either copy these two files from opencv/data/haarscascades to your current folder, or change these locations
String face_cascade_name = "lbpcascade_frontalface.xml";
String eyes_cascade_name = "haarcascade_eye.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_name = "Capture - Face detection";
RNG rng(12345);

/**
 * @function main
 */
String maskPath = "Code_image_processing\\LbpFindOpenCv\\LbpFindOpenCv\\face_detection\\Mask\\";
String trainPath = "Code_image_processing\\LbpFindOpenCv\\LbpFindOpenCv\\face_detection\\training\\";
int main( void )
{

	//double *rgbHist[256][256][256];
	 int*** nonSkin;
	 int ***skin;    // 3D array definition;

	 int nonSkinCount = 0, skinCount = 0;
	 double ***skinPr, ***nonSkinPr;
// begin memory allocation
     nonSkin = new int**[256];
	 skin = new int**[256];

	 nonSkinPr = new double**[256];
	 skinPr = new double**[256];

	 for(int x = 0; x < 256; ++x) {
		 nonSkin[x] = new int*[256];
			 for(int y = 0; y < 256; ++y) {
				nonSkin[x][y] = new int[256];
					for(int z = 0; z < 256; ++z) { // initialize the values to whatever you want the default to be
						 nonSkin[x][y][z] = 0;
					}
			}
	  }
	 for(int x = 0; x < 256; ++x) {
		 skin[x] = new int*[256];
			 for(int y = 0; y < 256; ++y) {
				skin[x][y] = new int[256];
					for(int z = 0; z < 256; ++z) { // initialize the values to whatever you want the default to be
						 skin[x][y][z] = 0;
					}
			}
	  }



	 for(int x = 0; x < 256; ++x) {
		 nonSkinPr[x] = new double*[256];
			 for(int y = 0; y < 256; ++y) {
				nonSkinPr[x][y] = new double[256];
					for(int z = 0; z < 256; ++z) { // initialize the values to whatever you want the default to be
						 nonSkinPr[x][y][z] = 0.0;
					}
			}
	  }
	 for(int x = 0; x < 256; ++x) {
		 skinPr[x] = new double*[256];
			 for(int y = 0; y < 256; ++y) {
				skinPr[x][y] = new double[256];
					for(int z = 0; z < 256; ++z) { // initialize the values to whatever you want the default to be
						 skinPr[x][y][z] = 0.0;
					}
			}
	  }

	  for(int a = 0; a < 555; a++){

				String imageName = "";
				stringstream ss;
				ss << a;
				string str = ss.str();
				if(a<10){
					
					
					imageName = "000"+str;
				}
				else if(a> 10 && a<100){
					imageName = "00"+str;
				}
				else{
					imageName = "0"+str;
				}
				Mat maskImg = imread(maskPath+imageName+".bmp");
			
				Mat trainImg = imread(trainPath+imageName+".jpg");
				vector<Mat> rgb;
				if( maskImg.data && trainImg.data )
				{
					//imshow("mask",maskImg);
				 //   waitKey(10);

				//	imshow("train",trainImg);
				 //   waitKey(10);
					//detectAndSave(img, ent->d_name);
					//detectAndDisplay(img);
					split(trainImg,rgb);

				//	imshow("rgb",rgb[0]);
				//	waitKey(10);
					int r,g,b;
					for(int i = 0; i<rgb[0].rows; i++){
						for(int j = 0; j< rgb[0].cols; j++){

							r = rgb[0].at<uchar>(i,j);
							g = rgb[1].at<uchar>(i,j);
							b = rgb[2].at<uchar>(i,j);

							if(maskImg.at<uchar>(i,j)>220){
								nonSkin[r][g][b]++;
								nonSkinCount++;
							}
							else{
								skin[r][g][b]++;
								skinCount++;
							}
							
							//rgbHist[rgb[0].at<uchar>(i,j)][rgb[1].at<uchar>(i,j)][rgb[2].at<uchar>(i,j)]++;
						}
					}
			  }
				//ofs2.close();
				printf("Processed image %d\n",a);
		} 
	   std::ofstream ofs1, ofs2;
			 ofs1.open ("trainPrSkin.txt", std::ofstream::out | std::ofstream::app);
		    // ofs2.open ("trainPrNonSkin.txt", std::ofstream::out | std::ofstream::app);

			 for(int x = 0; x < 256; ++x) {
				 for(int y = 0; y < 256; ++y) {
					for(int z = 0; z < 256; ++z) { 

						skinPr[x][y][z] =(double)skin[x][y][z]/(double)skinCount;
						nonSkinPr[x][y][z] =(double)nonSkin[x][y][z]/(double)nonSkinCount;
						
						//printf("Skinpr %lf and Non Skinpr %lf\n",skinPr[x][y][z], nonSkinPr[x][y][z]);
						//printf("Skin %d and Non Skin %d\n",skin[x][y][z], nonSkin[x][y][z]);
						if(nonSkinPr[x][y][z]==0.0){
							nonSkinPr[x][y][z]=0.0001;
						}
						double skinNonskinRatio = skinPr[x][y][z]/nonSkinPr[x][y][z]; 
						//ofs1 << skinNonskinRatio;
						//ofs1 << " ";

						//printf("Skin %lf \n",skinNonskinRatio);

					//	ofs2 << nonSkinPr[x][y][z];
					//	ofs2 << " ";
					}
				 }
			 }
			 	ofs1.close();
			//Testing
	    Mat testImg = imread("picnic.jpg");
	
		vector<Mat> rgb;
		
		if( testImg.data )
		{
				split(testImg,rgb);

				cvtColor(testImg, testImg, COLOR_RGB2GRAY);
				int r,g,b;
				for(int i = 0; i<testImg.rows; i++){
					for(int j = 0; j< testImg.cols; j++){

						r = rgb[0].at<uchar>(i,j);
						g = rgb[1].at<uchar>(i,j);
						b = rgb[2].at<uchar>(i,j);
						int pixel = testImg.at<uchar>(i,j);
					    if(skinPr[r][g][b]/nonSkinPr[r][g][b]>0.6){

							testImg.at<uchar>(i,j)=255;
							//rgb[0].at<uchar>(i,j) = 255;
						}
						else{
							testImg.at<uchar>(i,j)=0;
							//rgb[0].at<uchar>(i,j) = 0;
						}
						   
					}
				}
		}
		imshow("Detected Skin",testImg);
		waitKey(0);
		return 0;
}
