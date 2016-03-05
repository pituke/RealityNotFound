#pragma once

#include <osg/PositionAttitudeTransform>

namespace Clustering
{
	class Floor : public osg::PositionAttitudeTransform
	{
	private:
		float baseSize;
		float floorHeight;

	public:
		Floor();
		void setBaseSize(float size);
		float getBaseSize() const;
		void setFloorHeight(float height);
		float getFloorHeight() const;
		void refresh();
		static float getFloorMinHeight();
	};
}
