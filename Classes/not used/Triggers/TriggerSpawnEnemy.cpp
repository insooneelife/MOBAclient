#include "TriggerSpawnEnemy.h"
#include <sstream>
#include <fstream>

USING_NS_CC;

TriggerSpawnEnemy::TriggerSpawnEnemy(
	GameWorld* const world,
	Layer* const view_layer,
	geometry::Shape* const bounding_shape,
	const std::string& name,
	const Vec2&	pos,
	const Vec2&	margin,
	double radius,
	int graph_node_index,
	int team)
	:
	TriggerRespawning<AbstCharacter>(world, bounding_shape),
	_pos(pos),
	_team(team),
	_view_layer(view_layer)
{
	CCLOG("SPAWN CREATE\n");
	//CCLOG("x: %f y: %f\n", pos.x, pos.y);
	_numSpawn = 0;

	setName(name);
	setPos(pos);
	setMargin(margin);
	setBRadius(radius);
	setGraphNodeIndex(graph_node_index);

	setRespawnDelay(50);  // respawn delay.
	//setType(type_health);

	std::string foldername = Prm.StructureFolderName;
	_now_sprite = Sprite::create();
	_now_sprite->setPosition(getPos());
	view_layer->addChild(_now_sprite);

	_label = Label::createWithTTF(
		getName(), Prm.FontFolderName + "/" + "arial" + ".ttf", 15);
	RenderUtil::labelSetting(_now_sprite, _label, Vec2());

}

TriggerSpawnEnemy::~TriggerSpawnEnemy()
{
	CCLOG("REMOVE SPAWN TRIGGER\n");
	_label->removeFromParent();
	_now_sprite->removeFromParent();
}

void TriggerSpawnEnemy::update(double time_elapsed)
{
	if (_numSpawn < Prm.NumSpawn && _num_update_for_respawn-- == 0)
	{
		_world->createEnemy(_pos, _team);
		_numSpawn++;
		_num_update_for_respawn = 50;
	}

}

void TriggerSpawnEnemy::actIfOverlap(AbstCharacter* bot)
{
	// do nothing in this trigger

  /*	if (isActive() && isTouchingTrigger(bot->getPos(), bot->getBoundingRadius()))
	{
		//put event in here

		deactivate();
	}
	*/
}

void TriggerSpawnEnemy::render()
{
	_bounding_shape->render(Debug.getBoundingShapePanel());

	std::stringstream ss;
	ss << _num_update_for_respawn;
	_label->setString(getName() + "\n" + ss.str());
}

Sprite* TriggerSpawnEnemy::getNowSprite() const
{
	return _now_sprite;
}

Label* TriggerSpawnEnemy::getLabel() const
{
	return _label;
}