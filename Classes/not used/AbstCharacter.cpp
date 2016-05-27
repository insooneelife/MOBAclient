#include <list>
#include <string>
#include "AbstCharacter.h"
#include "AbstWeaponSystem.h"
#include "GlobalValues.h"
#include "GameWorld.h"
#include "GameMap.h"
#include "Util.h"
#include "Message/Telegram.h"
#include "Motion.h"
#include "SteeringBehavior.h"
#include "CollisionArea.h"
#include "EntityFunctionTemplates.h"
#include "SensoryMemory.h"
#include "Goals\GoalThink.h"
#include "Geometry\Geometry.h"
#include "DebugRenderer.h"

USING_NS_CC;

AbstCharacter::AbstCharacter(
	Layer* const			view_layer,
	const std::string&		cname,
	GameWorld* const		world,
	const cocos2d::Vec2&	position,
	const cocos2d::Vec2&	margin,
	double					rotation,
	double					mass,
	double					max_force,
	double					max_speed,
	double					max_turn_rate,
	double					bounding_radius,
	int						meleeAttackPower,
	int						worth)
	:
	MovingEntity(
		world,
		new geometry::Circle(position, bounding_radius),
		CHARACTER,
		cname,
		position,
		bounding_radius,
		Vec2(),
		max_speed,
		Vec2(sin(rotation), -cos(rotation)),
		mass,
		margin,
		max_turn_rate,
		max_force,
		Prm.PenetrationConstraint),
	_motion(new Motion<AbstCharacter>(cname, Prm.CharacterFolderName, this, position, 0)),
	_direction(0),
	_motion_name("Neutral"),
	_status(ALIVE),
	_meleeAttackPower(meleeAttackPower),
	_worth(worth)
{

	_gold = 0;
	//--------------------------"Rendering Setting-------------------------------//
	_now_sprite = Sprite::create();

	float min_x = _now_sprite->getBoundingBox().getMinX();
	float min_y = _now_sprite->getBoundingBox().getMinY();
	float max_x = _now_sprite->getBoundingBox().getMaxX();
	float max_y = _now_sprite->getBoundingBox().getMaxY();

	float textureX = max_x - min_x;
	float textureY = max_y - min_y;

	_now_sprite->setPosition(position);
	_now_sprite->setGlobalZOrder(util::zOrdering(position.x, position.y));
	_now_sprite->setLocalZOrder(util::zOrdering(position.x, position.y));

	_shadow_sprite = Sprite::create();
	RenderUtil::shadowSetting(
		view_layer,
		_now_sprite,
		_shadow_sprite,
		_pos,
		Vec2(Prm.LightX, Prm.LightY),
		Prm.LightScale);

	_label = Label::createWithTTF(getName(), Prm.FontFolderName + "/" + "arial" + ".ttf", 10);
	_label->setAnchorPoint(Vec2(0, 0));
	_label->setVisible(Prm.RenderMainLabel);
	RenderUtil::labelSetting(_now_sprite, _label, Vec2(0, 0));

	view_layer->addChild(_now_sprite);

	_path_line = DrawNode::create();
	_path_line->setVisible(Prm.RenderPathLine);
	view_layer->addChild(_path_line);

	_goal_label = Label::createWithTTF("", Prm.FontFolderName + "/" + "arial" + ".ttf", 11);
	_goal_label->setVerticalAlignment(cocos2d::TextVAlignment::TOP);
	_goal_label->setHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
	_goal_label->setAnchorPoint(Vec2(1, 1));
	_goal_label->setVisible(Prm.RenderGoalLabel);
	RenderUtil::labelSetting(_now_sprite, _goal_label, Vec2(textureX * 0.8f, textureY));

	_bounding_shape->render(Debug.getBoundingShapePanel());

	_health_label = Label::createWithTTF("", Prm.FontFolderName + "/" + "arial" + ".ttf", 11);
	_health_label->setVerticalAlignment(cocos2d::TextVAlignment::BOTTOM);
	_health_label->setHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
	_health_label->setAnchorPoint(Vec2(1, 1));
	_health_label->setVisible(Prm.RenderHealthLabel);
	RenderUtil::labelSetting(_now_sprite, _health_label, Vec2(textureX * 0.8f, textureY));

	_gold_label = Label::createWithTTF("", Prm.FontFolderName + "/" + "arial" + ".ttf", 11);
	_gold_label->setVerticalAlignment(cocos2d::TextVAlignment::BOTTOM);
	_gold_label->setHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
	_gold_label->setAnchorPoint(Vec2(1, 1));
	_gold_label->setVisible(Prm.RenderHealthLabel);
	RenderUtil::labelSetting(_now_sprite, _gold_label, Vec2(textureX * 0.8f, textureY));

}

