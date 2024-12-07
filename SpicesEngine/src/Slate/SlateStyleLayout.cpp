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

	void SlateStyleLayout::SetLayout(StyleLayout layout)
	{
		SPICES_PROFILE_ZONE;

		switch (layout)
		{
		case StyleLayout::Default:
			ImGui::LoadIniSettingsFromDisk("DefaultLayout.ini");
		default:
			ImGui::LoadIniSettingsFromDisk("DefaultLayout.ini");
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

	void SlateStyleLayout::LoadLayoutInMemoru()
	{
		SPICES_PROFILE_ZONE;

		ImGui::LoadIniSettingsFromMemory(m_LayoutCache, m_LayoutCacheBytes);
	}

	void SlateStyleLayout::QuickStoreLayout()
	{
		SPICES_PROFILE_ZONE;
	}

	void SlateStyleLayout::QuickLoadLayout()
	{
		SPICES_PROFILE_ZONE;
	}
}