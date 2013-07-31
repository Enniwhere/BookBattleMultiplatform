//
//  SwordSlashLayer.cpp
//  BookBattleAlpha
//
//  Created by Simon Enni on 7/3/13.
//
//

#include "SwordSlashLayer.h"

#include "CCBlade.h"

#define kFileStreak "WaterColorStrikeLarge.png"

USING_NS_CC;

SwordSlashLayer::SwordSlashLayer()
{
    setTouchEnabled(true);
}

SwordSlashLayer* SwordSlashLayer::create()
{
    SwordSlashLayer *pRet = new SwordSlashLayer();
    pRet->autorelease();
    return pRet;
}

void SwordSlashLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    if (currentTouch < 5) {
        
            touch = (CCTouch *)*pTouches->begin();
            CCBlade *blade = CCBlade::create(kFileStreak, 5, 50);
            _map[touch] = blade;
            
            
            blade->setColor(ccc3(250,0,50));
            blade->setOpacity(255);
            blade->setDrainInterval(1.0/25);
            
            touchPoints[currentTouch] = convertTouchToNodeSpace(touch);
        currentTouch++;
	}
}

void SwordSlashLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    if (currentTouch < 5) {

        touchPoints[currentTouch] = convertTouchToNodeSpace(touch);

        currentTouch++;
	}
}

void SwordSlashLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch * tempTouch = (CCTouch *)*pTouches->begin();
    for(int i = currentTouch; i < 5; i++)
    {
        touchPoints[currentTouch] = convertTouchToNodeSpace(tempTouch);
    }
    currentTouch = 0;
}

void SwordSlashLayer::slash()
{
    
    CCBlade *blade = _map[touch];
    addChild(blade);
    for (int i = 0; i < 5; i++) {
        blade->push(touchPoints[i]);
        
    }
    /*
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++) {
        CCTouch *touch = (CCTouch *)*it;
        if (_map.find(touch) == _map.end()) continue;
     
        CCBlade *blade = _map[touch];
        blade->autoCleanup();
        _map.erase(touch);
    }*/
}

CCPoint* SwordSlashLayer::currentSlash()
{
    return touchPoints;
}