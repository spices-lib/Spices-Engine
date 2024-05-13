#pragma once
#include "Core/Log/Log.h"
#include "Core/Library/ClassLibrary.h"

namespace scl {

    class runtime_memory_block
    {
    private:
        void* begin_ = nullptr;
        uint32_t size = 0;
        std::unordered_map<std::string, uint32_t> object_;
        
    public:
        runtime_memory_block() {};
        virtual ~runtime_memory_block();
        
        void AddElement(const std::string& name, const std::string& type);

        void Build();

        template<typename T>
        void ExplainElement(const std::string& name, const T& value);

        void for_each(std::function<void(const std::string& name, void* pt)> fn);

        void* GetAddr() { return begin_; };
    };

    template <typename T>
    void runtime_memory_block::ExplainElement(const std::string& name, const T& value)
    {
        if(object_.find(name) == object_.end())
        {
            std::stringstream ss;
            ss << "runtime_memory_list:: explain failed: without the element: " << name;
            
            SPIECS_CORE_WARN(ss.str());
            return;
        }
        void* mem = begin_ + object_[name];
        *mem = value;
    }
}
