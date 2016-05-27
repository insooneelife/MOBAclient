#pragma once

#include "Singleton.h"
#include "cocos2d.h"
#include "Geometry\Geometry.h"

//provide easy access
#define Debug DebugRenderer::getInstance()

class DebugRenderer : public Singleton<DebugRenderer>
{
public:
	cocos2d::DrawNode* getCustomShapePanel() const;

	cocos2d::DrawNode* getBoundingShapePanel() const;

	cocos2d::DrawNode* getBoundingBoxPanel() const;

	cocos2d::DrawNode* getViewCirclePanel() const;

	cocos2d::DrawNode* getGraphPanel() const;

	cocos2d::DrawNode* getCellBoxPanel() const;

	cocos2d::DrawNode* getPathLinePanel() const;


	void initCustomShapePanel(bool draw, int line_width);

	void initBoundingShapePanel(bool draw, int line_width);

	void initBoundingBoxPanel(bool draw, int line_width);

	void initViewCirclePanel(bool draw, int line_width);

	void initGraphPanel(bool draw, int line_width);

	void initCellBoxPanel(bool draw, int line_width);

	void initPathLinePanel(bool draw, int line_width);

private:
	DebugRenderer(const DebugRenderer&) = delete; // no copies
	DebugRenderer& operator=(const DebugRenderer&) = delete; // no self-assignments
	DebugRenderer() {}
	friend class Singleton<DebugRenderer>;


	cocos2d::DrawNode* _custom_shape_panel;

	cocos2d::DrawNode* _bounding_shape_panel;

	cocos2d::DrawNode* _bounding_box_panel;

	cocos2d::DrawNode* _view_circle_panel;

	cocos2d::DrawNode* _graph_panel;

	cocos2d::DrawNode* _cell_box_panel;

	cocos2d::DrawNode* _path_line_panel;
};