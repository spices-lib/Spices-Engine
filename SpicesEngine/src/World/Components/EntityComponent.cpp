/**
* @file EntityComponent.cpp.
* @brief The EntityComponent Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "EntityComponent.h"
#include "Render/FrameInfo.h"
#include "World/World/World.h"
#include "World/Entity.h"

namespace Spices {

	void EntityComponent::OnSerialize()
	{
		
	}
	void EntityComponent::OnDeSerialize()
	{
		
	}

	void EntityComponent::DrawThis()
	{
		SPICES_PROFILE_ZONE;

		if (ImGui::BeginChild("Entity Attachments", ImVec2(0, std::min(m_Entities.size() + 0.2f, 10.0f) * ImGuiH::GetLineItemSize().x), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
		{
			for (auto& item : m_Entities)
			{
				std::stringstream ss;
				ss << "Attchments [" << item << "]";

				ImGuiH::DrawPropertyItem(ss.str().c_str(), 0, nullptr, [&]() {
					ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
					char buffer[12] = {};
					ImGui::InputTextWithHint("##", std::to_string(item).c_str(), buffer, sizeof(buffer));
					ImGui::PopItemWidth();
					ImGui::SameLine();
					ImGuiH::DrawResetIcon(false);
				});
			}

			ImGui::EndChild();
		}
	}

	void EntityComponent::AddEntity(uint32_t entity)
	{
		SPICES_PROFILE_ZONE;

		m_Entities.insert(entity);
		
		Entity e((entt::entity)entity, FrameInfo::Get().m_World.get());
		e.RemoveFromRoot();
	}

	void EntityComponent::RemoveEntity(uint32_t entity)
	{
		SPICES_PROFILE_ZONE;

		m_Entities.erase(entity);
		Entity e((entt::entity)entity, FrameInfo::Get().m_World.get());
		e.AddToRoot();
	}
}
