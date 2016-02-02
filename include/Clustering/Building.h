#pragma once

#include <osg/PositionAttitudeTransform>
#include <QList>
#include "Floor.h"
#include "Vehicle.h"

namespace Clustering
{
	class Building : public osg::PositionAttitudeTransform
	{
	private:
		QList<Floor*> floors;
		float width, height;

	public:
		Building(const QList<Floor*>& inputFloors, const QList<Vehicle*>& inputVehicles);
		float getWidth() const;
		float getHeight() const;
	};
}
