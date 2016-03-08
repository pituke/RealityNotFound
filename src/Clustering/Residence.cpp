#include "Clustering/Residence.h"
#include <Layout/LayoutAlgorithms.h>
#include <Clustering/Cuboid.h>

namespace Clustering
{
	static const float RESIDENCE_SECTOR_HEIGHT = 0.05;
	static const float BUILDING_SPACING = 0.2;

	Residence::Residence()
	{
		attributesBuildingsNode = new osg::PositionAttitudeTransform();
		addChild(attributesBuildingsNode);
	}

	void Residence::addAttributeBuilding(Building* attrBuilding)
	{
		attributesBuildings << attrBuilding;
	}

	void Residence::refresh()
	{
		attributesBuildingsNode->removeChildren(0, attributesBuildingsNode->getNumChildren());

		for (auto& b : attributesBuildings)
		{
			b->setBaseSize(0.8);
			b->setHeight(1);
		}
		QList<osg::Vec3> outputs;
		Layout::LayoutAlgorithms::layoutInsideRegion(attributesBuildings.first()->getBoundingBox(), attributesBuildings.count(), RESIDENCE_SECTOR_HEIGHT, BUILDING_SPACING, &outputs);
		for (uint i = 0; i < attributesBuildings.count(); ++i)
		{
			auto& b = attributesBuildings[i];
			b->setPosition(outputs[i]);
			b->refresh();
			attributesBuildingsNode->addChild(b);
		}
	}
}
