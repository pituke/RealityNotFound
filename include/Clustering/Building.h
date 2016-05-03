#pragma once

#include <osg/PositionAttitudeTransform>
#include "Floor.h"
#include <QList>

namespace Clustering
{
	class Building : public osg::PositionAttitudeTransform
	{
	private:
		QString name;
		QList<osg::ref_ptr<Floor>> floors;
		float minBuildingHeight;
		float buildingHeight;
		bool triangleRoof;
		bool lieOnGround;
		osg::ref_ptr<osg::Geode> label;
		bool labelVisible;

	public:
		Building(const QString& name = QString(), const QList<Floor*>& inputFloors = QList<Floor*>());
		void setHeight(float height);
		float getHeight(bool includeRoof = false) const;
		void setBaseSize(float size);
		float getBaseSize() const;
		void setTriangleRoof(bool state);
		void setLieOnGround(bool state);
		void setLabel(const QString& name);
		void showLabel(bool state);
		osg::BoundingBox getBoundingBox() const;
		void refresh();
		float getMinHeight() const;
	};
}
