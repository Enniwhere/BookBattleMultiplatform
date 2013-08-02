//
//  LosingScene.cpp
//  BookBattleAlpha
//
//  Created by Simon Enni on 7/6/13.
//
//

#include "LosingScene.h"

#include "cocos2d.h"
#include "Player.h"

using namespace cocos2d;

CCScene* LosingScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    LosingScene *layer = LosingScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool LosingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCLabelTTF* pLabel = CCLabelTTF::create("A FAILURE IS YOU", "Thonburi", 100);
    //CCLabelTTF* pLabel2 = CCLabelTTF::create("How are you feeling?", "Thonburi", 24);
    
    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    // position the label on the center of the screen
    pLabel->setPosition( Point(size.width / 2, size.height / 2) );
    //pLabel2->setPosition( Point(size.width / 2, size.height - 60) );
    
    // add the label as a child to the scene
    this->addChild(pLabel, 2);
    this->setTouchEnabled(true);
    Player::getPlayer()->healDamage(9999);
    return true;
}

void LosingScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    
    // run
    pDirector->popToRootScene();
}