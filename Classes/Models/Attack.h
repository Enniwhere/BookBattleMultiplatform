//
//  Attack.h
//  BookBattleAlpha
//
//  Created by Simon Enni on 7/8/13.
//
//

#ifndef __BookBattleAlpha__Attack__
#define __BookBattleAlpha__Attack__

#include <iostream>
#include "Model.h"
#include <functional>


class Attack : public CCObject
{
    
    public :
    // A reference to the attacker and the defender
    Model * attacker;
    Model * defender;
    int currentPart = 0;
    std::string name = "";
    
    // The vector holding each part of the attack as a function.
    // One is supposed to use the callback in CCCallfunc to call the next part of the attack
    std::vector<std::function<void (Model *, Model*)>> attackVector;
    
    // The default constructor
    Attack()
    {
    }
    
    void setName(std::string name)
    {
        this->name = name;
    }

    // This function initializes the vector holding the functions responsible for the different parts of the attack.
    // The reason why it isn't initialized in the constructor is, that some of the parts require a reference to the
    // Attack object, so the object has to be initialized before the vector is initialized.
    void initVector(std::vector<std::function<void (Model*, Model*)>> inputVector)
    {
        attackVector = inputVector;
    }
    
    // This function is supposed to set the attacker and defender and then run the first part of the attack.
    void attack(Model * attacker, Model * defender)
    {
        this->attacker = attacker;
        this->defender = defender;
        runNextPart();
    }
    
    // This function is responsible for running each part of the animation.
    void runNextPart()
    {
        if (currentPart < attackVector.size())
        {
            currentPart++;
            attackVector.at(currentPart-1)(this->attacker,this->defender);
        }
    }
    
    // This function resets the animation to make it ready for another go.
    void resetAttack()
    {
        currentPart = 0;
    }
};



#endif /* defined(__BookBattleAlpha__Attack__) */
