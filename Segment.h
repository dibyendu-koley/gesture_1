/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Segment.h
 * Author: root
 *
 * Created on 18 August, 2016, 2:38 PM
 */

#ifndef SEGMENT_H
#define SEGMENT_H
#include <string.h>
#include <vector>

#include "hand.h"

#include <cv.h>
#include <highgui.h>
#include <cvaux.h>

using namespace std;
using namespace cv;
class Segment{
    public:
        Segment();
        bool saveRegions;
        bool showSteps;
        vector<Hand> run(Mat input);
        Mat remove_face(Mat original, Mat skin);
    private:
        vector<Hand> segmentHand(Mat input);
        bool verifySizes(RotatedRect mr);
        Mat histeq(Mat in);
};


#endif /* SEGMENT_H */

