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

CCPoint * mathExt::pointAdd(CCPoint * A,CCPoint * B)
{
    return new CCPoint(A->x+B->x,A->y+B->y);
}

CCPoint * mathExt::pointSub(CCPoint * A,CCPoint * B)
{
    return new CCPoint(A->x-B->x,A->y-B->y);
}

float mathExt::pointDist(CCPoint * A, CCPoint * B)
{
    return sqrt(pow(A->x - B->x, 2) + pow(A->y - B->y, 2));
}

float mathExt::dotProduct(CCPoint * A, CCPoint * B)
{
    return (A->x * B->x + A->y * B->y);
}

float mathExt::crossProductMagnitude(CCPoint * A, CCPoint * B)
{
    return (A->x * B->y - A->y * B->x);
}

float mathExt::angleFromAtoB(CCPoint * A, CCPoint * B)
{
    return atan2f(crossProductMagnitude(A, B),dotProduct(A, B))*180/M_PI;
}