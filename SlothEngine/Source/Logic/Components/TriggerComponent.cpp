#include "TriggerComponent.h"
#include <Logic/Components/TransformComponent.h>
#include <Logic/Actor.h>

using slth::TriggerComponent;

TriggerComponent::TriggerComponent()
	: m_radius(9999.f)
	, m_active(false)
	, m_type(TriggerType::kRadius)
	, m_visionArc(0.f)
	, m_unique(false)
	, m_triggeredBy(0)
{
	//
}

slth::IComponent* TriggerComponent::Copy(slth::Actor* pNewOwner)
{
	TriggerComponent* pCopy = new TriggerComponent();
	pCopy->SetOwner(pNewOwner);
	pCopy->m_radius = m_radius;
	pCopy->m_tag = m_tag;
	pCopy->m_type = m_type;
	pCopy->m_unique = m_unique;
	pCopy->m_triggeredBy = 0;
	
	return pCopy;
}

bool slth::TriggerComponent::Init(XmlData* xmlElement)
{
	const char* type = xmlElement->Attribute("type");
	m_type = TriggerType::kRadius;

	if (strcmp(type, "Cone") == 0)
	{
		m_type = TriggerType::kCone;
	}

	m_radius = xmlElement->FloatAttribute("radius");
	m_tag = xmlElement->Attribute("tag");
	m_visionArc = xmlElement->FloatAttribute("arc");
	m_unique = xmlElement->BoolAttribute("unique");
	return true;
}

bool slth::TriggerComponent::PointIsInTrigger(Vector2f point)
{
	TransformComponent* pTransform = m_pOwner->GetComponent<TransformComponent>();
	if (pTransform->GetPosition().DistanceTo(point) <= m_radius)
	{
		if (m_type == TriggerType::kRadius)
		{
			// radius is all that matters, it's true
			return true;
		}
		else if (m_type == TriggerType::kCone)
		{
			// check vision cone as well
			float angleBetween = slth::Vector2f::AngleBetween(pTransform->GetPosition(), point);
			float rotation = pTransform->GetRotation();
			if (std::abs(angleBetween - rotation) < m_visionArc / 2.f)
			{
				return true;
			}
		}
	}

	return false;
}

void slth::TriggerComponent::LogInteraction(Id32 actorID)
{
	Interaction interaction = { InteractionType::kNone, 0.f };
	m_interactions[actorID] = interaction;
}

bool slth::TriggerComponent::CheckInteraction(Id32 actorID)
{
	auto foundInteraction = m_interactions.find(actorID);
	return foundInteraction != m_interactions.end();
	
}

void slth::TriggerComponent::Activate(Id32 actorId)
{
	m_active = true;
	LogInteraction(actorId);
	m_triggeredBy = actorId;
}

void slth::TriggerComponent::Deactivate()
{
	m_active = false;
	m_triggeredBy = 0;
}


