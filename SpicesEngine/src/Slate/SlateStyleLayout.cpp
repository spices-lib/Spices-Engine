/**
* @file SlateStyleLayout.cpp.
* @brief The SlateStyleLayout Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "SlateStyleLayout.h"
#include "Imgui/ImguiHelper.h"

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
	}

	void SlateStyleLayout::StoreLayout()
	{
		SPICES_PROFILE_ZONE;
	}

	void SlateStyleLayout::StoreLayoutInMemory()
	{
		SPICES_PROFILE_ZONE;

		m_LayoutCache = const_cast<char*>(ImGui::SaveIniSettingsToMemory(&m_LayoutCacheBytes));
	}

	void SlateStyleLayout::LoadLayoutInMemory()
	{
		SPICES_PROFILE_ZONE;

		ImGui::LoadIniSettingsFromMemory(m_LayoutCache, m_LayoutCacheBytes);
	}

	void SlateStyleLayout::QuickStoreLayout()
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;

		ss << "saved/QuickSlateLayout/";
		std::filesystem::create_directory(ss.str());

		ss << "QuickLayout.ini";
		ImGui::SaveIniSettingsToDisk(ss.str().c_str());
	}

	void SlateStyleLayout::QuickLoadLayout()
	{
		SPICES_PROFILE_ZONE;

		std::stringstream ss;
		ss << "saved/QuickSlateLayout/QuickLayout.ini";

		ImGui::LoadIniSettingsFromDisk(ss.str().c_str());
	}
}