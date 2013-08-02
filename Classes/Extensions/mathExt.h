//
//  mathExt.h
//  BookBattleAlpha
//
//  Created by Simon Enni on 7/6/13.
//
//

#ifndef __BookBattleAlpha__mathExt__
#define __BookBattleAlpha__mathExt__

#include <iostream>
#include "CCGeometry.h"

using namespace cocos2d;

class mathExt
{
public:
    static cocos2d::Point * pointAdd(cocos2d::Point * A, cocos2d::Point * B);
    static cocos2d::Point * pointSub(cocos2d::Point * A, cocos2d::Point * B);
    static float pointDist(cocos2d::Point * A, cocos2d::Point * B);
    static float dotProduct(cocos2d::Point * A, cocos2d::Point * B);
    static float crossProductMagnitude(cocos2d::Point * A, cocos2d::Point * B);
    static float angleFromAtoB(cocos2d::Point * A, cocos2d::Point * B);
};

#endif /* defined(__BookBattleAlpha__mathExt__) */
