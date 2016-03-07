#pragma once

#include <QList>
#include <osg/BoundingBox>

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
		static void layoutInsideRegion(const osg::BoundingBox& elementDimension, uint elementCount, float groundTopPosition, float spacing, QList<ElementLayout>* layouts);
		static void layoutAroundRegion(const QList<osg::BoundingBox>& elementsDimension, const osg::BoundingBox& region, float spacing, QList<ElementLayout>* layouts);
	};
}
