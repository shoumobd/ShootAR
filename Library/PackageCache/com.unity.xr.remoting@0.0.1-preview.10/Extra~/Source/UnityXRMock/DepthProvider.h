#pragma once
#include <vector>
#include <mutex>

#include "IUnityXRDepth.deprecated.h"
#include "IUnityXRRaycast.h"
#include "XRProvider.h"
#include "Ray.h"

class DepthProvider : public XRProvider<DepthProvider, IUnityXRDepthProvider>
{
public:

    UnitySubsystemErrorCode RegisterAsCProvider(
        UnitySubsystemHandle handle, IUnityXRDepthInterface* depthInterface);

    void SetDepthData(
        const UnityXRVector3* positions, const float* confidences, int count);

    void ClearPoints();

    void AddDepthPoint(float x, float y, float z);

    std::vector<UnityXRRaycastHit> Raycast(const Ray& ray, UnityXRTrackableType hitFlags) const;

private:

    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticGetPointCloud(
        UnitySubsystemHandle handle, void* userData, const UnityXRDepthDataAllocator * allocator);

    bool UNITY_INTERFACE_API GetPointCloud(IUnityXRDepthDataAllocator& allocator);

    std::vector<UnityXRVector3> m_Positions;

    std::vector<float> m_Confidences;

    mutable std::mutex m_Mutex;

    IUnityXRDepthInterface* m_CInterface = nullptr;
};
