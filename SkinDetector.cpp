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
void SkinDetector::setSkinCrCb(cv::Rect rRect, cv::Mat img)
{
    double Y_min,Y_max,Cr_min,Cr_max,Cb_min,Cb_max;
    std::vector<float> Y,Cr,Cb;
    for (int row(0); row < img.rows; ++row)
    {
        for (int col(0); col < img.cols; ++col)
        {
            //cv::Vec3f pixel = img.at<cv::Vec3b>(row, col);
            Y.push_back(img.at<cv::Vec3b>(row, col)[0]);
            Cr.push_back(img.at<cv::Vec3b>(row, col)[1]);
            Cb.push_back(img.at<cv::Vec3b>(row, col)[2]);
            //std::cout << "row: " << row << "; col: " << col << "; Y: " << pixel[0] << "; Cr: " << pixel[1] << "; Cb: " << pixel[2] << std::endl;
        }
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
//this function will return a skin masked image
cv::Mat SkinDetector::getSkin(cv::Mat input)
{
cv::Mat skin;
//first convert our RGB image to YCrCb
cv::cvtColor(input,skin,cv::COLOR_BGR2YCrCb);
//cv::imshow("YCrCb Color Space",skin);
cout<<"------------------start-------------------\n";
cout<<cv::Scalar(Y_MIN,Cr_MIN,Cb_MIN)<<"\n";
cout<<cv::Scalar(Y_MAX,Cr_MAX,Cb_MAX)<<"\n";
cout<<"------------------end-------------------\n";
//filter the image in YCrCb color space
cv::inRange(skin,cv::Scalar(Y_MIN,Cr_MIN,Cb_MIN),cv::Scalar(Y_MAX,Cr_MAX,Cb_MAX),skin);

return skin;
}
//end of SkinDetector.cpp file