AbstCharacter::~AbstCharacter()
{
	_shadow_sprite->removeFromParent();
	_label->removeFromParent();

	_path_line->removeFromParent();
	_goal_label->removeFromParent();
	_health_label->removeFromParent();

	
	//_now_sprite must be deleted after child renderers.
	_sensory_memory.reset();
	_now_sprite->removeFromParent();

	//brain must be deleted before _motion, because it uses _motion's function.
	_brain.reset();
	_motion.reset();
	
#ifdef SHOW_DTOR_INFO
	CCLOG("~AbstCharacter()");
#endif
}

void AbstCharacter::update(double time_elapsed) 
{
	_brain->process();

	if (isReadyForGoalArbitration())
	{
		_brain->arbitrate();
	}
	updateMovement(time_elapsed);
}

void AbstCharacter::update(cocos2d::Vec2 pos, int frame, const std::string& motion_name)
{
	setPos(pos);
	_motion->setFrame(frame);
	setMotionName(motion_name);
}

void AbstCharacter::updateMovement(double time_elapsed)
{
	//update the time elapsed
	_time_elapsed = time_elapsed;

	//keep a record of its old position so we can update its cell later in this method
	Vec2 old_pos = _pos;
	Vec2 steering_force;

	//calculate the combined force from each steering behavior in the  vehicle's list
	steering_force = _steering->calculate();

	Vec2 acceleration = steering_force / _mass;

	//update velocity
	_velocity += acceleration *time_elapsed;

	//make sure vehicle does not exceed maximum velocity
	if (_velocity.getLength() > _max_speed)
	{
		_velocity.normalize();
		_velocity *= _max_speed;
	}

	//update the position
	movePos(_velocity *time_elapsed);

	//update the heading if the vehicle has a non zero velocity
	if (_velocity.getLengthSq() > 0.00000001)
	{
		_heading = _velocity;
		_heading = _heading_smoother.update(_heading);
		_heading.normalize();

		_side = _heading.getPerp();
	}

	int new_dir = util::make_direction(_heading, _motion->isDirectionEight());

	if (new_dir != _direction) {
		setDirection(new_dir);
		this->setMotionDirect(_motion_name);
	}
	_motion->update(_motion_name, _pos, new_dir);
	

	//enforce a non-penetration constraint if desired
	if (!_penetrate)
	{
		enforceNonPenetrationCellSpace(
			this, _game_world->getGameMap()->getAgentCellSpace());

		enforceNonPenetrationCellSpace(
			this, _game_world->getGameMap()->getCollisionAreaCellSpace());
	}

	_game_world->getGameMap()->getAgentCellSpace()->updateEntity(this, old_pos);
}

//------------------------ isAtPosition ---------------------------------------
//
//  returns true if the bot is close to the given position
//-----------------------------------------------------------------------------
bool AbstCharacter::isAtPosition(cocos2d::Vec2 pos)const
{
	const static float tolerance = 10.0f;

	return (getPos() - pos).getLengthSq() < tolerance * tolerance;
}

void AbstCharacter::reduceHealth(int damage)
{
	_health -= damage;

	if (_health <= 0)
	{
		setDead();
		
	}

	//m_bHit = true;
	//m_iNumUpdatesHitPersistant = (int)(FrameRate * script->GetDouble("HitFlashTime"));
}

//----------------- CalculateExpectedTimeToReachPosition ----------------------
//
//  returns a value indicating the time in seconds it will take the bot
//  to reach the given position at its current speed.
//-----------------------------------------------------------------------------
double AbstCharacter::calculateTimeToReachPosition(cocos2d::Vec2 pos)const
{
	return (getPos() - pos).getLengthSq() / (getMaxSpeed() * 500);
}

void AbstCharacter::render() 
{
	//_brain->render();
	_motion->render();
	//_sensory_memory->render();
	_bounding_shape->render(Debug.getBoundingShapePanel());
	

	/* 
	std::stringstream ss;
	std::stringstream ss1;
	ss << getHealth();
	ss1 << getMaxHealth();
	std::string str = ss.str() + "/" + ss1.str();

	_health_label->setString(str);
	*/

	/*std::stringstream ss;
	ss << getGold();
	std::string str = ss.str();
	_gold_label->setString(str);*/

}

