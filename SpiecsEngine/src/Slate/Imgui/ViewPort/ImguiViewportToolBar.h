#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spiecs {

	/**
	* @brief The  StageIcon Texture's ImTextureID.
	*/
	struct ViewportToolBarIconID
	{
		ImTextureID optionIcon;     // ImTextureID of Option.
		ImTextureID renderingIcon;  // ImTextureID of Rendering.
		ImTextureID viewerIcon;     // ImTextureID of Viewer.
		ImTextureID cameraIcon;     // ImTextureID of Camera.
	};

	class ImguiViewportToolBar : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		* @param[in] owner The slate Owner.
		*/
		ImguiViewportToolBar(
			const std::string& panelName,
			FrameInfo& frameInfo,
			ImguiSlate* owner
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiViewportToolBar() {};

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
		virtual void OnEvent(Event& event) {};

	private:

		/**
		* @brief The owner, usually is a ImguiViewport class.
		*/
		ImguiSlate* m_Owner;

		/**
		* @brief The instance of ViewportToolBarIconID.
		*/
		ViewportToolBarIconID m_IconID;
	};
}
