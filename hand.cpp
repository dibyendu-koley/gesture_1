/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "hand.h"

Hand::Hand(){
}
Hand::Hand(Mat img){
    handImg=img;
}
Hand::Hand(Mat img, Rect pos){
    handImg=img;
    position=pos;
}
