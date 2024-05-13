#include "Pchheader.h"
#include "runtime_memory_block.h"
#include "Core/Reflect/TypeReflect.h"

namespace scl {
    
    runtime_memory_block::~runtime_memory_block()
    {
        free(begin_);
    }

    void runtime_memory_block::AddElement(const std::string& name, const std::string& type)
    {
        if(object_.find(name) != object_.end())
        {
            std::stringstream ss;
            ss << "runtime_memory_list:: add failed: already has the element: " << name;
            
            SPIECS_CORE_WARN(ss.str());
            return;
        }
        
        object_[name] = size;
        size += StrType2Size(type);
    }

    void runtime_memory_block::Build()
    {
        if(begin_) free(begin_);
        begin_ = malloc(size);
    }

    void runtime_memory_block::for_each(std::function<void(const std::string& name, void* pt)> fn)
    {
        for(auto& pair : object_)
        {
            void* it = begin_ + pair.second;
            fn(pair.first, it);
        }
    }
}
