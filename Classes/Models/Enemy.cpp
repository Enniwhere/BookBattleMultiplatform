//
//  Enemy.cpp
//  BookBattle
//
//  Created by Simon Enni on 6/18/13.
//
//

#include "Enemy.h"
#include "Player.h"
#include "cocos-ext.h"
#include "VictoryScene.h"


// Constructor for the enemy. The layer is initialized in Model's constructor.
Enemy::Enemy(int x, int y) : Model(x,y)
{
    // Create a default NodeLoaderLibrary.
    NodeLoaderLibrary* nodeLoaderLibrary;
    nodeLoaderLibrary = NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
    
    // Create a new CCBReader with a default NodeLoaderLibrary
    // This can take a lot of parameters to use code connections and more
    CCBReader* ccbReader = new CCBReader(nodeLoaderLibrary);
    
    // Load the main node from the CocosBuilder file
    modelNodes = ccbReader->readNodeGraphFromFile("Orc.ccbi");
    
    // Get the animationmanager so we can animate the thing afterwards
    animationManager = ccbReader->getAnimationManager();
    modelNodes->setPosition( Point(x, y));
    //modelNodes->setScale(0.5f);
    
    Node* rShoulder = modelNodes->getChildByTag(1)->getChildByTag(4)->getChildByTag(1);
    rShoulder->setZOrder(0);
    Node* lHip = modelNodes->getChildByTag(1)->getChildByTag(1);
    lHip->setZOrder(0);
    Node* rHip = modelNodes->getChildByTag(1)->getChildByTag(2);
    rHip->setZOrder(0);
    Node* rUpperArm = rShoulder->getChildByTag(2);
    rUpperArm->setZOrder(-1);
    Node* chest = modelNodes->getChildByTag(1)->getChildByTag(4)->getChildByTag(2);
    chest->setZOrder(0);
    Node* hips = modelNodes->getChildByTag(1)->getChildByTag(3);
    hips->setZOrder(0);
    modelNodes->getChildByTag(1)->getChildByTag(4)->setZOrder(0);
    
    Node* lShoulder = modelNodes->getChildByTag(1)->getChildByTag(4)->getChildByTag(4);
    lShoulder->setZOrder(3);
    Node* rElbow = rShoulder->getChildByTag(1);
    rElbow->setZOrder(1);
    Node* rLowerArm = rShoulder->getChildByTag(1)->getChildByTag(1);
    rLowerArm->setZOrder(2);
    Node* rHand = rShoulder->getChildByTag(1)->getChildByTag(2);
    rHand->getChildByTag(1)->setVisible(true);
    rHand->setZOrder(2);
    
    Node* neck = modelNodes->getChildByTag(1)->getChildByTag(4)->getChildByTag(3);
    neck->setZOrder(2);
    
    
    this->addChild(modelNodes); // Add the loaded node to the scene (this)
    
    healthStatus->setPosition(Point(x-500, y+100));
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
    std::map<int,std::function<void(Model*,Model*)>>::iterator it = attackMap.find(attackNumber);
    
    if(it != attackMap.end())
    {
        attackMap[attackNumber](this, opponent);
    }
    
}
void Enemy::initAttacks()
{
    const std::function<int(Model*)> dmgFunc1 = [] (Model* atk)->int {return atk->getBaseDmg()+1*atk->getDmgStat()+(rand()%1*atk->getVariantDmg()+4*atk->getDmgStat());};
    attackMap[1] = Player::playerAttack001Init(dmgFunc1,2.0f);
    const std::function<int(Model*)> dmgFunc2 = [] (Model* atk)->int {return atk->getBaseDmg()+2*atk->getDmgStat()+(rand()%atk->getVariantDmg()+atk->getDmgStat());};
    attackMap[2] = Player::playerAttack002Init(dmgFunc2,2.0f);

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
    Director *pDirector = Director::getInstance();
    
    // create a scene. it's an autorelease object
    Scene *pScene = VictoryScene::scene();
    
    // run
    pDirector->pushScene(pScene);
    //delete player;
    //delete this;
}
