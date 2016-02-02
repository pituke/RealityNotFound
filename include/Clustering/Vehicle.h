#pragma once

#include <osg/PositionAttitudeTransform>
#include <QString>

namespace Clustering
{
	class Vehicle : public osg::PositionAttitudeTransform
	{
	public:
		Vehicle(const QString& displayText);
		static float getWidth();
		static float getDepth();
	};
}
