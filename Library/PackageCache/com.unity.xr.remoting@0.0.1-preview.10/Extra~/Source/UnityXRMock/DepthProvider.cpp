#include <cstring>
#include "DepthProvider.h"
#include "UnityMath.h"

static const float kAngleThresholdInDegrees = 5.f;

static const float kCosHalfAngleThreshold =
    std::cos(kAngleThresholdInDegrees * 3.1415926f / 180.f * .5f);

extern "C"
{
    void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityXRMock_setDepthData(
        const UnityXRVector3* positions, const float* confidences, int count)
    {
        if (DepthProvider::GetInstance())
            DepthProvider::GetInstance()->SetDepthData(positions, confidences, count);
    }
}

struct DepthDataAllocatorWrapper : public IUnityXRDepthDataAllocator
{
    DepthDataAllocatorWrapper(const UnityXRDepthDataAllocator* allocator, IUnityXRDepthInterface* unityInterface)
        : m_Allocator(allocator)
        , m_UnityInterface(unityInterface)
    {}

    virtual void UNITY_INTERFACE_API SetNumberOfPoints(size_t numPoints) override
    {
        m_UnityInterface->Allocator_SetNumberOfPoints(m_Allocator, numPoints);
    }

    virtual UnityXRVector3* UNITY_INTERFACE_API GetPointsBuffer() const override
    {
        return m_UnityInterface->Allocator_GetPointsBuffer(m_Allocator);
    }

    virtual float* UNITY_INTERFACE_API GetConfidenceBuffer() const override
    {
        return m_UnityInterface->Allocator_GetConfidenceBuffer(m_Allocator);
    }

private:
    const UnityXRDepthDataAllocator* m_Allocator;

    IUnityXRDepthInterface* m_UnityInterface;
};

void DepthProvider::ClearPoints()
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_Positions.clear();
}

void DepthProvider::AddDepthPoint(float x, float y, float z)
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_Positions.push_back(UnityXRVector3{x, y, z});
}

void DepthProvider::SetDepthData(const UnityXRVector3* positions, const float* confidences, int count)
{
    std::lock_guard<std::mutex> lock(m_Mutex);

    m_Positions.clear();
    m_Confidences.clear();

    if (positions == nullptr)
        return;

    m_Positions.resize(count);
    std::copy(positions, positions + count, m_Positions.data());

    if (confidences)
    {
        m_Confidences.resize(count);
        std::copy(confidences, confidences + count, m_Confidences.data());
    }
}

std::vector<UnityXRRaycastHit> DepthProvider::Raycast(const Ray& ray, UnityXRTrackableType hitFlags) const
{
    std::vector<UnityXRRaycastHit> hits;

    if ((hitFlags & kUnityXRTrackableTypePoint) == kUnityXRTrackableTypeNone)
        return hits;

    std::lock_guard<std::mutex> lock(m_Mutex);

    for (const auto& position : m_Positions)
    {
        const auto toPoint = Sub(position, ray.origin);
        const float length = Length(toPoint);
        const float cosAngle = Dot(toPoint, ray.direction) / length;
        if (kCosHalfAngleThreshold <= cosAngle)
        {
            UnityXRRaycastHit hit;
            hit.pose.position = position;
            hit.pose.rotation = UnityXRVector4{0, 0, 0, 1};
            hit.distance = length;
            hit.hitType = kUnityXRTrackableTypePoint;
            hits.push_back(hit);
        }
    }

    return hits;
}

bool UNITY_INTERFACE_API DepthProvider::GetPointCloud(IUnityXRDepthDataAllocator& allocator)
{
    std::lock_guard<std::mutex> lock(m_Mutex);

    allocator.SetNumberOfPoints(m_Positions.size());
    std::copy(m_Positions.begin(), m_Positions.end(), allocator.GetPointsBuffer());
    if (m_Confidences.size() > 0)
        std::copy(m_Confidences.begin(), m_Confidences.end(), allocator.GetConfidenceBuffer());

    return true;
}

UnitySubsystemErrorCode DepthProvider::RegisterAsCProvider(UnitySubsystemHandle handle, IUnityXRDepthInterface* depthInterface)
{
    m_CInterface = depthInterface;
    UnityXRDepthProvider depthProvider = {};
    depthProvider.userData = this;
    depthProvider.GetPointCloud = &StaticGetPointCloud;
    return depthInterface->RegisterDepthProvider(handle, &depthProvider);
}

UnitySubsystemErrorCode UNITY_INTERFACE_API DepthProvider::StaticGetPointCloud(UnitySubsystemHandle handle, void* userData, const UnityXRDepthDataAllocator * allocator)
{
    if (DepthProvider* provider = DepthProvider::GetInstance())
    {
        DepthDataAllocatorWrapper wrappedAllocator(allocator, provider->m_CInterface);
        return provider->GetPointCloud(wrappedAllocator) ? kUnitySubsystemErrorCodeSuccess : kUnitySubsystemErrorCodeFailure;
    }

    return kUnitySubsystemErrorCodeFailure;
}
