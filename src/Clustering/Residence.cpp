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
		gettersSettersBuildingsNode = new osg::PositionAttitudeTransform();
		internalMethodsBuildingsNode = new osg::PositionAttitudeTransform();
		interfaceMethodsBuildingsNode = new osg::PositionAttitudeTransform();
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
		gettersSettersBuildingsNode->removeChildren(0, gettersSettersBuildingsNode->getNumChildren());
		internalMethodsBuildingsNode->removeChildren(0, internalMethodsBuildingsNode->getNumChildren());
		interfaceMethodsBuildingsNode->removeChildren(0, interfaceMethodsBuildingsNode->getNumChildren());

		osg::BoundingBox attrRegion;
		if (!attributesBuildings.empty())
		{
			QList<osg::Vec3> attrLayouts;
			Layout::LayoutAlgorithms::layoutInsideRegion(attributesBuildings.first()->getBoundingBox(), attributesBuildings.count(), RESIDENCE_SECTOR_HEIGHT, BUILDING_SPACING, &attrLayouts, &attrRegion);
			for (uint i = 0; i < attributesBuildings.count(); ++i)
			{
				auto& b = attributesBuildings[i];
				b->setPosition(attrLayouts[i]);
				b->refresh();
				attributesBuildingsNode->addChild(b);
			}
			attributesBuildingsNode->addChild(new Clustering::Cuboid(attrRegion.xMax() - attrRegion.xMin(), RESIDENCE_SECTOR_HEIGHT, attrRegion.yMax() - attrRegion.yMin(), osg::Vec3(0, 0, RESIDENCE_SECTOR_HEIGHT / 2)));
		}

		if (!gettersSettersBuildings.empty())
		{
			QList<Layout::ElementLayout> getSetLayouts;
			Layout::LayoutAlgorithms::layoutAroundRegion(gettersSettersBuildings.first()->getBoundingBox(), gettersSettersBuildings.count(), attrRegion, BUILDING_SPACING, &getSetLayouts);
			for (uint i = 0; i < gettersSettersBuildings.count(); ++i)
			{
				auto& b = gettersSettersBuildings[i];
				b->setPosition(getSetLayouts[i].position);
				b->setAttitude(osg::Quat(getSetLayouts[i].yawRotation, osg::Vec3(0.0f, 0.0f, 1.0f)));
				b->refresh();
				gettersSettersBuildingsNode->addChild(b);
			}
		}
	}
}
