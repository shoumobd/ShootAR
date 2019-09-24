#pragma once
#include "UnityXRTrackable.h"

#include <cstddef>
#include <cstdint>

namespace std
{
    template<>
    struct hash<UnityXRTrackableId>
    {
        std::size_t operator()(const UnityXRTrackableId& trackableId) const
        {
            return trackableId.idPart[0] ^ trackableId.idPart[1];
        }
    };
}

static const UnityXRTrackableId kInvalidId = {};

UnityXRTrackableId GenerateTrackableId();
