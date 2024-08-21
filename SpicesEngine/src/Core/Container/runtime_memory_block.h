/**
* @file runtime_memory_block.h.
* @brief The runtime_memory_block Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

namespace scl {

    /**
    * @brief The container is wrapper of a continue memory block.
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
        size_t bytes_ = 0;

        /**
        * @brief The data information of the continue memory block handled.
        * Data: parameter name - parameter position offset with begin_.
        */
        std::unordered_map<std::string, size_t> object_;
        
    public:

        /**
        * @brief Constructor Function.
        */
        runtime_memory_block() = default;

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
        * @tparam T The type of parameter.
        * @param[in] name The name of parameter.
        * @param[in] value The value of parameter.
        */
        template<typename T>
        void explain_element(const std::string& name, const T& value);

        /**
        * @brief Iter the object_ and call explain_element() to filling data.
        * @param[in] fn The function pointer of how to fill in data.
        * @noto Not Unit Test, so do not use it.
        */
        void for_each(std::function<bool(const std::string& name, void* pt)> fn) const;

        /**
        * @brief Get value that explained by name.
        * @tparam T the type of parameter.
        * @param[in] name The name of parameter.
        * @return Returns the value of parameter.
        */
        template<typename T>
        T& get_value(const std::string& name);
        
        /**
        * @brief Get the begin_.
        * @return Return the begin_.
        */
        void* get_addr() const { return begin_; }

        /**
        * @brief Get the bytes_.
        * @return Return the bytes_.
        */
        size_t get_bytes() const { return bytes_; }

        /**
        * @brief Get the size of object_.
        * @return Return the size of object_.
        */
        size_t size() const { return object_.size(); }

        /**
        * @brief Get item location in blocks.
        * @param[in] name Item Name.
        * @return Returns item location.
        */
        size_t item_location(const std::string& name);
    };

    template <typename T>
    void runtime_memory_block::explain_element(const std::string& name, const T& value)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Only allow explain the memory block with parameter that already added.
        */
        if(object_.find(name) == object_.end())
        {
            std::stringstream ss;
            ss << "runtime_memory_block:: explain failed: without the element: " << name;
            
            throw std::runtime_error(ss.str());
        }

        /**
        * @brief Offest the begin_ to correct position.
        */
        void* mem = static_cast<char*>(begin_) + object_[name];

        /**
        * @brief Writing the value of parameter.
        */
        *static_cast<T*>(mem) = value;
    }

    template <typename T>
    T& runtime_memory_block::get_value(const std::string& name)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Only allow get value the memory block with parameter that already added.
        */
        if(object_.find(name) == object_.end())
        {
            std::stringstream ss;
            ss << "runtime_memory_block:: get_value failed: without the element: " << name;
            
            throw std::runtime_error(ss.str());
        }

        /**
        * @brief Offest the begin_ to correct position.
        */
        void* mem = static_cast<char*>(begin_) + object_[name];

        return *static_cast<T*>(mem);
    }
}
