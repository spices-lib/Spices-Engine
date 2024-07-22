#pragma once

#include "../HoudiniCore.h"
#include <sstream>

namespace HoudiniEngine {

    static std::string GetString(HAPI_Session& session, HAPI_StringHandle stringHandle)
    {
        if (stringHandle == 0)
        {
            return "";
        }

        int bufferLength;
        HAPI_GetStringBufLength(&session, stringHandle, &bufferLength);

        char* buffer = new char[bufferLength];
        HAPI_GetString(&session, stringHandle, buffer, bufferLength);

        std::string result(buffer);
        delete[] buffer;

        return result;
    }

	static void Marshalling_Geometry_Into_Houdini()
	{
        HAPI_Session session;

        HAPI_ThriftServerOptions serverOptions{ 0 };
        serverOptions.autoClose = true;
        serverOptions.timeoutMs = 3000.0f;

        HE_CHECK(HAPI_StartThriftNamedPipeServer(&serverOptions, "hapi", nullptr, nullptr))

        HAPI_SessionInfo sessionInfo = HAPI_SessionInfo_Create();

        HE_CHECK(HAPI_CreateThriftNamedPipeSession(&session, "hapi", &sessionInfo))

        HAPI_CookOptions cookOptions = HAPI_CookOptions_Create();
        HE_CHECK(HAPI_Initialize(&session, &cookOptions, true, -1, nullptr, nullptr, nullptr, nullptr, nullptr))

        HAPI_NodeId newNode;

        HE_CHECK(HAPI_CreateInputNode(&session, -1, &newNode, "Marshalling_Point_Clouds"))
        HE_CHECK(HAPI_CookNode(&session, newNode, &cookOptions))

        int cookStatus;
        HAPI_Result cookResult;

        do
        {
            cookResult = HAPI_GetStatus(&session, HAPI_STATUS_COOK_STATE, &cookStatus);
        } 
        while (cookStatus > HAPI_STATE_MAX_READY_STATE && cookResult == HAPI_RESULT_SUCCESS);
        HE_CHECK(cookResult)
        HE_CHECK_COOK(cookStatus)

        HAPI_PartInfo newNodePart = HAPI_PartInfo_Create();

        newNodePart.type = HAPI_PARTTYPE_MESH;
        newNodePart.faceCount = 1;
        newNodePart.vertexCount = 3;
        newNodePart.pointCount = 3;

        HE_CHECK(HAPI_SetPartInfo(&session, newNode, 0, &newNodePart))

        HAPI_AttributeInfo newNodePointInfo = HAPI_AttributeInfo_Create();
        newNodePointInfo.count = 3;
        newNodePointInfo.tupleSize = 3;
        newNodePointInfo.exists = true;
        newNodePointInfo.storage = HAPI_STORAGETYPE_FLOAT;
        newNodePointInfo.owner = HAPI_ATTROWNER_POINT;

        HE_CHECK(HAPI_AddAttribute(&session, newNode, 0, "P", &newNodePointInfo))

        float positions[9] = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f };

        HE_CHECK(HAPI_SetAttributeFloatData(&session, newNode, 0, "P", &newNodePointInfo, positions, 0, 3))

        int vertices[3] = { 0, 1, 2 };

        HE_CHECK(HAPI_SetVertexList(&session, newNode, 0, vertices, 0, 3))

        int face_counts[1] = { 3 };

        HE_CHECK(HAPI_SetFaceCounts(&session, newNode, 0, face_counts, 0, 1))

        char** strs = new char* [3];
        strs[0] = _strdup("strPoint1");
        strs[1] = _strdup("strPoint2");
        strs[2] = _strdup("strPoint3");

        newNodePointInfo.count = 3;
        newNodePointInfo.tupleSize = 1;
        newNodePointInfo.exists = true;
        newNodePointInfo.storage = HAPI_STORAGETYPE_STRING;
        newNodePointInfo.owner = HAPI_ATTROWNER_POINT;

