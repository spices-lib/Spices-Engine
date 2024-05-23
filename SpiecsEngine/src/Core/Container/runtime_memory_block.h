/**
* @file runtime_memory_block.h.
* @brief The runtime_memory_block Class Definitions.
* @author Spiecs.
*/

#pragma once
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
        size_t bytes_ = 0;

        /**
        * @brief The data information of the continue memory block handled.
        * Data: parameter name - parameter position offest with begin_.
        */
        std::unordered_map<std::string, size_t> object_;
        
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
        * @noto Not Unit Test, so do not use it.
        */
        void for_each(std::function<bool(const std::string& name, void* pt)> fn);

        /**
        * @brief Get value that explained by name.
        * @param[in] T the type of parameter.
        * @param[in] name The name of parameter.
        * @return Returns the value of parameter.
        */
        template<typename T>
        T& get_value(const std::string& name);
        
        /**
        * @brief Get the begin_.
        * @return Return the begin_.
        */
        void* get_addr() const { return begin_; };

        /**
        * @brief Get the bytes_.
        * @return Return the bytes_.
        */
        size_t get_bytes() const { return bytes_; };

        /**
        * @brief Get the size of object_.
        * @return Return the size of object_.
        */
        size_t size() { return object_.size(); };
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
            
            throw std::runtime_error(ss.str());
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

    template <typename T>
    T& runtime_memory_block::get_value(const std::string& name)
    {
        /**
        * @brief Only allow get value the memory block with parameter that already added.
        */
        if(object_.find(name) == object_.end())
        {
            std::stringstream ss;
            ss << "runtime_memory_list:: get_value failed: without the element: " << name;
            
            throw std::runtime_error(ss.str());
        }

        /**
        * @brief Offest the begin_ to correct position.
        */
        void* mem = reinterpret_cast<char*>(begin_) + object_[name];

        return *reinterpret_cast<T*>(mem);
    }
}
