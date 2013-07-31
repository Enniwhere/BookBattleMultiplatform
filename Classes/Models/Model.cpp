//
//  Model.cpp
//  BookBattle
//
//  Created by Simon Enni on 6/12/13.
//
//

#include "Model.h"

Model::Model(int x, int y)
{
    
    // Initialize the layer
    this->init();
    this->setPosition(0.0f, 0.0f);
    this->x = x;
    this->y = y;
    this->hp = this->hpStat*20;
}

Model::~Model()
{
}

void Model::takeDamage(int dmg)
{
    if (this->hp > this->hpStat*20)
    {
        this->hp = this->hpStat*20;
    }
    if (this->hp < dmg)
    {
        this->hp = 0;
    }
    else
    {
        this->hp -= dmg;
    }
    std::stringstream ss;//create a stringstream
    ss << dmg;//add number to the stream
    
    CCLabelTTF * dmgLabel = CCLabelTTF::create(ss.str().c_str(), "Thonburi", 40);
    dmgLabel->setColor(ccc3(255,0,0));
    dmgLabel->setPosition(ccp(x, y+120));
    this->addChild(dmgLabel,2);
    dmgLabel->runAction(CCMoveBy::create(1.0, ccp(0,80)));
    dmgLabel->runAction(CCFadeOut::create(1.0));
    
    updateHealthStatus();
}

void Model::healDamage(int dmg)
{
    if (dmg > this->hpStat*20-this->hp)
    {
        this->hp = this->hpStat*20;
    }
    else
    {
        this->hp += dmg;
    }
    updateHealthStatus();
}

void Model::dealDamage(int dmg)
{
    int atkDefDiff = this->atkStat - opponent->defStat;
    if (atkDefDiff > 0)
    {
        dmg = dmg*(1+atkDefDiff*0.1);
    }
    else if (atkDefDiff < 0)
    {
        dmg = dmg/(1-atkDefDiff*0.1);
    }
    
    opponent->takeDamage(dmg);
    
}

void Model::gainXp(int xp)
{
    this->xp += xp;
}

void Model::updateHealthStatus()
{
    if(hp > hpStat*20/100*90)
    {
        healthStatus->setString("Ikke skadet");
        healthStatus->setColor(ccc3(0,255,0));
    }
    else if(hp > hpStat*20/100*70)
    {
        healthStatus->setString("Lidt skadet");
        healthStatus->setColor(ccc3(120, 255, 0));
    }
    else if(hp > hpStat*20/100*50)
    {
        healthStatus->setString("Skadet");
        healthStatus->setColor(ccc3(255, 255, 0));
    }
    else if(hp > hpStat*20/100*20)
    {
        healthStatus->setString("Meget skadet");
        healthStatus->setColor(ccc3(255, 120, 0));
    }
    else if(hp > 0)
    {
        healthStatus->setString("Nær død");
        healthStatus->setColor(ccc3(255, 0, 0));
    }
    else if(hp == 0)
    {
        healthStatus->setString("Død");
        healthStatus->setColor(ccc3(255, 0, 0));
    }
}


CCPoint Model::getCenter()
{
    return modelNodes->getPosition();
}

