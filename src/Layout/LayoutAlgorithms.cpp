#include "Layout/LayoutAlgorithms.h"
#include <QVector>

namespace Layout
{
	void LayoutAlgorithms::layoutInsideRegion(const osg::BoundingBox& elementDimension, uint elementCount, float groundTopPosition, float spacing, QList<ElementLayout>* layouts)
	{
		layouts->clear();
		uint limit = 1;
		uint xNum = 0;
		uint yNum = 0;
		for (uint i = 0; i < elementCount; ++i)
		{
			// do layout
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
	}
}
