#include "../GameClient/GameMessage.hpp"
#include "../GameClient/GameClientManager.h"

#include "PacketClick_generated.h"

#include "PacketCreateCharacter_generated.h"
#include "PacketDeleteCharacter_generated.h"
#include "PacketRenderCharacter_generated.h"
#include "PacketRenderCharacterSet_generated.h"

#include "PacketAssignID_generated.h"

#include "PacketCreateSkill_generated.h"
#include "PacketDeleteSkill_generated.h"
#include "PacketRenderSkill_generated.h"
#include "PacketRenderSkillSet_generated.h"

#include "PacketStartTeamSelectScene_generated.h"
#include "PacketCreateUserCharacter_generated.h"
#include "PacketSelectName_generated.h"
#include "PacketSelectCharacter_generated.h"
#include "PacketSelectTeam_generated.h"
#include "PacketClientSelectedCharacter_generated.h"
#include "PacketClientSelectedTeam_generated.h"
#include "PacketClickedReady_generated.h"
#include "PacketClientClickedReady_generated.h"
#include "PacketCreateAnimationOnPos_generated.h"

#include "PacketChatMessageToServer_generated.h"
#include "PacketChatMessageToClient_generated.h"

#include "PacketWorldData_generated.h"
#include "PacketGameOver_generated.h"

#include <queue>
#include <tuple>

#include "PacketFactory.h"
#include "../../GameWorld.h"
#include "../../TeamSelectScene.h"
#include "../../ItemStoreLayer.h"
#include "../../Entity/Character.h"
#include "../../Entity/Skill.h"
#include "../../Motion.hpp"

#include "cocos2d.h"
#include "../../GameWorldScene.h"
#include "../../EndingScene.h"

#include "SimpleAudioEngine.h"

PacketFactory::PacketFactory() : 
	_game_world(nullptr),
	_team_select_scene(nullptr),
	_ending_scene(nullptr)
{}

PacketFactory& PacketFactory::getInstance()
{
	static PacketFactory instance;
	return instance;
}

//------------------------------ client -> server -----------------------------------//

GameMessage PacketFactory::createPacketClick(
	unsigned int id,
	float x,
	float y,
	packets::MouseButton mouse_button,
	int command,
	unsigned int clicked_id)
{
	flatbuffers::FlatBufferBuilder builder;

	auto click_data =
		packets::CreatePacketClick(builder, id, x, y, mouse_button, command, clicked_id);

	builder.Finish(click_data);

	return GameMessage(builder, PacketClick);
}

GameMessage PacketFactory::createPacketClientIsReady()
{
	return GameMessage(PacketClientIsReady);
}

GameMessage PacketFactory::createPacketSelectName(
	unsigned int id,
	const std::string& client_name)
{
	flatbuffers::FlatBufferBuilder builder;

	auto encode_name = builder.CreateString(client_name);

	auto selection_data = packets::CreatePacketSelectName(builder, id, encode_name);

	builder.Finish(selection_data);

	return GameMessage(builder, PacketSelectName);
}

GameMessage PacketFactory::createPacketSelectCharacter(
	unsigned int id,
	const std::string& cname)
{
	flatbuffers::FlatBufferBuilder builder;

	auto encode_cname = builder.CreateString(cname);

	auto selection_data = packets::CreatePacketSelectCharacter(builder, id, encode_cname);

	builder.Finish(selection_data);

	return GameMessage(builder, PacketSelectCharacter);
}

GameMessage PacketFactory::createPacketSelectTeam(
	unsigned int id,
	int team)
{
	flatbuffers::FlatBufferBuilder builder;

	auto selection_data = packets::CreatePacketSelectTeam(builder, id, team);

	builder.Finish(selection_data);

	return GameMessage(builder, PacketSelectTeam);
}

GameMessage PacketFactory::createPacketClickedReady(
	unsigned int id,
	int ready_flag)
{
	flatbuffers::FlatBufferBuilder builder;

	auto ready_data = packets::CreatePacketClickedReady(builder, id, ready_flag);

	builder.Finish(ready_data);

	return GameMessage(builder, PacketClickedReady);
}

