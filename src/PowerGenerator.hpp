/**
 * @file PowerGenerator.hpp
 *
 * @ingroup Modules
 *
 * @brief Power generator module
 *
 * @author mylibh
*/

#ifndef __POWER_GENERATOR_HPP_INCLUDED__
#define __POWER_GENERATOR_HPP_INCLUDED__

#include "Module.hpp"

namespace MobileRobots
{
	class PowerGenerator final : public Module
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param powerSupplyLevel Power generator supply level
		 * @param priority Power generator priority
		 * @param isActive Power generator state
		*/
		inline constexpr PowerGenerator(const unsigned powerSupplyLevel, const unsigned priority, const bool isActive = true) noexcept :
			Module(isActive, priority),
			m_powerSupplyLevel(powerSupplyLevel)
		{ }

		/**
		 * @brief Destructor 
		*/
		inline ~PowerGenerator() noexcept override = default;

		/**
		 * @brief Returns power generator supply level
		 *
		 * @return Power generator supply level
		*/
		[[nodiscard]]
		inline constexpr auto getPowerSupplyLevel() const noexcept { return m_powerSupplyLevel; }

		/**
		 * @brief Constructs string with module info
		 *
		 * @return String with power generator info
		*/
		inline std::string toString() const override
		{
			return Module::toStringHelper("PowerGenerator") + "\t<b>powerSupplyLevel:</b>" + std::to_string(m_powerSupplyLevel) + "<br/>";
		}

	private:
		unsigned m_powerSupplyLevel;
	};
} // namespace MobileRobots

#endif /* !__POWER_GENERATOR_HPP_INCLUDED__ */