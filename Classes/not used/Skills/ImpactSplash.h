#ifndef IMPACT_SPLASH_H
#define IMPACT_SPLASH_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   ImpactSplash.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   Splash impacting strategy for owner skill.
//
//-----------------------------------------------------------------------------

#include "ImpactStrategy.h"

class ImpactSplash : public ImpactStrategy
{
public:

	ImpactSplash::ImpactSplash(
		AbstSkill* const owner_skill,
		float splash_radius);

	virtual ~ImpactSplash()
	{}

	virtual void impact() override;

public:

	//the radius of damage, once the rocket has impacted
	float	_splash_radius;

	//this strategy impacts only once
	bool	_impacted;
};


#endif