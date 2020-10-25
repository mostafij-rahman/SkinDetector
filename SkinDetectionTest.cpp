
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

RNG rng(12345);

/**
 * @function main
 */
int main( void )
{
		DIR *dir;
		struct dirent *ent;
		char *inputFolder = "inputImages//";

		vector<double> trainPr(0);
		ifstream inputFile("trainPrSkin.txt");
		if (inputFile) {       
			double value;
			// read the elements in the file into a vector  
			while ( inputFile >> value ) {
				trainPr.push_back(value);
			}
		}
		inputFile.close();

		if ((dir = opendir (inputFolder)) != NULL) {
			while ((ent = readdir (dir)) != NULL) {
				std::ostringstream oss;
				oss << inputFolder << ent->d_name;
				Mat img = imread(oss.str());
				vector<Mat> rgb;
				if( img.data ){
					int v=0;

					imshow("Input",img);
					waitKey(10);
					split(img,rgb);
					cvtColor(img, img, COLOR_RGB2GRAY);
					int r,g,b;
					for(int i = 0; i<img.rows; i++){
						for(int j = 0; j< img.cols; j++){
							r = rgb[0].at<uchar>(i,j);
							g = rgb[1].at<uchar>(i,j);
							b = rgb[2].at<uchar>(i,j);
							int index = (r*256*256)+(g*256)+b;
				
				//index = index;
							int pixel = img.at<uchar>(i,j);
							double ratio = trainPr.at(index);
							if(ratio > 0.5){
								img.at<uchar>(i,j)=pixel;
							//rgb[0].at<uchar>(i,j) = 255;
							}
							else{
								img.at<uchar>(i,j)=0;
							//rgb[0].at<uchar>(i,j) = 0;
							}   
						}
					}
				}
				imshow("Skin",img);
				waitKey(10);
			}
			closedir (dir);
			waitKey(0);
		} 	
		else {
			perror ("");
			return EXIT_FAILURE;
		}
	  return 0;
}
