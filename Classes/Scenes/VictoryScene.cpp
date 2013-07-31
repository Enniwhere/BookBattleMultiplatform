//
//  VictoryScene.cpp
//  BookBattle
//
//  Created by Simon Enni on 6/20/13.
//
//

#include "cocos2d.h"
#include "VictoryScene.h"
#include "Player.h"

using namespace cocos2d;

CCScene* VictoryScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    VictoryScene *layer = VictoryScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool VictoryScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCLabelTTF* pLabel = CCLabelTTF::create("WINNING", "Thonburi", 100);
    //CCLabelTTF* pLabel2 = CCLabelTTF::create("How are you feeling?", "Thonburi", 24);
    
    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    // position the label on the center of the screen
    pLabel->setPosition( ccp(size.width / 2, size.height / 2) );
    //pLabel2->setPosition( ccp(size.width / 2, size.height - 60) );
    
    // add the label as a child to the scene
    this->addChild(pLabel, 2);
    this->setTouchEnabled(true);
    Player::getPlayer()->healDamage(100);
    return true;
}

void VictoryScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    Player::getPlayer()->checkLevelUp();
}