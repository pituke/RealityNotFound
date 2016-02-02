#include "Clustering/Vehicle.h"
#include <osg/Geode>
#include <Clustering/Cuboid.h>

namespace Clustering
{
	osg::ref_ptr<osg::Geode> vehicleGeom;
	osg::BoundingBox vehicleBoundBox;
	static bool initedVehicle = false;

	void initVehicle()
	{
		vehicleGeom = new Cuboid(0.1, 0.05, 0.2, osg::Vec3(0, 0, -0.05));
		vehicleBoundBox = vehicleGeom->getBoundingBox();
	}

	Vehicle::Vehicle(const QString& displayText)
	{
		if (!initedVehicle)
		{
			initVehicle();
			initedVehicle = true;
		}

		Group::addChild(vehicleGeom);
	}

	float Vehicle::getWidth()
	{
		return vehicleBoundBox.xMax() - vehicleBoundBox.xMin();
	}

	float Vehicle::getDepth()
	{
		return  vehicleBoundBox.yMax() - vehicleBoundBox.yMin();
	}
}
