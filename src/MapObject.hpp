#ifndef __MAP_OBJECT_HPP_INCLUDED__ 
#define __MAP_OBJECT_HPP_INCLUDED__

#include "Coord.hpp"

#include <memory>
#include <string>

namespace MobileRobots
{
	class EnvironmentDescriptor;

	class MapObject
	{
	public:
		static void setEnvironmentDescriptor(std::shared_ptr<EnvironmentDescriptor> envDesc) noexcept;

	protected:
		inline virtual std::string toStringHelper(std::string&& type) const
		{
			return "<b>x:</b> " + std::to_string(m_pos.x) + "<br/><b>y:</b> " + std::to_string(m_pos.y) + "<br/><b>type:</b> " + type + "<br/>";
		}

	public:
		MapObject() = delete;

		inline constexpr explicit MapObject(const Coord& coord) noexcept :
			m_pos(coord)
		{ }

		inline virtual ~MapObject() noexcept = default;

		[[nodiscard]]
		inline constexpr auto getX() const noexcept { return m_pos.x; }

		[[nodiscard]]
		inline constexpr auto getY() const noexcept { return m_pos.y; }

		[[nodiscard]]
		inline constexpr auto getPos() const noexcept { return m_pos; }

		virtual std::string toString() const = 0;

	protected:
		static std::shared_ptr<EnvironmentDescriptor> sEnvDesc;

	protected:
		Coord m_pos;
	};
} // namespace MobileRobots

#endif /* !__MAP_OBJECT_HPP_INCLUDED__ */
