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
    static CCPoint * pointAdd(CCPoint * A, CCPoint * B);
    static CCPoint * pointSub(CCPoint * A, CCPoint * B);
    static float pointDist(CCPoint * A, CCPoint * B);
    static float dotProduct(CCPoint * A, CCPoint * B);
    static float crossProductMagnitude(CCPoint * A, CCPoint * B);
    static float angleFromAtoB(CCPoint * A, CCPoint * B);
};

#endif /* defined(__BookBattleAlpha__mathExt__) */
