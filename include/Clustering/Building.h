#pragma once

#include <osg/PositionAttitudeTransform>
#include <QtCore/QList>
#include "Floor.h"

namespace Clustering
{
	class Building : public osg::PositionAttitudeTransform
	{
	private:
		QList<Floor*> floors;

	public:
		Building(const QList<Floor*>& inputFloors);
	};
}
