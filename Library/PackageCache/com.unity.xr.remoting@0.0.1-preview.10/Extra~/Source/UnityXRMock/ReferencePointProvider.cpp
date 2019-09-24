#include <cstring>
#include "ReferencePointProvider.h"
#include "PlaneProvider.h"
#include "TrackableIdHelpers.h"
#include "UnityMath.h"

extern "C"
{
    struct AddReferenceResult
    {
        unsigned long id1;
        unsigned long id2;

        int trackingState;
        bool result;
    };

    struct ReferencePointData
    {
        unsigned long id1;
        unsigned long id2;

        float px, py, pz;
        float rx, ry, rz, rw;

        int trackingState;
    };

    //should return void* instead ?

    typedef bool(UNITY_INTERFACE_API *RequestRemoveReferencePoint)(
        uint64_t id1, uint64_t id2);

    static RequestRemoveReferencePoint s_RemoveReferencePointHandler = nullptr;

    typedef unsigned char*(UNITY_INTERFACE_API *RequestAddReferencePoint)(
        float px, float py, float pz,
        float rx, float ry, float rz, float rw);

    static RequestAddReferencePoint s_AddReferencePointHandler = nullptr;

    UNITY_INTERFACE_EXPORT void UnityARMock_setAddReferencePointHandler(RequestAddReferencePoint unityFunctionPointer,
        RequestRemoveReferencePoint removeUnityFunctionPointer)
    {
        s_AddReferencePointHandler = unityFunctionPointer;
        s_RemoveReferencePointHandler = removeUnityFunctionPointer;
    }

    UNITY_INTERFACE_EXPORT void UpdateReferencePoints(unsigned char* data, int size)
    {
        ReferencePointData* referencePointData = reinterpret_cast<ReferencePointData*>(data);

        UnityXRReferencePoint point;

        UnityXRTrackableId trackableId;
        trackableId.idPart[0] = referencePointData->id1;
        trackableId.idPart[1] = referencePointData->id2;

        point.id = trackableId;

        point.pose.position.x = referencePointData->px;
        point.pose.position.y = referencePointData->py;
        point.pose.position.z = referencePointData->pz;

        point.pose.rotation.x = referencePointData->rx;
        point.pose.rotation.y = referencePointData->ry;
        point.pose.rotation.z = referencePointData->rz;
        point.pose.rotation.x = referencePointData->rw;

        point.trackingState = (UnityXRTrackingState)referencePointData->trackingState;

        ReferencePointProvider::GetInstance()->AddReferencePoint(point);
    }

    //Test passing all params without need for marshaling
    UNITY_INTERFACE_EXPORT void UnityXRMock_addReferenceResultData(unsigned long id0, unsigned long id1, bool result, int trackingState)
    {
        ReferencePointProvider::GetInstance()->SetAddResponseData(id0, 
            id1, 
            result, 
            trackingState);
    }

    UnityXRTrackableId UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityXRMock_attachReferencePoint(
        UnityXRTrackableId planeId, UnityXRPose pose)
    {
        if (ReferencePointProvider::GetInstance())
            return ReferencePointProvider::GetInstance()->AttachReferencePoint(planeId, pose);

        return kInvalidId;
    }

    void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityXRMock_updateReferencePoint(
        UnityXRTrackableId trackableId, UnityXRPose pose, UnityXRTrackingState trackingState)
    {
        if (ReferencePointProvider::GetInstance())
            return ReferencePointProvider::GetInstance()->UpdateReferencePoint(trackableId, pose, trackingState);
    }
}

UnityXRTrackableId ReferencePointProvider::AttachReferencePoint(const UnityXRTrackableId& planeId, const UnityXRPose& pose)
{
    auto planeProvider = PlaneProvider::GetInstance();
    if (planeProvider == nullptr)
        return kInvalidId;

    auto newId = GenerateTrackableId();
    if (newId == kInvalidId)
        return kInvalidId;

    UnityXRPlane plane;
    if (!planeProvider->TryGetPlaneWithoutBoundary(planeId, &plane))
        return kInvalidId;

    AttachedReferencePoint attachment = {};
    attachment.id = newId;
    attachment.pose = pose;
    attachment.trackingState = kUnityXRTrackingStateTracking;
    attachment.attacheeId = planeId;

    const auto planeNormal = Mul(plane.pose.rotation, kUp);
    attachment.distance = Dot(Sub(pose.position, plane.pose.position), planeNormal);

    std::lock_guard<std::mutex> lock(m_Mutex);
    m_Attachments[newId] = attachment;

    return newId;
}

