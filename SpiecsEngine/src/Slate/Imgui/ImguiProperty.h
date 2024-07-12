/**
* @file ImguiProperty.h
* @brief The ImguiProperty Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

#include "World/Entity.h"

namespace Spiecs {

	/**
	* @brief The ImguiProperty Class.
	* This class defines how to render a Property.
	*/
	class ImguiProperty : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		*/
		ImguiProperty(
			const std::string& panelName , 
			FrameInfo&         frameInfo
		) 
			: ImguiSlate(panelName, frameInfo) 
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiProperty() override = default;

		/**
		* @brief This interface is called On SlateSystem Update.
		* @param[in] ts TimeStep.
		*/
		virtual void OnUpdate(TimeStep& ts) override {}

		/**
		* @brief This interface is called On SlateRenderer Render.
		*/
		virtual void OnRender() override;

		/**
		* @brief This interface is called On Global Event Function Pointer is called.
		*/
		virtual void OnEvent(Event& event) override {}

	private:

		/**
		* @brief Draw Entity Component on Property Panel.
		* @tparam T Component Class.
		* @param[in] name ComponentName.
		* @param[in] entity Entity.
		*/
		template<typename T>
		static void DrawComponent(const std::string& name, Entity entity);
	};

	template<typename T>
	void ImguiProperty::DrawComponent(const std::string& name, Entity entity)
	{
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();

			constexpr ImGuiTreeNodeFlags treeNodeFlags = 
				ImGuiTreeNodeFlags_DefaultOpen      | 
				ImGuiTreeNodeFlags_AllowItemOverlap | 
				ImGuiTreeNodeFlags_Framed           | 
				ImGuiTreeNodeFlags_FramePadding     ;
			
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.196f, 0.204f, 0.2f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.164f, 0.18f, 0.184f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.164f, 0.18f, 0.184f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.196f, 0.204f, 0.2f, 1.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 6.0f));

			const bool open = ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(T).hash_code()), treeNodeFlags, name.c_str());
			
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.16f, 0.16f, 0.16f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.608f, 0.608f, 0.608f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.396f, 0.439f, 0.168f, 1.0f));
			if (ImGui::Button(ICON_MD_FILTER_VINTAGE, ImGuiH::GetLineItemSize()))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
			
			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				component.DrawThis();
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.RemoveComponent<T>();
			}
			
			ImGui::PopStyleColor(4);
		}
	}
}