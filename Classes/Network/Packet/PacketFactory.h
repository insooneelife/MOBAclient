#pragma once

#include "../GameClient/GameMessage.hpp"
#include "PACKETClick_generated.h"
#include "PACKETRenderCharacter_generated.h"
#include <boost\lockfree\queue.hpp>
#include <queue>

class GameWorld;
class TeamSelectScene;
class GameWorldScene;
class ItemStoreLayer;
class EndingScene;

class PacketFactory
{
public:

	// server -> client
	enum
	{
		PacketAssignID, PacketCreateUserCharacter, PacketStartGameScene,
		PacketStartTeamSelectScene, PacketClientJoinTeamSelectRoom, PacketClientLeaveTeamSelectRoom,
		PacketRenderCharacterSet, PacketRenderSkillSet,
		PacketCreateCharacter, PacketCreateSkill,
		PacketDeleteCharacter, PacketDeleteSkill,
		PacketClientSelectedCharacter, PacketClientSelectedTeam, PacketClientClickedReady,
		PacketGameChatMessageToClient, PacketRoomChatMessageToClient,
		PacketCreateAnimationOnPos, PacketWorldData, PacketGameOver
	};

	// client -> server
	enum
	{
		PacketClick,
		PacketClickedReady, PacketClientIsReady,
		PacketSelectName, PacketSelectCharacter, PacketSelectTeam, 
		PacketGameChatMessageToServer, PacketRoomChatMessageToServer
	};

	static PacketFactory& getInstance();

	void setGameWorld(GameWorld* world);

	void setTeamSelectScene(TeamSelectScene* scene);

	void setGameWorldScene(GameWorldScene* scene);

	void setEndingScene(EndingScene* scene);

	boost::lockfree::queue<GameMessage, boost::lockfree::capacity<1000> >& getQ();


	// client : send to server

	GameMessage createPacketClick(
		unsigned int id,
		float x,
		float y,
		packets::MouseButton mouse_button,
		int command,
		unsigned int clicked_id);

	GameMessage createPacketClientIsReady();

	GameMessage createPacketSelectName(
		unsigned int id,
		const std::string& cname);

	GameMessage createPacketSelectCharacter(
		unsigned int id,
		const std::string& cname);

	GameMessage createPacketSelectTeam(
		unsigned int id,
		int team);

	GameMessage createPacketClickedReady(
		unsigned int id,
		int ready_flag);

	GameMessage createPacketGameChatMessageToServer(
		unsigned int id,
		const std::string& chat_message);

	GameMessage createPacketRoomChatMessageToServer(
		unsigned int id,
		const std::string& chat_message);


	// client : receive from server

	void decodeByType(GameMessage& msg);

	void decodePacketRenderCharacterSet(uint8_t* body);

	void decodePacketCreateCharacter(uint8_t* body);

	void decodePacketAssignID(uint8_t* body);

	void decodePacketCreateUserCharacter(uint8_t* body);

	void decodePacketStartGameScene();

	void decodePacketCreateSkill(uint8_t* body);

	void decodePacketRenderSkillSet(uint8_t* body);

	void decodePacketDeleteCharacter(uint8_t* body);

	void decodePacketDeleteSkill(uint8_t* body);

	void decodePacketStartTeamSelectScene(uint8_t* body);

	void decodePacketClientJoinTeamSelectRoom(uint8_t* body);

	void decodePacketClientLeaveTeamSelectRoom(uint8_t* body);

	void decodePacketClientSelectedCharacter(uint8_t* body);

	void decodePacketClientSelectedTeam(uint8_t* body);

	void decodePacketClientClickedReady(uint8_t* body);

	void decodePacketGameChatMessageToClient(uint8_t* body);

	void decodePacketRoomChatMessageToClient(uint8_t* body);

	void decodePacketCreateAnimationOnPos(uint8_t* body);

	void decodePacketWorldData(uint8_t* body);

	void decodePacketGameOver(uint8_t* body);

private:

	PacketFactory();

	~PacketFactory()
	{}

	GameWorld* _game_world;

	TeamSelectScene* _team_select_scene;

	GameWorldScene* _game_world_scene;

	EndingScene* _ending_scene;

	boost::lockfree::queue<GameMessage, boost::lockfree::capacity<1000> > _read_msgs;

};