#include "Clustering/Building.h"

namespace Clustering
{
	static const float VEHICLE_SPACING = 0.1;

	Building::Building(const QList<Floor*>& inputFloors, const QList<Vehicle*>& inputVehicles)
	{
		// floors
		QList<Floor*> floors = inputFloors;
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

		floorsBaseSize = maxSize;
		floorsHeight = pos.z();

		// vehicles
		osg::Vec3 nearPosition(-FLT_MAX, -floorsBaseSize / 2, 0);
		float nearLimit = -floorsBaseSize / 2;
		bool nearFirstAlongWall = true, nearFirstUprightWall = true;
		osg::Vec3 leftPosition(-floorsBaseSize / 2, -FLT_MAX, 0);
		float leftLimit = -floorsBaseSize / 2;
		bool leftFirstAlongWall = true, leftFirstUprightWall = true;
		bool nextLine = true;
		auto inputVehicleIterator = inputVehicles.begin();
		while (inputVehicleIterator != inputVehicles.end())
		{
			auto v = *inputVehicleIterator;
			if (nextLine)
			{
				nearPosition.x() = floorsBaseSize / 2;
				nearPosition.y() -= (nearFirstUprightWall ? v->getDepth() / 2 : v->getDepth()) + VEHICLE_SPACING;
				nearFirstUprightWall = false;
				nearFirstAlongWall = true;
				leftPosition.y() = floorsBaseSize / 2;
				leftPosition.x() -= (leftFirstUprightWall ? v->getDepth() / 2 : v->getDepth()) + VEHICLE_SPACING;
				leftFirstUprightWall = false;
				leftFirstAlongWall = true;
				nextLine = false;
			}
			else
			{
				if (nearPosition.x() >= leftPosition.y())
				{
					if (nearPosition.x() - v->getWidth() - VEHICLE_SPACING < nearLimit)
					{
						nextLine = true;
						continue;
					}
					nearPosition.x() -= (nearFirstAlongWall ? v->getWidth() / 2 : v->getWidth()) + VEHICLE_SPACING;
					v->setPosition(nearPosition);
					nearFirstAlongWall = false;
				}
				else
				{
					if (leftPosition.y() - v->getWidth() - VEHICLE_SPACING < leftLimit)
					{
						nextLine = true;
						continue;
					}
					leftPosition.y() -= (leftFirstAlongWall ? v->getWidth() / 2 : v->getWidth()) + VEHICLE_SPACING;
					v->setPosition(leftPosition);
					v->setAttitude(osg::Quat(osg::PI_2, osg::Vec3(0, 0, 1)));
					leftFirstAlongWall = false;
				}
				Group::addChild(v);
				++inputVehicleIterator;
			}
		}
	}

	float Building::getWidth() const
	{
		return 0;
	}

	float Building::getHeight() const
	{
		return 0;
	}
}
