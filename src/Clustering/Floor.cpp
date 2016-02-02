#include "Clustering/Floor.h"
#include <osg/Geode>
#include <osgDB/ReadFile>
#include <QVector>
#include <QtCore/QMap>

namespace Clustering
{
	// distances from specific border of floor mesh which define a region for merge vertices to moving belong the side when the floor is resizing
	static const float FLOOR_LEFT_SIDE_VERTICES_MERGE_DISTANCE = 0.1;
	static const float FLOOR_LEFT_SIDE_VERTICES_MERGE_DISTANCE_CORNER = 0.4;
	static const float FLOOR_RIGHT_SIDE_VERTICES_MERGE_DISTANCE = 0.1;
	static const float FLOOR_BOTTOM_SIDE_VERTICES_MERGE_DISTANCE = 0.1;
	static const float FLOOR_TOP_SIDE_VERTICES_MERGE_DISTANCE = 0.1;
	static const float FLOOR_FAR_SIDE_VERTICES_MERGE_DISTANCE = 0.1;
	static const float FLOOR_FAR_SIDE_VERTICES_MERGE_DISTANCE_CORNER = 0.4;
	static const float FLOOR_NEAR_SIDE_VERTICES_MERGE_DISTANCE = 0.1;

	static const float WINDOW_TOP_MARGIN = 0.05f; // space between window and top edge of floor
	static const float WINDOW_BOTTOM_MARGIN = 0.05f; // space between window and bottom edge of floor
	static const float WINDOW_SPACING = 0.2f; // constant space between windows belong to floor wall

	struct GeometrySet
	{
		QVector<osg::Vec3> vertices;
		osg::ref_ptr<osg::Array> normals;
		osg::Geometry::PrimitiveSetList primitives;
		QList<uint> viLeft, viRight, viBottom, viTop, viFar, viNear;
		osg::BoundingBox boundBox;
	};

	static GeometrySet floorNoncorner, floorCorner;
	static float floorHeight, wallHeight, windowHeight, wallVertCenter, cornerSize, borderSize;
	static QMap<QString, osg::ref_ptr<osg::Geometry>> floorGeometries;
	static bool initedFloor = false;

	osg::BoundingBox findBoundBox(const QVector<osg::Vec3>& vertices, const QList<uint>& vIndexes)
	{
		osg::BoundingBox b(FLT_MAX, FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX);
		foreach(auto i, vIndexes)
		{
			const auto& v = vertices[i];
			if (v.x() < b.xMin()) b.xMin() = v.x();
			if (v.x() > b.xMax()) b.xMax() = v.x();
			if (v.y() < b.yMin()) b.yMin() = v.y();
			if (v.y() > b.yMax()) b.yMax() = v.y();
			if (v.z() < b.zMin()) b.zMin() = v.z();
			if (v.z() > b.zMax()) b.zMax() = v.z();
		}
		return b;
	}

	void fillGeometrySet(const QString& meshFilepath, GeometrySet& gs)
	{
		osg::ref_ptr<osg::Geode> geode = osgDB::readNodeFile(meshFilepath.toStdString())->asGroup()->getChild(0)->asGeode();

		const bool isCornerFloor = &gs == &floorCorner;

		auto geom = geode->getDrawable(0)->asGeometry();

		auto va = geom->getVertexArray();
		gs.vertices.resize(va->getNumElements());
		const auto& vs = static_cast<const osg::Vec3*>(va->getDataPointer());
		for (uint i = 0; i < va->getNumElements(); ++i)
			gs.vertices[i] = vs[i];
		gs.normals = geom->getNormalArray();
		gs.primitives = geom->getPrimitiveSetList();
		gs.boundBox = geode->getBoundingBox();

		// osg use different coordinate system --> +x to right, +y into screen, +z to up
		const float leftTreshold = gs.boundBox.xMin() + (isCornerFloor ? FLOOR_LEFT_SIDE_VERTICES_MERGE_DISTANCE_CORNER : FLOOR_LEFT_SIDE_VERTICES_MERGE_DISTANCE);
		const float rightTreshold = gs.boundBox.xMax() - FLOOR_RIGHT_SIDE_VERTICES_MERGE_DISTANCE;
		const float bottomTreshold = gs.boundBox.zMin() + FLOOR_BOTTOM_SIDE_VERTICES_MERGE_DISTANCE;
		const float topTreshold = gs.boundBox.zMax() - FLOOR_TOP_SIDE_VERTICES_MERGE_DISTANCE;
		const float farTreshold = gs.boundBox.yMin() + (isCornerFloor ? FLOOR_FAR_SIDE_VERTICES_MERGE_DISTANCE_CORNER : FLOOR_FAR_SIDE_VERTICES_MERGE_DISTANCE);
		const float nearTreshold = gs.boundBox.yMax() - FLOOR_NEAR_SIDE_VERTICES_MERGE_DISTANCE;
		for (uint vi = 0; vi < gs.vertices.count(); ++vi)
		{
			const auto& v = gs.vertices[vi];
			if (v.x() < leftTreshold) gs.viLeft << vi;
			if (v.x() > rightTreshold) gs.viRight << vi;
			if (v.z() < bottomTreshold) gs.viBottom << vi;
			if (v.z() > topTreshold) gs.viTop << vi;
			if (v.y() < farTreshold) gs.viFar << vi;
			if (v.y() > nearTreshold) gs.viNear << vi;
		}
	}

