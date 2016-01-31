#pragma once

#include <osg/Geometry>
#include <QString>
#include <QtCore/QMap>

namespace Manager
{
	/**
	 * \class ResourceManager
     * \brief Manager provide meshes, textures and other resources
     *
     * Class is implemented as singleton.
     *
     * \author Stefan Horvath
     * \date 26.01.2016
     */
	class ResourceManager
	{
	private:
		ResourceManager();

	public:
		static ResourceManager* getInstance();
		osg::ref_ptr<osg::Node> getMesh(const QString& path);
	};
}
