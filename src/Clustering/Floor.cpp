#include "Clustering/Floor.h"
#include <Manager/ResourceManager.h>
#include <Clustering/Cuboid.h>

namespace Clustering
{
	static const float FLOOR_MIN_HEIGHT = 0.1f;
	static const float FLOOR_DIVIDE_BORDER_STICK_UP = 0.05;
	static const float FLOOR_DIVIDE_BORDER_HEIGHT = 0.02;

	Floor::Floor(bool divideBorder)
	{
		this->divideBorder = divideBorder;
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
		const float realFloorDivideBorderHeight = divideBorder ? FLOOR_DIVIDE_BORDER_HEIGHT : 0.0f;
		const float floorDivideBorderBaseSize = baseSize + FLOOR_DIVIDE_BORDER_STICK_UP;
		const float floorDivideBorderGroundOffset = realFloorDivideBorderHeight / 2;
		const float floorWallHeight = floorHeight - realFloorDivideBorderHeight;
		const float floorGroundOffset = realFloorDivideBorderHeight + floorWallHeight / 2;

		removeChildren(0, getNumChildren());

		if (divideBorder)
			addChild(new Cuboid(floorDivideBorderBaseSize, FLOOR_DIVIDE_BORDER_HEIGHT, floorDivideBorderBaseSize, osg::Vec3(0.0f, 0.0f, floorDivideBorderGroundOffset)));
		addChild(new Cuboid(baseSize, floorWallHeight, baseSize, osg::Vec3(0.0f, 0.0f, floorGroundOffset)));
	}

	float Floor::getFloorMinHeight()
	{
		return FLOOR_MIN_HEIGHT;
	}
}
