#ifndef MAP_H
#define MAP_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Map.h
//
//  Desc:   This class creates and stores all the entities that make up the
//          map environment. (walls, structures)
//			For rendering, it references a view layer and has a tilemap.
//			For working system, it has trigger system for control game flows,
//			cell space partition for make more performance interacting logic,
//			and has navigation graph for agent to find path.
//			It also has reference to world, so it can interact with it. 
//
//  Author: Insub Im (insooneelife@naver.com)
//
//-----------------------------------------------------------------------------
#include <vector>
#include <string>
#include <list>
#include <memory>
#include <unordered_map>
#include <vector>
#include <array>

#include "cocos2d.h"
#include "Geometry\Geometry.h"

class BaseGameEntity;
class Wall;
class GameWorld;
class MovingEntity;
class AbstCharacter;
class CollisionArea;
class Structure;

template <class Trigger> class TriggerSystem;

class GameMap
{
public:

	typedef std::vector<std::unique_ptr<CollisionArea> >	CollisionAreaVector;
	typedef std::vector<std::unique_ptr<Structure> >		StructureVector;

	GameMap(GameWorld* const game_world, cocos2d::Layer* const view_layer);

	~GameMap();


	GameWorld*						getGameWorld() const;

	const CollisionAreaVector&		getCollisionAreas() const;

	const StructureVector&			getStructures() const;

	cocos2d::TMXTiledMap*			getTileMap() const;

	float							getSizeX() const;

	float							getSizeY() const;

	void createStructures();

	void createCollisionAreas();

	void renderGraph();

	void renderCellSpace();

	void renderDestinations();

	void render();
	

private:

	GameMap(const GameMap&) = delete; // no copies
	GameMap& operator=(const GameMap&) = delete; // no self-assignments
	GameMap() = delete;

	float				_sizeX;
	float				_sizeY;

	//references game world
	GameWorld* const						_game_world;

	//references the game entity view layer
	cocos2d::Layer* const 					_view_layer;

	//collision areas
	CollisionAreaVector						_collision_areas;

	//structures
	StructureVector							_structures;

	//tile map
	cocos2d::TMXTiledMap*					_tile_map;

};



#endif