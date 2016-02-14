#include "Clustering/Cuboid.h"
#include <osg/Vec3>
#include <osg/Geometry>
#include <QVector>

namespace Clustering
{
	Cuboid::Cuboid(float width, float height, float depth, const osg::Vec3& offset)
	{
		const float right = (width / 2) + offset.x();
		const float left = -(width / 2) + offset.x();
		const float top = (height / 2) + offset.z();
		const float bottom = -(height / 2) + offset.z();
		const float farr = (depth / 2) + offset.y();
		const float nearr = -(depth / 2) + offset.y();

		const osg::Vec3 vs[] =
		{
			osg::Vec3(left, nearr, bottom),		// 0
			osg::Vec3(right, nearr, bottom),	// 1
			osg::Vec3(right, nearr, top),		// 2
			osg::Vec3(left, nearr, top),		// 3
			osg::Vec3(left, farr, bottom),		// 4
			osg::Vec3(right, farr, bottom),		// 5
			osg::Vec3(right, farr, top),		// 6
			osg::Vec3(left, farr, top)			// 7
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

		QVector<osg::Vec3> vertices;
		QVector<osg::Vec3> normals;
		QVector<GLuint> quads;
		for (GLuint i = 0; i < sizeof(fs)/sizeof(GLuint); ++i)
		{
			vertices << vs[fs[i]];
			normals << ns[i / 4];
			quads << i;
		}

		auto geom = new osg::Geometry;
		geom->setVertexArray(new osg::Vec3Array(vertices.count(), vertices.data()));
		geom->setNormalArray(new osg::Vec3Array(normals.count(), normals.data()));
		geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
		for (GLuint i = 0; i < quads.count(); i += 4)
			geom->addPrimitiveSet(new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 4, quads.data() + i));
		Geode::addDrawable(geom);
	}
}
