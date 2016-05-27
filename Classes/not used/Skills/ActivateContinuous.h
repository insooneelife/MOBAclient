#ifndef ACTIVATE_CONTINUOUS_H
#define ACTIVATE_CONTINUOUS_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   ActivateContinuous.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   
//
//-----------------------------------------------------------------------------

#include "ActivateStrategy.h"
#include "cocos2d.h"

class ActivateContinuous : public ActivateStrategy
{
public:

	ActivateContinuous::ActivateContinuous(AbstSkill* const owner_skill);

	virtual ~ActivateContinuous()
	{}

	virtual bool test() override;
};

#endif