/**
* @file runtime_memory_block.cpp.
* @brief The runtime_memory_block Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "runtime_memory_block.h"
#include "Core/Reflect/TypeReflect.h"

namespace scl {
    
    runtime_memory_block::~runtime_memory_block()
    {
        /**
        * @brief Free the memoty that handled.
        */
        if (begin_) free(begin_);
    }

    void runtime_memory_block::add_element(const std::string& name, const std::string& type)
    {
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
        /**
        * @brief Free the memory if begin_ has mallocked.
        */
        if(begin_) free(begin_);

        /**
        * @brief Malloc memory to begin_ with bytes_.
        */
        begin_ = malloc(bytes_);
    }

    void runtime_memory_block::for_each(std::function<bool(const std::string& name, void* pt)> fn)
    {
        /**
        * @brief Iter without order.
        */
        for(auto& pair : object_)
        {
            /**
            * @brief Move begin_ to correct position.
            */
            void* it = reinterpret_cast<char*>(begin_) + pair.second;

            /**
            * @brief The function pointer of how to explain the memory with parameter.
            * @param[in] name The name of parameter.
            * @param[in] pt The pointer of start of parameter occupied.
            */
            if(fn(pair.first, it)) break;
        }
    }
}
