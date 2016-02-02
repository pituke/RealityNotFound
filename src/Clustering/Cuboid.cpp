#include "Clustering/Cuboid.h"
#include <osg/Vec3>
#include <osg/Geometry>

namespace Clustering
{
	Cuboid::Cuboid(float width, float height, float depth, const osg::Vec3& centerOffset)
	{
		const float right = (width / 2) - centerOffset.x();
		const float left = -(width / 2) - centerOffset.x();
		const float top = (height / 2) - centerOffset.z();
		const float bottom = -(height / 2) - centerOffset.z();
		const float far = (depth / 2) - centerOffset.y();
		const float near = -(depth / 2) - centerOffset.y();

		const osg::Vec3 vs[] =
		{
			osg::Vec3(left, near, bottom),	// 0
			osg::Vec3(right, near, bottom),	// 1
			osg::Vec3(right, near, top),	// 2
			osg::Vec3(left, near, top),		// 3
			osg::Vec3(left, far, bottom),	// 4
			osg::Vec3(right, far, bottom),	// 5
			osg::Vec3(right, far, top),		// 6
			osg::Vec3(left, far, top)		// 7
		};

		const osg::Vec3 ns[] =
		{
			osg::Vec3(0, -1, 0),
			osg::Vec3(1, 0, 0),
			osg::Vec3(0, 1, 0),
			osg::Vec3(-1, 0, 0),
			osg::Vec3(0, 0, 1),
			osg::Vec3(0, 0, -1)
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
		geom->setNormalArray(new osg::Vec3Array(sizeof(ns) / sizeof(osg::Vec3), ns));
		geom->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
		for (GLuint i = 0; i < sizeof(fs)/sizeof(GLuint); i += 4)
			geom->addPrimitiveSet(new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, sizeof(GLuint) * 4, &fs[i]));
		Geode::addDrawable(geom);
	}
}
