/**
* @file ImguiOperation.h
* @brief The ImguiOperation Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spices {

    /**
    * @brief The ImguiOperation Class.
    * This class defines how to render a Operation Panel.
    */
    class ImguiOperation : public ImguiSlate
    {
    public:

        /**
        * @brief Constructor Function.
        * @param[in] panelName The Slate's name to show.
        * @param[in] frameInfo The Frame Date that in use.
        */
        ImguiOperation(
            const std::string& panelName , 
            FrameInfo&         frameInfo
        ) 
            : ImguiSlate(panelName, frameInfo) 
        {}

        /**
        * @brief Destructor Function.
        */
        virtual ~ImguiOperation() override = default;

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
    };
}