void AbstCharacter::render(cocos2d::Vec2 pos, int frame, const std::string& motion_name)
{
	setPos(pos);
	//_brain->render();
	_motion->render(pos, frame, motion_name);
	//_sensory_memory->render();
	_bounding_shape->render(Debug.getBoundingShapePanel());
}

bool AbstCharacter::handleMessage(const Telegram& telegram)
{
	if (getBrain()->handleMessage(telegram)) return true;
	
	switch (telegram.msg)
	{
	case MSG_PATH_READY:
	{
		//CCLOG("MSG_PATH_READY");
		return true;
	}
	case MSG_NOPATH_AVAILABLE:
	{
		//CCLOG("MSG_NOPATH_AVAILABLE");
	}
	break;
	}

	return false; 
}

MovingEntity* AbstCharacter::findEnemyInRange()
{
	_game_world->getGameMap()->getAgentCellSpace()->calculateNeighborsForSmall(
		_pos, &geometry::Circle(this->getPos(), _steering->getViewDistance()));

	for (MovingEntity* iter = _game_world->getGameMap()->getAgentCellSpace()->begin();
		!_game_world->getGameMap()->getAgentCellSpace()->end();
		iter = _game_world->getGameMap()->getAgentCellSpace()->next())
	{
		if (iter != this)
		{
			return iter;
		}
	}
	return nullptr;
}


//distance between target->pos and this->pos is smaller than range => return true
bool AbstCharacter::targetInRange(double range) const
{
	return util::inRange(_steering->getTarget(), getPos(), range);
}

bool AbstCharacter::inAttackRange() const
{
	CCASSERT(_steering->getTargetAgent() != nullptr, "No TargetAgent!");
	return ((_steering->getTargetAgent()->getPos() - getPos()).getLength() 
		< Prm.CharacterBaseAttackRange);
}

//returns true if this bot can move directly to the given position
//without bumping into any walls
bool AbstCharacter::canWalkTo(Vec2 pos)const
{
	return _game_world->isLOSOkay(getPos(), pos);
}

//similar to above. Returns true if the bot can move between the two
//given positions without bumping into any walls
bool AbstCharacter::canWalkBetween(Vec2 from, Vec2 to)const
{
	return _game_world->isLOSOkay(from, to);
}

//------------------ RotateFacingTowardPosition -------------------------------
//
//  given a target position, this method rotates the bot's facing vector
//  by an amount not greater than m_dMaxTurnRate until it
//  directly faces the target.
//
//  returns true when the heading is facing in the desired direction
//----------------------------------------------------------------------------
bool AbstCharacter::rotateFacingTowardPosition(cocos2d::Vec2 target)
{
	cocos2d::Vec2 toTarget = (target - _pos).getNormalized();

	double dot = _heading.dot(toTarget);

	//clamp to rectify any rounding errors
	util::clamp(dot, -1, 1);

	//determine the angle between the heading vector and the target
	double angle = acos(dot);

	//return true if the bot's facing is within WeaponAimTolerance degs of
	//facing the target
	const double WeaponAimTolerance = 0.01; //2 degs approx

	if (angle < WeaponAimTolerance)
	{
		_heading = toTarget;
		return true;
	}

	//clamp the amount to turn to the max turn rate
	if (angle > _max_turn_rate) angle = _max_turn_rate;

	//The next few lines use a rotation matrix to rotate the player's facing
	//vector accordingly
	C2DMatrix RotationMatrix;

	//notice how the direction of rotation has to be determined when creating
	//the rotation matrix
	RotationMatrix.Rotate(angle * util::sign(_heading, toTarget));
	RotationMatrix.TransformVector2Ds(_heading);

	return false;
}

//---------------------------- isReadyForGoalArbitration ------------------------
//
//  returns true if the bot is ready to be goal arbitration
//-----------------------------------------------------------------------------
bool AbstCharacter::isReadyForGoalArbitration()const
{
	return _goal_arbitration_regulator->isReady();
}

//---------------------------- isReadyForTriggerUpdate ------------------------
//
//  returns true if the bot is ready to be tested against the world triggers
//-----------------------------------------------------------------------------
bool AbstCharacter::isReadyForTriggerUpdate()const
{
	return _trigger_test_regulator->isReady();
}

bool AbstCharacter::isPossessed()const 
{
	return _possessed; 
}

bool AbstCharacter::isDead()const 
{
	return _status == DEAD; 
}

bool AbstCharacter::isAlive()const 
{
	return _status == ALIVE; 
}

