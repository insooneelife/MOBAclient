#include <sstream>
#include <fstream>
#include "TriggerHealthGiver.h"
#include "DebugRenderer.h"

USING_NS_CC;

TriggerHealthGiver::TriggerHealthGiver(
	GameWorld* const world,
	Layer* const view_layer,
	geometry::Shape* const bounding_shape,
	const std::string& name,
	const Vec2&	pos,
	const Vec2&	margin,
	double radius,
	int graph_node_index,
	int health_given,
	unsigned int respawn_delay)
	:
	TriggerRespawning<AbstCharacter>(world, bounding_shape),
	_health_given(health_given)
{
	setName(name);
	setPos(pos);
	setMargin(margin);
	setBRadius(radius);
	setGraphNodeIndex(graph_node_index);

	//create this trigger's region of fluence

	setRespawnDelay(respawn_delay);
	//setType(type_health);

	std::string foldername = Prm.StructureFolderName;
	_now_sprite = Sprite::create();
	_now_sprite->setPosition(getPos());
	view_layer->addChild(_now_sprite);

	_label = Label::createWithTTF(
		getName(), Prm.FontFolderName + "/" + "arial" + ".ttf", 15);
	RenderUtil::labelSetting(_now_sprite, _label, Vec2());
}

TriggerHealthGiver::~TriggerHealthGiver()
{
	_label->removeFromParent();
	_now_sprite->removeFromParent();
}

void TriggerHealthGiver::actIfOverlap(AbstCharacter* bot)
{
	if (isActive() && isTouchingTrigger(bot->getPos(), bot->getBRadius()))
	{
		//put event in here

		deactivate();
	}
}

void TriggerHealthGiver::render()
{
	_bounding_shape->render(Debug.getBoundingShapePanel());

	if (!isActive()) 
	{
		std::stringstream ss;
		ss << _num_update_remaining;
		_label->setString(getName() + "\n" + ss.str());
	}
}

Sprite* TriggerHealthGiver::getNowSprite() const
{
	return _now_sprite;
}

Label* TriggerHealthGiver::getLabel() const
{
	return _label;
}