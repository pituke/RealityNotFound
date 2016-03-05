#include "Clustering/Building.h"

namespace Clustering
{
	static const float BUILDING_BASE_SIZE = 1.0f;

	Building::Building(const QList<Floor*>& inputFloors)
	{
		floors = inputFloors;
		if (floors.empty())
			floors << new Floor();
		buildingHeight = Floor::getFloorMinHeight() * floors.count();
		for (const auto& f : floors)
		{
			f->setBaseSize(BUILDING_BASE_SIZE);
			Group::addChild(f);
		}
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

	void Building::refresh()
	{
		osg::Vec3 pos(0.0f, 0.0f, 0.0f);
		for (auto& f : floors)
		{
			f->refresh();
			f->setPosition(pos);
			pos.z() += f->getFloorHeight();
		}
	}

	float Building::getMinHeight()
	{
		return Floor::getFloorMinHeight();
	}
}
