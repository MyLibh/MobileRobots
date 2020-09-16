#ifndef __MODULE_HPP_INCLUDED__
#define __MODULE_HPP_INCLUDED__

#include <string>

namespace MobileRobots
{
	class Module
	{
	protected:
		inline virtual std::string toStringHelper(std::string&& type) const
		{
			return "<b>type:</b> " + type + "<br/><b>isActive:</b> " + std::to_string(m_isActive) + "<br/><b>priority:</b> " + std::to_string(m_priority) + "<br/>";
		};

	public:
		inline constexpr Module(const bool isActive, const unsigned priority) noexcept :
			m_isActive(isActive),
			m_priority(priority)
		{ }

		inline virtual ~Module() noexcept = default;

		[[nodiscard]]
		inline constexpr auto getPriority() const noexcept { return m_priority; }

		inline constexpr bool isActive() const noexcept { return m_isActive; }

		inline void turnOn() noexcept { m_isActive = true; }

		inline void turnOff() noexcept { m_isActive = false; }

		virtual std::string toString() const = 0;

	protected:
		bool     m_isActive;
		unsigned m_priority;
	};
} // namespace MobileRobots

#endif /* !__MODULE_HPP_INCLUDED__ */