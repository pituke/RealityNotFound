#include "Layout/LayoutAlgorithms.h"
#include <QVector>

namespace Layout
{
	void LayoutAlgorithms::layoutInsideRegion(const osg::BoundingBox& elementDimension, uint elementCount, float groundTopPosition, float spacing, QList<osg::Vec3>* layouts, osg::BoundingBox* aroundRegion)
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
		if (aroundRegion)
		{
			const float regionX = xMax / 2 + spacing;
			const float regionY = yMax / 2 + spacing;
			*aroundRegion = osg::BoundingBox(-regionX, -regionY, 0, regionX, regionY, 0);
		}
	}

	struct Element
	{
		osg::BoundingBox boundingBox;
		uint index;
		ElementLayout layout;
	};

	struct ElementLine
	{
		enum StickSide { NONE, LEFT, RIGHT };

		float maxSize;
		float curSize;
		bool full;
		QList<Element> elements;
		StickSide stickSide;

		ElementLine()
		{
			this->maxSize = 0;
			this->curSize = 0;
			this->full = false;
			this->stickSide = NONE;
		}

		bool tryAddElement(const Element& e, float spacing)
		{
			const float nextSize = curSize + (e.boundingBox.xMax() - e.boundingBox.xMin()) + (elements.empty() ? 0.0f : spacing);
			if (nextSize <= maxSize)
			{
				elements << e;
				curSize = nextSize;
				return true;
			}
			full = true;
			return false;
		}
	};

	struct IndentLines : public QVector<ElementLine>
	{
		IndentLines(uint count) : QVector<ElementLine>(count) {};
	};

	void LayoutAlgorithms::layoutAroundRegion(const osg::BoundingBox& elementDimension, uint elementCount, const osg::BoundingBox& region, float spacing, QList<ElementLayout>* layouts, osg::BoundingBox* aroundRegion)
	{
		static const uint EDGES_COUNT = 4;

		const uint elementsOnEdgeCount = elementCount / EDGES_COUNT;
		const uint elementsRemainCount = elementCount % EDGES_COUNT;
		const float regionWidth = region.xMax() - region.xMin();
		const float regionDepth = region.yMax() - region.yMin();
		const float elementWidth = elementDimension.xMax() - elementDimension.xMin();
		const float elementDepth = elementDimension.yMax() - elementDimension.yMin();
		const float elementHeight = elementDimension.zMax() - elementDimension.zMin();
		const float elementVerticalOffset = -(region.zMin() + elementDimension.zMin());

		QList<IndentLines> indents;
		float baseMaxWidth = 0;
		float baseMaxDepth = 0;
		IndentLines* curIndent = nullptr;
		uint elementIndex = 0;
		uint edgeIndex = 0;
		uint tryCount = 0;
		while (elementIndex < elementCount)
		{
			if (!curIndent)
			{
				baseMaxWidth = baseMaxWidth == 0 ? regionWidth : baseMaxWidth + 2 * (elementDepth + spacing);
				baseMaxDepth = baseMaxDepth == 0 ? regionWidth : baseMaxDepth + 2 * (elementDepth + spacing);
				indents << IndentLines(EDGES_COUNT);
				curIndent = &indents.last();
				for (edgeIndex = 0; edgeIndex < EDGES_COUNT; ++edgeIndex)
					(*curIndent)[edgeIndex].maxSize = edgeIndex % 2 == 0 ? baseMaxWidth : baseMaxDepth;
				edgeIndex = 0;
			}
			if ((*curIndent)[edgeIndex].tryAddElement(Element{ elementDimension, elementIndex }, spacing))
			{
				tryCount = 0;
				elementIndex++;
			}
			else
				tryCount++;
			edgeIndex = (edgeIndex + 1) % EDGES_COUNT;
			if (tryCount == EDGES_COUNT)
				curIndent = nullptr;
		}
		for (uint indentIndex = 0; indentIndex < indents.count(); ++indentIndex)
		{
			auto& indent = indents[indentIndex];
			for (edgeIndex = 0; edgeIndex < EDGES_COUNT; ++edgeIndex)
			{
				auto& edge = indent[edgeIndex];
				const float rot = edgeIndex * osg::PI_2;
				const float coefForAlong = (edgeIndex / 2) % 2 == 0 ? -1 : 1;
				const float coefForIndent = edgeIndex == 0 || edgeIndex == 3 ? -1 : 1;
				float alongEdgeValue = edge.maxSize / 2 * coefForAlong + elementWidth / 2 * -coefForAlong;
				const float indentEdgeValue = ((edgeIndex % 2 == 0 ? regionDepth / 2 : regionWidth / 2) + elementDepth / 2 + spacing + indentIndex * (elementDepth + spacing)) * coefForIndent;
				float spacingForUse;
				if (edge.full)
					spacingForUse = (edge.maxSize - edge.elements.count() * elementWidth) / (edge.elements.count() - 1);
				else
				{
					spacingForUse = (edge.maxSize - edge.elements.count() * elementWidth) / (edge.elements.count() + 1);
					alongEdgeValue += spacingForUse * -coefForAlong;
				}
				for (auto& element : edge.elements)
				{
					element.layout.position = osg::Vec3(edgeIndex % 2 == 0 ? alongEdgeValue : indentEdgeValue, edgeIndex % 2 == 1 ? alongEdgeValue : indentEdgeValue, elementVerticalOffset);
					element.layout.yawRotation = rot;
					alongEdgeValue += (elementWidth + spacingForUse) * -coefForAlong;
				}
			}
		}
		QVector<ElementLayout> tmpLayouts(elementCount);
		for (uint indentIndex = 0; indentIndex < indents.count(); ++indentIndex)
		{
			auto& indent = indents[indentIndex];
			for (edgeIndex = 0; edgeIndex < EDGES_COUNT; ++edgeIndex)
			{
				auto& edge = indent[edgeIndex];
				for (auto& element : edge.elements)
					tmpLayouts[element.index] = element.layout;
			}
		}
		*layouts = tmpLayouts.toList();
		if (aroundRegion)
		{
			if (indents.empty())
				*aroundRegion = osg::BoundingBox(region.xMin() - spacing, region.yMin() - spacing, 0, region.xMax() + spacing, region.yMax() + spacing, 0);
			else
			{
				const int indentLastIndex = indents.count() - 1;
				const int indentPrevLastIndex = indentLastIndex - 1;
				float left, right, near, far;
				QVector<float*> aroundRegionEdges;
				aroundRegionEdges << &near << &right << &far << &left;
				for (edgeIndex = 0; edgeIndex < EDGES_COUNT; ++edgeIndex)
				{
					//uint indentCount = indents[indentLastIndex][edgeIndex].elements.count() > 0 ? indents.count() : (indentPrevLastIndex >= 0 ? indents[indentPrevLastIndex][edgeIndex].elements.count() : 0);
					*aroundRegionEdges[edgeIndex] = indents.count() * elementDepth + (indents.count() + 1) * spacing;
				}
				*aroundRegion = osg::BoundingBox(region.xMin() - left, region.yMin() - near, 0, region.xMax() + right, region.yMax() + far, 0);
			}
		}
	}
}
