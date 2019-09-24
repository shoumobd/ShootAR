#pragma once
#include <unordered_map>

#include "IUnityXRRaycast.deprecated.h"
#include "XRProvider.h"

class RaycastProvider : public XRProvider<RaycastProvider, IUnityXRRaycastProvider>
{
public:

    UnitySubsystemErrorCode RegisterAsCProvider(UnitySubsystemHandle handle, IUnityXRRaycastInterface* raycastInterface);

private:

    bool UNITY_INTERFACE_API Raycast(
        float screenX,
        float screenY,
        UnityXRTrackableType hitFlags,
        IUnityXRRaycastAllocator& allocator) final;

    bool RaycastImpl(
        float screenX,
        float screenY,
        UnityXRTrackableType hitFlags,
        IUnityXRRaycastAllocator& allocator) const;

    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticRaycast(
        UnitySubsystemHandle handle,
        void* userData,
        float screenX,
        float screenY,
        UnityXRTrackableType hitFlags,
        UnityXRRaycastDataAllocator * allocator);

    IUnityXRRaycastInterface* m_CInterface = nullptr;
};
