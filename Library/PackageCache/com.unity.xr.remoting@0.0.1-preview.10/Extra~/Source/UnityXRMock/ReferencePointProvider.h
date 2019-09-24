#pragma once
#include "IUnityXRReferencePoint.deprecated.h"
#include "XRProvider.h"
#include "TrackableIdHelpers.h"

#include <unordered_map>
#include <mutex>

struct AttachedReferencePoint : UnityXRReferencePoint
{
    UnityXRTrackableId attacheeId;
    float distance;
};

typedef std::unordered_map<UnityXRTrackableId, UnityXRReferencePoint> IdToReferencePointMap;
typedef std::unordered_map<UnityXRTrackableId, AttachedReferencePoint> IdToAttachmentsMap;

class ReferencePointProvider : public XRProvider<ReferencePointProvider, IUnityXRReferencePointProvider>
{
public:

    UnitySubsystemErrorCode RegisterAsCProvider(UnitySubsystemHandle handle, IUnityXRReferencePointInterface* referencePointInterface);

    void AddReferencePoint(UnityXRReferencePoint referencePoint);

    void SetAddResponseData(unsigned long id0, unsigned long id1, bool result, int tracking);

    UnityXRTrackableId AttachReferencePoint(const UnityXRTrackableId& planeId, const UnityXRPose& pose);

    void UpdateReferencePoint(UnityXRTrackableId trackableId, UnityXRPose pose, UnityXRTrackingState trackingState);

private:

    bool UNITY_INTERFACE_API TryAddReferencePoint(const UnityXRPose& referencePointPose, UnityXRTrackableId& outReferencePointId, UnityXRTrackingState& outTrackingState) final;

    bool UNITY_INTERFACE_API TryRemoveReferencePoint(const UnityXRTrackableId& referencePointId) final;

    bool UNITY_INTERFACE_API GetAllReferencePoints(IUnityXRReferencePointAllocator& allocator) final;

    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticTryAddReferencePoint(
        UnitySubsystemHandle handle,
        void* userData,
        const UnityXRPose * referencePointPose,
        UnityXRTrackableId * outReferencePointId,
        UnityXRTrackingState * outTrackingState);

    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticTryRemoveReferencePoint(
        UnitySubsystemHandle handle,
        void* userData,
        const UnityXRTrackableId * referencePointId);

    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticGetAllReferencePoints(
        UnitySubsystemHandle handle,
        void* userData,
        UnityXRReferencePointDataAllocator * allocator);


    unsigned long  m_id0;
    unsigned long  m_id1;
    bool m_result;
    int m_tracking;

    IdToReferencePointMap m_ReferencePoints;

    IdToAttachmentsMap m_Attachments;

    std::mutex m_Mutex;

    IUnityXRReferencePointInterface* m_CInterface = nullptr;
};
