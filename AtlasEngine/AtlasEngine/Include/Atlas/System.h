#pragma once
#include <memory>
#include <vector>
#include <bitset>

#include "Bitsize.h"
#include "GameObject.h"
#include "ComponentHandleManager.h"
#include "NonCopyable.h"
#include "ComponentTypes.h"

namespace Atlas
{
	class FWorld;
	/**
	* Base class for all Systems
	* If a system only processes one type of component, it should retrieve the
	* component type list directly from the gameobject manager. It is also best if these
	* systems override the CheckInterest and RemoveObject virtual functions so that they 
	* evaluate to nothing since the gameobject list will not be used.
	* If a system processes 2 or more components from gameobjects, the gameobject
	* list can be used to retrieve a list of gameobjects with the required components.
	*/
	class ISystem : public NonCopyable
	{
	public:
		/**
		* Creates a new System.
		*/
		ISystem(FWorld& World);
		virtual ~ISystem(){};

		/**
		* Proccesses all components relevant to this system.
		*/
		virtual void Update() = 0;

		/**
		* Checks to see if the System is interested in an GameObject based on which
		* components it owns. This is called every time a GameObject removes or adds
		* a component type. If the System is no longer interested in a GameObject that
		* it already contains, the GameObject will be removed from the system.
		* @param GameObject - The GameObject to be checked
		*/
		virtual void CheckInterest(FGameObject& GameObject);

		/**
		* Retrieves the system type bits that are assigned to this system.
		* @return Bitset assigned to this system
		*/
		std::bitset<BITSIZE> GetSystemBitMask() const;

	protected:
		/**
		* Get the world object this system is a part of.
		*/
		FWorld& GetWorld();

		template <EComponent::Type Type>
		/**
		* Adds a component type for the system to process.
		*/
		void AddComponentType();

		/**
		* Retrieves the component bit mask that the system will process.
		* @return Bitset of component types
		*/
		std::bitset<BITSIZE> GetTypeBitMask() const;

		/**
		* Retrieves the IDs of interested GameObjects
		* @return List of IDs
		*/
		const std::vector<FGameObject::ID>& GetGameObjectIDs() const;

	private:
		friend class FSystemManager;  // Give full access to SystemManager

		/**
		* Removes a GameObject from the System.
		* @params GameObject - The GameObject to be removed
		*/
		virtual void RemoveObject(FGameObject& GameObject);

		/**
		* Assigns the system type bit for the System
		* @params Bit - The bit to be assigned
		*/
		void SetSystemBitMask(const std::bitset<BITSIZE>& Bit);

	private:
		FWorld&                         mWorld;
		std::bitset<BITSIZE>            mTypeBitMask;
		std::bitset<BITSIZE>            mSystemBitMask;
		std::vector<FGameObject::ID>    mGameObjectIDs;
	};

	template <EComponent::Type Type>
	inline void ISystem::AddComponentType()
	{
		mTypeBitMask |= SComponentHandleManager::GetBitMask(Type);
	}

	inline std::bitset<BITSIZE> ISystem::GetSystemBitMask() const
	{
		return mSystemBitMask;
	}

	inline std::bitset<BITSIZE> ISystem::GetTypeBitMask() const
	{
		return mTypeBitMask;
	}

	inline const std::vector<FGameObject::ID>& ISystem::GetGameObjectIDs() const
	{
		return mGameObjectIDs;
	}

	inline void ISystem::SetSystemBitMask(const std::bitset<BITSIZE>& BitMask)
	{
		mSystemBitMask = BitMask;
	}

	inline FWorld& ISystem::GetWorld()
	{
		return mWorld;
	}
}