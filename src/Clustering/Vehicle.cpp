#include "Clustering/Vehicle.h"
#include <osg/Geode>
#include <Clustering/Cuboid.h>

namespace Clustering
{
	static osg::ref_ptr<osg::Geode> vehicleGeom;
	static osg::BoundingBox vehicleBoundBox;
	static bool initedVehicle = false;

	void initVehicle()
	{
		vehicleGeom = new Cuboid(0.2, 0.05, 0.1, osg::Vec3(0, 0, -0.025));
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

	float Vehicle::getWidth() const
	{
		return vehicleBoundBox.xMax() - vehicleBoundBox.xMin();
	}

	float Vehicle::getHeight() const
	{
		return vehicleBoundBox.zMax() - vehicleBoundBox.zMin();
	}

	float Vehicle::getDepth() const
	{
		return  vehicleBoundBox.yMax() - vehicleBoundBox.yMin();
	}
}
