/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   hand.h
 * Author: root
 *
 * Created on 18 August, 2016, 2:14 PM
 */

#ifndef HAND_H
#define HAND_H
#include <string.h>
#include <vector>

#include <cv.h>
#include <highgui.h>
#include <cvaux.h>

using namespace std;
using namespace cv;

class Hand{
    public:
        Hand();
        Hand(Mat img);
        Hand(Mat img, Rect pos);
        Rect position;
        Mat handImg;
};
#endif /* HAND_H */

