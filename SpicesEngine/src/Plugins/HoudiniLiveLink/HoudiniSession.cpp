#include "Pchheader.h"
#include "HoudiniSession.h"

namespace HoudiniEngine {

    void HoudiniSession::StartSession()
    {
        HAPI_Session session;

        HAPI_ThriftServerOptions serverOptions{ 0 };
        serverOptions.autoClose = true;
        serverOptions.timeoutMs = 3000.0f;

        HAPI_StartThriftNamedPipeServer(&serverOptions, "hapi", nullptr, nullptr);

        HAPI_SessionInfo sessionInfo = HAPI_SessionInfo_Create();

        HAPI_Result result = HAPI_CreateThriftNamedPipeSession(&session, "hapi", &sessionInfo);

    }
}