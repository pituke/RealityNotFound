#pragma once

#include <osg/PositionAttitudeTransform>
#include <QString>

namespace Clustering
{
	class Vehicle : public osg::PositionAttitudeTransform
	{
	public:
		Vehicle(const QString& displayText);
		float getWidth() const;
		float getHeight() const;
		float getDepth() const;
	};
}
