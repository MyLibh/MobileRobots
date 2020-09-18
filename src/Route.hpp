/**
 * @file Route.hpp
 *
 * @ingroup Map
 *
 * @brief Class representing the route
 *
 * @author mylibh
*/

#ifndef __ROUTE_HPP_INCLUDED__
#define __ROUTE_HPP_INCLUDED__

#include "Coord.hpp"

#include <queue>

namespace MobileRobots
{
	class Route final
	{
	public:
		/**
		 * @brief Default constructor 
		*/
		inline Route() noexcept :
			m_to{ -1U, -1U }
		{ }

		/**
		 * @brief Constructor
		 * @param path Queue of coordinates to visit
		*/
		inline Route(std::queue<Coord> path) noexcept :
			m_path(std::move(path)),
			m_to{ -1U, -1U }
		{ 
			if (!m_path.empty())
				m_to = m_path.back();
		}

		/**
		 * @brief Returns next point in the route
		 *
		 * @return If route is finished returns final point, else next route point
		*/
		[[nodiscard]]
		inline Coord getNext() noexcept
		{
			if (!m_path.empty())
			{
				auto&& coord = m_path.front();
				m_path.pop();

				return coord;
			}

			return m_to;
		}

		/**
		 * @brief Returns final point
		 *
		 * @return Final route point
		*/
		[[nodiscard]]
		inline auto to() const noexcept { return m_to; }

		/**
		 * @brief Checks if the route is finished
		 * 
		 * @return True if route is finished, False overwise
		*/
		inline bool isFinished() const noexcept { return m_path.empty(); }

		/**
		 * @brief Returns the state of route
		 * 
		 * @return False if route is finished, True overwise
		 * 
		 * @see isFinished
		*/
		inline explicit operator bool() const noexcept { return !isFinished(); }

	private:
		std::queue<Coord> m_path;
		Coord             m_to;
	};
} // namespace MbileRobots

#endif /* !__ROUTE_HPP_INCLUDED__ */