LARGE_INTEGER AbstCharacter::getTime() const 
{
	return _time; 
}

void AbstCharacter::addTime(LONGLONG delta) 
{
	_time.QuadPart += delta; 
}

void AbstCharacter::addTime(LARGE_INTEGER delta) 
{
	addTime(delta.QuadPart); 
}

//getters & setters

const std::unique_ptr<SteeringBehavior>& AbstCharacter::getSteering() const
{
	return _steering;
}

const std::unique_ptr<Motion>& AbstCharacter::getMotion() const
{
	return _motion;
}

const std::unique_ptr<PathPlanner>& AbstCharacter::getPathPlanner() const
{
	return _path_planner; 
}

const std::unique_ptr<GoalThink>& AbstCharacter::getBrain() const
{
	return _brain; 
}

const std::unique_ptr<SensoryMemory>& AbstCharacter::getSensoryMemory() const
{
	return _sensory_memory;
}

const std::unique_ptr<AbstTargetingSystem>& AbstCharacter::getTargetSys() const
{
	return _target_system;
}

const std::unique_ptr<AbstWeaponSystem>& AbstCharacter::getWeaponSys() const 
{
	return _weapon_system; 
}

const Vec2 AbstCharacter::getAttackDirection() const
{
	return (_target_system->getTarget()->getPos() - getPos()).getNormalized();
}

Sprite* AbstCharacter::getNowSprite() const
{
	return _now_sprite;
}

Sprite* AbstCharacter::getShadowSprite() const
{
	return _shadow_sprite;
}

Label* AbstCharacter::getLabel() const
{
	return _label;
}
DrawNode* AbstCharacter::getPathLine() const
{
	return _path_line;
}

Label* AbstCharacter::getGoalLabel() const
{
	return _goal_label;
}

int AbstCharacter::getDirection() const
{
	return _direction;
}

void AbstCharacter::setDirection(int direction)
{
	_direction = direction;
}

const std::string& AbstCharacter::getMotionName() const
{
	return _motion_name;
}

void AbstCharacter::setMotionName(const std::string& motion)
{
	_motion_name = motion;
}

void AbstCharacter::setMotionDirect(const std::string& motion)
{
	_motion_name = motion;
	_motion->setMotion(motion, _direction);
}

int AbstCharacter::getHealth() const
{
	return _health;
}

void AbstCharacter::setHealth(int health)
{
	_health = health;
}

int AbstCharacter::getMaxHealth() const
{
	return _max_health;
}

void AbstCharacter::setMaxHealth(int maxhealth)
{
	_max_health = maxhealth;
}

int AbstCharacter::getScore() const
{
	return _score;
}

void AbstCharacter::setScore(int score)
{
	_score = score;
}

const Vec2& AbstCharacter::getFacing() const
{
	return _facing;
}

void AbstCharacter::setFacing(const Vec2 facing)
{
	_facing = facing;
}
double AbstCharacter::getViewOnField() const
{
	return _view_on_field;
}

void AbstCharacter::setViewOnField(double view)
{
	_view_on_field = view;
}

int AbstCharacter::getNumHitUpdates() const
{
	return _num_hit_updates;
}

void AbstCharacter::setNumHitUpdates(int num_hit_updates)
{
	_num_hit_updates = num_hit_updates;
}

bool AbstCharacter::getHit() const
{
	return _hit;
}

void AbstCharacter::setHit(bool hit)
{
	_hit = hit;
}

void AbstCharacter::setPathQ(std::queue<cocos2d::Vec2> pathQ)
{
	_pathQ = pathQ;
}

std::queue<cocos2d::Vec2>* AbstCharacter::getPathQ()
{
	return &_pathQ;
}

void AbstCharacter::setTeam(int team)
{
	_team = team;
}

int AbstCharacter::getTeam() const
{
	return _team;
}

int AbstCharacter::getMeleeAttackPower() const
{
	return _meleeAttackPower;
}

void AbstCharacter::setMeleeAttackPower(int power)
{
	_meleeAttackPower = power;
}


void AbstCharacter::addGold(int gold)
{
	_gold += gold;
}

int AbstCharacter::getGold() const
{
	return _gold;
}


void AbstCharacter::setWorth(int worth)
{
	_worth = worth;
}

int AbstCharacter::getWorth() const
{
	return _worth;
}

cocos2d::Label* AbstCharacter::getHealthLabel() const
{
	return _health_label;
}

cocos2d::Label* AbstCharacter::getGoldLabel() const
{
	return _gold_label;
}