GameMessage PacketFactory::createPacketGameChatMessageToServer(
	unsigned int id,
	const std::string& chat_message)
{
	flatbuffers::FlatBufferBuilder builder;

	auto encode_message = builder.CreateString(chat_message);

	auto chat_data = packets::CreatePacketChatMessageToServer(builder, id, encode_message);

	builder.Finish(chat_data);

	return GameMessage(builder, PacketGameChatMessageToServer);
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
GameMessage PacketFactory::createPacketRoomChatMessageToServer(
	unsigned int id,
	const std::string& chat_message)
{
	flatbuffers::FlatBufferBuilder builder;

	auto encode_message = builder.CreateString(chat_message);

	auto chat_data = packets::CreatePacketChatMessageToServer(builder, id, encode_message);

	builder.Finish(chat_data);

	return GameMessage(builder, PacketRoomChatMessageToServer);
}

//------------------------------ server -> client -----------------------------------//

void PacketFactory::decodeByType(GameMessage& msg)
{
	if (msg.getType() == PacketFactory::PacketRenderCharacterSet)
	{
		assert(packets::VerifyPacketRenderCharacterSetBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketRenderCharacterSet");

		PacketFactory::decodePacketRenderCharacterSet(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketCreateCharacter)
	{
		assert(packets::VerifyPacketCreateCharacterBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) && 
			"false in verifying PacketCreateCharacter");

		PacketFactory::decodePacketCreateCharacter(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketAssignID)
	{
		assert(packets::VerifyPacketAssignIDBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketAssignID");

		PacketFactory::decodePacketAssignID(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketCreateUserCharacter)
	{
		assert(packets::VerifyPacketCreateUserCharacterBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketCreateUserCharacter");

		PacketFactory::decodePacketCreateUserCharacter(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketStartGameScene)
	{
		PacketFactory::decodePacketStartGameScene();
	}
	else if (msg.getType() == PacketFactory::PacketCreateSkill)
	{
		assert(packets::VerifyPacketCreateSkillBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketCreateSkill");

		PacketFactory::decodePacketCreateSkill(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketRenderSkillSet)
	{
		assert(packets::VerifyPacketRenderSkillSetBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketRenderSkillSet");

		PacketFactory::decodePacketRenderSkillSet(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketDeleteCharacter)
	{
		assert(packets::VerifyPacketDeleteCharacterBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketDeleteCharacter");

		PacketFactory::decodePacketDeleteCharacter(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketDeleteSkill)
	{
		assert(packets::VerifyPacketDeleteSkillBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketDeleteSkill");

		PacketFactory::decodePacketDeleteSkill(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketStartTeamSelectScene)
	{
		assert(packets::VerifyPacketStartTeamSelectSceneBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketStartTeamSelectScene");

		PacketFactory::decodePacketStartTeamSelectScene(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketClientJoinTeamSelectRoom)
	{
		assert(packets::VerifyPacketStartTeamSelectSceneBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketClientJoinTeamSelectRoom == PacketStartTeamSelectScene");

		PacketFactory::decodePacketClientJoinTeamSelectRoom(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketClientLeaveTeamSelectRoom)
	{
		assert(packets::VerifyPacketStartTeamSelectSceneBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketClientLeaveTeamSelectRoom == PacketStartTeamSelectScene");

		PacketFactory::decodePacketClientLeaveTeamSelectRoom(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketClientSelectedCharacter)
	{
		assert(packets::VerifyPacketClientSelectedCharacterBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketClientSelectedCharacter");

		PacketFactory::decodePacketClientSelectedCharacter(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketClientSelectedTeam)
	{
		assert(packets::VerifyPacketClientSelectedTeamBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketClientSelectedTeam");

		PacketFactory::decodePacketClientSelectedTeam(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketClientClickedReady)
	{
		assert(packets::VerifyPacketClientClickedReadyBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketClientClickedReady");

		PacketFactory::decodePacketClientClickedReady(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketGameChatMessageToClient)
	{
		assert(packets::VerifyPacketChatMessageToClientBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketGameChatMessageToClient == PacketChatMessageToClient");

		PacketFactory::decodePacketGameChatMessageToClient(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketRoomChatMessageToClient)
	{
		assert(packets::VerifyPacketChatMessageToClientBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketRoomChatMessageToClient == PacketChatMessageToClient");

		PacketFactory::decodePacketRoomChatMessageToClient(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketCreateAnimationOnPos)
	{
		assert(packets::VerifyPacketCreateAnimationOnPosBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketCreateAnimationOnPos");

		PacketFactory::decodePacketCreateAnimationOnPos(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketWorldData)
	{
		assert(packets::VerifyPacketWorldDataBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketWorldData");

		PacketFactory::decodePacketWorldData(msg.getBody());
	}
	else if (msg.getType() == PacketFactory::PacketGameOver)
	{
		assert(packets::VerifyPacketGameOverBuffer(
			flatbuffers::Verifier(msg.getBody(), msg.getBodyLength())) &&
			"false in verifying PacketGameOver");

		PacketFactory::decodePacketGameOver(msg.getBody());
	}
	else
	{
		assert(false && "no packet type");
	}
}

void PacketFactory::decodePacketRenderCharacterSet(uint8_t* body)
{
	auto decoded = packets::GetPacketRenderCharacterSet(body);

	for (auto iter = decoded->characters()->begin();
		iter != decoded->characters()->end(); ++iter)
	{
		auto found = _game_world->getCharacters().find(iter->id());
		if (found != std::end(_game_world->getCharacters()))
		{
			found->second->update(
				cocos2d::Vec2(iter->x(), iter->y()),
				iter->frame(),
				std::string(iter->motion()->c_str()),
				iter->hp(),
				iter->mp(),
				iter->damage(),
				iter->mdamage(),
				iter->attack_speed(),
				iter->move_speed());
		}
	}
}


void PacketFactory::decodePacketCreateCharacter(uint8_t* body)
{
	auto decoded = packets::GetPacketCreateCharacter(body);

	CCLOG("decodePacketCreateCharacter");

	assert(_game_world && "world is not set");

	_game_world->addCharacter(
		decoded->id(), decoded->name()->c_str(),
		decoded->x(), decoded->y(), decoded->mx(), decoded->my(), decoded->bradius());
}


void PacketFactory::decodePacketAssignID(uint8_t* body)
{
	auto decoded = packets::GetPacketAssignID(body);

	CCLOG("decodePacketAssignID");

	GameClientManager::getInstance().getGameClient()->setId(decoded->id());

	// after id is assigned, give server character name
	GameClientManager::getInstance().send(
		PacketFactory::getInstance().createPacketSelectName(
			GameClientManager::getInstance().getGameClient()->getId(),
			GameClientManager::getInstance().getGameClient()->getClientName()));
}

void PacketFactory::decodePacketCreateUserCharacter(uint8_t* body)
{
	auto decoded = packets::GetPacketCreateUserCharacter(body);

	CCLOG("decodePacketCreateUserCharacter");

	assert(_game_world && "world is not set");

	_game_world->addUserCharacter(
		decoded->id(), decoded->character_name()->c_str(),
		decoded->x(), decoded->y(), decoded->mx(), decoded->my(),
		decoded->bradius(), decoded->client_name()->c_str());
}

void PacketFactory::decodePacketStartGameScene()
{
	auto background_music = CocosDenshion::SimpleAudioEngine::getInstance();
	background_music->playBackgroundMusic("gameSceneSound.mp3", true);

	auto director = Director::getInstance();
	director->popScene();

	assert(_game_world && "world is not set");

	GameClientManager::getInstance().send(
		PacketFactory::getInstance().createPacketClientIsReady());


	if (GameClientManager::getInstance().getGameClient()->getTeam() == TEAM_BLUE)
		_game_world_scene->setLookAt(cocos2d::Vec2(Prm.BlueStartX, Prm.BlueStartY));
	else if(GameClientManager::getInstance().getGameClient()->getTeam() == TEAM_RED)
		_game_world_scene->setLookAt(cocos2d::Vec2(Prm.RedStartX, Prm.RedStartY));

	_game_world_scene->updateUserSkillImage(
		GameClientManager::getInstance().getGameClient()->getCharacterName().c_str());
	_game_world_scene->updateUserCharacterImage(
		GameClientManager::getInstance().getGameClient()->getCharacterName().c_str());
	
	_game_world_scene->updateItemStoreLayer();

}

void PacketFactory::decodePacketCreateSkill(uint8_t* body)
{
	auto decoded = packets::GetPacketCreateSkill(body);

	CCLOG("decodePacketCreateSkill");

	assert(_game_world && "world is not set");

	_game_world->addSkill(
		decoded->id(), decoded->name()->c_str(),
		decoded->x(), decoded->y(), decoded->mx(), decoded->my(), decoded->bradius());
}

void PacketFactory::decodePacketRenderSkillSet(uint8_t* body)
{
	auto decoded = packets::GetPacketRenderSkillSet(body);

	for (auto iter = decoded->skills()->begin(); iter != decoded->skills()->end(); ++iter)
	{
		auto found = _game_world->getSkills().find(iter->id());
		if (found != std::end(_game_world->getSkills()))
		{
			found->second->update(cocos2d::Vec2(iter->x(), iter->y()), iter->activate());
		}
	}
}

void PacketFactory::decodePacketDeleteCharacter(uint8_t* body)
{
	auto decoded = packets::GetPacketDeleteCharacter(body);

	assert(_game_world && "world is not set");

	_game_world->deleteCharacter(decoded->id());
}

void PacketFactory::decodePacketDeleteSkill(uint8_t* body)
{
	auto decoded = packets::GetPacketCreateSkill(body);

	assert(_game_world && "world is not set");

	_game_world->deleteSkill(decoded->id());
}

void PacketFactory::decodePacketStartTeamSelectScene(uint8_t* body)
{
	CCLOG("decodePacketStartTeamSelectScene");
	auto decoded = packets::GetPacketStartTeamSelectScene(body);

	int slot_number = decoded->slot_number();
	std::string client_name = decoded->client_name()->c_str();

	GameClientManager::getInstance().getGameClient()->setClientName(decoded->client_name()->c_str());

	assert(_team_select_scene && "team_select_scene is not set!");
	assert((0 <= slot_number && slot_number < TeamSelectScene::SLOT_SIZE) && 
		"slot_number is not valid!");

	_team_select_scene->setSlotNumber(slot_number);
	_team_select_scene->setUserEventAlive(slot_number);
	_team_select_scene->getUserIdText(slot_number)->setString(client_name);
}

void PacketFactory::decodePacketClientJoinTeamSelectRoom(uint8_t* body)
{
	CCLOG("decodePacketClientJoinTeamSelectRoom");
	if (_team_select_scene == nullptr)
	{
		CCLOG("team_select_scene is not set");
		return;
	}

	// scenario is different but the packet type is same with PacketStartTeamSelectScene.
	auto decoded = packets::GetPacketStartTeamSelectScene(body);

	assert(_team_select_scene && "team_select_scene is not set");

	_team_select_scene->getUserIdText(decoded->slot_number())
		->setString(decoded->client_name()->c_str());
}

void PacketFactory::decodePacketClientLeaveTeamSelectRoom(uint8_t* body)
{
	CCLOG("decodePacketClientLeaveTeamSelectRoom");
	if (_team_select_scene == nullptr)
	{
		CCLOG("team_select_scene is not set");
		return;
	}

	// scenario is different but the packet type is same with PacketStartTeamSelectScene.
	auto decoded = packets::GetPacketStartTeamSelectScene(body);

	_team_select_scene->getUserIdText(decoded->slot_number())->setString("");
}

void PacketFactory::decodePacketClientSelectedCharacter(uint8_t* body)
{
	CCLOG("decodePacketClientSelectedCharacter");
	if (_team_select_scene == nullptr)
	{
		CCLOG("team_select_scene is not set");
		return;
	}

	auto decoded = packets::GetPacketClientSelectedCharacter(body);
	int slot = decoded->slot();
	std::string cname = decoded->cname()->c_str();
	_team_select_scene->getUserChampImage(slot)
		->loadTexture("characters/images/" + cname + ".png");
}

void PacketFactory::decodePacketClientSelectedTeam(uint8_t* body)
{
	CCLOG("decodePacketClientSelectedTeam");
	if (_team_select_scene == nullptr)
	{
		CCLOG("team_select_scene is not set");
		return;
	}

	auto decoded = packets::GetPacketClientSelectedTeam(body);

	int slot = decoded->slot();
	int team = decoded->team();
	
	if (team == TEAM_RED) 
	{
		_team_select_scene->getUserCheckRed(slot)->setVisible(true);
		_team_select_scene->getUserCheckBlue(slot)->setVisible(false);
	}
	else {
		_team_select_scene->getUserCheckRed(slot)->setVisible(false);
		_team_select_scene->getUserCheckBlue(slot)->setVisible(true);
	}
}

void PacketFactory::decodePacketClientClickedReady(uint8_t* body)
{
	CCLOG("decodePacketClientClickedReady");
	if (_team_select_scene == nullptr)
	{
		CCLOG("team_select_scene is not set");
		return;
	}

	auto decoded = packets::GetPacketClientClickedReady(body);

	int slot = decoded->slot();
	int ready_flag = decoded->ready_flag();

	if (ready_flag == CLICKED_READY)
		_team_select_scene->getSelectUserCheckChamp(slot)->setVisible(true);
	else
		_team_select_scene->getSelectUserCheckChamp(slot)->setVisible(false);
}

void PacketFactory::decodePacketGameChatMessageToClient(uint8_t* body)
{
	CCLOG("decodePacketGameChatMessageToClient");
	auto decoded = packets::GetPacketChatMessageToClient(body);

	std::string name = decoded->name()->c_str();
	std::string chat_message = decoded->chat_message()->c_str();

	std::cout << chat_message << std::endl;

	assert(_game_world_scene && "_game_world_scene is not set");

	_game_world_scene->setGameChatMessage(name, chat_message);
}

void PacketFactory::decodePacketRoomChatMessageToClient(uint8_t* body)
{
	CCLOG("decodePacketRoomChatMessageToClient");
	if (_team_select_scene == nullptr)
	{
		CCLOG("team_select_scene is not set");
		return;
	}

	auto decoded = packets::GetPacketChatMessageToClient(body);

	std::string name = decoded->name()->c_str();
	std::string chat_message = decoded->chat_message()->c_str();

	_team_select_scene->setRoomChatMessage(name, chat_message);
}

void PacketFactory::decodePacketCreateAnimationOnPos(uint8_t* body)
{
	CCLOG("decodePacketCreateAnimationOnPos");
	auto decoded = packets::GetPacketCreateAnimationOnPos(body);

	std::string aname = decoded->name()->c_str();
	float x = decoded->x();
	float y = decoded->y();
	
	assert(_game_world && "_game_world is not set");

	_game_world->animationOnWorld(aname, cocos2d::Vec2(x, y));
}

void PacketFactory::decodePacketWorldData(uint8_t* body)
{
	CCLOG("decodePacketWorldData");

	auto decoded = packets::GetPacketWorldData(body);

	for (auto iter = decoded->characters()->begin();
	iter != decoded->characters()->end(); ++iter)
	{
		_game_world->addCharacter(
			iter->id(), iter->name()->c_str(),
			iter->x(), iter->y(), iter->mx(), iter->my(), iter->bradius());
	}

	for (auto iter = decoded->user_characters()->begin();
	iter != decoded->user_characters()->end(); ++iter)
	{
		_game_world->addUserCharacter(
			iter->id(), iter->character_name()->c_str(),
			iter->x(), iter->y(), iter->mx(), iter->my(),
			iter->bradius(), iter->client_name()->c_str());
	}

	GameClientManager::getInstance().send(createPacketClientIsReady());
}

void PacketFactory::decodePacketGameOver(uint8_t* body)
{
	auto decoded = packets::GetPacketGameOver(body);

	CCLOG("@@@@@@@@@@@@@@@@@@@@@@@ name : %s   team %d", decoded->name()->c_str(), decoded->team());

	//cocos2d::Director::getInstance()->popScene();
	
	cocos2d::Director::getInstance()->replaceScene(EndingScene::createScene());
	_ending_scene->win(decoded->name()->c_str(), decoded->team());
}

void PacketFactory::setGameWorld(GameWorld* world)
{
	_game_world = world;
}

void PacketFactory::setTeamSelectScene(TeamSelectScene* scene)
{
	_team_select_scene = scene;
}

void PacketFactory::setGameWorldScene(GameWorldScene* scene)
{
	_game_world_scene = scene;
}

void PacketFactory::setEndingScene(EndingScene* scene)
{
	_ending_scene = scene;
}

boost::lockfree::queue<GameMessage, boost::lockfree::capacity<1000> >& 
PacketFactory::getQ()
{
	return _read_msgs;
}

