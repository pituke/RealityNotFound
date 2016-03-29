#pragma once

#include <osg/PositionAttitudeTransform>
#include "Floor.h"
#include <QList>

namespace Clustering
{
	class Building : public osg::PositionAttitudeTransform
	{
	private:
		QList<osg::ref_ptr<Floor>> floors;
		float minBuildingHeight;
		float buildingHeight;
		bool triangleRoof;

	public:
		Building(const QList<Floor*>& inputFloors = QList<Floor*>());
		void setHeight(float height);
		float getHeight() const;
		void setBaseSize(float size);
		float getBaseSize() const;
		void setTriangleRoof(bool state);
		osg::BoundingBox getBoundingBox() const;
		void refresh();
		float getMinHeight() const;
	};
}
