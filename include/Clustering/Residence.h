#pragma once

#include <osg/PositionAttitudeTransform>
#include "Building.h"
#include <QtCore/QList>

namespace Clustering
{
	class Residence : public osg::PositionAttitudeTransform
	{
	private:
		QList<Building> attributesBuildings;
		QList<Building> gettersSettersBuildings;
		QList<Building> internalMethodsBuildings;
		QList<Building> interfaceMethodsBuildings;

	public:
		Residence();
	};
}
