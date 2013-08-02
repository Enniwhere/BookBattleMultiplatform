//
//  CombatScene.cpp
//  BookBattle
//
//  Created by Simon Enni on 6/20/13.
//
//

#include "CombatScene.h"
#include <math.h>
#include "Player.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

CCScene* CombatScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CombatScene *layer = CombatScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CombatScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    // Create and add the background at z-level 0
    CCSprite * background = CCSprite::create("battlefield2.png");
    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    // position the label on the center of the screen
    background->setPosition( Point(size.width / 2, size.height / 2) );
    background->setScale(0.4);
    this->addChild(background, 0);
    
    // Create and position the player
    player = Player::getPlayer();
    
    // Create and position the enemy
    enemy = new Enemy(size.width*0.70f,size.height*0.70f);
    player->initWithEnemy(enemy);
    enemy->initWithPlayer(player);
    this->addChild(player,4);

    
    this->addChild(enemy,2);
    
    this->setTouchEnabled(true);
    player->takeTurn();
    return true;
}

void CombatScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    
}
