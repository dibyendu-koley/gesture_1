/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: root
 *
 * Created on 22 July, 2016, 10:15 AM
 */

#include <cstdlib>
#include "opencv2/objdetect/objdetect.hpp"
 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"
 #include"SkinDetector.h"

 #include <iostream>
 #include <stdio.h>

using namespace std;
using namespace cv;
/** Function Headers */

void condefects(vector<Vec4i> convexityDefectsSet, vector<Point> mycontour, Mat &original)
{
	for (int cDefIt = 0; cDefIt < convexityDefectsSet.size(); cDefIt++) {

		int startIdx = convexityDefectsSet[cDefIt].val[0]; Point ptStart(mycontour[startIdx]);

		int endIdx = convexityDefectsSet[cDefIt].val[1]; Point ptEnd(mycontour[endIdx]);

		int farIdx = convexityDefectsSet[cDefIt].val[2]; Point ptFar(mycontour[farIdx]);

		double depth = static_cast<double>(convexityDefectsSet[cDefIt].val[3]) / 256;
		//cout << "depth" << depth << endl;
		//display start points
		circle(original,ptStart,5,CV_RGB(255,0,0),2,8);
		//display all end points
		circle(original, ptEnd, 5, CV_RGB(255, 255, 0), 2, 8);
		//display all far points
		circle(original,ptFar,5,CV_RGB(0,0,255),2,8);
	}

}// condefects ends here

int findBiggestContour(vector<vector<Point> > contours){
    int indexOfBiggestContour = -1;
    int sizeOfBiggestContour = 0;
    for (int i = 0; i < contours.size(); i++){
        if(contours[i].size() > sizeOfBiggestContour){
            sizeOfBiggestContour = contours[i].size();
            indexOfBiggestContour = i;
        }
    }
    return indexOfBiggestContour;
}
int main()
{
int c = 0;
VideoCapture cap(0);
SkinDetector mySkinDetector;
Mat frame;
Mat edges;
Mat skinMat,skinMat_tmp;
Mat threshold_output;
cv::vector<cv::Vec4i> hierarchy;
std::vector<std::vector<cv::Point> > contours;
   Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
RNG rng(12345);
int largest_area = 0;
int largest_contour_index = 0;
cv::Rect rRect(Point(10,10),Point(20,20));

  while( true )
     {
    cap >> frame;
    //setSkin()
    cvtColor(frame, edges, CV_BGR2GRAY);
    skinMat= mySkinDetector.getSkin(frame);
    mySkinDetector.setSkinCrCb(rRect,frame);
    skinMat_tmp = mySkinDetector.getSkin(frame);
    Mat dst; // result matrix00000
    /// Create a structuring element (SE)
    int morph_size = 1;
    Mat element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    //cout<<element<<"\n";
    //Point3_<uchar>* p = frame.ptr<Point3_<uchar> >(20,200);
    
    //frame.at<cv::Vec3b>(10,10);
    for (int i=1;i<3;i++)
    { 
    morphologyEx( skinMat, dst, MORPH_CLOSE, element, Point(-1,-1), i );   
    }
    findContours( skinMat, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	/// Find the convex hull,contours and defects for each contour
	vector<vector<Point> >hull(contours.size());
	vector<vector<int> >inthull(contours.size());
	vector<vector<Vec4i> >defects(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		convexHull(Mat(contours[i]), hull[i], false);
		convexHull(Mat(contours[i]), inthull[i], false);
		if (inthull[i].size()>3)
			convexityDefects(contours[i], inthull[i], defects[i]);
	}
        /// Get the moments
  vector<Moments> mu(contours.size() );
  for( int i = 0; i < contours.size(); i++ )
     { mu[i] = moments( contours[i], false ); }
  ///  Get the mass centers:
  vector<Point2f> mc( contours.size() );
  for( int i = 0; i < contours.size(); i++ )
     { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }
        int s = findBiggestContour(contours);
        drawContours(frame, hull, s, CV_RGB(0, 0, 255), 2, 8, hierarchy);
               circle( frame, mc[s], 4, CV_RGB(250, 255, 255), -1, 8, 0 );
        condefects(defects[s], contours[s],frame);
    imshow("frame",frame);
    imshow("skin",skinMat);
    imshow("morf",dst);
    int c = waitKey(10);
       if( (char)c == 'q' ) { break; }
    //waitKey();
}
return 0;
}