#include "Pchheader.h"
#include "HoudiniSession.h"

namespace HoudiniEngine {

    void HoudiniSession::StartSession()
    {
        HAPI_Session session{};
        session.id = 0;
        session.type = HAPI_SessionType::HAPI_SESSION_THRIFT;

        HAPI_Result result = HAPI_CreateThriftNamedPipeSession(&session, "hapi");
        std::cout << "Hello" << std::endl;
    }
}