/**
 * @file ManagerModule.hpp
 *
 * @ingroup Modules
 *
 * @brief Manager module
 *
 * @author mylibh
*/

#ifndef __MANAGER_MODULE_HPP_INCLUDED__
#define __MANAGER_MODULE_HPP_INCLUDED__

#include "EnergyConsumer.hpp"
#include "ObservationCenter.hpp"

#include <vector>

namespace MobileRobots
{
	class AI;

	class ManagerModule final : public EnergyConsumer
	{
	public:
		/**
		 * @brief Setups the AI
		 * 
		 * @param ai Global AI
		*/
		static void setAI(std::shared_ptr<AI> ai) noexcept;

	private:
		/**
		 * @brief Checks if a device can be added
		 * 
		 * @param newDevice 
		 * 
		 * @return True if can, False overwise
		*/
		inline bool canAddDevice(std::shared_ptr<ObservationCenter> newDevice) const noexcept
		{ 
			return (static_cast<size_t>(m_maxDevices) > m_devices.size() && std::find(std::begin(m_devices), std::end(m_devices), newDevice) == std::end(m_devices)); 
		}

	public:
		/**
		 * @brief Deleted default constructor
		*/
		ManagerModule() = delete;

		/**
		 * @brief Constructor
		 * 
		 * @param maxDevices Manager module max number of managed devices
		 * @param powerUsage Energy consumer power usage
		 * @param radius Energy consumer radius if action
		 * @param priority Module priority
		 * @param isActive Module state
		*/
		inline ManagerModule(const unsigned maxDevices, const unsigned powerUsage, const unsigned radius, const unsigned priority, const bool isActive = true) noexcept :
			EnergyConsumer(powerUsage, radius, priority, isActive),
			m_maxDevices(maxDevices),
			m_devices()
		{ }

		/**
		 * @brief Destructor
		*/
		inline ~ManagerModule() noexcept override = default;

		/**
		 * @brief Constructs string with module info
		 *
		 * @return String with manager module info
		*/
		inline std::string toString() const override
		{
			return EnergyConsumer::toStringHelper("ManagerModule") + "<b>devices:</b> " + std::to_string(m_devices.size()) + "/" + std::to_string(m_maxDevices) + "<br/>";
		}

		/**
		 * @brief Returns managed devices
		 *
		 * @return Managed devices
		*/
		[[nodiscard]]
		inline auto& getDevices() const noexcept { return m_devices; }

		/**
		 * @brief Captures device control
		 * 
		 * @param newDevice The device to be added under control
		 * 
		 * @return True if device was added, False overwise
		*/
		inline bool addDevice(std::shared_ptr<ObservationCenter> newDevice) { return canAddDevice(newDevice) ? m_devices.emplace_back(newDevice), true : false; }

		/**
		 * @brief Checks if coordinate is already explored
		 * 
		 * @param coord Coordinate to check
		 * 
		 * @return True if it was explored, False overwise
		*/
		bool isExplored(const Coord& coord) const noexcept;

	private:
		static std::shared_ptr<AI> sAI;

	private:
		unsigned                                        m_maxDevices;
		std::vector<std::shared_ptr<ObservationCenter>> m_devices;
	};
} // namespace MobileRobots

#endif /* !__MANAGER_MODULE_HPP_INCLUDED__ */