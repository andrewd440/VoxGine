#pragma once

#include "SystemBitManager.h"
#include "NonCopyable.h"
#include "System.h"

#include <vector>
#include <memory>

namespace Atlas
{
	class FWorld;
	class FGameObject;

	/**
	* A manager for all Systems that are used.
	*/
	class FSystemManager : public NonCopyable
	{
	public:
		/**
		* Creates a new SystemManager.
		* @param world - the World object that owns this SystemManager
		*/
		FSystemManager(FWorld& World);

		~FSystemManager();

		/**
		* Initalizes all systems.
		*/
		void Start();

		template <typename T>
		/**
		* Adds a new System.
		* @tparam T - The type of System to add.
		* @return A reference to the new system.
		*/
		T& AddSystem();
		
		template <typename T, typename Param1>
		/**
		* Adds a new System.
		* @tparam T - The type of System to add.
		* @param P1 - The first argument for the system's constructor
		* @return A reference to the new system.
		*/
		T& AddSystem(const Param1& P1);

		template <typename T, typename Param1, typename Param2>
		/**
		* Adds a new System.
		* @tparam T - The type of System to add.
		* @param P1 - The first argument for the system's constructor
		* @param P1 - The second argument for the system's constructor
		*  @return A reference to the new system.
		*/
		T& AddSystem(Param1& P1, Param2& P2);

		template <typename T>
		/**
		* Retrieves a type of system.
		* @return A pointer to the System.
		*/
		T* GetSystem();

		/**
		* Retrieves a system by index into this manager. System
		* order is dependent on the order added to this manager. System
		* removals will affected ordering.
		* @param Index - The position that the system type was added.
		* @param A pointer to the system
		*/
		ISystem* GetSystem(const uint32_t Index);

		/**
		* Checks to see if the any Systems contained in the SystemManager is interested 
		* in a GameObject based on which Components it owns.
		* @param GameObject - The GameObject to be checked
		* @param UpdatedComponent - The component that was added/removed for the object.
		*/
		void CheckInterest(FGameObject& GameObject, IComponent& UpdatedComponent);

		template <typename T>
		/**
		* Retrieves a type of system.
		*/
		void RemoveSystem();

		/**
		* Removes a system by index.
		* @param Index - The position that the system type was added. System
		*				 removals will affected ordering.
		*/
		void RemoveSystem(const uint32_t Index);

	private:
		FWorld& mWorld;
		std::vector<std::unique_ptr<ISystem>> mSystems;
	};

	template <typename T>
	inline T& FSystemManager::AddSystem()
	{
		T* RawSystem = new T(mWorld);
		std::unique_ptr<ISystem> System{ RawSystem };

		RawSystem->SetSystemBitMask(SSystemBitManager::GetBitMaskFor(RawSystem));

		mSystems.push_back(std::move(System));
		return *RawSystem;
	}

	template <typename T, typename Param1>
	inline T& FSystemManager::AddSystem(const Param1& P1)
	{
		T* RawSystem = new T(mWorld, P1);
		std::unique_ptr<ISystem> System{ RawSystem };

		RawSystem->SetSystemBitMask(SSystemBitManager::GetBitMaskFor(RawSystem));

		mSystems.push_back(std::move(System));
		return *RawSystem;
	}

	template <typename T, typename Param1, typename Param2>
	inline T& FSystemManager::AddSystem(Param1& P1, Param2& P2)
	{
		T* RawSystem = new T(mWorld, P1, P2);
		std::unique_ptr<ISystem> System{ RawSystem };

		RawSystem->SetSystemBitMask(SSystemBitManager::GetBitMaskFor(RawSystem));

		mSystems.push_back(std::move(System));
		return *RawSystem;
	}

	inline void FSystemManager::RemoveSystem(const uint32_t Index)
	{
		mSystems.erase(mSystems.begin() + Index);
	}

	template <typename T>
	inline T* FSystemManager::GetSystem()
	{
		System* SystemPtr = nullptr;

		for (const auto& System : mSystems)
		{
			if (typeid(*System) == typeid(T))
			{
				SystemPtr = System.get();
				break;
			}
		}

		// make sure the System is there and cast is safe
		assert(dynamic_cast<T*>(SystemPtr));
		return static_cast<T*>(SystemPtr);
	}

	inline ISystem* FSystemManager::GetSystem(const uint32_t Index)
	{
		return mSystems[Index].get();
	}

	template <typename Type>
	inline void FSystemManager::RemoveSystem()
	{
		mSystems.erase(std::find_if(mSystems.begin(), mSystems.end(), [](std::unique_ptr<ISystem> Ptr){ return typeid(Ptr) == Type; }));
	}
}