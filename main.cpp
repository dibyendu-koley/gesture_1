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
/*
Real time Hand and fingers tracking
Simple static gesture recognition
using webcam only, it works on kinect too but I am lazy to record another video
Summary of algorithm/method:
1)background subtraction using YCrCb color channel, but process each channel separately and combine them together to use as a mask
2)skin color extraction using YCrCb color channel
3)contour extraction, remove noises using size threshold, also uses erosion and dilation, also uses gaussian smoothing
4)use Haar-cascade to detect the face region, then remove it
5)find convex hull and convexity defects, also find the K-curvature of convexity defects points, then determine it as fingertips if fullfill certain assumption
6)find the max inscribed circle and min enclosing circle
6)from those convexity defects that fullfill fingertips assumption, find the largest one, it is the thumb, then find the vector to determine whether it is left or right hand
7)then recognize the hand gesture based on simple assumption (too much to list here)
 */

#include <cstdlib>
#include "opencv2/objdetect/objdetect.hpp"
 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"
 #include"SkinDetector.h"
#include "Segment.h"
#include "hand.h"

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
VideoCapture cap(0);
SkinDetector mySkinDetector;
Segment s;
Mat frame;
Mat roi_blur,roi_YCbCr;
Mat skinMat;
string label1="put your palm inside red box and press i to insialize skin coller";
cv::Rect rRect1(Point(200,200),Point(250,250));
// Create a structuring element
int erosion_size = 1; 
int dilation_size = 1; 

//Mat element_erd = getStructuringElement(cv::MORPH_ELLIPSE,
//Mat element_erd = getStructuringElement(cv::MORPH_RECT,
//    cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
//    cv::Point(erosion_size, erosion_size) );
//Mat element_dlt = getStructuringElement( cv::MORPH_RECT,
//        Size( 2*dilation_size + 1, 2*dilation_size+1 ),
//        Point( dilation_size, dilation_size ) );
//Mat element = getStructuringElement(MORPH_RECT, Size(17, 3) );        //good
Mat element = getStructuringElement(MORPH_RECT, Size(15, 3) );
while( true )
     {
    cap >> frame;
    putText(frame, label1, Point(10, 10), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
    skinMat= mySkinDetector.getSkin(frame);
    //Mat skinMat_floodfill = skinMat.clone();
    Mat skinMat_no_face;
    skinMat_no_face=s.remove_face(frame,skinMat);
    
// Apply erosion or dilation on the image
//    erode(skinMat,skinMat_erd,element_erd);  // dilate(image,dst,element);
//    erode(skinMat_erd,skinMat_erd,element_erd);
//
//    morphologyEx(skinMat_erd, skinMat_erd, CV_MOP_CLOSE, element);
//    
    rectangle( frame, rRect1.tl(), rRect1.br(), Scalar(0,0,255), 2, 8, 0 );
//    
    imshow("Original",frame);
    imshow("Skin",skinMat);
    imshow("Skin no face",skinMat_no_face);
    int c = waitKey(10);
       if( (char)c == 'q' ) { break; }
       if( (char)c == 'i' ) {
           GaussianBlur( frame(rRect1), roi_blur, Size( 15, 15 ), 0, 0 );
           cv::cvtColor(roi_blur,roi_YCbCr,cv::COLOR_BGR2YCrCb);
           mySkinDetector.setSkinCrCb(rRect1,roi_YCbCr);
           label1="";
       }
}
return 0;
}