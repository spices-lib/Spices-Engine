#pragma once

#include "../HoudiniCore.h"
#include <iostream>
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
    
    static void PrintChildNodeInfo(HAPI_Session& session, std::vector<HAPI_NodeId>& childrenNodes)
    {
        std::cout << "Child Node Ids" << std::endl;
        for (int i = 0; i < childrenNodes.size(); ++i)
        {
            HAPI_NodeInfo nInfo;
            HE_CHECK(HAPI_GetNodeInfo(&session, childrenNodes[i], &nInfo))

            std::cout << "    " << childrenNodes[i] << " - " << (nInfo.createdPostAssetLoad ? "NEW" : "EXISTING") << std::endl;
        }
    }

    static void PrintCompleteNodeInfo(HAPI_Session& session, HAPI_NodeId nodeId, HAPI_AssetInfo& assetInfo)
    {
        HAPI_NodeInfo nodeInfo;
        HE_CHECK(HAPI_GetNodeInfo(&session, nodeId, &nodeInfo))

        int objectCount = 0;
        HAPI_ObjectInfo* objectInfos = 0;

        if (nodeInfo.type == HAPI_NODETYPE_SOP)
        {
            // For pure SOP asset, a parent object will be created automatically,
            // so use parent's ID to get the object info
            objectCount = 1;
            objectInfos = new HAPI_ObjectInfo[objectCount];
            HE_CHECK(HAPI_GetObjectInfo(&session, nodeInfo.parentId, &objectInfos[0]))
        }
        else if (nodeInfo.type == HAPI_NODETYPE_OBJ)
        {
            // This could have children objects or not.
            // If has children, get child object infos.
            // If no children, presume this node is the only object.

            HE_CHECK(HAPI_ComposeObjectList(&session, nodeId, nullptr, &objectCount))

            if (objectCount > 0)
            {
                objectInfos = new HAPI_ObjectInfo[objectCount];
                HE_CHECK(HAPI_GetComposedObjectList(&session, nodeInfo.parentId, objectInfos, 0, objectCount))
            }
            else
            {
                objectCount = 1;
                objectInfos = new HAPI_ObjectInfo[objectCount];
                HE_CHECK(HAPI_GetObjectInfo(&session, nodeId, &objectInfos[0]))
            }
        }
        else
        {
            std::cout << "Unsupported node type: " << HAPI_NODETYPE_OBJ << std::endl;
            return;
        }

        for (int objectIndex = 0; objectIndex < objectCount; ++objectIndex)
        {
            HAPI_ObjectInfo& objectInfo = objectInfos[objectIndex];
            HAPI_GeoInfo geoInfo;
            HE_CHECK(HAPI_GetDisplayGeoInfo(&session, objectInfo.nodeId, &geoInfo))

            for (int partIndex = 0; partIndex < geoInfo.partCount; ++partIndex)
            {
                //ProcessGeoPart(session, assetInfo, objectInfo.nodeId, geoInfo.nodeId, partIndex);
            }
        }

        delete[] objectInfos;
    }

	static void Marshalling_Geometry_Into_Houdini()
	{
        // Create Session
        HAPI_Session session;

        HAPI_ThriftServerOptions serverOptions{ 0 };
        serverOptions.autoClose = true;
        serverOptions.timeoutMs = 3000.0f;

        HE_CHECK(HAPI_StartThriftNamedPipeServer(&serverOptions, "hapi", nullptr, nullptr))

        HAPI_SessionInfo sessionInfo = HAPI_SessionInfo_Create();

        HE_CHECK(HAPI_CreateThriftNamedPipeSession(&session, "hapi", &sessionInfo))

        // Initialize Session
        HAPI_CookOptions cookOptions = HAPI_CookOptions_Create();
        HE_CHECK(HAPI_Initialize(&session, &cookOptions, true, -1, nullptr, nullptr, nullptr, nullptr, nullptr))

        // create null node
        HAPI_NodeId newNode;
        HE_CHECK(HAPI_CreateInputNode(&session, -1, &newNode, "Marshalling_Point_Clouds"))
        // cook null node
        HE_CHECK(HAPI_CookNode(&session, newNode, &cookOptions))

        // Wait for cook over.
        int cookStatus;
        HAPI_Result cookResult;

        do
        {
            cookResult = HAPI_GetStatus(&session, HAPI_STATUS_COOK_STATE, &cookStatus);
        } 
        while (cookStatus > HAPI_STATE_MAX_READY_STATE && cookResult == HAPI_RESULT_SUCCESS);
        HE_CHECK(cookResult)
        HE_CHECK_COOK(cookStatus)

        // null node part info
        HAPI_PartInfo newNodePart = HAPI_PartInfo_Create();
        newNodePart.type = HAPI_PARTTYPE_MESH;
        newNodePart.faceCount = 1;
        newNodePart.vertexCount = 3;
        newNodePart.pointCount = 3;
        HE_CHECK(HAPI_SetPartInfo(&session, newNode, 0, &newNodePart))

        // null node attribute info
        HAPI_AttributeInfo newNodePointInfo = HAPI_AttributeInfo_Create();
        newNodePointInfo.count = 3;
        newNodePointInfo.tupleSize = 3;
        newNodePointInfo.exists = true;
        newNodePointInfo.storage = HAPI_STORAGETYPE_FLOAT;
        newNodePointInfo.owner = HAPI_ATTROWNER_POINT;
        HE_CHECK(HAPI_AddAttribute(&session, newNode, 0, "P", &newNodePointInfo))

        // set null node attribute
        float positions[9] = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f };
        HE_CHECK(HAPI_SetAttributeFloatData(&session, newNode, 0, "P", &newNodePointInfo, positions, 0, 3))

        // set null node vertices
        int vertices[3] = { 0, 1, 2 };
        HE_CHECK(HAPI_SetVertexList(&session, newNode, 0, vertices, 0, 3))

        // set null node faces
        int face_counts[1] = { 3 };
        HE_CHECK(HAPI_SetFaceCounts(&session, newNode, 0, face_counts, 0, 1))

        // set null node str attriute
        std::array<char*, 3> strs;
        strs[0] = _strdup("strPoint1");
        strs[1] = _strdup("strPoint2");
        strs[2] = _strdup("strPoint3");

        newNodePointInfo.count = 3;
        newNodePointInfo.tupleSize = 1;
        newNodePointInfo.exists = true;
        newNodePointInfo.storage = HAPI_STORAGETYPE_STRING;
        newNodePointInfo.owner = HAPI_ATTROWNER_POINT;

        HE_CHECK(HAPI_AddAttribute(&session, newNode, 0, "srcData", &newNodePointInfo))
        HE_CHECK(HAPI_SetAttributeStringData(&session, newNode, 0, "srcData", &newNodePointInfo, (const char**)strs.data(), 0, 3))

        // commit null node
        HE_CHECK(HAPI_CommitGeo(&session, newNode));

        // save to hip file
        HE_CHECK(HAPI_SaveHIPFile(&session, "C:/Users/Administrator/Desktop/Marshalling_Point_Clouds.hip", false))
        
        // clean up session, needs recall initialize
        HE_CHECK(HAPI_Cleanup(&session))

        return;
	}

    static void Marshalling_Point_Clouds()
    {
        // Create Session
        HAPI_Session session;

        HAPI_ThriftServerOptions serverOptions{ 0 };
        serverOptions.autoClose = true;
        serverOptions.timeoutMs = 3000.0f;

        HE_CHECK(HAPI_StartThriftNamedPipeServer(&serverOptions, "hapi", nullptr, nullptr))

        HAPI_SessionInfo sessionInfo = HAPI_SessionInfo_Create();

        HE_CHECK(HAPI_CreateThriftNamedPipeSession(&session, "hapi", &sessionInfo))

        // Initialize Session
        HAPI_CookOptions cookOptions = HAPI_CookOptions_Create();
        HE_CHECK(HAPI_Initialize(&session, &cookOptions, true, -1, nullptr, nullptr, nullptr, nullptr, nullptr))

        // create null node
        HAPI_NodeId newNode;
        HE_CHECK(HAPI_CreateInputNode(&session, -1, &newNode, "Marshalling_Point_Clouds"))
        // cook null node
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

        // get null node geo info
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

        std::vector<HAPI_ParmInfo> paramInfos;
        paramInfos.resize(curveNodeInfo.parmCount);
        HE_CHECK(HAPI_GetParameters(&session, curveNode, paramInfos.data(), 0, curveNodeInfo.parmCount))

        int coordsParmIndex = -1;
        int typeParmIndex = -1;
        for (int i = 0; i < curveNodeInfo.parmCount; i++)
        {
            std::string parmName = GetString(session, paramInfos[i].nameSH);
            if (parmName == "coords")
            {
                coordsParmIndex = i;
            }
            if (parmName == "type")
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

        HE_CHECK(HAPI_Cleanup(&session))

        return;
    }

    static void Curve_Marshalling()
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
        HE_CHECK(HAPI_CreateInputNode(&session, -1, &newNode, "Curve"))
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
        newNodePart.type = HAPI_PARTTYPE_CURVE;
        newNodePart.faceCount = 1;
        newNodePart.vertexCount = 4;
        newNodePart.pointCount = 4;
        HE_CHECK(HAPI_SetPartInfo(&session, newNode, 0, &newNodePart))

        HAPI_CurveInfo curveInfo;
        curveInfo.curveType = HAPI_CURVETYPE_NURBS;
        curveInfo.curveCount = 1;
        curveInfo.vertexCount = 4;
        curveInfo.knotCount = 8;
        curveInfo.isPeriodic = false;
        curveInfo.order = 4;
        curveInfo.hasKnots = true;
        HE_CHECK(HAPI_SetCurveInfo(&session, newNode, 0, &curveInfo))

        int curveCount = 4;
        HE_CHECK(HAPI_SetCurveCounts(&session, newNode, newNodePart.id, &curveCount, 0, 1))

        float curveKnots[8] = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f };
        HE_CHECK(HAPI_SetCurveKnots(&session, newNode, newNodePart.id, curveKnots, 0, 8))

        HAPI_AttributeInfo attrInfo = HAPI_AttributeInfo_Create();
        attrInfo.count = 4;
        attrInfo.tupleSize = 3;
        attrInfo.exists = true;
        attrInfo.storage = HAPI_STORAGETYPE_FLOAT;
        attrInfo.owner = HAPI_ATTROWNER_POINT;

        HE_CHECK(HAPI_AddAttribute(&session, newNode, 0, "P", &attrInfo))

        float positions[12] = { -4.0f, 0.0f, 4.0f, -4.0f, 0.0f, -4.0f, 4.0f, 0.0f, -4.0f, 4.0f, 0.0f, 4.0f };
        HE_CHECK(HAPI_SetAttributeFloatData(&session, newNode, 0, "P", &attrInfo, positions, 0, 4))

        HE_CHECK(HAPI_CommitGeo(&session, newNode))
        HE_CHECK(HAPI_SaveHIPFile(&session, "C:/Users/Administrator/Desktop/Curve_Marshalling.hip", true))

        HE_CHECK(HAPI_Cleanup(&session))

        return;
    }

    static void Curve_Output()
    {
        const char* hdaFile = "examples/nurbs_curve.hda";
        HAPI_CookOptions cookOptions = HAPI_CookOptions_Create();

        HAPI_Session session;
        HAPI_SessionInfo sessionInfo = HAPI_SessionInfo_Create();
        HE_CHECK(HAPI_CreateInProcessSession(&session, &sessionInfo))
        HE_CHECK(HAPI_Initialize(&session, &cookOptions, true, -1, nullptr, nullptr, nullptr, nullptr, nullptr))

        HAPI_AssetLibraryId assetLibId;
        HE_CHECK(HAPI_LoadAssetLibraryFromFile(&session, hdaFile, true, &assetLibId))

        int assetCount;
        HE_CHECK(HAPI_GetAvailableAssetCount(&session, assetLibId, &assetCount))

        if(assetCount > 1)
        {
            std::cout << "Should only be loading 1 asset here" << std::endl;
            HE_CHECK(HAPI_Cleanup(&session))
            return;
        }


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
        const char* hdaFile = "";

        HAPI_Session session;
        
        HAPI_ThriftServerOptions serverOptions{ 0 };
        serverOptions.autoClose = true;
        serverOptions.timeoutMs = 3000.0f;

        HE_CHECK(HAPI_StartThriftNamedPipeServer(&serverOptions, "hapi", nullptr, nullptr))

        HAPI_SessionInfo sessionInfo = HAPI_SessionInfo_Create();
        HE_CHECK(HAPI_CreateThriftNamedPipeSession(&session, "hapi", &sessionInfo))

        HAPI_CookOptions cookOptions = HAPI_CookOptions_Create();
        HE_CHECK(HAPI_Initialize(&session, &cookOptions, true, -1, nullptr, nullptr, nullptr, nullptr, nullptr))

        HAPI_AssetLibraryId assetLibId;
        HE_CHECK(HAPI_LoadAssetLibraryFromFile(&session, hdaFile, true, &assetLibId));

        int assetCount;
        HE_CHECK(HAPI_GetAvailableAssetCount(&session, assetLibId, &assetCount))

        if (assetCount > 1)
        {
            std::cout << "Should only be loading 1 asset here" << std::endl;
        }

        HAPI_StringHandle assetSh;
        HE_CHECK(HAPI_GetAvailableAssets(&session, assetLibId, &assetSh, assetCount))

        std::string assetName = GetString(session, assetSh);

        HAPI_NodeId nodeId;
        HE_CHECK(HAPI_CreateNode(&session, -1, assetName.c_str(), "BrandonTest", false, &nodeId))

        HE_CHECK(HAPI_CookNode(&session, nodeId, &cookOptions))

        int cookStatus;
        HAPI_Result cookResult;

        do
        {
            cookResult = HAPI_GetStatus(&session, HAPI_STATUS_COOK_STATE, &cookStatus);
        }
        while (cookStatus > HAPI_STATE_MAX_READY_STATE && cookResult == HAPI_RESULT_SUCCESS);

        HE_CHECK(cookResult);
        HE_CHECK_COOK(cookStatus);

        HAPI_GeoInfo geoInfo;
        HE_CHECK(HAPI_GetDisplayGeoInfo(&session, nodeId, &geoInfo))

        HAPI_PartInfo partInfo;
        HE_CHECK(HAPI_GetPartInfo(&session, geoInfo.nodeId, 0, &partInfo))

        bool areAllTheSame = false;
        std::vector<HAPI_NodeId> materialIds(partInfo.faceCount);
        HE_CHECK(HAPI_GetMaterialNodeIdsOnFaces(&session, geoInfo.nodeId, partInfo.id, &areAllTheSame, &materialIds.front(), 0, partInfo.faceCount))

        if(!areAllTheSame)
        {
            std::cout << "All materials should be the same." << std::endl;
        }

        for (int i = 0; i < partInfo.faceCount; ++i)
        {
            if (materialIds[i] != materialIds[0])
            {
                std::cout << "All material ids should be the same." << std::endl;
            }
        }

        HAPI_MaterialInfo materialInfo;
        HE_CHECK(HAPI_GetMaterialInfo(&session, materialIds[0], &materialInfo))

        if (materialInfo.nodeId != materialIds[0] ||
            materialInfo.nodeId < 0 ||
            materialInfo.exists != true ||
            materialInfo.hasChanged != true)
        {
            std::cout << "Did not successfully extract the first material" << std::endl;
        }

        HAPI_NodeInfo materialNodeInfo;
        HE_CHECK(HAPI_GetNodeInfo(&session, materialInfo.nodeId, &materialNodeInfo))

        std::cout << GetString(session, materialNodeInfo.nameSH) << std::endl;

        std::vector<HAPI_ParmInfo> parmInfos(materialNodeInfo.parmCount);
        HE_CHECK(HAPI_GetParameters(&session, materialNodeInfo.id, parmInfos.data(), 0, materialNodeInfo.parmCount))

        int baseColorMapIndex = -1;
        for (int i = 0; i < materialNodeInfo.parmCount; ++i)
        {
            if (GetString(session, parmInfos[i].nameSH) == "baseColorMap")
            {
                baseColorMapIndex = i;
                break;
            }
        }

        if (baseColorMapIndex < 0)
        {
            std::cout << "Could not find the base color map parameter" << std::endl;
        }

        HAPI_StringHandle basePath;
        HE_CHECK(HAPI_GetParmStringValue(&session, materialNodeInfo.id, "baseColorMap", 0, true, &basePath))

        std::cout << "Base Color Map Path: " << GetString(session, basePath) << std::endl;

        HE_CHECK(HAPI_RenderTextureToImage(&session, materialNodeInfo.id, baseColorMapIndex))

        HAPI_ImageInfo imgInfo;
        HE_CHECK(HAPI_GetImageInfo(&session, materialNodeInfo.id, &imgInfo))

        std::cout << "Image Width = " << imgInfo.xRes << std::endl
            << "Image Height = " << imgInfo.yRes << std::endl
            << "Image Format = " << GetString(session, imgInfo.imageFileFormatNameSH) << std::endl;

        HE_CHECK(HAPI_SetImageInfo(&session, materialNodeInfo.id, &imgInfo))

        int imagePlaneCount;
        HE_CHECK(HAPI_GetImagePlaneCount(&session, materialNodeInfo.id, &imagePlaneCount))

        std::vector<HAPI_StringHandle> imagePlanes(imagePlaneCount);
        HE_CHECK(HAPI_GetImagePlanes(&session, materialNodeInfo.id, imagePlanes.data(), imagePlaneCount))

        for (int j = 0; j < imagePlaneCount; ++j)
        {
            std::string imagePlaneName = GetString(session, imagePlanes[j]);
            std::cout << "Image Plane [ " << j << " ] = " << imagePlaneName << std::endl;

            int destinationFilePath;
            HE_CHECK(HAPI_ExtractImageToFile(&session, materialNodeInfo.id, nullptr, imagePlaneName.c_str(), "./examples/", nullptr, &destinationFilePath))
        }

        HE_CHECK(HAPI_Cleanup(&session));
    }

    static void Node_Creation_Sample()
    {
        const char* hdaFile = "";

        HAPI_Session session;

        HAPI_ThriftServerOptions serverOptions{ 0 };
        serverOptions.autoClose = true;
        serverOptions.timeoutMs = 3000.0f;

        HE_CHECK(HAPI_StartThriftNamedPipeServer(&serverOptions, "hapi", nullptr, nullptr))

        HAPI_SessionInfo sessionInfo = HAPI_SessionInfo_Create();
        HE_CHECK(HAPI_CreateThriftNamedPipeSession(&session, "hapi", &sessionInfo))

        HAPI_CookOptions cookOptions = HAPI_CookOptions_Create();
        HE_CHECK(HAPI_Initialize(&session, &cookOptions, true, -1, nullptr, nullptr, nullptr, nullptr, nullptr))

        HAPI_AssetLibraryId assetLibId;
        HE_CHECK(HAPI_LoadAssetLibraryFromFile(&session, hdaFile, true, &assetLibId))

        int assetCount;
        HE_CHECK(HAPI_GetAvailableAssetCount(&session, assetLibId, &assetCount))

        if (assetCount > 1)
        {
            std::cout << "Should only be loading 1 asset here" << std::endl;
        }

        HAPI_StringHandle assetSh;
        HE_CHECK(HAPI_GetAvailableAssets(&session, assetLibId, &assetSh, assetCount))

        std::string assetName = GetString(session, assetSh);

        HAPI_NodeId editableNetworkId;
        HE_CHECK(HAPI_CreateNode(&session, -1, assetName.c_str(), "FourShape", false, &editableNetworkId))

        HE_CHECK(HAPI_CookNode(&session, editableNetworkId, &cookOptions))

        int cookStatus;
        HAPI_Result cookResult;

        do
        {
            cookResult = HAPI_GetStatus(&session, HAPI_STATUS_COOK_STATE, &cookStatus);
        }
        while(cookStatus > HAPI_STATE_MAX_READY_STATE && cookResult == HAPI_RESULT_SUCCESS);

        HE_CHECK(cookResult);
        HE_CHECK_COOK(cookStatus);

        int childCount;
        HE_CHECK(HAPI_ComposeChildNodeList(&session, editableNetworkId, HAPI_NODETYPE_ANY, HAPI_NODEFLAGS_ANY, false, &childCount))

        std::cout << "Editable Node Network Child Count: " << childCount << std::endl;

        std::vector<HAPI_NodeId> childNodeIds(childCount);
        HE_CHECK(HAPI_GetComposedChildNodeList(&session, editableNetworkId, &childNodeIds.front(), childCount))

        PrintChildNodeInfo(session, childNodeIds);

        HAPI_NodeId anotherBoxNode;
        HE_CHECK(HAPI_CreateNode(&session, editableNetworkId, "geo", "ProgrammaticBox", false, &anotherBoxNode))

        HE_CHECK(HAPI_ConnectNodeInput(&session, anotherBoxNode, 0, childNodeIds[0], 0))
        HE_CHECK(HAPI_CookNode(&session, anotherBoxNode, &cookOptions))

        int boxCookStatus;
        HAPI_Result boxCookResult;

        do
        {
            boxCookResult = HAPI_GetStatus(&session, HAPI_STATUS_COOK_STATE, &boxCookStatus);
        }
        while (boxCookStatus > HAPI_STATE_MAX_READY_STATE && boxCookResult == HAPI_RESULT_SUCCESS);

        HE_CHECK(boxCookResult)
        HE_CHECK_COOK(boxCookStatus)

        HAPI_NodeId connectedNodeId;
        HE_CHECK(HAPI_QueryNodeInput(&session, anotherBoxNode, 0, &connectedNodeId))

        if (connectedNodeId != childNodeIds[0])
        {
            std::cout << "The connected node id is" << connectedNodeId << " When it should be " << editableNetworkId << std::endl;
        }

        HE_CHECK(HAPI_ComposeChildNodeList(&session, editableNetworkId, HAPI_NODETYPE_ANY, HAPI_NODEFLAGS_ANY, false, &childCount))

        std::vector<HAPI_NodeId> newChildNodes(childCount);
        HE_CHECK(HAPI_GetComposedChildNodeList(&session, editableNetworkId, &newChildNodes.front(), childCount))

        std::cout << "After CONNECT NODE" << std::endl;
        PrintChildNodeInfo(session, newChildNodes);

        HE_CHECK(HAPI_SaveHIPFile(&session, "", false))
        HE_CHECK(HAPI_DisconnectNodeInput(&session, anotherBoxNode, 0))
        HE_CHECK(HAPI_DeleteNode(&session, anotherBoxNode))

        std::cout << "After DELETING NODE" << std::endl;

        HE_CHECK(HAPI_ComposeChildNodeList(&session, editableNetworkId, HAPI_NODETYPE_ANY, HAPI_NODEFLAGS_ANY, false, &childCount))

        std::vector<HAPI_NodeId> finalChildList(childCount);
        HE_CHECK(HAPI_GetComposedChildNodeList(&session, editableNetworkId, &finalChildList.front(), childCount))
        PrintChildNodeInfo(session, finalChildList);

        HE_CHECK(HAPI_Cleanup(&session));
    }

    static void Parts_Sample()
    {
        const char* hdaFile = "";

        HAPI_Session session;
        HAPI_ThriftServerOptions serverOptions{ 0 };
        serverOptions.autoClose = true;
        serverOptions.timeoutMs = 3000.0f;

        HE_CHECK(HAPI_StartThriftNamedPipeServer(&serverOptions, "hapi", nullptr, nullptr))

        HAPI_SessionInfo sessionInfo = HAPI_SessionInfo_Create();
        HE_CHECK(HAPI_CreateThriftNamedPipeSession(&session, "hapi", &sessionInfo))

        HAPI_CookOptions cookOptions = HAPI_CookOptions_Create();
        HE_CHECK(HAPI_Initialize(&session, &cookOptions, true, -1, nullptr, nullptr, nullptr, nullptr, nullptr))

        HAPI_AssetLibraryId assetLibId;
        HE_CHECK(HAPI_LoadAssetLibraryFromFile(&session, hdaFile, true, &assetLibId))

        int assetCount;
        HE_CHECK(HAPI_GetAvailableAssetCount(&session, assetLibId, &assetCount))

        if (assetCount > 1)
        {
            std::cout << "Should only be loading 1 asset here" << std::endl;
        }

        HAPI_StringHandle assetSh;
        HE_CHECK(HAPI_GetAvailableAssets(&session, assetLibId, &assetSh, assetCount))

        std::string assetName = GetString(session, assetSh);

        HAPI_NodeId nodeId;
        HE_CHECK(HAPI_CreateNode(&session, -1, assetName.c_str(), "TestObject", false, &nodeId))
        HE_CHECK(HAPI_CookNode(&session, nodeId, &cookOptions))

        int cookStatus;
        HAPI_Result cookResult;

        do
        {
            cookResult = HAPI_GetStatus(&session, HAPI_STATUS_COOK_STATE, &cookStatus);
        }
        while(cookStatus > HAPI_STATE_MAX_READY_STATE && cookResult == HAPI_RESULT_SUCCESS);

        HE_CHECK(cookResult)
        HE_CHECK_COOK(cookStatus)

        HAPI_AssetInfo assetInfo;
        HE_CHECK(HAPI_GetAssetInfo(&session, nodeId, &assetInfo))

        PrintCompleteNodeInfo(session, nodeId, assetInfo);

        HE_CHECK(HAPI_Cleanup(&session));
    }
}
