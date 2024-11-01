/**
* @file ClassTraits.h.
* @brief The ClassTraits Class Definitions and Implementation.
* @author VisualGMQ.
*/

#pragma once
#include "Core/Core.h"
#include "FieldTraits.h"
#include "Core/Container/Tuple.h"
#include <string>
namespace Spices {

    /**
    * @brief class property/function type.
    */
	enum class ClassItemType
	{
		UnSetted  = 0,       // unset type.
 		Member    = 1,       // is a member.
		NonMember = 2        // not a member.
	};

    /**
    * @brief Declare of class_traits.
    */
	template<typename>
	struct class_traits;

#define UCLASS()                                                                                                                                       \
    /**                                                                                                                                                \
    * @brief specific template class of CLASS_SCOPE.                                                                                                   \
    */                                                                                                                                                 \
	template<>                                                                                                                                         \
	struct Spices::class_traits<CLASS_SCOPE>                                                                                                           \
	{                                                                                                                                                  \
        /**                                                                                                                                            \
        * @brief Instance pointer.                                                                                                                     \
        */                                                                                                                                             \
		CLASS_SCOPE* m_Instance;                                                                                                                       \
                                                                                                                                                       \
        /**                                                                                                                                            \
        * @brief Construct Function.                                                                                                                   \
        * @param[in] inst Specific Class instance.                                                                                                     \
        */                                                                                                                                             \
		class_traits(CLASS_SCOPE& inst)                                                                                                                \
		{                                                                                                                                              \
			m_Instance = &inst;                                                                                                                        \
		}                                                                                                                                              \
                                                                                                                                                       \
        /**                                                                                                                                            \
        * @brief Decpnstruct Function.                                                                                                                 \
        */                                                                                                                                             \
        virtual ~class_traits() = default;                                                                                                             \
                                                                                                                                                       \
        /**                                                                                                                                            \
        * @brief Get Instance.                                                                                                                         \
        * @return Returns Instance reference.                                                                                                          \
        */                                                                                                                                             \
        auto& GetInst() { return *m_Instance; }                                                                                                        \
                                                                                                                                                       \
        /**                                                                                                                                            \
        * @brief Get field_traits instance of property.                                                                                                \
        * @tparam T Property Type.                                                                                                                     \
        * @param[in] name Property Type.                                                                                                               \
        * @return Returns tuple of type and pointer.                                                                                                   \
        */                                                                                                                                             \
        template<typename T>                                                                                                                           \
        std::tuple<ClassItemType, void*> GetProperty(const std::string& name)                                                                          \
        {                                                                                                                                              \
            ClassItemType type = ClassItemType::UnSetted;                                                                                              \
                                                                                                                                                       \
            field_traits<T CLASS_SCOPE::*>* member_trait;                                                                                              \
            field_traits<T*>* nonmember_trait;                                                                                                         \
                                                                                                                                                       \
            scl::IterTuple(properties, [&](auto& elem) {                                                                                               \
                if (elem.name == name)                                                                                                                 \
                {                                                                                                                                      \
                    if (elem.is_member())                                                                                                              \
                    {                                                                                                                                  \
                        type = ClassItemType::Member;                                                                                                  \
                                                                                                                                                       \
                        if constexpr (std::is_same_v<decltype(&elem), const field_traits<T CLASS_SCOPE::*>*>)                                          \
                        {                                                                                                                              \
                            member_trait = const_cast<field_traits<T CLASS_SCOPE::*>*>(&elem);                                                         \
                        }                                                                                                                              \
                    }                                                                                                                                  \
                    else                                                                                                                               \
                    {                                                                                                                                  \
                        type = ClassItemType::NonMember;                                                                                               \
                                                                                                                                                       \
                        if constexpr (std::is_same_v<decltype(&elem), const field_traits<T*>*>)                                                        \
                        {                                                                                                                              \
                            nonmember_trait = const_cast<field_traits<T*>*>(&elem);                                                                    \
                        }                                                                                                                              \
                    }                                                                                                                                  \
                }                                                                                                                                      \
            });                                                                                                                                        \
                                                                                                                                                       \
            switch (type)                                                                                                                              \
            {                                                                                                                                          \
            case ClassItemType::Member:                                                                                                                \
                return std::make_tuple(type, (void*)member_trait);                                                                                     \
            case ClassItemType::NonMember:                                                                                                             \
                return std::make_tuple(type, (void*)nonmember_trait);                                                                                  \
            case ClassItemType::UnSetted:                                                                                                              \
                std::stringstream ss;                                                                                                                  \
                ss << "Property: " << name << " not finded in this class";                                                                             \
                SPICES_CORE_ERROR(ss.str());                                                                                                           \
                return std::make_tuple(type, nullptr);                                                                                                 \
            }                                                                                                                                          \
        }                                                                                                                                              \
                                                                                                                                                       \
        /**                                                                                                                                            \
        * @brief Get Property by name.                                                                                                                 \
        * @tparam T Property Type.                                                                                                                     \
        * @param[in] property Property Name.                                                                                                           \
        * @return Returns Property pointer.                                                                                                            \
        */                                                                                                                                             \
        template<typename T>                                                                                                                           \
        T* GetProperty_V(const std::string& name)                                                                                                      \
        {                                                                                                                                              \
            SPICES_PROFILE_ZONE;                                                                                                                       \
                                                                                                                                                       \
            auto[type, rowPointer] = GetProperty<T>(name);                                                                                             \
                                                                                                                                                       \
            switch (type)                                                                                                                              \
            {                                                                                                                                          \
                case ClassItemType::Member:                                                                                                            \
                {                                                                                                                                      \
                    field_traits<T CLASS_SCOPE::*>* pointer = static_cast<field_traits<T CLASS_SCOPE::*>*>(rowPointer);                                \
                    return (T*)((char*)m_Instance + pointer->offset);                                                                                  \
                }                                                                                                                                      \
                case ClassItemType::NonMember:                                                                                                         \
                {                                                                                                                                      \
                    field_traits<T*>* pointer = static_cast<field_traits<T*>*>(rowPointer);                                                            \
                    return pointer->pointer;                                                                                                           \
                }                                                                                                                                      \
                case ClassItemType::UnSetted:                                                                                                          \
                {                                                                                                                                      \
                    std::stringstream ss;                                                                                                              \
                    ss << "Property: " << name << " not finded in this class";                                                                         \
                    SPICES_CORE_ERROR(ss.str());                                                                                                       \
                    return nullptr;                                                                                                                    \
                }                                                                                                                                      \
            }                                                                                                                                          \
        }                                                                                                                                              \
       
#define UCONSTRUCT(T, N)      field_traits<T>{ nullptr, #N }
#define UDECONSTRUCT(T, N)    field_traits<T>{ nullptr, #N }
#define UFUNCTIONS(...)       static constexpr auto functions  = std::make_tuple(__VA_ARGS__);
#define UPROPERTYS(...)       static constexpr auto properties = std::make_tuple(__VA_ARGS__);
#define UFUNCTION_T(T, N)     field_traits<T>{ nullptr, #N }
#define UFUNCTION(N)          field_traits<decltype(&CLASS_SCOPE::N)>{ nullptr, #N }
#define UPROPERTY(N)          field_traits<decltype(&CLASS_SCOPE::N)>{ nullptr, #N, offsetof(CLASS_SCOPE, N) }
#define UPROPERTY_S(N)        field_traits<decltype(&CLASS_SCOPE::N)>{ &CLASS_SCOPE::N, #N, 0 }
#define END_CLASS             };

	template<typename T>
	auto class_traits_i(T& inst)
	{
		auto traits = class_traits<T>(inst);

		return std::move(traits);
	}

}