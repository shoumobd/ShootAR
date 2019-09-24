#include <cstring>
#include <vector>

#include "RaycastProvider.h"
#include "PlaneProvider.h"
#include "DepthProvider.h"
#include "CameraProvider.h"

typedef int(UNITY_INTERFACE_API * Raycaster)(float x, float y, unsigned char type);

static Raycaster s_Raycaster = nullptr;

static std::vector<UnityXRRaycastHit> s_XRRaycastHits;

extern "C"
{
    UNITY_INTERFACE_EXPORT void UnityARMock_setRaycastHandler(Raycaster raycastHandler)
    {
        s_Raycaster = raycastHandler;
    }

    void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityXRMock_setRaycastHits(
        const UnityXRRaycastHit* hits, int size)
    {
        s_XRRaycastHits.clear();
        if (hits == nullptr || size < 0)
            return;

        s_XRRaycastHits.resize(size);
        std::copy(hits, hits + size, s_XRRaycastHits.data());
    }
}

bool UNITY_INTERFACE_API RaycastProvider::Raycast(
    float screenX,
    float screenY,
    UnityXRTrackableType hitFlags,
    IUnityXRRaycastAllocator& allocator)
{
    if (s_Raycaster == nullptr)
        return RaycastImpl(screenX, screenY, hitFlags, allocator);

    s_XRRaycastHits.clear();
    s_Raycaster(screenX, screenY, hitFlags);

    std::copy(s_XRRaycastHits.begin(), s_XRRaycastHits.end(), allocator.SetNumberOfHits(s_XRRaycastHits.size()));
    return s_XRRaycastHits.size() > 0;
}

template<typename T>
static inline void InsertBack(std::vector<T>& dst, const std::vector<T>& src)
{
    dst.insert(dst.end(), src.begin(), src.end());
}

bool UNITY_INTERFACE_API RaycastProvider::RaycastImpl(
        float screenX,
        float screenY,
        UnityXRTrackableType hitFlags,
        IUnityXRRaycastAllocator& allocator) const
{
    if (CameraProvider::GetInstance() == nullptr)
        return false;

    Ray ray;
    if (!CameraProvider::GetInstance()->TryGetRay(screenX, screenY, &ray))
        return false;

    std::vector<UnityXRRaycastHit> hits;
    if (auto planeProvider = PlaneProvider::GetInstance())
        InsertBack(hits, planeProvider->Raycast(ray, hitFlags));

    if (auto depthProvider = DepthProvider::GetInstance())
        InsertBack(hits, depthProvider->Raycast(ray, hitFlags));

    if (hits.size() == 0)
        return false;

    std::sort(hits.begin(), hits.end(), [](const UnityXRRaycastHit& h1, const UnityXRRaycastHit& h2) -> bool
    {
        return h1.distance < h2.distance;
    });
    std::copy(hits.begin(), hits.end(), allocator.SetNumberOfHits(hits.size()));

    return true;
}

UnitySubsystemErrorCode RaycastProvider::RegisterAsCProvider(UnitySubsystemHandle handle, IUnityXRRaycastInterface* raycastInterface)
{
    m_CInterface = raycastInterface;
    UnityXRRaycastProvider provider = {};
    provider.userData = this;
    provider.Raycast = &StaticRaycast;
    return raycastInterface->RegisterRaycastProvider(handle, &provider);
}

struct RaycastAllocatorWrapper : public IUnityXRRaycastAllocator
{
    RaycastAllocatorWrapper(IUnityXRRaycastInterface* unityInterface, UnityXRRaycastDataAllocator* allocator)
        : m_UnityInterface(unityInterface)
        , m_Allocator(allocator)
    {}

    virtual UnityXRRaycastHit* UNITY_INTERFACE_API SetNumberOfHits(size_t numHits) override
    {
        return m_UnityInterface->Allocator_SetNumberOfHits(m_Allocator, numHits);
    }

    virtual UnityXRRaycastHit* UNITY_INTERFACE_API ExpandBy(size_t numHits) override
    {
        return m_UnityInterface->Allocator_ExpandBy(m_Allocator, numHits);
    }

private:
    IUnityXRRaycastInterface * m_UnityInterface;

    UnityXRRaycastDataAllocator* m_Allocator;
};

UnitySubsystemErrorCode UNITY_INTERFACE_API RaycastProvider::StaticRaycast(
    UnitySubsystemHandle handle,
    void* userData,
    float screenX,
    float screenY,
    UnityXRTrackableType hitFlags,
    UnityXRRaycastDataAllocator * allocator)
{
    if (RaycastProvider* provider = RaycastProvider::GetInstance())
    {
        RaycastAllocatorWrapper wrappedAllocator(provider->m_CInterface, allocator);
        return provider->Raycast(screenX, screenY, hitFlags, wrappedAllocator) ? kUnitySubsystemErrorCodeSuccess : kUnitySubsystemErrorCodeFailure;
    }
    
    return kUnitySubsystemErrorCodeFailure; 
}
