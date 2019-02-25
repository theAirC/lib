//! Raills Network Protocol
//! Layer 3 (Rail)

#pragma once

/// Includes

    #include "Rail_retCode.h"

/// Settings
#ifndef Rail_Settings
#define Rail_Settings
#warning "Using default Rail_Settings."
    #define Rail_RIF_Count 1
#endif

/// Library structures and typedefs

    typedef struct {
        reg ID;
        reg ID_reverse;
        enum Rail_retCode (*sendPacket)(reg RIF_ID, byte *packet, size_t packetLength);
    } Rail_RIF;

    typedef struct {
        size_t (*acceptPayload)(byte *payload, size_t payloadLength);
    } Rail_Service;
