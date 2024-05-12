#pragma once
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
	struct convert<Spiecs::Material::TextureParam>
	{
		static Node encode(const Spiecs::Material::TextureParam& param)
		{
			Node node;
			node.push_back(param.set);
			node.push_back(param.binding);
			node.push_back(param.index);
			node.push_back(param.textureType);
			node.push_back(param.texturePath);
			return node;
		}

		static bool decode(const Node& node, Spiecs::Material::TextureParam& param)
		{
			if (!node.IsSequence() || node.size() != 5)
			{
				return false;
			}
			
			param.set = node[0].as<uint32_t>();
			param.binding = node[1].as<uint32_t>();
			param.index = node[2].as<uint32_t>();
			param.textureType = node[3].as<std::string>();
			param.texturePath = node[4].as<std::string>();
		}
	};

	template<>
	struct convert<Spiecs::Material::ConstantParam>
	{
		static Node encode(const Spiecs::Material::ConstantParam& param)
		{
			Node node;
			node.push_back(param.set);
			node.push_back(param.binding);
			node.push_back(param.paramType);

			if     (param.paramType == "float4") node.push_back(std::any_cast<glm::vec4>(param.paramValue));
			else if(param.paramType == "float3") node.push_back(std::any_cast<glm::vec3>(param.paramValue));
			else if(param.paramType == "float2") node.push_back(std::any_cast<glm::vec2>(param.paramValue));
			else if(param.paramType == "float")  node.push_back(std::any_cast<float>(param.paramValue));
			else
			{
				std::stringstream ss;
				ss << "YAML::convert<Spiecs::Material::ConstantParam>: not supported paramType: " << param.paramType;
				
				SPIECS_CORE_WARN(ss.str());
			}
			
			return node;
		}

		static bool decode(const Node& node, Spiecs::Material::ConstantParam& param)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				std::stringstream ss;
				ss << "YAML::convert<Spiecs::Material::ConstantParam>: bad node size, current size is" << node.size() << ", required 4";
				
				SPIECS_CORE_WARN(ss.str());
				return false;
			}

			param.set = node[0].as<uint32_t>();
			param.binding = node[1].as<uint32_t>();
			param.paramType = node[2].as<std::string>();

			if     (param.paramType == "float4") param.paramValue = node[3].as<glm::vec4>();
			else if(param.paramType == "float3") param.paramValue = node[3].as<glm::vec3>();
			else if(param.paramType == "float2") param.paramValue = node[3].as<glm::vec2>();
			else if(param.paramType == "float")  param.paramValue = node[3].as<float>();
			else
			{
				std::stringstream ss;
				ss << "YAML::convert<Spiecs::Material::ConstantParam>: not supported paramType: " << param.paramType;
				
				SPIECS_CORE_WARN(ss.str());
			}

			return true;
		}
	};
}

namespace Spiecs {

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

	YAML::Emitter& operator<<(YAML::Emitter& out, const Material::TextureParam& p)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << p.set << p.binding << p.index << p.textureType << p.texturePath << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const Material::ConstantParam& p)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << p.set << p.binding << p.paramType;
		
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
		else
		{
			std::stringstream ss;
			ss << "YAML::operator<<: not supported paramType: " << p.paramType;
				
			SPIECS_CORE_WARN(ss.str());
			
			out << YAML::EndSeq;
		}

		out << YAML::EndSeq;
		return out;
	}
}