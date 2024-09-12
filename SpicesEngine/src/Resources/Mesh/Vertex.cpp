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

		vertexOffset    = m.vertex_offset;
		nVertices       = m.vertex_count;
		primitiveOffset = 0;                   /* @brief Need more work here. */
		nPrimitives     = m.triangle_count;

		boundCenter.x   = bounds.center[0];
		boundCenter.y   = bounds.center[1];
		boundCenter.z   = bounds.center[2];

		boundRadius     = bounds.radius;

		coneApex.x      = bounds.cone_apex[0];
		coneApex.y      = bounds.cone_apex[1];
		coneApex.z      = bounds.cone_apex[2];
				        
		coneCutoff      = bounds.cone_cutoff;
				        
		coneAxis.x      = bounds.cone_axis[0];
		coneAxis.y      = bounds.cone_axis[1];
		coneAxis.z      = bounds.cone_axis[2];
	}
}