#pragma once

#include <osg/PositionAttitudeTransform>
#include "Building.h"
#include <QtCore/QList>

namespace Clustering
{
	class Residence : public osg::PositionAttitudeTransform
	{
	private:
		QList<Building*> attributesBuildings;
		osg::ref_ptr<osg::PositionAttitudeTransform> attributesBuildingsNode;
		QList<Building*> gettersSettersBuildings;
		osg::ref_ptr<osg::PositionAttitudeTransform> gettersSettersBuildingsNode;
		QList<Building*> internalMethodsBuildings;
		osg::ref_ptr<osg::PositionAttitudeTransform> internalMethodsBuildingsNode;
		QList<Building*> interfaceMethodsBuildings;
		osg::ref_ptr<osg::PositionAttitudeTransform> interfaceMethodsBuildingsNode;

	public:
		Residence();
		void addAttributeBuilding(Building* attrBuilding);
		void addGetterSeterBuilding(Building* getSetBuilding);
		void addInternalBuilding(Building* internalBuilding);
		void addInterfaceBuilding(Building* interfaceBuilding);
		void refresh();
	};
}
