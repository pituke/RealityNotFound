#include "Clustering/Building.h"
#include <Clustering/Cuboid.h>

namespace Clustering
{
	static const float VEHICLE_SPACING = 0.1;
	static const float PARKING_HEIGHT = 0.02;
	static const float PARKING_STICK_UP = 0.05;

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
		buldingHeight = pos.z();

		// vehicles
		float borderLimit = -floorsBaseSize / 2;
		if (!inputVehicles.empty())
		{
			osg::Vec3 nearPosition(-FLT_MAX, -floorsBaseSize / 2, 0);
			bool nearFirstAlongWall = true, nearFirstUprightWall = true;
			osg::Vec3 leftPosition(-floorsBaseSize / 2, -FLT_MAX, 0);
			bool leftFirstAlongWall = true, leftFirstUprightWall = true;
			borderLimit = FLT_MAX;
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
					borderLimit = borderLimit == FLT_MAX ? -floorsBaseSize / 2 : borderLimit - (v->getDepth() + VEHICLE_SPACING);
					nextLine = false;
				}
				else
				{
					if (nearPosition.x() >= leftPosition.y())
					{
						const float nearNextPosition = nearPosition.x() - (nearFirstAlongWall ? v->getWidth() / 2 : (v->getWidth() + VEHICLE_SPACING));
						const float nearNextPositionLeft = nearNextPosition - v->getWidth() / 2;
						if (nearNextPositionLeft < borderLimit)
						{
							const float borderLimitForPotenciallyAddVehicle = borderLimit - VEHICLE_SPACING - v->getDepth();
							if (nearNextPositionLeft >= borderLimitForPotenciallyAddVehicle)
							{
								nearPosition.x() = nearNextPosition;
								v->setPosition(nearPosition);
								Group::addChild(v);
								++inputVehicleIterator;
							}
							nextLine = true;
							continue;
						}
						nearPosition.x() = nearNextPosition;
						v->setPosition(nearPosition);
						nearFirstAlongWall = false;
					}
					else
					{
						const float leftNextPosition = leftPosition.y() - (leftFirstAlongWall ? v->getWidth() / 2 : (v->getWidth() + VEHICLE_SPACING));
						const float leftNextPositionLeft = leftNextPosition - v->getWidth() / 2;
						if (leftNextPositionLeft < borderLimit)
						{
							const float borderLimitForPotenciallyAddVehicle = borderLimit - VEHICLE_SPACING - v->getDepth();
							if (leftNextPositionLeft >= borderLimitForPotenciallyAddVehicle)
							{
								leftPosition.y() = leftNextPosition;
								v->setPosition(leftPosition);
								v->setAttitude(osg::Quat(osg::PI_2, osg::Vec3(0, 0, 1)));
								Group::addChild(v);
								++inputVehicleIterator;
							}
							nextLine = true;
							continue;
						}
						leftPosition.y() = leftNextPosition;
						v->setPosition(leftPosition);
						v->setAttitude(osg::Quat(osg::PI_2, osg::Vec3(0, 0, 1)));
						leftFirstAlongWall = false;
					}
					Group::addChild(v);
					++inputVehicleIterator;
				}
			}
		}

		buildingBaseSize = -borderLimit * 2;

		// parking place
		const float parkingLeftBorder = borderLimit - (inputVehicles.empty() ? 0 : VEHICLE_SPACING + inputVehicles.first()->getDepth()) - PARKING_STICK_UP;
		const float parkingRightBorder = floorsBaseSize / 2 + PARKING_STICK_UP;
		const float parkingNearBorder = borderLimit - (inputVehicles.empty() ? 0 : VEHICLE_SPACING + inputVehicles.first()->getDepth()) - PARKING_STICK_UP;
		const float parkingFarBorder = floorsBaseSize / 2 + PARKING_STICK_UP;
		const osg::Vec3 pivotOffset((parkingLeftBorder + parkingRightBorder) / 2, (parkingNearBorder + parkingFarBorder) / 2, PARKING_HEIGHT / 2);
		auto parkingGeode = new Cuboid(parkingRightBorder - parkingLeftBorder, PARKING_HEIGHT, parkingFarBorder - parkingNearBorder);
		// do not add as child, it is not desirable do the next processing

		// move all building element nodes to reach pivot at the center
		for (uint i = 0; i < getNumChildren(); ++i)
		{
			auto pat = static_cast<osg::PositionAttitudeTransform*>(getChild(i));
			auto pos = pat->getPosition();
			pos -= pivotOffset;
			pat->setPosition(pos);
		}

		Group::addChild(parkingGeode);
	}

	float Building::getWidth() const
	{
		return buildingBaseSize;
	}

	float Building::getHeight() const
	{
		return buldingHeight;
	}
}
