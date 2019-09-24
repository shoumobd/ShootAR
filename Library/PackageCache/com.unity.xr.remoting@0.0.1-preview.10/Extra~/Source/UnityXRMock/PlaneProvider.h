#pragma once
#include "IUnityXRPlane.deprecated.h"
#include "IUnityXRRaycast.h"
#include "XRProvider.h"
#include "TrackableIdHelpers.h"
#include "Ray.h"

#include <vector>
#include <unordered_map>
#include <mutex>

struct PlaneWithBoundary
{
    PlaneWithBoundary() = default;
    PlaneWithBoundary(const UnityXRPlane& xrPlane) : plane(xrPlane) {}

    UnityXRPlane plane = {};
    std::vector<UnityXRVector3> boundaryPoints;
};

typedef std::unordered_map<UnityXRTrackableId, PlaneWithBoundary> IdToPlaneMap;
typedef std::unordered_map<UnityXRTrackableId, UnityXRPlane> IdToUnityXRPlaneMap;

class PlaneProvider : public XRProvider<PlaneProvider, IUnityXRPlaneProvider>
{
public:

    void SetPlaneData(const PlaneWithBoundary& plane);

    void RemovePlane(const UnityXRTrackableId& planeId);

    std::vector<UnityXRRaycastHit> Raycast(const Ray& ray, UnityXRTrackableType hitFlags) const;

    bool TryGetPlaneWithoutBoundary(
        const UnityXRTrackableId& planeId, UnityXRPlane* planeOut) const;

    IdToUnityXRPlaneMap GetPlanesWithoutBoundaries() const;

    UnitySubsystemErrorCode RegisterAsCProvider(UnitySubsystemHandle handle, IUnityXRPlaneInterface* planeInterface);

private:

    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticGetAllPlanes(
        UnitySubsystemHandle handle,
        void* userData,
        UnityXRPlaneDataAllocator* allocator);

    bool UNITY_INTERFACE_API GetAllPlanes(IUnityXRPlaneDataAllocator& allocator);

    IdToPlaneMap m_Planes;

    IUnityXRPlaneInterface* m_CInterface = nullptr;

    mutable std::mutex m_PlaneMutex;
};
