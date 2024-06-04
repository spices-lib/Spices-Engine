/**
* @file ImguiConsole.h
* @brief The ImguiConsole Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spiecs {

	/**
	* @brief Forward Declear.
	*/
	class Console;

	/**
	* @brief The ImguiConsole Class.
	* This class defines how to render a Console.
	*/
	class ImguiConsole : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		* @param[in] console The shared pointer of Console.
		*/
		ImguiConsole(
			const std::string&       panelName , 
			FrameInfo&               frameInfo , 
			std::shared_ptr<Console> console
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiConsole() {};

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


		/**
		* @brief Input Text Call back function.
		* @param[in] data ImGuiInputTextCallbackData.
		* @return Returns a int.
		*/
		static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);

	private:

		/**
		* @brief The shared pointer of Console.
		*/
		std::shared_ptr<Console> m_Console;

		/**
		* @brief The Filter of console.
		*/
		ImGuiTextFilter m_Filter;

		/**
		* @brief The information level that console show.
		*/
		int m_Level = 0;

		/**
		* @brief The boolean of enable Cmd Input.
		*/
		bool m_EnableCmdInput = false;
	};
}