#ifndef __POWER_GENERATOR_HPP_INCLUDED__
#define __POWER_GENERATOR_HPP_INCLUDED__

#include "Module.hpp"

namespace MobileRobots
{
	class PowerGenerator final : public Module
	{
	public:
		inline constexpr PowerGenerator(const unsigned powerSupplyLevel, const unsigned priority, const bool isActive = true) noexcept :
			Module(isActive, priority),
			m_powerSupplyLevel(powerSupplyLevel)
		{ }

		inline ~PowerGenerator() noexcept override = default;

		[[nodiscard]]
		inline constexpr auto getPowerSupplyLevel() const noexcept { return m_powerSupplyLevel; }

		inline std::string toString() const override {
			return Module::toStringHelper("PowerGenerator") + "\t<b>powerSupplyLevel:</b>" + std::to_string(m_powerSupplyLevel) + "<br/>";
		}

	private:
		unsigned m_powerSupplyLevel;
	};
} // namespace MobileRobots

#endif /* !__POWER_GENERATOR_HPP_INCLUDED__ */