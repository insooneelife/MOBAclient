#ifndef ROCKETLAUNCHER_H
#define ROCKETLAUNCHER_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   BaseCastingStrategy
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   class to implement a rocket launcher
//-----------------------------------------------------------------------------
#include "CastingStrategy.h"
#include "cocos2d.h"


class  AbstCharacter;

class BaseCastingStrategy : public CastingStrategy
{
private:

public:

	BaseCastingStrategy(AbstCharacter* owner);


	void render();

	void shootAt(cocos2d::Vec2 pos);
};



#endif