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
 void detectAndDisplay( Mat frame );
 int findBiggestContour(vector<vector<Point> >);

 /** Global variables */
 //String face_cascade_name = "haarcascade_frontalface_alt.xml";
 String face_cascade_name = "/usr/share/openalpr/runtime_data/region/eu.xml";
 //String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
 CascadeClassifier face_cascade;
 //CascadeClassifier eyes_cascade;
 string window_name = "Capture - Face detection";
 RNG rng(12345);
/*
 * 
 */
void readme();
 
int main(int argc, char** argv) {
     if( argc != 3 )
  { readme(); return -1; }
     Mat frame;
int option = atoi(argv[1]);
if(option == 2){
CvCapture* capture;

   

   //-- 1. Load the cascades
   if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
//   if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

   //-- 2. Read the video stream
   capture = cvCaptureFromCAM( -1 );
   if( capture )
   {
     while( true )
     {
   frame = cvQueryFrame( capture );

   //-- 3. Apply the classifier to the frame
       if( !frame.empty() )
       { detectAndDisplay( frame ); }
       else
       { printf(" --(!) No captured frame -- Break!"); break; }

       int c = waitKey(10);
       if( (char)c == 'c' ) { break; }
      }
   }
}
else {
    frame = imread(argv[2]);
detectAndDisplay( frame );
waitKey();
}
    return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
  SkinDetector mySkinDetector;
  std::vector<Rect> faces;
  Mat frame_gray;
  Mat skinMat;
  Mat ROI;
  Mat ROI_gray;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
    
  skinMat= mySkinDetector.getSkin(frame);
  findContours( skinMat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  int s = findBiggestContour(contours);
  Mat drawing = Mat::zeros( frame.size(), CV_8UC1 );
  
  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );
  
  approxPolyDP( Mat(contours[s]), contours_poly[s], 3, true );
  boundRect[s] = boundingRect( Mat(contours_poly[s]) );
  ROI = frame(boundRect[s]);
  rectangle( frame, boundRect[s].tl(), boundRect[s].br(), Scalar(0,0,255), 2, 8, 0 );
  

  //drawContours( drawing, contours, s, Scalar(255), -1, 8, hierarchy, 0, Point() );
  //rectangle( frame, contours[s], Scalar(0,0,255), 2, 8, 0 );
  imshow("ROI", ROI);
    
  cvtColor( frame, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );
  cvtColor( ROI, ROI_gray, CV_BGR2GRAY );
  equalizeHist( ROI_gray, ROI_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
  //face_cascade.detectMultiScale( ROI_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  for( size_t i = 0; i < faces.size(); i++ )
  {
    rectangle(frame,faces[i],Scalar(255,0,255),2,4,0);
    
  }
  //-- Show what you got
  
  imshow("Skin Image",skinMat);
  imshow( window_name, frame );
 }
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
void readme()
  { std::cout << " Usage: ./executable_file <option> <path_of_image_file OR video> \n" << std::endl;
   std::cout << " Option: 1 = Load image from file i.e image file path \n" << std::endl;
   std::cout << " Option: 2 = web cam \n" << std::endl;
}
