#include "Clustering/Window.h"
#include <Manager/ResourceManager.h>
#include <osg/Geode>
#include <osg/BoundingBox>

namespace Clustering
{
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
		auto newScale = getScale();
		newScale.x() = height;
		newScale.z() = height;
		setScale(newScale);
	}

	float Window::getWidth() const
	{
		const auto& boundBox = getChild(0)->asGroup()->getChild(0)->asGeode()->getBoundingBox();
		return (boundBox.xMax() - boundBox.xMin()) * getScale().x();
	}
}
