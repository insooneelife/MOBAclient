#include <fstream>
#include <map>
#include <vector>
#include <tuple>
#include <algorithm>
#include "GameMap.h"
#include "GameWorld.h"
#include "Entity/CollisionArea.h"
#include "Entity/Structure.h"
#include "EntityManager.h"
#include "Util.h"
#include "DebugRenderer.h"


USING_NS_CC;

GameMap::GameMap(
	GameWorld* const game_world,
	cocos2d::Layer* const view_layer)
	:
	_game_world(game_world),
	_view_layer(view_layer)
{
	_tile_map = cocos2d::TMXTiledMap::create("tilemap/100_160_2.tmx");
	//_tile_map = cocos2d::TMXTiledMap::create("tilemap/diablo2_outdoor_remove_walls.tmx");
	_view_layer->addChild(_tile_map);
	
	auto ms = _tile_map->getMapSize();
	auto ts = _tile_map->getTileSize();
	_sizeX = (ms.width + 0.5f) * ts.width;
	_sizeY = (ms.height + 1) * ts.height / 2;

	_tile_map->setGlobalZOrder(MAP_ZORDER);
	CCLOG("%f %f %f %f", _sizeX, _sizeY, ts.width, ts.height);

	createCollisionAreas();
	createStructures();

	renderDestinations();
	renderGraph();
	renderCellSpace();
}

GameMap::~GameMap()
{}

void GameMap::createStructures()
{
	//read structure data from tmx file
	auto ob_group = _tile_map->getObjectGroup("structures");
	CCASSERT(ob_group, "no object group structures");

	for (auto iter = std::begin(ob_group->getObjects()); 
		iter != std::end(ob_group->getObjects()); iter++)
	{
		float width = iter->asValueMap()["width"].asFloat();
		float height = iter->asValueMap()["height"].asFloat();
		float x = iter->asValueMap()["x"].asFloat() + width * 0.5f;
		float y = iter->asValueMap()["y"].asFloat() 
			- getSizeY() + _tile_map->getTileSize().height + height * 1.5f;
		
		float mx = iter->asValueMap()["marginX"].asFloat();
		float my = iter->asValueMap()["marginY"].asFloat();

		float shadow_mx = iter->asValueMap()["shadow_marginX"].asFloat();
		float shadow_my = iter->asValueMap()["shadow_marginY"].asFloat();

		std::string name = iter->asValueMap()["name"].asString();
		float radius = iter->asValueMap()["radius"].asFloat();
		bool shadow = iter->asValueMap()["shadow"].asBool();

		_structures.emplace_back(
			new Structure(
				_game_world,
				_view_layer,
				new geometry::Circle(Vec2(x, y) - Vec2(mx, my), radius),
				10,
				name,
				Vec2(x, y), 
				Vec2(mx, my),
				Vec2(shadow_mx, shadow_my),
				radius,
				shadow));
	}
}


void GameMap::createCollisionAreas()
{
	auto ob_group = _tile_map->getObjectGroup("structures_no_image");
	CCASSERT(ob_group, "no object group (structures_no_image)");

	for (auto iter = std::begin(ob_group->getObjects());
	iter != std::end(ob_group->getObjects()); iter++)
	{
		float width = iter->asValueMap()["width"].asFloat();
		float height = iter->asValueMap()["height"].asFloat();
		float x = iter->asValueMap()["x"].asFloat();
		float y = iter->asValueMap()["y"].asFloat()
			- getSizeY() + _tile_map->getTileSize().height;
		std::string type = iter->asValueMap()["type"].asString();

		if (type == "Polygon")
		{
			ValueVector points = iter->asValueMap()["polylinePoints"].asValueVector();
			std::vector<cocos2d::Vec2> poly;

			for (int i = 0; i < points.size(); i++)
			{
				ValueMap a = points[i].asValueMap();

				float xy = a["x"].asFloat();
				float yy = a["y"].asFloat();

				poly.push_back(cocos2d::Vec2(
					xy + x,
					y - yy));
			}
			poly.pop_back();

			geometry::Circle min_circle = geometry::enclosingCircleNaive(poly);

			_collision_areas.emplace_back(
				new CollisionArea(
					_game_world,
					new geometry::Polygon(poly),
					10,
					min_circle.origin,
					min_circle.radius));

		}
		else if (type == "Circle")
		{
			float radius = width / 2;
			_collision_areas.emplace_back(
				new CollisionArea(
					_game_world,
					new geometry::Circle(
						Vec2(x + width * 0.5f, y + height * 0.5f), radius),
					10,
					Vec2(x + width * 0.5f, y + height * 0.5f),
					radius));
		}
	}
}

