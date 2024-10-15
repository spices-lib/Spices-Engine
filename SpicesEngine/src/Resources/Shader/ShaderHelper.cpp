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
}