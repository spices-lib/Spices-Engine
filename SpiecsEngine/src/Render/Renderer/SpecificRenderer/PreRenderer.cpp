#include "Pchheader.h"
#include "PreRenderer.h"

namespace Spiecs {

	namespace PreR {

		/**
		* @brief This struct is specific SkyBoxRenderer PsuhConstant
		*/
		struct PushConstant
		{
			/**
			* @brief Meshpack ModelMatrix.
			*/
			glm::mat4 model = glm::mat4(1.0f);

			/**
			* @brief Entityid, cast from entt::entity.
			*/
			int entityID = -1;
		};

		/**
		* @breif Global View struct.
		*/
		struct View
		{
			/**
			* @brief Projection Matrix.
			*/
			glm::mat4 Projection = glm::mat4(1.0f);

			/**
			* @brief View Matrix.
			*/
			glm::mat4 View = glm::mat4(1.0f);

			/**
			* @brief inverse of View Matrix.
			*/
			glm::mat4 InView = glm::mat4(1.0f);

			/**
			* @brief SceneTexturesize(ViewportSize) in component xy, 1.0f / SceneTexturesize in component zw.
			*/
			glm::vec4 SceneTextureSize = glm::vec4(1.0f);

			/**
			* @brief WindowSize(ApplicationSize) in component xy, 1.0f / WindowSize in component zw.
			*/
			glm::vec4 WindowSize = glm::vec4(1.0f);
		};

		/**
		* @breif Global Input struct.
		*/
		struct Input
		{
			/**
			* @brief Viewport's MousePos in component xy, 1.0f / MousePos in component zw.
			*/
			glm::vec4 MousePos = glm::vec4(1.0f);

			/**
			* @brief Game's time.
			*/
			float GameTime = 0.0f;

			/**
			* @brief Frame's time.
			*/
			float FrameTime = 0.0f;
		};

	}

	void PreRenderer::CreateDescriptorSet()
	{
		DescriptorSetBuilder{ this }
		.AddPushConstant<PreR::PushConstant>()
		.AddBuffer<PreR::View>(0, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
		.AddBuffer<PreR::Input>(0, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
		.Build();
	}

	void PreRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.UpdateBuffer();

		auto& [invViewMatrix, projectionMatrix] = GetActiveCameraMatrix(frameInfo);
		ImVec2 sceneTextureSize = SlateSystem::GetRegister()->GetViewPort()->GetPanelSize();
		VkExtent2D windowSize = m_Device->GetSwapChainSupport().surfaceSize;

		PreR::View view;
		view.Projection = projectionMatrix;
		view.View = glm::inverse(invViewMatrix);
		view.InView = invViewMatrix;
		view.SceneTextureSize = { sceneTextureSize.x, sceneTextureSize.y, 1.0f / sceneTextureSize.x, 1.0f / sceneTextureSize.y };
		view.WindowSize = { windowSize.width, windowSize.height, 1.0f / windowSize.width, 1.0 / windowSize.height };

		m_Buffers[{0, 0}]->WriteToBuffer(&view);
		m_Buffers[{0, 0}]->Flush();

		auto& [x, y] = SlateSystem::GetRegister()->GetViewPort()->GetMousePosInViewport();

		PreR::Input input;
		input.GameTime = ts.gt();
		input.FrameTime = ts.ft();
		input.MousePos = glm::vec4((float)x, (float)y, 1.0f / float(x), 1.0f / float(y));

		m_Buffers[{0, 1}]->WriteToBuffer(&input);
		m_Buffers[{0, 1}]->Flush();
	}
}