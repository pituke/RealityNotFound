#include "Layout/LayoutAlgorithms.h"
#include <QVector>

namespace Layout
{
	void LayoutAlgorithms::layoutInsideRegion(const osg::BoundingBox& elementDimension, uint elementCount, float groundTopPosition, float spacing, QList<osg::Vec3>* layouts)
	{
		layouts->clear();
		const float width = elementDimension.xMax() - elementDimension.xMin();
		const float depth = elementDimension.yMax() - elementDimension.yMin();
		const float offsetX = width + spacing;
		const float offsetY = depth + spacing;
		const float elementConstOffsetX = -elementDimension.xMin();
		const float elementConstOffsetY = -elementDimension.yMin();
		const float elementConstOffsetZ = -elementDimension.zMin() + groundTopPosition;
		uint limit = 1;
		uint xNum = 0;
		uint yNum = 0;
		float xMax = -FLT_MAX;
		float yMax = -FLT_MAX;
		for (uint i = 0; i < elementCount; ++i)
		{
			osg::Vec3 pos(xNum * offsetX + elementConstOffsetX, yNum * offsetY + elementConstOffsetY, elementConstOffsetZ);
			if (xMax < pos.x()) xMax = pos.x();
			if (yMax < pos.y()) yMax = pos.y();
			*layouts << pos;
			if (xNum == limit)
			{
				if (yNum == limit)
				{
					limit++;
					yNum = 0;
					xNum = limit;
				}
				else
				{
					yNum++;
					xNum = (yNum == limit) ? 0 : limit;
				}
			}
			else
				xNum++;
		}
		xMax += width / 2;
		yMax += depth / 2;
		const float centerCorrectionOffsetX = -xMax / 2;
		const float centerCorrectionOffsetY = -yMax / 2;
		for (auto& l : *layouts)
		{
			l.x() += centerCorrectionOffsetX;
			l.y() += centerCorrectionOffsetY;
		}
	}
}
