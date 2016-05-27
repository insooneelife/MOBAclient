#pragma once

#include <memory>
#include <string>
#include <queue>
#include "Character.h"

//------------------------------------------------------------------------
//
//  Name:   UserCharacter.h
//
//  Desc:   
//
//  Author: Insub Im (insooneelife@naver.com)
//
//--------------------------------------------------------------------------

class UserCharacter : public Character
{
public:

	UserCharacter(
		cocos2d::Layer* const	view_layer,
		GameWorld* const		world,
		unsigned int			id,
		const std::string&		cname,
		const cocos2d::Vec2&	position,
		const cocos2d::Vec2&	margin,
		double					bounding_radius,
		const std::string&		clent_name);

	virtual ~UserCharacter() override;

	//getters && setters

	const std::string& getClientName() const;

	void setClientName(const std::string& client_name);

	cocos2d::Label* getClientNameLabel() const;

protected:
	UserCharacter(const UserCharacter&) = delete; // no copies
	UserCharacter& operator=(const UserCharacter&) = delete; // no self-assignments
	UserCharacter() = delete;

	std::string _client_name;
	
	//for rendering
	cocos2d::Label*				_client_name_label;
};


