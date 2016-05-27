#pragma once
#include "State.h"

class GameWorldScene;

class InformationShowState : public State<GameWorldScene>,
							 public Singleton<GameWorldScene>
{
public:
	InformationShowState()
	{}

	void enter(GameWorldScene* scene);

	void execute(GameWorldScene* scene);

	void exit(GameWorldScene* scene);

	bool onMessage(GameWorldScene* scene, const Telegram& telegram) {}

private:
	InformationShowState(const InformationShowState&) = delete; // no copies
	InformationShowState& operator=(const InformationShowState&) = delete; // no self-assignments
	InformationShowState() {}
	friend class Singleton<InformationShowState>;
};


