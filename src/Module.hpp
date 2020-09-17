/**
 * @file Module.hpp
 *
 * @defgroup Modules
 *
 * @brief Base class for modules
 *
 * @author mylibh
*/

#ifndef __MODULE_HPP_INCLUDED__
#define __MODULE_HPP_INCLUDED__

#include <string>

namespace MobileRobots
{
	class Module
	{
	protected:
		/**
		 * @brief Helper function to construct string with module info
		 *
		 * @param type Type of module
		 *
		 * @return Markdown string with module info
		*/
		inline virtual std::string toStringHelper(std::string&& type) const
		{
			return "<b>type:</b> " + type + "<br/><b>isActive:</b> " + std::to_string(m_isActive) + "<br/><b>priority:</b> " + std::to_string(m_priority) + "<br/>";
		};

	public:
		/**
		 * @brief Deleted default constructor
		*/
		Module() = delete;

		/**
		 * @brief Constructor
		 *
		 * @param priority Module priority
		 * @param isActive Module state
		*/
		inline constexpr Module(const unsigned priority, const bool isActive = true) noexcept :
			m_priority(priority),
			m_isActive(isActive)
		{ }

		/**
		 * @brief Destructor
		*/
		inline virtual ~Module() noexcept = default;

		/**
		 * @brief Returns module priority
		 *
		 * @return Module priority
		*/
		[[nodiscard]]
		inline constexpr auto getPriority() const noexcept { return m_priority; }

		/**
		 * @brief Gets module state
		 *
		 * @return True if on, False overwise
		*/
		inline constexpr bool isActive() const noexcept { return m_isActive; }

		/**
		 * @brief Turns module on
		*/
		inline void turnOn() noexcept { m_isActive = true; }

		/**
		 * @brief Turns module off
		*/
		inline void turnOff() noexcept { m_isActive = false; }

		/**
		 * @brief Constructs string with module info
		 *
		 * @return String with module info
		*/
		virtual std::string toString() const = 0;

	protected:
		bool     m_isActive;
		unsigned m_priority;
	};
} // namespace MobileRobots

#endif /* !__MODULE_HPP_INCLUDED__ */