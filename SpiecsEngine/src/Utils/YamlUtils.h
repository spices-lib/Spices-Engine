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
			node.push_back(param.isInUse.value());
			node.push_back(param.texturePath);
			node.push_back(param.set);
			node.push_back(param.binding);
			node.push_back(param.index);
			node.push_back(param.constant);
			node.push_back(param.intensity);
			return node;
		}

		static bool decode(const Node& node, Spiecs::Material::TextureParam& param)
		{
			if (!node.IsSequence() || node.size() != 7)
			{
				return false;
			}

			param.isInUse = node[0].as<uint32_t>();
			param.texturePath = node[1].as<std::string>();
			param.set = node[2].as<uint32_t>();
			param.binding = node[3].as<uint32_t>();
			param.index = node[4].as<uint32_t>();
			param.constant = node[5].as<glm::vec3>();
			param.intensity = node[6].as<float>();
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
		out << YAML::BeginSeq << p.isInUse.value() << p.texturePath << p.set << p.binding << p.index << p.constant << p.intensity << YAML::EndSeq;
		return out;
	}
}