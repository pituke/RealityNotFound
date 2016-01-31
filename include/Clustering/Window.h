#pragma once

#include <osg/PositionAttitudeTransform>
#include <QString>

namespace Clustering
{
	class Window : public osg::PositionAttitudeTransform
	{
	public:
		enum WindowType { SINGLE_WING, TRIPLE_WING, MULTI_WING };

	public:
		Window(const QString& displayText, WindowType windowType);
		void setHeight(float height);
		float getWidth() const;
	};
}
