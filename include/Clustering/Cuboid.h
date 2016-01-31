#pragma once

#include <osg/Geode>

namespace Clustering
{
	class Cuboid : public osg::Geode
	{
	public:
		Cuboid(float width, float height, float depth);
	};
}
