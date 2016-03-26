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
		Element(const osg::BoundingBox& boundingBox, uint index)
		{
			this->boundingBox = boundingBox;
			this->index = index;
		}
		osg::BoundingBox boundingBox;
		uint index;
		ElementLayout layout;
	};

	struct Edge
	{
		float maxEdgeSize;
		bool full;
		QList<Element> elements;

		Edge()
		{
			this->maxEdgeSize = 0;
			this->full = false;
		}
	};

	struct IndentEdges : public QVector<Edge>
	{
		IndentEdges(uint count) : QVector<Edge>(count) {};
	};

	void LayoutAlgorithms::layoutAroundRegion(const osg::BoundingBox& elementDimension, uint elementCount, const osg::BoundingBox& region, float spacing, QList<ElementLayout>* layouts, osg::BoundingBox* aroundRegion)
	{
		static const uint EDGES_COUNT = 4;

		const float regionWidth = region.xMax() - region.xMin();
		const float regionHalfWidth = regionWidth / 2;
		const float regionDepth = region.yMax() - region.yMin();
		const float regionHalfDepth = regionDepth / 2;
		const float elementWidth = elementDimension.xMax() - elementDimension.xMin();
		const float elementHalfWidth = elementWidth / 2;
		const float elementDepth = elementDimension.yMax() - elementDimension.yMin();
		const float elementHalfDepth = elementDepth / 2;
		const float elementAlongOffset = elementWidth + spacing;
		const float elementIndentOffset = elementDepth + spacing;
		const float elementVerticalOffset = -(region.zMin() + elementDimension.zMin());
		const float origRegionValuesForEdges[] = { region.yMin(), region.xMax(), region.yMax(), region.xMin() };
		const float offsetX = region.center().x();
		const float offsetY = region.center().y();

		float curWidth = regionWidth + (regionWidth <= regionDepth ? elementIndentOffset * 2 : 0);
		float curDepth = regionDepth + (regionWidth > regionDepth ? elementIndentOffset * 2 : 0);
		QList<IndentEdges> indents;
		IndentEdges* curIndent = nullptr;
		IndentEdges* prevIndent = nullptr;
		uint remainedElementsCount = elementCount;
		uint elementIndex = 0;

		while (remainedElementsCount > 0)
		{
			indents << IndentEdges(EDGES_COUNT);
			curIndent = &indents.last();
			const uint maxCountOnWidth = floorf((curWidth + spacing) / (elementWidth + spacing));
			const uint maxCountOnDepth = floorf((curDepth + spacing) / (elementWidth + spacing));
			const uint maxCountOnIndent = (maxCountOnWidth + maxCountOnDepth) * 2;
			const float fillCoef = 0.5;
			const uint elementsCountToAddForIndent = std::min(remainedElementsCount, maxCountOnIndent);
			remainedElementsCount -= elementsCountToAddForIndent;
			if ((float)elementsCountToAddForIndent < (float)maxCountOnIndent * fillCoef) // zaplna po stranach a plni ako sa len da
			{
				uint remainedElementsCountToAddForIndent = elementsCountToAddForIndent;
				uint edgeIndex = 0;
				while (remainedElementsCountToAddForIndent > 0)
				{
					const uint maxElemenstCountOnEdge = edgeIndex % 2 == 0 ? maxCountOnWidth : maxCountOnDepth;
					const uint elemenstCountOnEdge = std::min(maxElemenstCountOnEdge, remainedElementsCountToAddForIndent);
					auto& edge = (*curIndent)[edgeIndex];
					edge.maxEdgeSize = edgeIndex % 2 == 0 ? curWidth : curDepth;
					edge.full = elemenstCountOnEdge == maxElemenstCountOnEdge;
					edgeIndex++;
					for (uint i = 0; i < elemenstCountOnEdge; ++i)
						edge.elements << Element(elementDimension, elementIndex++);
					remainedElementsCountToAddForIndent -= elemenstCountOnEdge;
				}
			}
			else // zaplna rovnomerne dokola
			{
				if (elementsCountToAddForIndent == maxCountOnIndent)
				{
					for (uint edgeIndex = 0; edgeIndex < EDGES_COUNT; ++edgeIndex)
					{
						auto& edge = (*curIndent)[edgeIndex];
						edge.maxEdgeSize = edgeIndex % 2 == 0 ? curWidth : curDepth;
						edge.full = true;
						const uint countOnEdge = edgeIndex % 2 == 0 ? maxCountOnWidth : maxCountOnDepth;
						for (uint i = 0; i < countOnEdge; ++i)
							edge.elements << Element(elementDimension, elementIndex++);
					}
				}
				else
				{
					const float tmpBase = curWidth + curDepth;
					const float widthRatio = curWidth / tmpBase;
					const float depthRatio = curDepth / tmpBase;
					uint countOnWidth = roundf(elementsCountToAddForIndent / 2 * widthRatio);
					uint countOnDepth = roundf(elementsCountToAddForIndent / 2 * depthRatio);
					if (countOnWidth + countOnDepth > elementsCountToAddForIndent)
					{
						if (countOnWidth > countOnDepth) countOnWidth--;
						else countOnDepth--;
					}
					for (uint edgeIndex = 0; edgeIndex < EDGES_COUNT; ++edgeIndex)
					{
						auto& edge = (*curIndent)[edgeIndex];
						edge.maxEdgeSize = edgeIndex % 2 == 0 ? curWidth : curDepth;
						edge.full = edgeIndex % 2 == 0 ? countOnWidth == maxCountOnWidth : countOnDepth == maxCountOnDepth;
						const uint countOnEdge = edgeIndex % 2 == 0 ? countOnWidth : countOnDepth;
						for (uint i = 0; i < countOnEdge; ++i)
							edge.elements << Element(elementDimension, elementIndex++);
					}
				}
				
			}
			curWidth += elementIndentOffset * 2;
			curDepth += elementIndentOffset * 2;
		}

		for (uint indentIndex = 0; indentIndex < indents.count(); ++indentIndex)
		{
			auto& indent = indents[indentIndex];
			for (uint edgeIndex = 0; edgeIndex < EDGES_COUNT; ++edgeIndex)
			{
				auto& edge = indent[edgeIndex];
				const float rot = edgeIndex * osg::PI_2;
				const float coefForAlong = (edgeIndex / 2) % 2 == 0 ? -1 : 1;
				const float coefForIndent = edgeIndex == 0 || edgeIndex == 3 ? -1 : 1;
				const float baseAlongPos = edge.maxEdgeSize / 2;
				float alongEdgeValue = (baseAlongPos - elementHalfWidth) * coefForAlong;
				const float baseIndentPos = edgeIndex % 2 == 0 ? regionHalfDepth : regionHalfWidth;
				const float indentEdgeValue = (baseIndentPos + elementHalfDepth + spacing + indentIndex * elementIndentOffset) * coefForIndent;
				float spacingForUse;
				if (edge.full)
					spacingForUse = (edge.maxEdgeSize - edge.elements.count() * elementWidth) / (edge.elements.count() - 1);
				else
				{
					spacingForUse = (edge.maxEdgeSize - edge.elements.count() * elementWidth) / (edge.elements.count() + 1);
					alongEdgeValue += spacingForUse * -coefForAlong;
				}
				for (auto& element : edge.elements)
				{
					element.layout.position = osg::Vec3((edgeIndex % 2 == 0 ? alongEdgeValue : indentEdgeValue) + offsetX, (edgeIndex % 2 == 1 ? alongEdgeValue : indentEdgeValue) + offsetY, elementVerticalOffset);
					element.layout.yawRotation = rot;
					alongEdgeValue += (elementWidth + spacingForUse) * -coefForAlong;
				}
			}
		}
		QVector<ElementLayout> tmpLayouts(elementCount);
		for (uint indentIndex = 0; indentIndex < indents.count(); ++indentIndex)
		{
			auto& indent = indents[indentIndex];
			for (uint edgeIndex = 0; edgeIndex < EDGES_COUNT; ++edgeIndex)
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
				float newAroundRegionValuesForEdges[4];
				for (uint edgeIndex = 0; edgeIndex < EDGES_COUNT; ++edgeIndex)
				{
					const uint indentCountForEdge = indents.last()[edgeIndex].elements.count() > 0 ? indents.count() : indents.count() - 1;
					const float coefForIndent = edgeIndex == 0 || edgeIndex == 3 ? -1 : 1;
					newAroundRegionValuesForEdges[edgeIndex] = origRegionValuesForEdges[edgeIndex] + (indentCountForEdge * elementIndentOffset + spacing) * coefForIndent;
				}

				const float& newNear = newAroundRegionValuesForEdges[0];
				const float& newRight = newAroundRegionValuesForEdges[1];
				const float& newFar = newAroundRegionValuesForEdges[2];
				const float& newLeft = newAroundRegionValuesForEdges[3];
				*aroundRegion = osg::BoundingBox(newLeft, newNear, 0, newRight, newFar, 0);
			}
		}
	}
}
