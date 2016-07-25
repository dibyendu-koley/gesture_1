/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */



#include "SkinDetector.h"
#include"opencv2/opencv.hpp"

SkinDetector::SkinDetector(void)
{
//YCrCb threshold
// You can change the values and see what happens
Y_MIN  = 0;
Y_MAX  = 255;
Cr_MIN = 133;
Cr_MAX = 173;
Cb_MIN = 77;
Cb_MAX = 127;
}

SkinDetector::~SkinDetector(void)
{
}
void SkinDetector::setSkinCrCb(cv::Rect rRect, cv::Mat frame)
{
    std::vector<cv::Point3f> objectPoints,objectPointsYCbCr;
    cv::Mat roi = frame(rRect);
    
    //cout<<rRect.tl()<<"::"<<rRect.br()<<"\n";
    for(int y = rRect.y; y < rRect.y + rRect.height; y++)
    for(int x = rRect.x; x < rRect.x + rRect.width; x++)
    {
        objectPoints.push_back(cv::Point3f((int)frame.at<cv::Vec3b>(x,y)[0], (int)frame.at<cv::Vec3b>(x,y)[1], (int)frame.at<cv::Vec3b>(x,y)[2]));
        //cout<<"R: "<<(int)frame.at<cv::Vec3b>(x,y)[0]<<" G: "<<(int)frame.at<cv::Vec3b>(x,y)[1]<<" B: "<<(int)frame.at<cv::Vec3b>(x,y)[2]<<"\n";
    }
    objectPointsYCbCr = RGBToYCbCr(objectPoints);
    for (int i = 0; i < objectPointsYCbCr.size(); i++){
        cout<<objectPointsYCbCr[i].x<<":"<<objectPointsYCbCr[i].y<<":"<<objectPointsYCbCr[i].z<<"\n";
    }
	//return YCbCr(Y, Cb, Cr);
}
vector<cv::Point3f> SkinDetector::RGBToYCbCr(std::vector<cv::Point3f> objectPointsRGB) {
    std::vector<cv::Point3f> objectPointsYCbCr;
    for (int i = 0; i < objectPointsRGB.size(); i++){
	float fr = (float)objectPointsRGB[i].x / 255;
	float fg = (float)objectPointsRGB[i].y / 255;
	float fb = (float)objectPointsRGB[i].z / 255;
        

	float Y = (float)(0.2989 * fr + 0.5866 * fg + 0.1145 * fb);
	float Cb = (float)(-0.1687 * fr - 0.3313 * fg + 0.5000 * fb);
	float Cr = (float)(0.5000 * fr - 0.4184 * fg - 0.0816 * fb);
        objectPointsYCbCr.push_back(cv::Point3f(Y,Cb,Cr));
    }
	return objectPointsYCbCr;
}
//this function will return a skin masked image
cv::Mat SkinDetector::getSkin(cv::Mat input)
{
cv::Mat skin;
//first convert our RGB image to YCrCb
cv::cvtColor(input,skin,cv::COLOR_BGR2YCrCb);
cv::imshow("YCrCb Color Space",skin);

//filter the image in YCrCb color space
cv::inRange(skin,cv::Scalar(Y_MIN,Cr_MIN,Cb_MIN),cv::Scalar(Y_MAX,Cr_MAX,Cb_MAX),skin);

return skin;
}
//end of SkinDetector.cpp file