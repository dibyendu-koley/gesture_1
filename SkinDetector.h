/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SkinDetector.h
 * Author: root
 *
 * Created on 22 July, 2016, 11:10 AM
 */

#ifndef SKINDETECTOR_H
#define SKINDETECTOR_H

#pragma once
#include<opencv/cv.h>
using namespace std;
class SkinDetector
{
public:
SkinDetector(void);
~SkinDetector(void);

cv::Mat getSkin(cv::Mat input);
//cv::Mat setSkinCrCb(cv::Mat input);
void setSkinCrCb(cv::Rect rRect, cv::Mat frame);
cv::vector<cv::Point3f> RGBToYCbCr(std::vector<cv::Point3f> objectPoints);
private:
int Y_MIN;
int Y_MAX;
int Cr_MIN;
int Cr_MAX;
int Cb_MIN;
int Cb_MAX;
};


#endif /* SKINDETECTOR_H */