void ReferencePointProvider::UpdateReferencePoint(
    UnityXRTrackableId trackableId, UnityXRPose pose,
    UnityXRTrackingState trackingState)
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    auto iter = m_ReferencePoints.find(trackableId);
    if (iter == m_ReferencePoints.end())
        return;

    auto& referencePoint = iter->second;
    referencePoint.pose = pose;
    referencePoint.trackingState = trackingState;
}

void ReferencePointProvider::AddReferencePoint(UnityXRReferencePoint referencePoint)
{
    m_ReferencePoints[referencePoint.id] = referencePoint;
}

void ReferencePointProvider::SetAddResponseData(unsigned long id0, unsigned long id1, bool result, int tracking)
{
    m_id0 = id0;
    m_id1 = id1;
    m_result = result;
    m_tracking = tracking;
}

bool UNITY_INTERFACE_API ReferencePointProvider::TryAddReferencePoint(const UnityXRPose& referencePointPose, UnityXRTrackableId& outReferencePointId, UnityXRTrackingState& outTrackingState)
{
    if (s_AddReferencePointHandler)
    {
        unsigned char* resultBytes = s_AddReferencePointHandler(referencePointPose.position.x,
            referencePointPose.position.y,
            referencePointPose.position.z,
            referencePointPose.rotation.x,
            referencePointPose.rotation.y,
            referencePointPose.rotation.z,
            referencePointPose.rotation.w);

        AddReferenceResult* result = reinterpret_cast<AddReferenceResult*>(resultBytes);

        if (m_result)
        {
            outReferencePointId.idPart[0] = m_id0;// result->id1;
            outReferencePointId.idPart[1] = m_id1;//result->id2;

            outTrackingState = (UnityXRTrackingState)m_tracking;// result->trackingState;
        }

        return m_result;
    }
    else
    {
        auto newId = GenerateTrackableId();
        if (newId == kInvalidId)
            return false;

        std::lock_guard<std::mutex> lock(m_Mutex);
        m_ReferencePoints[newId] = UnityXRReferencePoint
        {
            newId,
            referencePointPose,
            kUnityXRTrackingStateTracking
        };

        outReferencePointId = newId;
        outTrackingState = kUnityXRTrackingStateTracking;
        return true;
    }
}

bool UNITY_INTERFACE_API ReferencePointProvider::TryRemoveReferencePoint(const UnityXRTrackableId& referencePointId)
{
    if (s_RemoveReferencePointHandler)
        return s_RemoveReferencePointHandler(referencePointId.idPart[0], referencePointId.idPart[1]);

    std::lock_guard<std::mutex> lock(m_Mutex);
    {
        auto iter = m_ReferencePoints.find(referencePointId);
        if (iter != m_ReferencePoints.end())
        {
            m_ReferencePoints.erase(iter);
            return true;
        }
    }

    {
        auto iter = m_Attachments.find(referencePointId);
        if (iter != m_Attachments.end())
        {
            m_Attachments.erase(iter);
            return true;
        }
    }

    return false;
}