	void initFloor()
	{
		fillGeometrySet("..\\..\\resources\\mesh\\floor_void.obj", floorNoncorner);
		fillGeometrySet("..\\..\\resources\\mesh\\floor_out.obj", floorCorner);

		floorHeight = floorCorner.boundBox.zMax() - floorCorner.boundBox.zMin();
		auto bottomBoundBox = findBoundBox(floorCorner.vertices, floorCorner.viBottom);
		wallHeight = floorCorner.boundBox.zMax() - bottomBoundBox.zMax();
		wallVertCenter = bottomBoundBox.zMax() + wallHeight / 2;
		auto rightBoundBox = findBoundBox(floorCorner.vertices, floorCorner.viRight);
		borderSize = rightBoundBox.xMax() - rightBoundBox.xMin();
		auto leftBoundBox = findBoundBox(floorCorner.vertices, floorCorner.viLeft);
		cornerSize = leftBoundBox.xMax() - leftBoundBox.xMin() - borderSize;
		windowHeight = wallHeight - WINDOW_TOP_MARGIN - WINDOW_BOTTOM_MARGIN;
	}

	osg::Geometry* generateFloorGeometry(float wallSize, bool corner)
	{
		const auto& geomSet = corner ? floorCorner : floorNoncorner;
		const float totalFloorSize = wallSize + 2 * borderSize + (corner ? cornerSize : 0);
		const float position = totalFloorSize / 2;
		auto g = new osg::Geometry();
		auto vs = geomSet.vertices;
		const float leftOffset = position + geomSet.boundBox.xMin();
		const float rightOffset = position - geomSet.boundBox.xMax();
		const float farOffset = position + geomSet.boundBox.yMin();
		const float nearOffset = position - geomSet.boundBox.yMax();
		foreach(auto vi, geomSet.viLeft)
			vs[vi].x() -= leftOffset;
		foreach(auto vi, geomSet.viRight)
			vs[vi].x() += rightOffset;
		foreach(auto vi, geomSet.viFar)
			vs[vi].y() -= farOffset;
		foreach(auto vi, geomSet.viNear)
			vs[vi].y() += nearOffset;
		g->setVertexArray(new osg::Vec3Array(vs.count(), vs.data()));
		g->setNormalArray(geomSet.normals);
		g->setPrimitiveSetList(geomSet.primitives);
		return g;
	}

	osg::ref_ptr<osg::Geometry> getFloorGeometry(float wallSize, bool corner)
	{
		const QString key = (corner ? 'c' : 'n') + QString::number(wallSize, 'f', 3);
		osg::ref_ptr<osg::Geometry> g;
		g = floorGeometries.value(key);
		if (!g.valid())
		{
			g = generateFloorGeometry(wallSize, corner);
			floorGeometries.insert(key, g);
		}
		return g;
	}

	Floor::Floor(const QString& displayText, const QList<Window*>& windows, const QString& cornerText, uint priority)
	{
		if (!initedFloor)
		{
			initFloor();
			initedFloor = true;
		}

		this->displayText = displayText;
		this->cornerText = cornerText;
		this->priority = priority;

		// calculate floor size, order windows among wall1 and wall2
		float width1 = 0, width2 = 0;
		foreach(auto w, windows)
		{
			w->setHeight(windowHeight);
			if (width1 <= width2)
			{
				width1 += WINDOW_SPACING + w->getWidth();
				floor1 << w;
			}
			else
			{
				width2 += WINDOW_SPACING + w->getWidth();
				floor2 << w;
			}
		}
		if (!floor1.empty()) width1 += WINDOW_SPACING;
		if (!floor2.empty()) width2 += WINDOW_SPACING;

		// final wall size (temporary) is logner one
		// final size will set by setSize and generate
		auto reqWallSize = std::max(width1, width2);
		reqFloorSize = reqWallSize + 2 * borderSize + (cornerText.isEmpty() ? 0 : cornerSize);
	}

	void Floor::setFloorSize(float floorSize)
	{
		this->reqFloorSize = floorSize;
	}

	float Floor::getFloorSize() const
	{
		return reqFloorSize	;
	}

	uint Floor::getPriority() const
	{
		return priority;
	}

	void Floor::refresh()
	{
		removeChildren(0, getNumChildren());

		auto geode = new osg::Geode();
		auto reqWallSize = reqFloorSize - 2 * borderSize - (cornerText.isEmpty() ? 0 : cornerSize);
		geode->addDrawable(getFloorGeometry(reqWallSize, !cornerText.isEmpty()));
		const auto bb = geode->getBoundingBox();
		const auto totalSize = bb.xMax() - bb.xMin();
		const auto wallHorPosition = totalSize / 2 - borderSize;
		
		// calculate and set windows position to wall1 relative to floor
		osg::Vec3 f1StartWindowPosition(-wallHorPosition + (cornerText.isEmpty() ? 0.0f : cornerSize), -wallHorPosition, wallVertCenter);
		for (uint i = 0; i < floor1.count(); ++i)
		{
			auto& w = floor1[i];
			f1StartWindowPosition.x() += WINDOW_SPACING + (i == 0 ? w->getWidth() / 2 : w->getWidth());
			w->setPosition(f1StartWindowPosition);
			addChild(w);
		}

		// calculate and set windows position to wall2 relative to floor
		osg::Vec3 f2StartWindowPosition(-wallHorPosition, -wallHorPosition + (cornerText.isEmpty() ? 0.0f : cornerSize), wallVertCenter);
		for (uint i = 0; i < floor2.count(); ++i)
		{
			auto& w = floor2[i];
			f2StartWindowPosition.y() += WINDOW_SPACING + (i == 0 ? w->getWidth() / 2 : w->getWidth());
			w->setPosition(f2StartWindowPosition);
			w->setAttitude(osg::Quat(osg::PI_2, osg::Vec3(0, 0, 1)));
			addChild(w);
		}
		
		addChild(geode);
	}

	float Floor::getFloorHeight()
	{
		return floorHeight;
	}
}
