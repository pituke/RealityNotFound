#include "Clustering/Cuboid.h"
#include <osg/Vec3>
#include <osg/Geometry>

namespace Clustering
{
	Cuboid::Cuboid(float width, float height, float depth)
	{
		const float right = width / 2;
		const float left = -right;
		const float top = height / 2;
		const float bottom = -top;
		const float near = depth / 2;
		const float far = -near;

		const osg::Vec3 vs[] =
		{
			osg::Vec3(left, bottom, near),	// 0
			osg::Vec3(right, bottom, near),	// 1
			osg::Vec3(right, top, near),	// 2
			osg::Vec3(left, top, near),		// 3
			osg::Vec3(left, bottom, far),	// 4
			osg::Vec3(right, bottom, far),	// 5
			osg::Vec3(right, top, far),		// 6
			osg::Vec3(left, top, far)		// 7
		};

		const GLuint fs[] =
		{
			0, 1, 2, 3,	// near
			1, 5, 6, 2,	// right
			5, 4, 7, 6,	// far
			4, 0, 3, 7,	// left
			3, 2, 6, 7,	// top
			4, 5, 1, 0	// bottom
		};

		auto geom = new osg::Geometry;
		geom->setVertexArray(new osg::Vec3Array(sizeof(vs) / sizeof(osg::Vec3), vs));
		geom->addPrimitiveSet(new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, sizeof(fs) / sizeof(GLuint), fs));
		Geode::addDrawable(geom);
	}
}
