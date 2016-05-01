#include "Clustering/Building.h"
#include <Clustering/QuadPyramide.h>
#include <Util/ApplicationConfig.h>
#include <Data/OsgNode.h>

namespace Clustering
{
	static float DEFAULT_BUILDING_DEFAULT_BASE_SIZE = 1.0f;
	static float DEFAULT_BUILDING_DEFAULT_ROOF_HEIGHT = 0.3f;
	static float DEFAULT_BUILDING_CAPTION_CHARACTER_SIZE = 3.0f;
	static float DEFAULT_BUILDING_CAPTION_OFFSET = 0.3f;

	Building::Building(const QString& name, const QList<Floor*>& inputFloors)
	{
		const float BUILDING_DEFAULT_BASE_SIZE = Util::ApplicationConfig::get()->getFloatValue("City.Building.DefaultBaseSize", DEFAULT_BUILDING_DEFAULT_BASE_SIZE);

		this->name = name;
		for (auto f : inputFloors)
			floors << f;
		this->triangleRoof = false;
		this->lieOnGround = true;
		if (floors.empty())
		{
			auto f = new Floor();
			f->setDivideBorder(false);
			floors << f;
		}
		minBuildingHeight = Floor::getFloorMinHeight() * floors.count();
		buildingHeight = minBuildingHeight;
		for (const auto& f : floors)
			f->setBaseSize(BUILDING_DEFAULT_BASE_SIZE);
	}

	void Building::setHeight(float height)
	{
		buildingHeight = 0.0f;
		const float floorHeight = height / floors.count();
		for (auto& f : floors)
		{
			f->setFloorHeight(floorHeight);
			buildingHeight += f->getFloorHeight();
		}
	}

	float Building::getHeight(bool includeRoof) const
	{
		const float BUILDING_DEFAULT_ROOF_HEIGHT = Util::ApplicationConfig::get()->getFloatValue("City.Building.DefaultRoofHeight", DEFAULT_BUILDING_DEFAULT_ROOF_HEIGHT);

		return buildingHeight + (includeRoof && triangleRoof ? BUILDING_DEFAULT_ROOF_HEIGHT : 0.0f);
	}

	void Building::setBaseSize(float size)
	{
		for (auto& f : floors)
			f->setBaseSize(size);
	}

	float Building::getBaseSize() const
	{
		return floors.first()->getBaseSize();
	}

	void Building::setTriangleRoof(bool state)
	{
		triangleRoof = state;
	}

	void Building::setLieOnGround(bool state)
	{
		lieOnGround = state;
	}

	osg::BoundingBox Building::getBoundingBox() const
	{
		const float BUILDING_BASE_SIZE_HALF = getBaseSize() / 2;
		return osg::BoundingBox(-BUILDING_BASE_SIZE_HALF, -BUILDING_BASE_SIZE_HALF, 0, BUILDING_BASE_SIZE_HALF, BUILDING_BASE_SIZE_HALF, getHeight());
	}

	void Building::refresh()
	{
		auto config = Util::ApplicationConfig::get();
		const float BUILDING_DEFAULT_ROOF_HEIGHT = config->getFloatValue("City.Building.DefaultRoofHeight", DEFAULT_BUILDING_DEFAULT_ROOF_HEIGHT);
		const float BUILDING_CAPTION_CHARACTER_SIZE = config->getFloatValue("City.Building.CaptionCharacterSize", DEFAULT_BUILDING_CAPTION_CHARACTER_SIZE);
		const float BUILDING_CAPTION_OFFSET = config->getFloatValue("City.Building.CaptionOffset", DEFAULT_BUILDING_CAPTION_OFFSET);

		removeChildren(0, getNumChildren());

		osg::Vec3 pos(0.0f, 0.0f, lieOnGround ? 0.0f : -getHeight() / 2.0f);
		for (auto& f : floors)
		{
			f->setPosition(pos);
			pos.z() += f->getFloorHeight();
			f->refresh();
			addChild(f);
		}
		if (triangleRoof)
		{
			auto roof = new osg::PositionAttitudeTransform();
			roof->setPosition(pos);
			roof->addChild(new QuadPyramide(getBaseSize(), getBaseSize(), BUILDING_DEFAULT_ROOF_HEIGHT));
			addChild(roof);
		}

		if (!name.isEmpty())
		{
			auto geode = new osg::Geode();
			auto text = new osgText::FadeText();
			text->setText(name.toStdString());
			text->setCharacterSize(BUILDING_CAPTION_CHARACTER_SIZE);
			text->setAutoRotateToScreen(true);
			geode->addDrawable(text);
			addChild(geode);
			text->setPosition(osg::Vec3(0.0f, 0.0f, getHeight(true) + BUILDING_CAPTION_OFFSET));
		}
	}

	float Building::getMinHeight() const
	{
		return minBuildingHeight;
	}
}
