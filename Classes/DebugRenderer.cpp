#include "DebugRenderer.h"
#include "GlobalValues.h"
#include "ParameterLoader.h"

void DebugRenderer::initCustomShapePanel(bool draw, int line_width)
{
	_custom_shape_panel = cocos2d::DrawNode::create();
	_custom_shape_panel->setGlobalZOrder(DEBUG_RENDER_ZORDER);
	_custom_shape_panel->setVisible(draw);
	_custom_shape_panel->setLineWidth(line_width);
	_custom_shape_panel->setCameraMask(1 << 2);
}

void DebugRenderer::initBoundingShapePanel(bool draw, int line_width)
{
	_bounding_shape_panel = cocos2d::DrawNode::create();
	_bounding_shape_panel->setGlobalZOrder(DEBUG_RENDER_ZORDER);
	_bounding_shape_panel->setVisible(draw);
	_bounding_shape_panel->setLineWidth(line_width);
	_bounding_shape_panel->setCameraMask(1 << 2);
}

void DebugRenderer::initBoundingBoxPanel(bool draw, int line_width)
{
	_bounding_box_panel = cocos2d::DrawNode::create();
	_bounding_box_panel->setGlobalZOrder(DEBUG_RENDER_ZORDER);
	_bounding_box_panel->setVisible(draw);
	_bounding_box_panel->setLineWidth(line_width);
	_bounding_box_panel->setCameraMask(1 << 2);
}

void DebugRenderer::initViewCirclePanel(bool draw, int line_width)
{
	_view_circle_panel = cocos2d::DrawNode::create();
	_view_circle_panel->setGlobalZOrder(DEBUG_RENDER_ZORDER);
	_view_circle_panel->setVisible(draw);
	_view_circle_panel->setLineWidth(line_width);
	_view_circle_panel->setCameraMask(1 << 2);
}

void DebugRenderer::initGraphPanel(bool draw, int line_width)
{
	_graph_panel = cocos2d::DrawNode::create();
	_graph_panel->setGlobalZOrder(DEBUG_RENDER_ZORDER);
	_graph_panel->setVisible(draw);
	_graph_panel->setLineWidth(line_width);
	_graph_panel->setCameraMask(1 << 2);
}

void DebugRenderer::initCellBoxPanel(bool draw, int line_width)
{
	_cell_box_panel = cocos2d::DrawNode::create();
	_cell_box_panel->setGlobalZOrder(DEBUG_RENDER_ZORDER);
	_cell_box_panel->setVisible(draw);
	_cell_box_panel->setLineWidth(line_width);
	_cell_box_panel->setCameraMask(1 << 2);
}

void DebugRenderer::initPathLinePanel(bool draw, int line_width)
{
	_path_line_panel = cocos2d::DrawNode::create();
	_path_line_panel->setGlobalZOrder(DEBUG_RENDER_ZORDER);
	_path_line_panel->setVisible(draw);
	_path_line_panel->setLineWidth(line_width);
	_path_line_panel->setCameraMask(1 << 2);
}


cocos2d::DrawNode* DebugRenderer::getCustomShapePanel() const
{
	return _custom_shape_panel;
}

cocos2d::DrawNode* DebugRenderer::getBoundingShapePanel() const
{
	return _bounding_shape_panel;
}

cocos2d::DrawNode* DebugRenderer::getBoundingBoxPanel() const
{
	return _bounding_box_panel;
}

cocos2d::DrawNode* DebugRenderer::getViewCirclePanel() const
{
	return _view_circle_panel;
}

cocos2d::DrawNode* DebugRenderer::getGraphPanel() const
{
	return _graph_panel;
}

cocos2d::DrawNode* DebugRenderer::getCellBoxPanel() const
{
	return _cell_box_panel;
}

cocos2d::DrawNode* DebugRenderer::getPathLinePanel() const
{
	return _path_line_panel;
}

