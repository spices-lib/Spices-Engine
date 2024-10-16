#pragma once
#include "Core/Core.h"

#include <yaml-cpp/yaml.h>
#include "Resources/Material/Material.h"
#include <glm/glm.hpp>

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();

			return true;
		}
	};

	template<>
	struct convert<Spices::TextureParam>
	{
		static Node encode(const Spices::TextureParam& param)
		{
			Node node;
			node.push_back(param.textureType);
			node.push_back(param.texturePath);
			return node;
		}

		static bool decode(const Node& node, Spices::TextureParam& param)
		{
			if (!node.IsSequence() || node.size() != 2)
			{
				return false;
			}
			
			param.textureType   = node[0].as<std::string>();
			param.texturePath   = node[1].as<std::string>();

			return true;
		}
	};

	template<>
	struct convert<Spices::ConstantParam>
	{
		static Node encode(const Spices::ConstantParam& param)
		{
			Node node;
			node.push_back(param.paramType);

			if     (param.paramType == "float4") node.push_back(std::any_cast<glm::vec4>(param.paramValue));
			else if(param.paramType == "float3") node.push_back(std::any_cast<glm::vec3>(param.paramValue));
			else if(param.paramType == "float2") node.push_back(std::any_cast<glm::vec2>(param.paramValue));
			else if(param.paramType == "float")  node.push_back(std::any_cast<float>(param.paramValue));
			else if(param.paramType == "int")    node.push_back(std::any_cast<int>(param.paramValue));
			else if(param.paramType == "bool")   node.push_back(std::any_cast<bool>(param.paramValue));
			else
			{
				std::stringstream ss;
				ss << "YAML::convert<Spices::Material::ConstantParam>: not supported paramType: " << param.paramType;
				
				SPICES_CORE_WARN(ss.str());
			}
			
			return node;
		}

		static bool decode(const Node& node, Spices::ConstantParam& param)
		{
			if (!node.IsSequence() || node.size() != 2)
			{
				std::stringstream ss;
				ss << "YAML::convert<Spices::Material::ConstantParam>: bad node size, current size is" << node.size() << ", required 4";
				
				SPICES_CORE_WARN(ss.str());
				return false;
			}

			param.paramType  = node[0].as<std::string>();

			if     (param.paramType == "float4") param.paramValue = node[1].as<glm::vec4>();
			else if(param.paramType == "float3") param.paramValue = node[1].as<glm::vec3>();
			else if(param.paramType == "float2") param.paramValue = node[1].as<glm::vec2>();
			else if(param.paramType == "float")  param.paramValue = node[1].as<float>();
			else if(param.paramType == "int")    param.paramValue = node[1].as<int>();
			else if(param.paramType == "bool")   param.paramValue = node[1].as<bool>();
			else
			{
				std::stringstream ss;
				ss << "YAML::convert<Spices::Material::ConstantParam>: not supported paramType: " << param.paramType;
				
				SPICES_CORE_WARN(ss.str());
			}

			return true;
		}
	};
}

namespace Spices {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const TextureParam& p)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << p.textureType << p.texturePath << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const ConstantParam& p)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << p.paramType;
		
		if(p.paramType == "float4")
		{
			out << std::any_cast<glm::vec4>(p.paramValue);
		}
		else if(p.paramType == "float3")
		{
			out << std::any_cast<glm::vec3>(p.paramValue);
		}
		else if(p.paramType == "float2")
		{
			out << std::any_cast<glm::vec2>(p.paramValue);
		}
		else if(p.paramType == "float")
		{
			out << std::any_cast<float>(p.paramValue);
		}
		else if(p.paramType == "int")
		{
			out << std::any_cast<int>(p.paramValue);
		}
		else if (p.paramType == "bool")
		{
			out << std::any_cast<bool>(p.paramValue);
		}
		else
		{
			std::stringstream ss;
			ss << "YAML::operator<<: not supported paramType: " << p.paramType;
				
			SPICES_CORE_WARN(ss.str());
			
			out << YAML::EndSeq;
		}

		out << YAML::EndSeq;
		return out;
	}
}