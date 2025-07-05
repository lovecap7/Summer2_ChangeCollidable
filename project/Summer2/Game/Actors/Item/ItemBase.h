#pragma once
#include "../Actor.h"

class ActorManager;
class ItemBase abstract:
    public Actor
{
public:
    ItemBase(Shape shape);
    ~ItemBase();
   
protected:
    //Å‰‚Ì”ƒtƒŒ[ƒ€‚Í“–‚½‚è”»’è‚ğ‚µ‚½‚­‚È‚¢‚Ì‚Å
    int m_noHitFrame;
};

