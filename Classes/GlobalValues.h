#pragma once
#include "cocos2d.h"

//axis for determine direction
const cocos2d::Vec2 axis1 = cocos2d::Vec2(cos(M_PI / 8), sin(M_PI / 8));
const cocos2d::Vec2 axis2 = cocos2d::Vec2(cos(3 * M_PI / 8), sin(3 * M_PI / 8));
const cocos2d::Vec2 axis3 = cocos2d::Vec2(cos(5 * M_PI / 8), sin(5 * M_PI / 8));
const cocos2d::Vec2 axis4 = cocos2d::Vec2(cos(7 * M_PI / 8), sin(7 * M_PI / 8));
const cocos2d::Vec2 axis5 = cocos2d::Vec2(cos(9 * M_PI / 8), sin(9 * M_PI / 8));
const cocos2d::Vec2 axis6 = cocos2d::Vec2(cos(11 * M_PI / 8), sin(11 * M_PI / 8));
const cocos2d::Vec2 axis7 = cocos2d::Vec2(cos(13 * M_PI / 8), sin(13 * M_PI / 8));
const cocos2d::Vec2 axis8 = cocos2d::Vec2(cos(15 * M_PI / 8), sin(15 * M_PI / 8));

const int MAP_ZORDER = 0;
const int SHADOW_ZORDER = 1; 
const int DEBUG_RENDER_ZORDER = 2;
const int CELL_BOX_ZORDER_ON = 3;
const int CELL_BOX_ZORDER_OFF = 2;
const int SKILL_ZORDER = 2000000000;
const int ANIMATION_ZORDER = 2000000000;
const int LABEL_ZORDER = 2000000001;
const double SMALLEST_DELAY = 0.25;

const double NORMAL = 1.0;
const double FAST = 0.5;
const double SLOW = 2;

//to make code easier to read
const double		SEND_MSG_IMMEDIATELY = 0.0;
const int			NO_ADDITIONAL_INFO = 0;
const unsigned int	SENDER_ID_IRRELEVANT = 0;

enum message_type
{
	MSG_BLANK,
	MSG_PATH_READY,
	MSG_NOPATH_AVAILABLE,
	MSG_TAKE_THAT_MF,
	MSG_YOU_GOT_ME,
	MSG_GOAL_QUEUE_EMPTY,
	MSG_GUN_SHOT_SOUND,
	MSG_USER_HAS_REMOVED_BOT
};

enum
{
	GOAL_THINK,
	GOAL_EXPLORE,
	GOAL_ARRIVE_AT_POS,
	GOAL_SEEK_TO_POS,
	GOAL_FOLLOW_PATH,
	GOAL_TRAVERSE_EDGE,
	GOAL_MOVE_TO_POS,
	GOAL_HUNT_TARGET,
	GOAL_WANDER,
	GOAL_ATTACK_TARGET,
	GOAL_GET_HEALTH,
	GOAL_SAY_PHRASE,
	goal_get_shotgun,
	goal_get_rocket_launcher,
	goal_get_railgun,
	goal_negotiate_door,
	goal_strafe,
	goal_adjust_range
};

enum
{
	TEAM_RED, TEAM_BLUE, TEAM_NATURAL
};

enum
{
	LINE_NONE = -1, LINE_TOP, LINE_BOTTOM
};

enum
{
	LOGIN_SCENE, TEAM_SELECT_SCENE, GAME_WORLD_SCENE
};

enum
{
	CLICKED_READY, CLICKED_NOT_READY
};

const int MAX_MESSAGES = 30;