void GameMap::renderGraph()
{
	std::map<int, std::tuple<int, float, float> > nodes;
	auto node_group = _tile_map->getObjectGroup("nodes");

	for (auto iter = std::begin(node_group->getObjects());
	iter != std::end(node_group->getObjects()); iter++)
	{
		int index = iter->asValueMap()["index"].asInt();
		int extra = iter->asValueMap()["extra"].asInt();
		float x = iter->asValueMap()["x"].asFloat();
		float y = iter->asValueMap()["y"].asFloat()
			- getSizeY() + _tile_map->getTileSize().height;

		nodes.emplace(std::make_pair(index, std::make_tuple(extra, x, y)));

		//render node
		Debug.getGraphPanel()->
			drawCircle(cocos2d::Vec2(x, y), 10, 2 * M_PI, 40, false, Color4F::MAGENTA);
	}

	auto edge_group = _tile_map->getObjectGroup("edges");
	for (auto iter = std::begin(edge_group->getObjects());
	iter != std::end(edge_group->getObjects()); iter++)
	{
		int from = iter->asValueMap()["from"].asInt();
		int to = iter->asValueMap()["to"].asInt();
		int flag = iter->asValueMap()["flag"].asInt();

		auto from_it = nodes.find(from);
		auto to_it = nodes.find(to);

		if (from_it != std::end(nodes) && to_it != std::end(nodes))
		{
			Debug.getGraphPanel()->
				drawLine(
					cocos2d::Vec2(std::get<1>(from_it->second), std::get<2>(from_it->second)),
					cocos2d::Vec2(std::get<1>(to_it->second), std::get<2>(to_it->second)),
					Color4F::GRAY);
		}
		else
		{
			CCASSERT(false, "not existing node in edge (from or to)");
		}
	}
}

void GameMap::renderCellSpace()
{
	//calculate bounds of each cell
	float cell_sizeX = _sizeX / Prm.NumCellsX;
	float cell_sizeY = _sizeY / Prm.NumCellsY;

	//create the cells
	for (int y = 0; y < Prm.NumCellsY; ++y)
	{
		for (int x = 0; x < Prm.NumCellsX; ++x)
		{
			float left = x * cell_sizeX;
			float right = left + cell_sizeX;
			float bot = y * cell_sizeY;
			float top = bot + cell_sizeY;

			Debug.getCellBoxPanel()->drawRect(Vec2(left, bot), Vec2(right, top), Color4F::WHITE);
		}
	}
}

void GameMap::renderDestinations()
{
	//read destinations data from tmx file
	auto ob_group = _tile_map->getObjectGroup("destinations");
	CCASSERT(ob_group, "no object group destinations");

	for (auto iter = std::begin(ob_group->getObjects());
	iter != std::end(ob_group->getObjects()); iter++)
	{
		float width = iter->asValueMap()["width"].asFloat();
		float height = iter->asValueMap()["height"].asFloat();
		float x = iter->asValueMap()["x"].asFloat() + width * 0.5f;
		float y = iter->asValueMap()["y"].asFloat()
			- getSizeY() + _tile_map->getTileSize().height + height * 0.5f;
		float radius = width / 2;

		Debug.getCustomShapePanel()->
			drawCircle(cocos2d::Vec2(x, y), radius, 2 * M_PI, 40, false, Color4F::RED);
	}
}


void GameMap::render()
{
	for (auto iter = std::begin(_collision_areas); 
		iter != std::end(_collision_areas); iter++)
	{
		(*iter)->render();
	}

	for (auto iter = std::begin(_structures); iter != std::end(_structures); iter++)
	{
		(*iter)->render();
	}
}

float GameMap::getSizeX()const
{
	return _sizeX;
}

float GameMap::getSizeY()const
{
	return _sizeY;
}

GameWorld* GameMap::getGameWorld() const
{
	return _game_world;
}

const GameMap::CollisionAreaVector& GameMap::getCollisionAreas() const
{
	return _collision_areas;
}

const GameMap::StructureVector& GameMap::getStructures() const
{
	return _structures;
}

cocos2d::TMXTiledMap* GameMap::getTileMap() const
{
	return _tile_map;
}
