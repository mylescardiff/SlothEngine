#pragma once
/** \file TriggerComponent.h */
// Created by Myles Cardiff

#include ".\IComponent.h"
#include <Utilities/Vector2f.h>
#include <unordered_map>

namespace slth
{
	/// <summary>
	/// Defines the shape of the trigger area for the TriggerComponent
	/// </summary>
	enum class TriggerType
	{
		kRadius,
		kCone
	};

	/// <summary>
	/// Placeholder for future improvement to interaction logging
	/// </summary>
	enum class InteractionType
	{
		kNone
	};

	/// <summary>
	/// Defines an actor interaction log so we can tell if this 
	/// trigger has already performed this action on a particular
	/// actor
	/// </summary>
	struct Interaction
	{
		InteractionType m_type;
		float m_expireTime;
	};

/// <summary>
/// A trigger component will fire when an actor enters the trigger area.
/// </summary>
class TriggerComponent
	: public IComponent
{
public:

	/// <summary>
	/// Default Constructor
	/// </summary>
	SLTH_API TriggerComponent();
	
	/// /// <summary>
	/// Creates a copy of this component
	/// </summary>
	/// <param name="pNewOwner"></param>
	/// <returns></returns>
	slth::IComponent* Copy(slth::Actor* pNewOwner);

	inline static constexpr const char* kStringId = "Trigger";
	static constexpr Id Identifier = Hasher::Hash32(kStringId);
	SLTH_API virtual const char* GetName() const override { return kStringId; }
	SLTH_API virtual const Id GetIdentifier() const override { return Identifier; }
	
	/// <summary>
	/// Initilizes the transform component from XML node (see /data/actors.xml for syntax)
	/// </summary>
	/// <param name="xmlElement">The tinyxml node pointer contianing the data</param>
	/// <returns></returns>
	SLTH_API virtual bool Init(XmlData* xmlElement) override;

	/// <summary>
	/// Tests if a point has entered the trigger
	/// </summary>
	/// <param name="point"></param>
	/// <returns></returns>
	SLTH_API virtual bool PointIsInTrigger(Vector2f point);
	
	/// <summary>
	/// Place an actor id and corresponding interaction in the log
	/// so we can tell if this trigger has already been fired for that actor
	/// </summary>
	/// <param name="actorID"></param>
	SLTH_API void LogInteraction(Id32 actorID);

	/// <summary>
	/// If this trigger is marked unique, it will check if it's 
	/// already been triggered by this actor
	/// 
	/// </summary>
	/// <param name="actorID"></param>
	/// <returns></returns>
	SLTH_API bool CheckInteraction(Id32 actorID);

	/// <summary>
	/// Set the trigger to active and logs who triggered it
	/// </summary>
	/// <param name="actorId"></param>
	SLTH_API void Activate(Id32 actorId);

	/// <summary>
	///  Release the trigger to be set off again
	/// </summary>
	SLTH_API void Deactivate();

private:
	
	TriggerType m_type;
	float m_radius;
	float m_visionArc;
	std::string m_tag;
	bool m_active;
	bool m_unique;
	Id32 m_triggeredBy;

	std::unordered_map<Id32, Interaction> m_interactions;
	
	static constexpr float kDefaultVisionArc = 60.f;


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

	SLTH_API const float GetRadius() const { return m_radius; }
	SLTH_API const std::string& GetTag() const { return m_tag; }
	SLTH_API const bool IsActive() const { return m_active; }
	SLTH_API void SetActive(bool active) { m_active = active; }
	SLTH_API bool IsUnique() { return m_unique; }
	SLTH_API Id32 GetTriggeredBy() { return m_triggeredBy; }

};
}