        HE_CHECK(HAPI_AddAttribute(&session, newNode, 0, "srcData", &newNodePointInfo))
        HE_CHECK(HAPI_SetAttributeStringData(&session, newNode, 0, "srcData", &newNodePointInfo, (const char**)strs, 0, 3))
        HE_CHECK(HAPI_CommitGeo(&session, newNode));
        HE_CHECK(HAPI_SaveHIPFile(&session, "C:/Users/Administrator/Desktop/Marshalling_Point_Clouds.hip", false))
        delete[] strs;
        HE_CHECK(HAPI_Cleanup(&session))

        return;
	}

    static void Marshalling_Point_Clouds()
    {
        HAPI_Session session;

        HAPI_ThriftServerOptions serverOptions{ 0 };
        serverOptions.autoClose = true;
        serverOptions.timeoutMs = 3000.0f;

        HE_CHECK(HAPI_StartThriftNamedPipeServer(&serverOptions, "hapi", nullptr, nullptr))

        HAPI_SessionInfo sessionInfo = HAPI_SessionInfo_Create();

        HE_CHECK(HAPI_CreateThriftNamedPipeSession(&session, "hapi", &sessionInfo))

        HAPI_CookOptions cookOptions = HAPI_CookOptions_Create();
        HE_CHECK(HAPI_Initialize(&session, &cookOptions, true, -1, nullptr, nullptr, nullptr, nullptr, nullptr))

        HAPI_NodeId newNode;

        HE_CHECK(HAPI_CreateInputNode(&session, -1, &newNode, "Marshalling_Point_Clouds"))
        HE_CHECK(HAPI_CookNode(&session, newNode, &cookOptions))

        int cookStatus;
        HAPI_Result cookResult;

        do
        {
            cookResult = HAPI_GetStatus(&session, HAPI_STATUS_COOK_STATE, &cookStatus);
        }
        while(cookStatus > HAPI_STATE_MAX_READY_STATE && cookResult == HAPI_RESULT_SUCCESS);

        HE_CHECK(cookResult)
        HE_CHECK_COOK(cookStatus)

        HAPI_GeoInfo newNodeGeoInfo;
        HE_CHECK(HAPI_GetDisplayGeoInfo(&session, newNode, &newNodeGeoInfo))

        HAPI_NodeId sopNodeId = newNodeGeoInfo.nodeId;

        HAPI_PartInfo newNodePart = HAPI_PartInfo_Create();

        newNodePart.type = HAPI_PARTTYPE_MESH;
        newNodePart.faceCount = 0;
        newNodePart.vertexCount = 0;
        newNodePart.pointCount = 8;

        HE_CHECK(HAPI_SetPartInfo(&session, sopNodeId, 0, &newNodePart))

        HAPI_AttributeInfo newNodePointInfo = HAPI_AttributeInfo_Create();
        newNodePointInfo.count = 8;
        newNodePointInfo.tupleSize = 3;
        newNodePointInfo.exists = true;
        newNodePointInfo.storage = HAPI_STORAGETYPE_FLOAT;
        newNodePointInfo.owner = HAPI_ATTROWNER_POINT;

        HE_CHECK(HAPI_AddAttribute(&session, sopNodeId, 0, "P", &newNodePointInfo))

        float positions[24] = { 
            0.0f, 0.0f, 0.0f, 
            1.0f, 0.0f, 0.0f, 
            1.0f, 0.0f, 1.0f, 
            0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f
        };

        HE_CHECK(HAPI_SetAttributeFloatData(&session, sopNodeId, 0, "P", &newNodePointInfo, positions, 0, 8))
        HE_CHECK(HAPI_CommitGeo(&session, sopNodeId))
        HE_CHECK(HAPI_SaveHIPFile(&session, "C:/Users/Administrator/Desktop/Marshalling_Point_Clouds.hip", false))
        HE_CHECK(HAPI_Cleanup(&session))

        return;
    }


    static void Connecting_Assets()
    {
        HAPI_Session session;

        HAPI_ThriftServerOptions serverOptions{ 0 };
        serverOptions.autoClose = true;
        serverOptions.timeoutMs = 3000.0f;

        HE_CHECK(HAPI_StartThriftNamedPipeServer(&serverOptions, "hapi", nullptr, nullptr))

        HAPI_SessionInfo sessionInfo = HAPI_SessionInfo_Create();

        HE_CHECK(HAPI_CreateThriftNamedPipeSession(&session, "hapi", &sessionInfo))

        HAPI_CookOptions cookOptions = HAPI_CookOptions_Create();
        HE_CHECK(HAPI_Initialize(&session, &cookOptions, true, -1, nullptr, nullptr, nullptr, nullptr, nullptr))

        HAPI_NodeId newNode;

        HE_CHECK(HAPI_CreateInputNode(&session, -1, &newNode, "Connecting_Assets"))
        HE_CHECK(HAPI_CookNode(&session, newNode, &cookOptions))

        int cookStatus;
        HAPI_Result cookResult;

        do
        {
            cookResult = HAPI_GetStatus(&session, HAPI_STATUS_COOK_STATE, &cookStatus);
        }
        while(cookStatus > HAPI_STATE_MAX_READY_STATE && cookResult == HAPI_RESULT_SUCCESS);

        HE_CHECK(cookResult)
        HE_CHECK_COOK(cookStatus)

        HAPI_PartInfo newNodePart = HAPI_PartInfo_Create();

        newNodePart.type = HAPI_PARTTYPE_MESH;
        newNodePart.faceCount = 6;
        newNodePart.vertexCount = 24;
        newNodePart.pointCount = 8;

        HE_CHECK(HAPI_SetPartInfo(&session, newNode, 0 , &newNodePart))

        HAPI_AttributeInfo newNodePointInfo = HAPI_AttributeInfo_Create();
        newNodePointInfo.count = 8;
        newNodePointInfo.tupleSize = 3;
        newNodePointInfo.exists = true;
        newNodePointInfo.storage = HAPI_STORAGETYPE_FLOAT;
        newNodePointInfo.owner = HAPI_ATTROWNER_POINT;

        HE_CHECK(HAPI_AddAttribute(&session, newNode, 0, "P", &newNodePointInfo))

        float positions[24] = {
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 
            0.0f, 1.0f, 0.0f, 
            0.0f, 1.0f, 1.0f, 
            1.0f, 0.0f, 0.0f, 
            1.0f, 0.0f, 1.0f, 
            1.0f, 1.0f, 0.0f, 
            1.0f, 1.0f, 1.0f
        };

        HE_CHECK(HAPI_SetAttributeFloatData(&session, newNode, 0, "P", &newNodePointInfo, positions, 0, 8))

        int vertices[24] = {
            0, 2, 6, 4, 
            2, 3, 7, 6, 
            2, 0, 1, 3, 
            1, 5, 7, 3,
            5, 4, 6, 7, 
            0, 4, 5, 1
        };

        HE_CHECK(HAPI_SetVertexList(&session, newNode, 0, vertices, 0, 24))

        int face_counts[6] = {4, 4, 4, 4, 4, 4};
        HE_CHECK(HAPI_SetFaceCounts(&session, newNode, 0, face_counts, 0, 6))
        HE_CHECK(HAPI_CommitGeo(&session, newNode))

        HAPI_NodeId subdivideNode;
        HE_CHECK(HAPI_CreateNode(&session, -1, "Sop/subdivide", "Cube Subdivider", true, &subdivideNode))
        HE_CHECK(HAPI_ConnectNodeInput(&session, subdivideNode, 0, newNode, 0))
        HE_CHECK(HAPI_SaveHIPFile(&session, "C:/Users/Administrator/Desktop/Connecting_Assets.hip", false))
        HE_CHECK(HAPI_Cleanup(&session))

        return;
    }

    static void Curve_Node()
    {
        HAPI_Session session;

        HAPI_ThriftServerOptions serverOptions{ 0 };
        serverOptions.autoClose = true;
        serverOptions.timeoutMs = 3000.0f;

        HE_CHECK(HAPI_StartThriftNamedPipeServer(&serverOptions, "hapi", nullptr, nullptr))

        HAPI_SessionInfo sessionInfo = HAPI_SessionInfo_Create();

        HE_CHECK(HAPI_CreateThriftNamedPipeSession(&session, "hapi", &sessionInfo))

        HAPI_CookOptions cookOptions = HAPI_CookOptions_Create();
        HE_CHECK(HAPI_Initialize(&session, &cookOptions, true, -1, nullptr, nullptr, nullptr, nullptr, nullptr))

        HAPI_NodeId curveNode;

        HE_CHECK(HAPI_CreateNode(&session, -1, "sop/curve", "NURBS", false, &curveNode))
        HE_CHECK(HAPI_CookNode(&session, curveNode, &cookOptions))
        
        int cookStatus;
        HAPI_Result cookResult;

        do
        {
            cookResult = HAPI_GetStatus(&session, HAPI_STATUS_COOK_STATE, &cookStatus);
        }
        while(cookStatus > HAPI_STATE_MAX_READY_STATE && cookResult == HAPI_RESULT_SUCCESS);

        HE_CHECK(cookResult)
        HE_CHECK_COOK(cookStatus)

        HAPI_NodeInfo curveNodeInfo;
        HE_CHECK(HAPI_GetNodeInfo(&session, curveNode, &curveNodeInfo))

        HAPI_ParmInfo* paramInfos = new HAPI_ParmInfo[curveNodeInfo.parmCount];
        HE_CHECK(HAPI_GetParameters(&session, curveNode, paramInfos, 0, curveNodeInfo.parmCount))

        int coordsParmIndex = -1;
        int typeParmIndex = -1;
        for (int i = 0; i < curveNodeInfo.parmCount; i++)
        {
            std::string parmName = GetString(session, paramInfos[i].nameSH);
            if (parmName == "coords")
            {
                coordsParmIndex = i;
            }
            if (parmName == "outputtype")
            {
                typeParmIndex = i;
            }
        }

        if (coordsParmIndex == -1 || typeParmIndex == -1)
        {
            std::stringstream ss;
            ss << "Could not find coords/type parameter on curve node";

            SPICES_CORE_WARN(ss.str());
            HE_CHECK(HAPI_Cleanup(&session))
            return;
        }

        HAPI_ParmInfo parm;
        HE_CHECK(HAPI_GetParameters(&session, curveNode, &parm, typeParmIndex, 1))

        int typeValue = 1;
        HE_CHECK(HAPI_SetParmIntValues(&session, curveNode, &typeValue, parm.intValuesIndex, 1))

        HE_CHECK(HAPI_GetParameters(&session, curveNode, &parm, coordsParmIndex, 1))
        HE_CHECK(HAPI_SetParmStringValue(&session, curveNode, "-4, 0, 4, -4, 0, -4, 4, 0, -4, 4, 0, 4", parm.id, 0))
        HE_CHECK(HAPI_SaveHIPFile(&session, "C:/Users/Administrator/Desktop/Curve_Node.hip", true))

        delete[] paramInfos;
        HE_CHECK(HAPI_Cleanup(&session))

        return;
    }

    static void Groups_Sample()
    {
        HAPI_Session session;

        HAPI_ThriftServerOptions serverOptions{ 0 };
        serverOptions.autoClose = true;
        serverOptions.timeoutMs = 3000.0f;

        HE_CHECK(HAPI_StartThriftNamedPipeServer(&serverOptions, "hapi", nullptr, nullptr))

        HAPI_SessionInfo sessionInfo = HAPI_SessionInfo_Create();

        HE_CHECK(HAPI_CreateThriftNamedPipeSession(&session, "hapi", &sessionInfo))

        HAPI_CookOptions cookOptions = HAPI_CookOptions_Create();
        HE_CHECK(HAPI_Initialize(&session, &cookOptions, true, -1, nullptr, nullptr, nullptr, nullptr, nullptr))

        HAPI_NodeId cubeNode;

        HE_CHECK(HAPI_CreateInputNode(&session, -1, &cubeNode, "Cube"))
        HE_CHECK(HAPI_CookNode(&session, cubeNode, &cookOptions))

        int cookStatus;
        HAPI_Result cookResult;

        do
        {
            cookResult = HAPI_GetStatus(&session, HAPI_STATUS_COOK_STATE, &cookStatus);
        }
        while(cookStatus > HAPI_STATE_MAX_READY_STATE && cookResult == HAPI_RESULT_SUCCESS);

        HE_CHECK(cookResult)
        HE_CHECK_COOK(cookStatus)

        HAPI_PartInfo cubePart = HAPI_PartInfo_Create();

        cubePart.type = HAPI_PARTTYPE_MESH;
        cubePart.faceCount = 6;
        cubePart.vertexCount = 24;
        cubePart.pointCount = 8;

        HE_CHECK(HAPI_SetPartInfo(&session, cubeNode, 0, &cubePart))

        HAPI_AttributeInfo attrInfo = HAPI_AttributeInfo_Create();

        attrInfo.count = 8;
        attrInfo.tupleSize = 3;
        attrInfo.exists = true;
        attrInfo.storage = HAPI_STORAGETYPE_FLOAT;
        attrInfo.owner = HAPI_ATTROWNER_POINT;

        HE_CHECK(HAPI_AddAttribute(&session, cubeNode, 0, "P", &attrInfo))

        float positions[24] = {
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f
        };

        HE_CHECK(HAPI_SetAttributeFloatData(&session, cubeNode, 0, "P", &attrInfo, positions, 0, 8))

        int vertices[24] = {
            0, 2, 6, 4, 
            2, 3, 7, 6, 
            2, 0, 1, 3, 
            1, 5, 7, 3,
            5, 4, 6, 7, 
            0, 4, 5, 1
        };

        HE_CHECK(HAPI_SetVertexList(&session, cubeNode, 0, vertices, 0, 24))

        int face_counts[6] = {4, 4, 4, 4, 4, 4};
        HE_CHECK(HAPI_SetFaceCounts(&session, cubeNode, 0, face_counts, 0, 6))

        HE_CHECK(HAPI_AddGroup(&session, cubeNode, cubePart.id, HAPI_GROUPTYPE_POINT, "pointGroup"))
        int groupElementCount = HAPI_PartInfo_GetElementCountByGroupType(&cubePart, HAPI_GROUPTYPE_POINT);

        int* pointMembership = new int[groupElementCount];
        for (int ii = 0; ii < groupElementCount; ++ii)
        {
            if (ii % 2)
            {
                pointMembership[ii] = 1;
            }
            else
            {
                pointMembership[ii] = 0;
            }
        }
        HE_CHECK(HAPI_SetGroupMembership(&session, cubeNode, cubePart.id, HAPI_GROUPTYPE_POINT, "pointGroup", pointMembership, 0, groupElementCount))
        HE_CHECK(HAPI_CommitGeo(&session, cubeNode))

        HAPI_NodeId xformNode;
        HE_CHECK(HAPI_CreateNode(&session, -1, "Sop/xform", "PointGroupManipulator", false, &xformNode))
        HE_CHECK(HAPI_ConnectNodeInput(&session, xformNode, 0, cubeNode, 0))

        HAPI_NodeInfo xformInfo;
        HE_CHECK(HAPI_GetNodeInfo(&session, xformNode, &xformInfo))

        HAPI_ParmInfo* parmInfos = new HAPI_ParmInfo[xformInfo.parmCount];
        HE_CHECK(HAPI_GetParameters(&session, xformNode, parmInfos, 0, xformInfo.parmCount))

        int groupParmIndex = -1;
        int tParmIndex = -1;

        for (int ii = 0; ii < xformInfo.parmCount; ++ii)
        {
            std::string parmName = GetString(session, parmInfos[ii].nameSH);
            if (parmName == "group")
            {
                groupParmIndex = ii;
            }
            if (parmName == "t")
            {
                tParmIndex = ii;
            }
        }

        if (groupParmIndex < 0 || tParmIndex < 0)
        {
            std::stringstream ss;
            ss << "Could not find coords/type parameter on curve node";

            SPICES_CORE_WARN(ss.str());
            HE_CHECK(HAPI_Cleanup(&session))
            return;
        }

        float tParmValue[3] = { 0.0f, 1.0f, 0.0f };
        HE_CHECK(HAPI_SetParmFloatValues(&session, xformNode, tParmValue, parmInfos[tParmIndex].floatValuesIndex, 3))
        HE_CHECK(HAPI_SetParmStringValue(&session, xformNode, "pointGroup", groupParmIndex, 0))
        HE_CHECK(HAPI_CookNode(&session, xformNode, &cookOptions))

        int xformCookStatus;
        HAPI_Result xformCookResult;

        do
        {
            xformCookResult = HAPI_GetStatus(&session, HAPI_STATUS_COOK_RESULT, &xformCookStatus);
        }
        while(xformCookStatus > HAPI_STATE_MAX_READY_STATE && xformCookResult == HAPI_RESULT_SUCCESS);

        HE_CHECK(xformCookResult)
        HE_CHECK_COOK(xformCookStatus)

        HE_CHECK(HAPI_SaveHIPFile(&session, "C:/Users/Administrator/Desktop/Groups_Sample.hip", false))

        HAPI_GeoInfo xformGeoInfo;
        HE_CHECK(HAPI_GetGeoInfo(&session, xformNode, &xformGeoInfo))

        int numGroups = HAPI_GeoInfo_GetGroupCountByType(&xformGeoInfo, HAPI_GROUPTYPE_POINT);

        HAPI_PartInfo partInfo;
        HE_CHECK(HAPI_GetPartInfo(&session, xformNode, 0, &partInfo))

        int numElementsInGroup = HAPI_PartInfo_GetElementCountByGroupType(&partInfo, HAPI_GROUPTYPE_POINT);

        int* memberShip = new int[numElementsInGroup];
        HE_CHECK(HAPI_GetGroupMembership(&session, xformNode, partInfo.id, HAPI_GROUPTYPE_POINT, "pointGroup", nullptr, memberShip, 0, numElementsInGroup))

        for(int ii = 0; ii < numElementsInGroup; ++ii)
        {
            if (memberShip[ii])
            {
                std::cout << "Point" << ii << "is in pointGroup" << std::endl;
            }
        }

        delete[] pointMembership;
        delete[] memberShip;
        HE_CHECK(HAPI_Cleanup(&session))

        return;
    }

    static void Materials_Sample()
    {
        HAPI_Session session;
        
        HAPI_ThriftServerOptions serverOptions{ 0 };
        serverOptions.autoClose = true;
        serverOptions.timeoutMs = 3000.0f;

        HE_CHECK(HAPI_StartThriftNamedPipeServer(&serverOptions, "hapi", nullptr, nullptr))

        HAPI_SessionInfo sessionInfo = HAPI_SessionInfo_Create();
        HE_CHECK(HAPI_CreateThriftNamedPipeSession(&session, "hapi", &sessionInfo))

        HAPI_CookOptions cookOptions = HAPI_CookOptions_Create();

    }
}
