/**
* @file runtime_memory_block.h.
* @brief The runtime_memory_block Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Log/Log.h"
#include "Core/Library/ClassLibrary.h"

namespace scl {

    /**
    * @brief The container is wapper of a continue memory block.
    * Used in Material::BuildMaterial(), helps to update buffer.
    */
    class runtime_memory_block
    {
    private:

        /**
        * @brief The begin pointer of the continue memory block handled.
        */
        void* begin_ = nullptr;

        /**
        * @brief The bytes of the continue memory block handled.
        */
        uint32_t bytes_ = 0;

        /**
        * @brief The data information of the continue memory block handled.
        * Data: parameter name - parameter position offest with begin_.
        */
        std::unordered_map<std::string, uint32_t> object_;
        
    public:

        /**
        * @brief Constructor Function.
        */
        runtime_memory_block() {};

        /**
        * @brief Destructor Function.
        */
        virtual ~runtime_memory_block();

        /**
        * @brief Add a element to object_, means a memory block will be occupied with given param type.
        * @param[in] name The name of parameter.
        * @param[in] type The type of parameter.
        */
        void add_element(const std::string& name, const std::string& type);

        /**
        * @brief Malloc a memory to begin_.
        */
        void build();

        /**
        * @brief Fill in a memory with given data. 
        * @param[in] T The type of parameter.
        * @param[in] name The name of parameter.
        * @param[in] value The value of parameter.
        */
        template<typename T>
        void explain_element(const std::string& name, const T& value);

        /**
        * @brief Iter the object_ and call explain_element() to filling data.
        * @param[in] fn The function pointer of how to fill in data.
        */
        void for_each(std::function<void(const std::string& name, void* pt)> fn);

        /**
        * @brief Get the begin_.
        * @return Return the begin_.
        */
        void* get_addr() { return begin_; };

        /**
        * @brief Get the bytes_.
        * @return Return the bytes_.
        */
        uint32_t get_bytes() { return bytes_; };

        /**
        * @brief Get the size of object_.
        * @return Return the size of object_.
        */
        uint32_t size() { return object_.size(); };
    };

    template <typename T>
    void runtime_memory_block::explain_element(const std::string& name, const T& value)
    {
        /**
        * @brief Only allow explain the memory block with parameter that already added.
        */
        if(object_.find(name) == object_.end())
        {
            std::stringstream ss;
            ss << "runtime_memory_list:: explain failed: without the element: " << name;
            
            SPIECS_CORE_WARN(ss.str());
            return;
        }

        /**
        * @brief Offest the begin_ to correct position.
        */
        void* mem = reinterpret_cast<char*>(begin_) + object_[name];

        /**
        * @brief Writing the value of parameter.
        */
        *reinterpret_cast<T*>(mem) = value;
    }
}
