#pragma once

#include "Clustering/Window.h"
#include <osg/PositionAttitudeTransform>
#include <QList>

namespace Clustering
{
	class Floor : public osg::PositionAttitudeTransform
	{
	private:
		QString displayText;
		QList<osg::ref_ptr<Window>> floor1, floor2;
		QString cornerText;
		float reqFloorSize;
		uint priority;

	public:
		Floor(const QString& displayText, const QList<Window*>& windows, const QString& cornerText, uint priority);
		void setFloorSize(float floorSize);
		float getFloorSize() const;
		uint getPriority() const;
		void refresh();
		static float getFloorHeight();
	};
}
