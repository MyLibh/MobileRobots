/**
 * @file Sensor.hpp
 *
 * @ingroup Modules
 *
 * @brief Sensor module
 *
 * @author mylibh
*/

#ifndef __SENSOR_HPP_INCLUDED__
#define __SENSOR_HPP_INCLUDED__

#include "EnergyConsumer.hpp"

namespace MobileRobots
{
	class Sensor final : public EnergyConsumer
	{
	public:
		/**
		 * @brief Deleted default constructor
		*/
		Sensor() = delete;

		/**
		 * @brief Constructor
		 * 
		 * @param direction Sensor direction
		 * @param angle Sensor angle of action
		 * @param powerUsage Energy consumer power usage
		 * @param radius Energy consumer radius if action
		 * @param priority Module priority
		 * @param isActive Module state 
		*/
		inline constexpr Sensor(const unsigned direction, const unsigned angle, const unsigned powerUsage, const unsigned radius, const unsigned priority) noexcept :
			EnergyConsumer(powerUsage, radius, priority),
			m_dir(direction),
			m_angle(angle)
		{ }

		/**
		 * @brief Destructor
		*/
		inline ~Sensor() noexcept override = default;

		/**
		 * @brief Constructs string with module info
		 *
		 * @return String with sensor module info
		*/
		inline std::string toString() const override
		{
			return EnergyConsumer::toStringHelper("Sensor") + "<b>direction:</b> " + std::to_string(m_dir) + "<br/><b>angle:</b> " + std::to_string(m_angle) + "<br/>";
		}

		/**
		 * @brief Returns sensor direction
		 *
		 * @return Sensor direction
		*/
		[[nodiscard]]
		inline constexpr auto getDirection() const noexcept { return m_dir; }

		/**
		 * @brief Returns sensor radius of action
		 *
		 * @return Sensor radius of action
		*/
		[[nodiscard]]
		inline constexpr auto getAngle() const noexcept { return m_angle; }

	private:
		unsigned m_dir;
		unsigned m_angle;
	};
} // namespace MobileRobots

#endif /* !__SENSOR_HPP_INCLUDED__ */