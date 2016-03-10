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
		addChild(gettersSettersBuildingsNode);
		addChild(internalMethodsBuildingsNode);
		addChild(interfaceMethodsBuildingsNode);
	}

	void Residence::addAttributeBuilding(Building* attrBuilding)
	{
		attributesBuildings << attrBuilding;
	}

	void Residence::addGetterSeterBuilding(Building* getSetBuilding)
	{
		gettersSettersBuildings << getSetBuilding;
	}

	void Residence::addInternalBuilding(Building* internalBuilding)
	{
		internalMethodsBuildings << internalBuilding;
	}

	void Residence::addInterfaceBuilding(Building* interfaceBuilding)
	{
		interfaceMethodsBuildings << interfaceBuilding;
	}

	void Residence::refresh()
	{
		attributesBuildingsNode->removeChildren(0, attributesBuildingsNode->getNumChildren());

		QList<osg::Vec3> outputs;
		osg::BoundingBox region;
		Layout::LayoutAlgorithms::layoutInsideRegion(attributesBuildings.first()->getBoundingBox(), attributesBuildings.count(), RESIDENCE_SECTOR_HEIGHT, BUILDING_SPACING, &outputs, &region);
		for (uint i = 0; i < attributesBuildings.count(); ++i)
		{
			auto& b = attributesBuildings[i];
			b->setPosition(outputs[i]);
			b->refresh();
			attributesBuildingsNode->addChild(b);
		}
		attributesBuildingsNode->addChild(new Clustering::Cuboid(region.xMax() - region.xMin(), RESIDENCE_SECTOR_HEIGHT, region.yMax() - region.yMin(), osg::Vec3(0, 0, RESIDENCE_SECTOR_HEIGHT / 2)));
	}
}
