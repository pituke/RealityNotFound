#pragma once

#include <osg/PositionAttitudeTransform>
#include "Floor.h"
#include <QList>

namespace Clustering
{
	class Building : public osg::PositionAttitudeTransform
	{
	private:
		QList<Floor*> floors;
		float buildingHeight;

	public:
		Building(const QList<Floor*>& inputFloors = QList<Floor*>());
		void setHeight(float height);
		float getHeight() const;
		osg::BoundingBox getBoundingBox() const;
		void refresh();
		static float getMinHeight();
	};
}
