//
//  mathExt.cpp
//  BookBattleAlpha
//
//  Created by Simon Enni on 7/6/13.
//
//

#include "mathExt.h"
#include <math.h>
#define _USE_MATH_DEFINES

cocos2d::Point * mathExt::pointAdd(cocos2d::Point * A,cocos2d::Point * B)
{
    return new cocos2d::Point(A->x+B->x,A->y+B->y);
}

cocos2d::Point * mathExt::pointSub(cocos2d::Point * A,cocos2d::Point * B)
{
    return new cocos2d::Point(A->x-B->x,A->y-B->y);
}

float mathExt::pointDist(cocos2d::Point * A, cocos2d::Point * B)
{
    return sqrt(pow(A->x - B->x, 2) + pow(A->y - B->y, 2));
}

float mathExt::dotProduct(cocos2d::Point * A, cocos2d::Point * B)
{
    return (A->x * B->x + A->y * B->y);
}

float mathExt::crossProductMagnitude(cocos2d::Point * A, cocos2d::Point * B)
{
    return (A->x * B->y - A->y * B->x);
}

float mathExt::angleFromAtoB(cocos2d::Point * A, cocos2d::Point * B)
{
    return atan2f(crossProductMagnitude(A, B),dotProduct(A, B))*180/M_PI;
}