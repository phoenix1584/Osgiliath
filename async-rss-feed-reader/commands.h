#pragma once

#include <cstdint>

enum class RSSCommand : uint32_t{
    Server_GetStatus,
    Server_GetPing,
    
    Client_Accepted,
    Client_AssignID
};