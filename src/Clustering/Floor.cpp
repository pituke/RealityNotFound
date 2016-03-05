#include "Clustering/Floor.h"
#include <Manager/ResourceManager.h>
#include <Clustering/Cuboid.h>

namespace Clustering
{
	static const float FLOOR_MIN_HEIGHT = 0.1f;
	static const float FLOOR_DIVIDE_BORDER_STICK_UP = 0.05;
	static const float FLOOR_DIVIDE_BORDER_HEIGHT = 0.02;

	Floor::Floor()
	{
		baseSize = 0;
		floorHeight = FLOOR_MIN_HEIGHT;
	}

	void Floor::setBaseSize(float size)
	{
		baseSize = size;
	}

	float Floor::getBaseSize() const
	{
		return baseSize;
	}

	void Floor::setFloorHeight(float height)
	{
		floorHeight = std::max(height, FLOOR_MIN_HEIGHT);
	}

	float Floor::getFloorHeight() const
	{
		return floorHeight;
	}

	void Floor::refresh()
	{
		const float floorDivideBorderBaseSize = baseSize + FLOOR_DIVIDE_BORDER_STICK_UP;
		const float floorDivideBorderGroundOffset = FLOOR_DIVIDE_BORDER_HEIGHT / 2;
		const float floorWallHeight = floorHeight - FLOOR_DIVIDE_BORDER_HEIGHT;
		const float floorGroundOffset = FLOOR_DIVIDE_BORDER_HEIGHT + floorWallHeight / 2;

		if (getNumChildren() > 0)
			removeChildren(0, getNumChildren());

		addChild(Manager::ResourceManager::getInstance()->getShape("floorBase", [&](const QString& params) { return new Cuboid(floorDivideBorderBaseSize, FLOOR_DIVIDE_BORDER_HEIGHT, floorDivideBorderBaseSize, osg::Vec3(0.0f, 0.0f, floorDivideBorderGroundOffset)); }));
		addChild(Manager::ResourceManager::getInstance()->getShape("floor", [&](const QString& params) { return new Cuboid(baseSize, floorWallHeight, baseSize, osg::Vec3(0.0f, 0.0f, floorGroundOffset)); }));
	}

	float Floor::getFloorMinHeight()
	{
		return FLOOR_MIN_HEIGHT;
	}
}
