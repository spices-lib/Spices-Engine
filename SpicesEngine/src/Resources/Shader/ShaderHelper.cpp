/**
* @file ShaderHelper.cpp.
* @brief The ShaderHelper Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ShaderHelper.h"

namespace Spices {

	std::string ShaderHelper::ToString(ShaderStage stage)
	{
		SPICES_PROFILE_ZONE;

		switch (stage)
		{
		case Spices::ShaderStage::vert:
			return "vert";
		case Spices::ShaderStage::geom:
			return "geom";
		case Spices::ShaderStage::tesc:
			return "tesc";
		case Spices::ShaderStage::tese:
			return "tese";
		case Spices::ShaderStage::frag:
			return "frag";
		case Spices::ShaderStage::task:
			return "task";
		case Spices::ShaderStage::mesh:
			return "mesh";
		case Spices::ShaderStage::rgen:
			return "rgen";
		case Spices::ShaderStage::rint:
			return "rint";
		case Spices::ShaderStage::rchit:
			return "rchit";
		case Spices::ShaderStage::rahit:
			return "rahit";
		case Spices::ShaderStage::rmiss:
			return "rmiss";
		case Spices::ShaderStage::comp:
			return "comp";
		case Spices::ShaderStage::glsl:
			return "glsl";
		default:
			SPICES_CORE_WARN("Not supported shader stage");
			return "none";
		}
	}

	ShaderStage ShaderHelper::ToStage(std::string stage)
	{
		SPICES_PROFILE_ZONE;

		if (stage == "vert")
		{
			return ShaderStage::vert;
		}
		else if (stage == "geom")
		{
			return ShaderStage::geom;
		}
		else if (stage == "tesc")
		{
			return ShaderStage::tesc;
		}
		else if (stage == "tese")
		{
			return ShaderStage::tese;
		}
		else if (stage == "frag")
		{
			return ShaderStage::frag;
		}
		else if (stage == "task")
		{
			return ShaderStage::task;
		}
		else if (stage == "mesh")
		{
			return ShaderStage::mesh;
		}
		else if (stage == "rgen")
		{
			return ShaderStage::rgen;
		}
		else if (stage == "rint")
		{
			return ShaderStage::rint;
		}
		else if (stage == "rchit")
		{
			return ShaderStage::rchit;
		}
		else if (stage == "rahit")
		{
			return ShaderStage::rahit;
		}
		else if (stage == "rmiss")
		{
			return ShaderStage::rmiss;
		}
		else if (stage == "comp")
		{
			return ShaderStage::comp;
		}
		else if (stage == "glsl")
		{
			return ShaderStage::glsl;
		}
		else
		{
			SPICES_CORE_WARN("Not supported shader stage");
			return ShaderStage::vert;
		}
	}

	shaderc_shader_kind ShaderHelper::ToShaderCKind(ShaderStage stage)
	{
		SPICES_PROFILE_ZONE;

		switch (stage)
		{
		case Spices::ShaderStage::vert:
			return shaderc_shader_kind::shaderc_vertex_shader;
		case Spices::ShaderStage::geom:
			return shaderc_shader_kind::shaderc_geometry_shader;
		case Spices::ShaderStage::tesc:
			return shaderc_shader_kind::shaderc_tess_control_shader;
		case Spices::ShaderStage::tese:
			return shaderc_shader_kind::shaderc_tess_evaluation_shader;
		case Spices::ShaderStage::frag:
			return shaderc_shader_kind::shaderc_fragment_shader;
		case Spices::ShaderStage::task:
			return shaderc_shader_kind::shaderc_task_shader;
		case Spices::ShaderStage::mesh:
			return shaderc_shader_kind::shaderc_mesh_shader;
		case Spices::ShaderStage::rgen:
			return shaderc_shader_kind::shaderc_raygen_shader;
		case Spices::ShaderStage::rint:
			return shaderc_shader_kind::shaderc_intersection_shader;
		case Spices::ShaderStage::rchit:
			return shaderc_shader_kind::shaderc_closesthit_shader;
		case Spices::ShaderStage::rahit:
			return shaderc_shader_kind::shaderc_anyhit_shader;
		case Spices::ShaderStage::rmiss:
			return shaderc_shader_kind::shaderc_miss_shader;
		case Spices::ShaderStage::comp:
			return shaderc_shader_kind::shaderc_compute_shader;
		default:
			SPICES_CORE_WARN("Not supported shader stage");
			return shaderc_shader_kind::shaderc_glsl_infer_from_source;
		}
	}

	VkShaderStageFlagBits ShaderHelper::ToFlagBits(ShaderStage stage)
	{
		SPICES_PROFILE_ZONE;

		switch (stage)
		{
		case Spices::ShaderStage::vert:
			return VK_SHADER_STAGE_VERTEX_BIT;
		case Spices::ShaderStage::geom:
			return VK_SHADER_STAGE_GEOMETRY_BIT;
		case Spices::ShaderStage::tesc:
			return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		case Spices::ShaderStage::tese:
			return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		case Spices::ShaderStage::frag:
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		case Spices::ShaderStage::task:
			return VK_SHADER_STAGE_TASK_BIT_EXT;
		case Spices::ShaderStage::mesh:
			return VK_SHADER_STAGE_MESH_BIT_EXT;
		case Spices::ShaderStage::rgen:
			return VK_SHADER_STAGE_RAYGEN_BIT_KHR;
		case Spices::ShaderStage::rint:
			return VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
		case Spices::ShaderStage::rchit:
			return VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
		case Spices::ShaderStage::rahit:
			return VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
		case Spices::ShaderStage::rmiss:
			return VK_SHADER_STAGE_MISS_BIT_KHR;
		case Spices::ShaderStage::comp:
			return VK_SHADER_STAGE_COMPUTE_BIT;
		default:
			SPICES_CORE_WARN("Not supported shader stage");
			return VK_SHADER_STAGE_VERTEX_BIT;
		}
	}
}