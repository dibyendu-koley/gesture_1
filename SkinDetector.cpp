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
//;       //actual
//Y_MIN  = 120;
//Y_MAX  = 210;
//Cr_MIN = 110;
//Cr_MAX = 171;
//Cb_MIN = 70;
//Cb_MAX = 171;
}

SkinDetector::~SkinDetector(void)
{
}
void SkinDetector::printSkinCrCb(cv::Rect rRect, cv::Mat img)
{
    float Y_min,Y_max,Cr_min,Cr_max,Cb_min,Cb_max;
    std::vector<float> Y,Cr,Cb;
    for(int x = rRect.x; x < rRect.x + rRect.width ; x++)
    {
    for(int y = rRect.y; y < rRect.y + rRect.height ; y++){
    
        
        Y.push_back(img.at< cv::Vec3b>(x, y)[0]);
        Cr.push_back(img.at< cv::Vec3b>(x, y)[1]);
        Cb.push_back(img.at< cv::Vec3b>(x, y)[2]);
        //cout<<Y_min<<":"<<Cr_min<<":"<<Cb_min<<"-";
        cout<<x<<":"<<y<<"-";
    }
    cout<<"\n";
    }
    Y_min = *min_element(Y.begin(), Y.end());
    Y_max = *max_element(Y.begin(), Y.end());
    Cb_min = *min_element(Cb.begin(), Cb.end());
    Cb_max = *max_element(Cb.begin(), Cb.end());
    Cr_min = *min_element(Cr.begin(), Cr.end());
    Cr_max = *max_element(Cr.begin(), Cr.end());
    //cout<<Y_min<<":"<<Cr_min<<":"<<Cb_min<<"\n";
    //cout<<Y_max<<":"<<Cr_max<<":"<<Cb_max<<"\n";
    Y_MIN  = Y_min;
Y_MAX  = Y_max;
Cr_MIN = Cr_min;
Cr_MAX = Cr_max;
Cb_MIN = Cb_min;
Cb_MAX = Cb_max;

}
void SkinDetector::setSkinCrCb(cv::Rect rRect, cv::Mat frame)
{
    unsigned char b, g, r;
  
    std::vector<cv::Point3f> objectPoints,objectPointsYCbCr;
    cv::Mat roi = frame(rRect);
    
    //cout<<rRect.tl()<<"::"<<rRect.br()<<"\n";
    for(int x = rRect.x; x < rRect.x + rRect.width; x++){
    for(int y = rRect.y; y < rRect.y + rRect.height; y++){
    
        b = frame.at< cv::Vec3b>(x, y)[0];
        g = frame.at< cv::Vec3b>(x, y)[1];
        r = frame.at< cv::Vec3b>(x, y)[2];

        //cout<<"X,Y: "<<x<<","<<y<<" "<<"R: "<<(int)frame.at<cv::Vec3b>(x,y)[0]<<" G: "<<(int)frame.at<cv::Vec3b>(x,y)[1]<<" B: "<<(int)frame.at<cv::Vec3b>(x,y)[2]<<"\n";
        //objectPoints.push_back(cv::Point3f((int)frame.at<cv::Vec3b>(x,y)[2], (int)frame.at<cv::Vec3b>(x,y)[1], (int)frame.at<cv::Vec3b>(x,y)[0]));
        objectPoints.push_back(cv::Point3f(r,g,b));
    }
    }
    objectPointsYCbCr = RGBToYCbCr(objectPoints);
    for (int i = 0; i < objectPointsYCbCr.size(); i++){
        cout<<"Y:"<<objectPointsYCbCr[i].x<<" Cr"<<":"<<objectPointsYCbCr[i].y<<" Cb"<<":"<<objectPointsYCbCr[i].z<<"\n";
        cout<<"R:"<<objectPoints[i].x<<" G"<<":"<<objectPoints[i].y<<" R"<<":"<<objectPoints[i].z<<"\n";

    }
	//return YCbCr(Y, Cb, Cr);
}
vector<cv::Point3f> SkinDetector::RGBToYCbCr(std::vector<cv::Point3f> objectPointsRGB) {
    std::vector<cv::Point3f> objectPointsYCbCr;
    for (int i = 0; i < objectPointsRGB.size(); i++){
//	float fr = (float)objectPointsRGB[i].x / 255;
//	float fg = (float)objectPointsRGB[i].y / 255;
//	float fb = (float)objectPointsRGB[i].z / 255;
//        
//
//	float Y = (float)(0.2989 * fr + 0.5866 * fg + 0.1145 * fb);
//	float Cb = (float)(-0.1687 * fr - 0.3313 * fg + 0.5000 * fb);
//	float Cr = (float)(0.5000 * fr - 0.4184 * fg - 0.0816 * fb);
//        
        

//        int Y  = (float)( 0.299   * (float)objectPointsRGB[i].x + 0.587   * (float)objectPointsRGB[i].y + 0.114   * (float)objectPointsRGB[i].z)+12;
//        int Cr = (float)(( (float)objectPointsRGB[i].x -  Y) * 0.713) +128 ;
//        int Cb = (float)(((float)objectPointsRGB[i].z -  Y) * 0.564) +128;
//        
        float Y = (float)(299*objectPointsRGB[i].x + 587*objectPointsRGB[i].y + 114*objectPointsRGB[i].z)/(float)1000; //y
        float Cb = 0.5643*(float)(objectPointsRGB[i].z - (float) Y) + 128; //cb
        float Cr = 0.7132*(float)(objectPointsRGB[i].z - (float) Y) + 128; //cr
        
        //        int Cb = (int)(-0.16874 * (float)objectPointsRGB[i].x - 0.33126 * (float)objectPointsRGB[i].y + 0.50000 * (float)objectPointsRGB[i].z)+128;
//       int Cr = (int)( 0.50000 * (float)objectPointsRGB[i].x - 0.41869 * (float)objectPointsRGB[i].y - 0.08131 * (float)objectPointsRGB[i].z)+128;


        
//        float Y = 16 + float (65.481*(float)objectPointsRGB[i].x+128.553*(float)objectPointsRGB[i].y+24.966*(float)objectPointsRGB[i].z);
//        float Cb = 128 + float (-37.797*(float)objectPointsRGB[i].x-74.203*(float)objectPointsRGB[i].y+112.0*(float)objectPointsRGB[i].z);
//        float Cr = 128 + float (112.0*(float)objectPointsRGB[i].x-93.786*(float)objectPointsRGB[i].y-18.214*(float)objectPointsRGB[i].z);
        objectPointsYCbCr.push_back(cv::Point3f(Y,Cr,Cb));
    }
	return objectPointsYCbCr;
}
//this function will return a skin masked image
cv::Mat SkinDetector::getSkin(cv::Mat input)
{
cv::Mat skin;
//first convert our RGB image to YCrCb
cv::cvtColor(input,skin,cv::COLOR_BGR2YCrCb);
//cv::imshow("YCrCb Color Space",skin);
cout<<cv::Scalar(Y_MIN,Cr_MIN,Cb_MIN)<<"\n";
//filter the image in YCrCb color space
cv::inRange(skin,cv::Scalar(Y_MIN,Cr_MIN,Cb_MIN),cv::Scalar(Y_MAX,Cr_MAX,Cb_MAX),skin);

return skin;
}
//end of SkinDetector.cpp file