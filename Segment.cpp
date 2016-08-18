/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "Segment.h"
Segment::Segment(){
    //showSteps=true;
    showSteps=false;
    saveRegions=false;
}
bool Segment::verifySizes(RotatedRect mr){

    float error=0.4;
    //Spain car plate size: 52x11 aspect 4,7272
    float aspect=4.7272;
    //Set a min and max area. All other patchs are discarded
    int min= 15*aspect*15; // minimum area
    int max= 125*aspect*125; // maximum area
    //Get only patchs that match to a respect ratio.
    float rmin= aspect-aspect*error;
    float rmax= aspect+aspect*error;

    int area= mr.size.height * mr.size.width;
    float r= (float)mr.size.width / (float)mr.size.height;
    if(r<1)
        r= (float)mr.size.height / (float)mr.size.width;

    if(( area < min || area > max ) || ( r < rmin || r > rmax )){
        return false;
    }else{
        return true;
    }

}
vector<Hand> Segment::segmentHand(Mat skin){
    vector<Hand> output;

    //Find contours of possibles plates
    vector< vector< Point> > contours;
    findContours(skin,
            contours, // a vector of contours
            CV_RETR_EXTERNAL, // retrieve the external contours
            CV_CHAIN_APPROX_NONE); // all pixels of each contours

    //Start to iterate to each contour founded
    vector<vector<Point> >::iterator itc= contours.begin();
    vector<RotatedRect> rects;
    //remove unwanted conturs
    while (itc!=contours.end()) {
        //Create bounding rect of object
        RotatedRect mr= minAreaRect(Mat(*itc));
        if( !verifySizes(mr)){
            itc= contours.erase(itc);
        }else{
            ++itc;
            rects.push_back(mr);
        }
    }
    return output;
}
Mat Segment::remove_face(Mat original, Mat skin)
{
    Mat gray_image;
    cvtColor( original, gray_image, CV_BGR2GRAY );
    std::vector<Rect> rois;
    // Load Face cascade (.xml file)
    CascadeClassifier face_cascade;
    face_cascade.load( "haarcascade_frontalface_alt.xml" );
    // Detect faces
    std::vector<Rect> faces;
    face_cascade.detectMultiScale( gray_image, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(100, 100));
    Mat roi;
    for( int i = 0; i < faces.size(); i++ )
    {
        skin(faces[i])=cv::Scalar(0,0,0);
        //skin(faces[i])=cv::Scalar(255,0,0);
        //roi = original(faces[i]);
        //rois.push_back(faces[i]);
    }
    return skin;
}
vector<Hand> Segment::run(Mat input){
    
    //Segment image by white 
    vector<Hand> tmp=segmentHand(input);

    //return detected and posibles regions
    return tmp;
}

