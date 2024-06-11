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
		{};

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiProperty() {};

		/**
		* @brief This interface is called On SlateSystem Update.
		* @param[in] ts TimeStep.
		*/
		virtual void OnUpdate(TimeStep& ts) override {};

		/**
		* @brief This interface is called On SlateRenderer Render.
		*/
		virtual void OnRender() override;

		/**
		* @brief This interface is called On Global Event Function Pointer is called.
		*/
		virtual void OnEvent(Event& event) override {};

	private:

		/**
		* @brief Draw Entity Component on Property Panel.
		* @param[in] T Component Class.
		* @param[in] name ComponentName.
		* @param[in] entity Entity.
		*/
		template<typename T>
		static void DrawComponent(const std::string& name, Entity entity);
	};

	template<typename T>
	inline void ImguiProperty::DrawComponent(const std::string& name, Entity entity)
	{
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();

			const ImGuiTreeNodeFlags treeNodeFlags = 
				ImGuiTreeNodeFlags_DefaultOpen      | 
				ImGuiTreeNodeFlags_AllowItemOverlap | 
				ImGuiTreeNodeFlags_Framed           | 
				ImGuiTreeNodeFlags_FramePadding     ;

			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());

			/*ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
			if (ImGui::Button("+", ImGuiH::GetLineItemSize()))
			{
				ImGui::OpenPopup("ComponentSettings");
			}*/

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
		}
	}
}