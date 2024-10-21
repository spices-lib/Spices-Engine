/**
* @file RuntimeMemoryBlock.cpp.
* @brief The RuntimeMemoryBlock Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "RuntimeMemoryBlock.h"
#include "Core/Reflect/TypeReflect.h"

namespace scl {
    
    runtime_memory_block::~runtime_memory_block()
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Free the memoty that handled.
        */
        if (begin_) free(begin_);
    }

    void runtime_memory_block::add_element(const std::string& name, const std::string& type)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Only allow add to object_ with parameter that has not added.
        */
        if(object_.find(name) != object_.end())
        {
            std::stringstream ss;
            ss << "runtime_memory_block:: add failed: already has the element: " << name;
            
            SPICES_CORE_WARN(ss.str());
            return;
        }

        /**
        * @brief Recording current parameter's position to object_.
        */
        object_[name] = bytes_;

        /**
        * @brief Add to bytes_ with parameter type.
        */
        bytes_ += Spices::StrType2Size(type);
    }

    void runtime_memory_block::build()
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Free the memory if begin_ has mallocked.
        */
        if(begin_) free(begin_);

        /**
        * @brief Malloc memory to begin_ with bytes_.
        */
        begin_ = malloc(bytes_);
    }

    void runtime_memory_block::for_each(std::function<bool(const std::string& name, void* pt)> fn) const
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Iter without order.
        */
        for(auto& pair : object_)
        {
            /**
            * @brief Move begin_ to correct position.
            */
            void* it = static_cast<char*>(begin_) + pair.second;

            /**
            * @brief The function pointer of how to explain the memory with parameter.
            * @param[in] name The name of parameter.
            * @param[in] pt The pointer of start of parameter occupied.
            */
            if(fn(pair.first, it)) break;
        }
    }

    size_t runtime_memory_block::item_location(const std::string& name)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Return location if finded.
        */
        if (object_.find(name) == object_.end()) return UINT32_MAX;
        return object_[name];
    }

    bool runtime_memory_block::has_value(const std::string& name)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Return true if finded.
        */
        if (object_.find(name) != object_.end()) return true;
        return false;
    }
}
