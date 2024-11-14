#include "Pchheader.h"
#include "GltfUtils.h"

namespace Spices {

	std::string GltfUtils::ToString(GltfObjectType type)
	{
		SPICES_PROFILE_ZONE;

		switch (type)
		{
			case GltfObjectType::asset       : return "asset";
			case GltfObjectType::accessors   : return "accessors";
			case GltfObjectType::animations  : return "animations";
			case GltfObjectType::buffers     : return "buffers";
			case GltfObjectType::bufferViews : return "bufferViews";
			case GltfObjectType::images      : return "images";
			case GltfObjectType::materials   : return "materials";
			case GltfObjectType::meshes      : return "meshes";
			case GltfObjectType::nodes       : return "nodes";
			case GltfObjectType::samplers    : return "samplers";
			case GltfObjectType::scenes      : return "scenes";
			case GltfObjectType::scene       : return "scene";
			case GltfObjectType::textures    : return "textures";
		}
	}

	GltfObjectType GltfUtils::ToType(const std::string& type)
	{
		SPICES_PROFILE_ZONE;

		     if (type == "asset"      ) return GltfObjectType::asset;
		else if (type == "accessors"  ) return GltfObjectType::accessors;
		else if (type == "animations" ) return GltfObjectType::animations;
		else if (type == "buffers"    ) return GltfObjectType::buffers;
		else if (type == "bufferViews") return GltfObjectType::bufferViews;
		else if (type == "images"     ) return GltfObjectType::images;
		else if (type == "materials"  ) return GltfObjectType::materials;
		else if (type == "meshes"     ) return GltfObjectType::meshes;
		else if (type == "nodes"      ) return GltfObjectType::nodes;
		else if (type == "samplers"   ) return GltfObjectType::samplers;
		else if (type == "scenes"     ) return GltfObjectType::scenes;
		else if (type == "scene"      ) return GltfObjectType::scene;
		else if (type == "textures"   ) return GltfObjectType::textures;
	}
}