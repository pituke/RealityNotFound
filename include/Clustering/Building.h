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
		Building(const QList<Floor*>& inputFloors);
		void setHeight(float height);
		float getHeight() const;
		void refresh();
		static float getMinHeight();
	};
}
