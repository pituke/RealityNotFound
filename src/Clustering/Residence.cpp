#include "Clustering/Residence.h"
#include <Layout/LayoutAlgorithms.h>
#include <Clustering/Cuboid.h>

namespace Clustering
{
	static const float RESIDENCE_SECTOR_HEIGHT = 0.2;
	static const float BUILDING_SPACING = 0.5;

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
			attributesBuildingsNode->addChild(new Cuboid(attrRegion.xMax() - attrRegion.xMin(), RESIDENCE_SECTOR_HEIGHT, attrRegion.yMax() - attrRegion.yMin(), osg::Vec3(0, 0, RESIDENCE_SECTOR_HEIGHT / 2)));
		}

		osg::BoundingBox getSetRegion;
		if (!gettersSettersBuildings.empty())
		{
			QList<Layout::ElementLayout> getSetLayouts;
			Layout::LayoutAlgorithms::layoutAroundRegion(gettersSettersBuildings.first()->getBoundingBox(), gettersSettersBuildings.count(), attrRegion, BUILDING_SPACING, &getSetLayouts, &getSetRegion);
			for (uint i = 0; i < gettersSettersBuildings.count(); ++i)
			{
				auto& b = gettersSettersBuildings[i];
				b->setPosition(getSetLayouts[i].position);
				b->setAttitude(osg::Quat(getSetLayouts[i].yawRotation, osg::Vec3(0.0f, 0.0f, 1.0f)));
				b->refresh();
				gettersSettersBuildingsNode->addChild(b);
			}
			gettersSettersBuildingsNode->addChild(new Cuboid(getSetRegion.xMax() - getSetRegion.xMin(), RESIDENCE_SECTOR_HEIGHT, getSetRegion.yMax() - getSetRegion.yMin(), osg::Vec3(0, 0, RESIDENCE_SECTOR_HEIGHT / 2)));
		}

		osg::BoundingBox internalRegion;
		if (!internalMethodsBuildings.empty())
		{
			QList<Layout::ElementLayout> internalLayouts;
			Layout::LayoutAlgorithms::layoutAroundRegion(internalMethodsBuildings.first()->getBoundingBox(), internalMethodsBuildings.count(), getSetRegion, BUILDING_SPACING, &internalLayouts, &internalRegion);
			for (uint i = 0; i < internalMethodsBuildings.count(); ++i)
			{
				auto& b = internalMethodsBuildings[i];
				b->setPosition(internalLayouts[i].position);
				b->setAttitude(osg::Quat(internalLayouts[i].yawRotation, osg::Vec3(0.0f, 0.0f, 1.0f)));
				b->refresh();
				internalMethodsBuildingsNode->addChild(b);
			}
			internalMethodsBuildingsNode->addChild(new Cuboid(internalRegion.xMax() - internalRegion.xMin(), RESIDENCE_SECTOR_HEIGHT, internalRegion.yMax() - internalRegion.yMin(), osg::Vec3(0, 0, RESIDENCE_SECTOR_HEIGHT / 2)));
		}

		osg::BoundingBox interfaceRegion;
		if (!interfaceMethodsBuildings.empty())
		{
			QList<Layout::ElementLayout> interfaceLayouts;
			Layout::LayoutAlgorithms::layoutAroundRegion(interfaceMethodsBuildings.first()->getBoundingBox(), interfaceMethodsBuildings.count(), internalRegion, BUILDING_SPACING, &interfaceLayouts, &interfaceRegion);
			for (uint i = 0; i < interfaceMethodsBuildings.count(); ++i)
			{
				auto& b = interfaceMethodsBuildings[i];
				b->setPosition(interfaceLayouts[i].position);
				b->setAttitude(osg::Quat(interfaceLayouts[i].yawRotation, osg::Vec3(0.0f, 0.0f, 1.0f)));
				b->refresh();
				interfaceMethodsBuildingsNode->addChild(b);
			}
			interfaceMethodsBuildingsNode->addChild(new Cuboid(interfaceRegion.xMax() - interfaceRegion.xMin(), RESIDENCE_SECTOR_HEIGHT, interfaceRegion.yMax() - interfaceRegion.yMin(), osg::Vec3(0, 0, RESIDENCE_SECTOR_HEIGHT / 2)));
		}

		float residenceSectorOffset = 0;
		if (!interfaceMethodsBuildings.empty())
		{
			interfaceMethodsBuildingsNode->setPosition(osg::Vec3(0, 0, residenceSectorOffset));
			residenceSectorOffset += RESIDENCE_SECTOR_HEIGHT;
		}
		if (!internalMethodsBuildings.empty())
		{
			internalMethodsBuildingsNode->setPosition(osg::Vec3(0, 0, residenceSectorOffset));
			residenceSectorOffset += RESIDENCE_SECTOR_HEIGHT;
		}
		if (!gettersSettersBuildings.empty())
		{
			gettersSettersBuildingsNode->setPosition(osg::Vec3(0, 0, residenceSectorOffset));
			residenceSectorOffset += RESIDENCE_SECTOR_HEIGHT;
		}
		if (!attributesBuildings.empty())
		{
			attributesBuildingsNode->setPosition(osg::Vec3(0, 0, residenceSectorOffset));
			residenceSectorOffset += RESIDENCE_SECTOR_HEIGHT;
		}
	}
}
