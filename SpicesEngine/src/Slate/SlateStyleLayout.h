/**
* @file SlateStyleLayout.h.
* @brief The SlateStyleLayout Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace Spices {

	/**
	* @brief Enum of Slate StyleLayout
	*/
	enum class StyleLayoutEnum
	{
		Default          = 0,
		Animation        = 1,
		AnimationGraph   = 2,
		Paint            = 3,
		Rendering        = 4,
		VisualScripting  = 5,
		PhysicsAuthoring = 6,
	};

	/**
	* @brief Wrappers of Slate StyleLayout Functions.
	*/
	class SlateStyleLayout
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		SlateStyleLayout() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~SlateStyleLayout() = default;

		/**
		* @brief Get this single instance.
		* @return Returns this single instance.
		*/
		static std::shared_ptr<SlateStyleLayout> Get();

		/**
		* @brief Set Slate Layout.
		* @param[in] layout StyleLayoutEnum.
		*/
		void SetLayout(StyleLayoutEnum layout);

		/**
		* @brief Store Slate Layout to disk.
		*/
		void StoreLayout();

		/**
		* @brief Load Slate Layout in disk.
		*/
		void LoadLayout();

		/**
		* @brief Store Slate Layout Cache.
		*/
		void StoreLayoutCache();

		/**
		* @brief Load Slate Layout Cache.
		*/
		void LoadLayoutCache();

		/**
		* @brief Quick Store Slate Layout to disk.
		*/
		void QuickStoreLayout();

		/**
		* @brief Quick Load Slate Layout to disk.
		*/
		void QuickLoadLayout();

	private:

		/**
		* @brief SlateStyleLayout single instance.
		*/
		static std::shared_ptr<SlateStyleLayout> m_Layout;
	};
}