bool UNITY_INTERFACE_API ReferencePointProvider::GetAllReferencePoints(IUnityXRReferencePointAllocator& allocator)
{
    std::vector<UnityXRReferencePoint> referencePoints;
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        referencePoints.reserve(m_Attachments.size() + m_ReferencePoints.size());

        for (auto iter : m_ReferencePoints)
            referencePoints.push_back(iter.second);

        if (auto planeProvider = PlaneProvider::GetInstance())
        {
            auto planes = planeProvider->GetPlanesWithoutBoundaries();

            std::vector<UnityXRTrackableId> attachmentsToRemove;
            for (auto iter : m_Attachments)
            {
                auto& attacher = iter.second;
                auto planeIter = planes.find(attacher.attacheeId);

                // The plane may have been removed. If so, remove
                // the attached reference point.
                if (planeIter == planes.end())
                {
                    attachmentsToRemove.push_back(iter.first);
                    continue;
                }

                const auto& plane = planeIter->second;

                // Update position based on current distance to plane
                const auto planeNormal = Mul(plane.pose.rotation, kUp);
                const float distanceToPlane = Dot(Sub(attacher.pose.position, plane.pose.position), planeNormal);
                attacher.pose.position = Add(attacher.pose.position, Mul(planeNormal, (attacher.distance - distanceToPlane)));

                referencePoints.push_back(attacher);
            }

            for (const auto& id : attachmentsToRemove)
            {
                m_Attachments.erase(id);
            }
        }
    }

    std::copy(
        referencePoints.begin(),
        referencePoints.end(),
        allocator.AllocateReferencePoints(referencePoints.size()));

    return true;
}

struct ReferencePointAllocatorWrapper : public IUnityXRReferencePointAllocator
{
    ReferencePointAllocatorWrapper(IUnityXRReferencePointInterface* unityInterface, UnityXRReferencePointDataAllocator* allocator)
        : m_UnityInterface(unityInterface)
        , m_Allocator(allocator)
    {}

    virtual UnityXRReferencePoint* AllocateReferencePoints(
        size_t numReferencePoints) override
    {
        return m_UnityInterface->Allocator_AllocateReferencePoints(m_Allocator, numReferencePoints);
    }

private:
    IUnityXRReferencePointInterface * m_UnityInterface;
    UnityXRReferencePointDataAllocator* m_Allocator;
};

UnitySubsystemErrorCode ReferencePointProvider::RegisterAsCProvider(UnitySubsystemHandle handle, IUnityXRReferencePointInterface* referencePointInterface)
{
    m_CInterface = referencePointInterface;
    UnityXRReferencePointProvider provider = {};
    provider.userData = this;
    provider.GetAllReferencePoints = &StaticGetAllReferencePoints;
    provider.TryAddReferencePoint = &StaticTryAddReferencePoint;
    provider.TryRemoveReferencePoint = &StaticTryRemoveReferencePoint;
    return referencePointInterface->RegisterReferencePointProvider(handle, &provider);
}

UnitySubsystemErrorCode UNITY_INTERFACE_API ReferencePointProvider::StaticTryAddReferencePoint(
    UnitySubsystemHandle handle,
    void* userData,
    const UnityXRPose * referencePointPose,
    UnityXRTrackableId * outReferencePointId,
    UnityXRTrackingState * outTrackingState)
{
    if (ReferencePointProvider* provider = ReferencePointProvider::GetInstance())
    {
        return provider->TryAddReferencePoint(*referencePointPose, *outReferencePointId, *outTrackingState) ? kUnitySubsystemErrorCodeSuccess : kUnitySubsystemErrorCodeFailure;
    }

    return kUnitySubsystemErrorCodeFailure;
}

UnitySubsystemErrorCode UNITY_INTERFACE_API ReferencePointProvider::StaticTryRemoveReferencePoint(
    UnitySubsystemHandle handle,
    void* userData,
    const UnityXRTrackableId * referencePointId)
{
    if (ReferencePointProvider* provider = ReferencePointProvider::GetInstance())
    {
        return provider->TryRemoveReferencePoint(*referencePointId) ? kUnitySubsystemErrorCodeSuccess : kUnitySubsystemErrorCodeFailure;
    }

    return kUnitySubsystemErrorCodeFailure;
}

UnitySubsystemErrorCode UNITY_INTERFACE_API ReferencePointProvider::StaticGetAllReferencePoints(
    UnitySubsystemHandle handle,
    void* userData,
    UnityXRReferencePointDataAllocator * allocator)
{
    if (ReferencePointProvider* provider = ReferencePointProvider::GetInstance())
    {
        ReferencePointAllocatorWrapper wrappedAllocator(provider->m_CInterface, allocator);
        return provider->GetAllReferencePoints(wrappedAllocator) ? kUnitySubsystemErrorCodeSuccess : kUnitySubsystemErrorCodeFailure;
    }

    return kUnitySubsystemErrorCodeFailure;
}
