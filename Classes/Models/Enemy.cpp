//
//  Enemy.cpp
//  BookBattle
//
//  Created by Simon Enni on 6/18/13.
//
//

#include "Enemy.h"
#include "cocos-ext.h"
#include "VictoryScene.h"


// Constructor for the enemy. The layer is initialized in Model's constructor.
Enemy::Enemy(int x, int y) : Model(x,y)
{
    // Create a default CCNodeLoaderLibrary.
    CCNodeLoaderLibrary* nodeLoaderLibrary;
    nodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    // Create a new CCBReader with a default CCNodeLoaderLibrary
    // This can take a lot of parameters to use code connections and more
    CCBReader* ccbReader = new CCBReader(nodeLoaderLibrary);
    
    // Load the main node from the CocosBuilder file
    modelNodes = ccbReader->readNodeGraphFromFile("Orc.ccbi");
    
    // Get the animationmanager so we can animate the thing afterwards
    animationManager = ccbReader->getAnimationManager();
    modelNodes->setPosition( ccp(x, y));
    //modelNodes->setScale(0.5f);
    
    CCNode* rShoulder = modelNodes->getChildByTag(1)->getChildByTag(4)->getChildByTag(1);
    rShoulder->setZOrder(0);
    CCNode* lHip = modelNodes->getChildByTag(1)->getChildByTag(1);
    lHip->setZOrder(0);
    CCNode* rHip = modelNodes->getChildByTag(1)->getChildByTag(2);
    rHip->setZOrder(0);
    CCNode* rUpperArm = rShoulder->getChildByTag(2);
    rUpperArm->setZOrder(-1);
    CCNode* chest = modelNodes->getChildByTag(1)->getChildByTag(4)->getChildByTag(2);
    chest->setZOrder(0);
    CCNode* hips = modelNodes->getChildByTag(1)->getChildByTag(3);
    hips->setZOrder(0);
    modelNodes->getChildByTag(1)->getChildByTag(4)->setZOrder(0);
    
    CCNode* lShoulder = modelNodes->getChildByTag(1)->getChildByTag(4)->getChildByTag(4);
    lShoulder->setZOrder(3);
    CCNode* rElbow = rShoulder->getChildByTag(1);
    rElbow->setZOrder(1);
    CCNode* rLowerArm = rShoulder->getChildByTag(1)->getChildByTag(1);
    rLowerArm->setZOrder(2);
    CCNode* rHand = rShoulder->getChildByTag(1)->getChildByTag(2);
    rHand->getChildByTag(1)->setVisible(true);
    rHand->setZOrder(2);
    
    CCNode* neck = modelNodes->getChildByTag(1)->getChildByTag(4)->getChildByTag(3);
    neck->setZOrder(2);
    
    
    this->addChild(modelNodes); // Add the loaded node to the scene (this)
    
    healthStatus->setPosition(ccp(x-500, y+100));
    this->addChild(healthStatus);
    
    initAttacks();
    
    // As nobody called ccbReader->autoRelease(), returning now would cause
    // a memory leak. We can call autoRelease or delete it ourselves.
    delete ccbReader;
    
}

void Enemy::initWithPlayer(Model * player)
{
    this->opponent = player;
}

void Enemy::takeTurn()
{
    if (hp < 1)
    {
        die();
    }
    else
    {
        attack();
    }
}

void Enemy::attack()
{

    int attackNumber = rand()%2+1;
    std::map<int,Attack>::iterator it = attackMap.find(attackNumber);
    
    if(it != attackMap.end())
    {
        attackMap[attackNumber].attack(this, opponent);
    }
    
}
void Enemy::initAttacks()
{
    Enemy::enemyAttack001Init(this, 1);
    Enemy::enemyAttack002Init(this, 2);
}

void Enemy::endTurn()
{
    opponent->takeTurn();
}

void Enemy::rewardPlayer()
{
    opponent->gainXp(this->xpReward);
}

void Enemy::die()
{
    rewardPlayer();
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    
    // create a scene. it's an autorelease object
    CCScene *pScene = VictoryScene::scene();
    
    // run
    pDirector->pushScene(pScene);
    //delete player;
    //delete this;
}

