#include "Clustering/Window.h"
#include <Manager/ResourceManager.h>
#include <osg/Geode>
#include <osg/BoundingBox>

namespace Clustering
{
	static const float WINDOW_DEPTH_SCALE = 1.0;

	Window::Window(const QString& displayText, WindowType windowType)
	{
		static const QString MESHES_DIR = "..\\..\\resources\\mesh\\";

		auto mgr = Manager::ResourceManager::getInstance();
		if (windowType == SINGLE_WING)
			Group::addChild(mgr->getMesh(MESHES_DIR + "window_single_wing.obj"));
		else if (windowType == TRIPLE_WING)
			Group::addChild(mgr->getMesh(MESHES_DIR + "window_triple_wing.obj"));
		else if (windowType == MULTI_WING)
			Group::addChild(mgr->getMesh(MESHES_DIR + "window_multi_wing.obj"));
		else
			throw std::exception("Unsupported window type");
	}

	void Window::setHeight(float height)
	{
		setScale(osg::Vec3(height, WINDOW_DEPTH_SCALE, height));
	}

	float Window::getWidth() const
	{
		const auto& boundBox = getChild(0)->asGroup()->getChild(0)->asGeode()->getBoundingBox();
		return (boundBox.xMax() - boundBox.xMin()) * getScale().x();
	}
}
