#ifndef __SENSOR_HPP_INCLUDED__
#define __SENSOR_HPP_INCLUDED__

#include "EnergyConsumer.hpp"

namespace MobileRobots
{
	class Sensor final : public EnergyConsumer
	{
	public:
		Sensor() = delete;

		inline constexpr Sensor(const unsigned direction, const unsigned angle, const unsigned powerUsage, const unsigned radius, const unsigned priority) noexcept :
			EnergyConsumer(powerUsage, radius, priority),
			m_dir(direction),
			m_angle(angle)
		{ }

		inline ~Sensor() noexcept override = default;

		inline std::string toString() const override
		{
			return EnergyConsumer::toStringHelper("Sensor") + "<b>direction:</b> " + std::to_string(m_dir) + "<br/><b>angle:</b> " + std::to_string(m_angle) + "<br/>";
		}

		[[nodiscard]]
		inline constexpr auto getDirection() const noexcept { return m_dir; }

		[[nodiscard]]
		inline constexpr auto getAngle() const noexcept { return m_angle; }

	private:
		unsigned m_dir;
		unsigned m_angle;
	};
} // namespace MobileRobots

#endif /* !__SENSOR_HPP_INCLUDED__ */