void Enemy::enemyAttack001Init(Enemy * attacker,int atkNumber)
{
    attacker->attackMap[atkNumber] = Attack();
    Attack* tempAttack = &attacker->attackMap[atkNumber];
    
    // This function is used to initialize all attacks used by the player.
    // Functions used to add new attacks and so on will probably be added later.
    auto atkpart1 = [=](Model * atk,Model * def)->void
    {
        atk->animationManager->runAnimationsForSequenceNamed("Ready 12 Jump");
        CCFiniteTimeAction* jump = CCSpawn::createWithTwoActions(CCScaleTo::create(1.0f, 2),CCJumpTo::create(1.0f, ccp(def->getCenter().x+200,def->getCenter().y+70),250,1));
        atk->modelNodes->runAction(CCSequence::create(CCSequence::create(CCMoveBy::create(0.2, ccp(1,0)),jump),
                                                      CCCallFunc::create(tempAttack,callfunc_selector(Attack::runNextPart))));
    };
    auto atkpart2 = [=](Model * atk,Model * def)->void
    {
        atk->animationManager->runAnimationsForSequenceNamed("Strike 12 Jump");
        atk->modelNodes->runAction(CCSequence::create(CCMoveBy::create(0.2, ccp(-1,-2)),
                                                      CCCallFunc::create(tempAttack,callfunc_selector(Attack::runNextPart))));
    };
    auto atkpart3 = [=](Model * atk,Model * def)->void
    {
        attacker->dealDamage(attacker->getBaseDmg()+2*attacker->getDmgStat()+(rand()%attacker->getVariantDmg()+5*attacker->getDmgStat()));
        // This function slashes the enemy
        atk->modelNodes->runAction(CCSequence::create(CCMoveBy::create(1.0, ccp(-10,-20)),
                                                      CCCallFunc::create(tempAttack,callfunc_selector(Attack::runNextPart))));
        
    };
    auto atkpart4 = [=](Model * atk,Model * def)->void
    {
        // This function moves the player back again
        atk->animationManager->runAnimationsForSequenceNamed("Back 12 Jump");
        CCFiniteTimeAction* moveBack = CCMoveTo::create(1.5, ccp(attacker->getX(),attacker->getY()));
        CCFiniteTimeAction* actionMoveDone =
        CCCallFuncN::create( attacker,
                            callfuncN_selector(Model::endTurn));
        atk->modelNodes->runAction(CCScaleTo::create(1.5, 1));
        // Reset the attack
        tempAttack->resetAttack();
        // Run the sequence of animations
        atk->modelNodes->runAction(CCSequence::create(moveBack,actionMoveDone));
    };
    tempAttack->attackVector.clear();
    tempAttack->attackVector.push_back(atkpart1);
    tempAttack->attackVector.push_back(atkpart2);
    tempAttack->attackVector.push_back(atkpart3);
    tempAttack->attackVector.push_back(atkpart4);
}

void Enemy::enemyAttack002Init(Enemy * attacker, int atkNumber)
{
    attacker->attackMap[atkNumber] = Attack();
    Attack* tempAttack = &attacker->attackMap[atkNumber];
    auto atkpart1 = [=](Model * atk,Model * def)->void
    {
        atk->animationManager->runAnimationsForSequenceNamed("Ready 10");
        atk->modelNodes->runAction(CCScaleTo::create(1,2));
        atk->modelNodes->runAction(CCSequence::create(CCMoveTo::create(1.0, ccp(def->getCenter().x+120, def->getCenter().y+70) ),
                                                      CCCallFunc::create(tempAttack,callfunc_selector(Attack::runNextPart))));
    };
    auto atkpart2 = [=](Model * atk,Model * def)->void
    {
        atk->animationManager->runAnimationsForSequenceNamed("Strike 10");
        // This function slashes the enemy
        atk->modelNodes->runAction(CCSequence::create(CCMoveBy::create(0.2, ccp(1,0)),
                                                      CCCallFunc::create(tempAttack,
                                                                         callfunc_selector(Attack::runNextPart))));
        
    };
    auto atkpart3 = [=](Model * atk,Model * def)->void
    {
        attacker->dealDamage(attacker->baseDmg+3*attacker->dmgStat+rand()%(attacker->variantDmg+2*attacker->dmgStat));
        // This function slashes the enemy
        atk->modelNodes->runAction(CCSequence::create(CCMoveBy::create(1.0, ccp(10,0)),
                                                      CCCallFunc::create(tempAttack,
                                                                         callfunc_selector(Attack::runNextPart))));
        
    };
    auto atkpart4 = [=](Model * atk,Model * def)->void
    {
        // This function moves the player back again
        CCFiniteTimeAction* moveBack = CCMoveTo::create(1.5, ccp(atk->getX(),atk->getY()));
        CCFiniteTimeAction* actionMoveDone =
        CCCallFuncN::create( attacker,
                            callfuncN_selector(Model::endTurn));
        atk->animationManager->runAnimationsForSequenceNamed("Back 10");
        atk->modelNodes->runAction(CCScaleTo::create(1.5, 1));
        // Reset the attack
        tempAttack->resetAttack();
        // Run the sequence of animations
        atk->modelNodes->runAction(CCSequence::create(moveBack,actionMoveDone));
    };
    tempAttack->attackVector.clear();
    tempAttack->attackVector.push_back(atkpart1);
    tempAttack->attackVector.push_back(atkpart2);
    tempAttack->attackVector.push_back(atkpart3);
    tempAttack->attackVector.push_back(atkpart4);
    
}
