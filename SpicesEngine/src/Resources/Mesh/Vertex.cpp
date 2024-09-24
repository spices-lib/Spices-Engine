/**
* @file Vertex.cpp.
* @brief The Vertex Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "Vertex.h"

namespace Spices {

	void Meshlet::FromMeshopt(const meshopt_Meshlet& m, const meshopt_Bounds& bounds)
	{
		SPICES_PROFILE_ZONE;

		vertexOffset           = m.vertex_offset;
		primitiveOffset        = 0;                   /* @brief Need more work here. */
		nVertices              = m.vertex_count;
		nPrimitives            = m.triangle_count;
							   
		lod                    = 0;
						       
		boundSphere.c.x        = bounds.center[0];
		boundSphere.c.y        = bounds.center[1];
		boundSphere.c.z        = bounds.center[2];
		boundSphere.r          = bounds.radius;
							   
		clusterBoundSphere.c   = glm::vec3(0.0f);
		clusterBoundSphere.r   = 0.0f;
							   
		coneApex.x             = bounds.cone_apex[0];
		coneApex.y             = bounds.cone_apex[1];
		coneApex.z             = bounds.cone_apex[2];
						       
		coneAxis.x             = bounds.cone_axis[0];
		coneAxis.y             = bounds.cone_axis[1];
		coneAxis.z             = bounds.cone_axis[2];
						       
		coneCutoff             = bounds.cone_cutoff;
	}
}