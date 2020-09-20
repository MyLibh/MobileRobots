/**
 * @file MapLoader.hpp
 *
 * @defgroup Map
 *
 * @brief Map loader
 *
 * @author mylibh
*/

#ifndef __MAP_LOADER_HPP_INCLUDED__
#define __MAP_LOADER_HPP_INCLUDED__

#include <QString>

#include <memory>

namespace MobileRobots
{
	class EnvironmentDescriptor;

	class MapLoader
	{
	public:
		/**
		 * @brief Loads the map
		 * 
		 * @param path Path to json file, containing map info
		 * 
		 * @return Loaded map environment descriptor
		*/
		static std::shared_ptr<EnvironmentDescriptor> load(const QString& path);
	};
} // namespace MobileRobots

#endif /* !__MAP_LOADER_HPP_INCLUDED__ */