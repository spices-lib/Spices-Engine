/**
* @file SlateStyleLayout.cpp.
* @brief The SlateStyleLayout Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "SlateStyleLayout.h"
#include "Imgui/ImguiHelper.h"
#include "Core/Library/FileLibrary.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "..\Core\Thread\ThreadModel.h"

namespace Spices {

	std::shared_ptr<SlateStyleLayout> SlateStyleLayout::m_Layout = nullptr;

	std::shared_ptr<SlateStyleLayout> SlateStyleLayout::Get()
	{
		SPICES_PROFILE_ZONE;

		if (!m_Layout)
		{
			m_Layout = std::make_shared<SlateStyleLayout>();
		}

		return m_Layout;
	}

	void SlateStyleLayout::SetLayout(StyleLayoutEnum layout)
	{
		SPICES_PROFILE_ZONE;

		switch (layout)
		{
			case StyleLayoutEnum::Default:
			{
				std::stringstream ss;
				ss << SPICES_ENGINE_ASSETS_PATH << "SlateLayout/DefaultLayout.ini";
				ImGui::LoadIniSettingsFromDisk(ss.str().c_str());
			}
			default:
			{
				std::stringstream ss;
				ss << SPICES_ENGINE_ASSETS_PATH << "SlateLayout/DefaultLayout.ini";
				ImGui::LoadIniSettingsFromDisk(ss.str().c_str());
			}
		}
	}

	void SlateStyleLayout::LoadLayout()
	{
		SPICES_PROFILE_ZONE;

		std::string filepath = FileLibrary::FileLibrary_OpenInExplore(
			"Slate Layout (*.ini)\0*.ini\0",
			glfwGetWin32Window((GLFWwindow*)VulkanRenderBackend::GetState().m_Windows)
		);

		if (!filepath.empty())
		{
			AsyncTask(ThreadPoolEnum::Game, [=]() {
				ImGui::LoadIniSettingsFromDisk(filepath.c_str());
			});
		}
	}

	void SlateStyleLayout::StoreLayout()
	{
		SPICES_PROFILE_ZONE;

		std::string filepath = FileLibrary::FileLibrary_SaveInExplore(
			"Slate Layout (*.ini)\0*.ini\0",
			glfwGetWin32Window((GLFWwindow*)VulkanRenderBackend::GetState().m_Windows)
		);

		filepath.append(".ini");

		if (!filepath.empty())
		{
			AsyncTask(ThreadPoolEnum::Game, [=]() {
				ImGui::SaveIniSettingsToDisk(filepath.c_str());
			});
		}
	}

	void SlateStyleLayout::StoreLayoutCache()
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;

		ss << "saved/SlateCache/";
		std::filesystem::create_directory(ss.str());

		ss << "CacheLayout.ini";
		ImGui::SaveIniSettingsToDisk(ss.str().c_str());
	}

	void SlateStyleLayout::LoadLayoutCache()
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;
		ss << "saved/SlateCache/CacheLayout.ini";

		ImGui::LoadIniSettingsFromDisk(ss.str().c_str());
	}

	void SlateStyleLayout::QuickStoreLayout()
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;

		ss << "saved/SlateCache/";
		std::filesystem::create_directory(ss.str());

		ss << "QuickLayout.ini";
		ImGui::SaveIniSettingsToDisk(ss.str().c_str());
	}

	void SlateStyleLayout::QuickLoadLayout()
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;
		ss << "saved/SlateCache/QuickLayout.ini";

		ImGui::LoadIniSettingsFromDisk(ss.str().c_str());
	}
}