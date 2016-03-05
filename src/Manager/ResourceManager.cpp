#include "Manager/ResourceManager.h"
#include <osgDB/ReadFile>

namespace Manager
{
	static ResourceManager* instance = nullptr;
	static QMap<QString, osg::ref_ptr<osg::Node>> meshes;
	static QMap<QString, osg::ref_ptr<osg::Geode>> shapes;

	ResourceManager::ResourceManager()
	{
	}

	ResourceManager* ResourceManager::getInstance()
	{
		if (!instance)
			instance = new ResourceManager();
		return instance;
	}

	osg::ref_ptr<osg::Node> ResourceManager::getMesh(const QString& path)
	{
		osg::ref_ptr<osg::Node> n = meshes.value(path);
		if (!n.valid())
		{
			n = osgDB::readNodeFile(path.toStdString());
			meshes.insert(path, n);
		}
		return n;
	}

	osg::ref_ptr<osg::Geode> ResourceManager::getShape(const QString& params, GeodeCreator creator)
	{
		osg::ref_ptr<osg::Geode> s = shapes.value(params);
		if (!s.valid())
		{
			s = creator(params);
			shapes.insert(params, s);
		}
		return s;
	}
}
