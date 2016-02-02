#include "Clustering/Building.h"

namespace Clustering
{
	Building::Building(const QList<Floor*>& inputFloors, const QList<Vehicle*>& inputVehicles)
	{
		floors = inputFloors;
		std::sort(floors.begin(), floors.end(), [](const Floor* a, const Floor* b)
		{
			return a->getPriority() < b->getPriority();
		});

		auto maxSizeFloor = std::max_element(floors.begin(), floors.end(), [](const Floor* a, const Floor* b)
		{
			return a->getFloorSize() < b->getFloorSize();
		});
		auto maxSize = (*maxSizeFloor)->getFloorSize();

		foreach(auto f, floors)
		{
			f->setFloorSize(maxSize);
			f->refresh();
		}

		auto floorHeight = Floor::getFloorHeight();
		osg::Vec3 pos(0, 0, 0);
		foreach(auto f, floors)
		{
			f->setPosition(pos);
			Group::addChild(f);
			pos.z() += floorHeight;
		}

		width = maxSize;
		height = pos.z();
	}

	float Building::getWidth() const
	{
		return width;
	}

	float Building::getHeight() const
	{
		return height;
	}
}
