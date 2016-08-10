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
Mat roi,roi_blur,roi_YCbCr;
Mat edges;
Mat skinMat,skinMat_erd,skinMat_dlt;
string label1="put your palm inside red box and press i to insialize skin coller";
cv::Rect rRect1(Point(200,200),Point(250,250));
// Create a structuring element
int erosion_size = 1; 
int dilation_size = 2; 

//Mat element_erd = getStructuringElement(cv::MORPH_ELLIPSE,
Mat element_erd = getStructuringElement(cv::MORPH_RECT,
    cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
    cv::Point(erosion_size, erosion_size) );
Mat element_dlt = getStructuringElement( cv::MORPH_RECT,
        Size( 2*dilation_size + 1, 2*dilation_size+1 ),
        Point( dilation_size, dilation_size ) );
//cv::Rect rRect2(Point(100,120),Point(105,125));
//cv::Rect rRect3(Point(120,100),Point(125,105));
//cv::Rect rRect4(Point(120,120),Point(125,125));
//std::vector<cv::Rect> vRect;
//vRect.push_back(rRect1);vRect.push_back(rRect2);vRect.push_back(rRect3);vRect.push_back(rRect4);


//Mat imgYCC1;
  while( true )
     {
    cap >> frame;
    putText(frame, label1, Point(10, 10), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
    //cv::circle(frame,cv::Point(rRect.x,rRect.y),5,cv::Scalar(255,255,0),-1,8);
    //cv::circle(frame,cv::Point(rRect.x+rRect.width,rRect.y),5,cv::Scalar(255,255,0),-1,8);

    skinMat= mySkinDetector.getSkin(frame);
    // Apply erosion or dilation on the image
    erode(skinMat,skinMat_erd,element_erd);  // dilate(image,dst,element);
    dilate( skinMat_erd, skinMat_dlt, element_dlt );   
    rectangle( frame, rRect1.tl(), rRect1.br(), Scalar(0,0,255), 2, 8, 0 );
    
    imshow("frame",frame);
    imshow("frame2",skinMat);
    imshow("frame3",skinMat_erd);
    imshow("frame4",skinMat_dlt);
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