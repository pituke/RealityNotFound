#pragma once

#include <QList>
#include <osg/BoundingBox>
#include <QRectF>

namespace Layout
{
	struct ElementLayout
	{
		osg::Vec3 position;
		float yawRotation;
	};

	class LayoutAlgorithms
	{
	public:
		static void layoutInsideRegion(const osg::BoundingBox& elementDimension, uint elementCount, float groundTopPosition, float spacing, QList<osg::Vec3>* layouts, osg::BoundingBox* aroundRegion);
		static void layoutAroundRegion(const osg::BoundingBox& elementDimension, uint elementCount, const osg::BoundingBox& region, float spacing, QList<ElementLayout>* layouts);
	};
}
