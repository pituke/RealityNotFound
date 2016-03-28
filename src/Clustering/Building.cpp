#include "Clustering/Building.h"
#include <Clustering/QuadPyramide.h>

namespace Clustering
{
	static const float BUILDING_DEFAULT_BASE_SIZE = 1.0f;
	static const float BUILDING_DEFAULT_ROOF_HEIGHT = 0.4f;

	Building::Building(const QList<Floor*>& inputFloors)
	{
		floors = inputFloors;
		this->triangleRoof = false;
		if (floors.empty())
			floors << new Floor(false);
		buildingHeight = Floor::getFloorMinHeight() * floors.count();
		for (const auto& f : floors)
			f->setBaseSize(BUILDING_DEFAULT_BASE_SIZE);
	}

	void Building::setHeight(float height)
	{
		buildingHeight = 0.0f;
		const float floorHeight = height / floors.count();
		for (auto& f : floors)
		{
			f->setFloorHeight(floorHeight);
			buildingHeight += f->getFloorHeight();
		}
	}

	float Building::getHeight() const
	{
		return buildingHeight;
	}

	void Building::setBaseSize(float size)
	{
		for (auto& f : floors)
			f->setBaseSize(size);
	}

	float Building::getBaseSize() const
	{
		return floors.first()->getBaseSize();
	}

	void Building::setTriangleRoof(bool state)
	{
		triangleRoof = state;
	}

	osg::BoundingBox Building::getBoundingBox() const
	{
		static const float BUILDING_BASE_SIZE_HALF = getBaseSize() / 2;
		return osg::BoundingBox(-BUILDING_BASE_SIZE_HALF, -BUILDING_BASE_SIZE_HALF, 0, BUILDING_BASE_SIZE_HALF, BUILDING_BASE_SIZE_HALF, getHeight());
	}

	void Building::refresh()
	{
		removeChildren(0, getNumChildren());

		osg::Vec3 pos(0.0f, 0.0f, 0.0f);
		for (auto& f : floors)
		{
			f->setPosition(pos);
			pos.z() += f->getFloorHeight();
			f->refresh();
			addChild(f);
		}
		if (triangleRoof)
		{
			auto roof = new osg::PositionAttitudeTransform();
			roof->setPosition(pos);
			roof->addChild(new QuadPyramide(getBaseSize(), getBaseSize(), BUILDING_DEFAULT_ROOF_HEIGHT));
			addChild(roof);
		}
	}

	float Building::getMinHeight()
	{
		return Floor::getFloorMinHeight();
	}
}
