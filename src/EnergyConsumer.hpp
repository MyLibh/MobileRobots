/**
 * @file EnergyConsumer.hpp
 *
 * @ingroup Modules
 *
 * @brief Base class for modules using power
 *
 * @author mylibh
*/

#ifndef __ENERGY_CONSUMER_HPP_INCLUDED__
#define __ENERGY_CONSUMER_HPP_INCLUDED__

#include "Module.hpp"

namespace MobileRobots
{
	class EnergyConsumer : public Module
	{
	protected:
		/**
		 * @brief Helper function to construct string with module info
		 *
		 * @param type Type of module
		 *
		 * @return Markdown string with module info
		*/
		inline virtual std::string toStringHelper(std::string&& type) const override
		{
			return Module::toStringHelper(std::move(type)) + "<b>powerUsage:</b> " + std::to_string(m_powerUsage) + "<br/><b>r:</b> " + std::to_string(m_r) + "<br/>";
		}

	public:
		/**
		 * @brief Constructor
		 * 
		 * @param powerUsage Module power usage
		 * @param radius Module radius of action
		 * @param priority Module priority
		 * @param isActive Module state
		*/
		inline constexpr EnergyConsumer(const unsigned powerUsage, const unsigned radius, const unsigned priority, const bool isActive = true) noexcept :
			Module(priority, isActive),
			m_powerUsage(powerUsage),
			m_r(radius)
		{ }

		/**
		 * @brief Destructor 
		*/
		inline virtual ~EnergyConsumer() noexcept override = default;

		/**
		 * @brief Constructs string with module info
		 *
		 * @return String with energy consumer info
		*/
		inline virtual std::string toString() const override { return EnergyConsumer::toStringHelper("EnergyConsumer"); }

		/**
		 * @brief Returns energy consumer power usage
		 *
		 * @return Energy consumer power usage
		*/
		[[nodiscard]]
		inline constexpr auto getPowerUsage() const noexcept { return m_powerUsage; }

		/**
		 * @brief Returns energy consumer action radius
		 *
		 * @return Energy consumer radius of action
		*/
		[[nodiscard]]
		inline constexpr auto getRadius() const noexcept { return m_r; }

	protected:
		unsigned m_powerUsage;
		unsigned m_r;
	};
} // namespace MobileRobots

#endif /* !__ENERGY_CONSUMER_HPP_INCLUDED__ */