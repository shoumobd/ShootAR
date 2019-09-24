#include <cstring>
#include "PlaneProvider.h"
#include "UnityMath.h"

extern "C"
{   
    struct PlaneData
    {
        int eventType;

        float x, y, z;

        float px, py, pz;
        float rx, ry, rz, rw;

        float bx, by;

        int alignment;

        unsigned long id1;
        unsigned long id2;

        unsigned long sid1;
        unsigned long sid2;
    };

    void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityXRMock_setPlaneData(
        UnityXRTrackableId planeId, UnityXRPose pose, UnityXRVector3 center,
        UnityXRVector2 bounds, UnityXRVector3* boundaryPoints, int numPoints)
    {
        if (PlaneProvider::GetInstance() == nullptr)
            return;

        PlaneWithBoundary plane;
        plane.plane.id = planeId;
        plane.plane.center = center;
        plane.plane.pose = pose;
        plane.plane.bounds = bounds;
        plane.plane.wasUpdated = true;

        if (boundaryPoints && numPoints > 0)
            plane.boundaryPoints.insert(plane.boundaryPoints.end(), boundaryPoints, boundaryPoints + numPoints);

        PlaneProvider::GetInstance()->SetPlaneData(plane);
    }

    void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityXRMock_removePlane(UnityXRTrackableId id)
    {
        if (PlaneProvider::GetInstance())
            PlaneProvider::GetInstance()->RemovePlane(id);
    }

    UNITY_INTERFACE_EXPORT void UnityXRMock_processPlaneEvent(unsigned char* data, int size)
    {
        if (PlaneProvider::GetInstance() == nullptr)
            return;

        PlaneData* planeData = reinterpret_cast<PlaneData*>(data);

        uint64_t id_1 = planeData->id1;
        uint64_t id_2 = planeData->id2;

        UnityXRTrackableId trackableId;
        trackableId.idPart[0] = id_1;
        trackableId.idPart[1] = id_2;

        if (planeData->eventType == 3)
        {
            PlaneProvider::GetInstance()->RemovePlane(trackableId);
            return;
        }

        UnityXRVector3 center;
        center.x = planeData->x;
        center.y = planeData->y;
        center.z = planeData->z;

        UnityXRVector3 position;
        position.x = planeData->px;
        position.y = planeData->py;
        position.z = planeData->pz;

        UnityXRVector4 rotation;
        rotation.x = planeData->rx;
        rotation.y = planeData->ry;
        rotation.z = planeData->rz;
        rotation.w = planeData->rw;

        UnityXRPlane plane;
        plane.center.x = center.x;
        plane.center.y = center.y;
        plane.center.z = center.z;

        plane.pose.position.x = position.x;
        plane.pose.position.y = position.y;
        plane.pose.position.z = position.z;

        plane.pose.rotation.x = rotation.x;
        plane.pose.rotation.y = rotation.y;
        plane.pose.rotation.z = rotation.z;
        plane.pose.rotation.w = rotation.w;

        plane.bounds.x = planeData->bx;
        plane.bounds.y = planeData->by;

        if (planeData->eventType == 1)
        {
            plane.wasUpdated = false;
        }
        else if (planeData->eventType == 2)
        {
            plane.wasUpdated = true;
        }

        plane.wasMerged = false;

        plane.id = trackableId;

       PlaneProvider::GetInstance()->SetPlaneData(PlaneWithBoundary(plane));
    }
}

struct PlaneDataAllocatorWrapper : public IUnityXRPlaneDataAllocator
{
    PlaneDataAllocatorWrapper(IUnityXRPlaneInterface* unityInterface, UnityXRPlaneDataAllocator* allocator)
        : m_UnityInterface(unityInterface)
        , m_Allocator(allocator)
    {}

    virtual UnityXRPlane* AllocatePlaneData(size_t numPlanes) override
    {
        return m_UnityInterface->Allocator_AllocatePlaneData(m_Allocator, numPlanes);
    }

    virtual UnityXRVector3* AllocateBoundaryPoints(const UnityXRTrackableId& planeId, size_t numPoints) override
    {
        return m_UnityInterface->Allocator_AllocateBoundaryPoints(m_Allocator, &planeId, numPoints);
    }

private:
    IUnityXRPlaneInterface * m_UnityInterface;

    UnityXRPlaneDataAllocator* m_Allocator;
};

void PlaneProvider::SetPlaneData(const PlaneWithBoundary& plane)
{
    std::lock_guard<std::mutex> lock(m_PlaneMutex);
    m_Planes[plane.plane.id] = plane;
}

void PlaneProvider::RemovePlane(const UnityXRTrackableId& id)
{
    std::lock_guard<std::mutex> lock(m_PlaneMutex);
    m_Planes.erase(id);
}

bool PlaneProvider::TryGetPlaneWithoutBoundary(const UnityXRTrackableId& planeId, UnityXRPlane* planeOut) const
{
    std::lock_guard<std::mutex> lock(m_PlaneMutex);
    const auto iter = m_Planes.find(planeId);
    if (iter == m_Planes.end())
        return false;

    *planeOut = iter->second.plane;
    return true;
}

IdToUnityXRPlaneMap PlaneProvider::GetPlanesWithoutBoundaries() const
{
    IdToUnityXRPlaneMap planes;
    std::lock_guard<std::mutex> lock(m_PlaneMutex);
    for (auto iter : m_Planes)
        planes[iter.first] = iter.second.plane;

    return planes;
}

bool UNITY_INTERFACE_API PlaneProvider::GetAllPlanes(IUnityXRPlaneDataAllocator& allocator)
{
    UnityXRPlane* planesOut = allocator.AllocatePlaneData(m_Planes.size());
    for (auto& iter : m_Planes)
    {
        auto& planeWithBoundary = iter.second;

        UnityXRPlane& plane = planeWithBoundary.plane;
        *planesOut = plane;
        plane.wasUpdated = false;
        ++planesOut;

        const auto& boundaryPoints = planeWithBoundary.boundaryPoints;
        if (boundaryPoints.size() > 0)
        {
            auto pointsOut = allocator.AllocateBoundaryPoints(plane.id, boundaryPoints.size());
            std::copy(boundaryPoints.begin(), boundaryPoints.end(), pointsOut);
        }
    }

    return true;
}

UnitySubsystemErrorCode PlaneProvider::RegisterAsCProvider(UnitySubsystemHandle handle, IUnityXRPlaneInterface* planeInterface)
{
    m_CInterface = planeInterface;
    UnityXRPlaneProvider provider = {};
    provider.userData = this;
    provider.GetAllPlanes = &StaticGetAllPlanes;
    return planeInterface->RegisterPlaneProvider(handle, &provider);
}

UnitySubsystemErrorCode UNITY_INTERFACE_API PlaneProvider::StaticGetAllPlanes(
    UnitySubsystemHandle handle,
    void* userData,
    UnityXRPlaneDataAllocator * allocator)
{
    if (PlaneProvider* provider = PlaneProvider::GetInstance())
    {
        PlaneDataAllocatorWrapper wrappedAllocator(provider->m_CInterface, allocator);
        return provider->GetAllPlanes(wrappedAllocator) ? kUnitySubsystemErrorCodeSuccess : kUnitySubsystemErrorCodeFailure;
    }

    return kUnitySubsystemErrorCodeFailure;
}

static inline bool WithinBounds(
    const UnityXRVector2& position,
    const UnityXRVector2& bounds)
{
    const UnityXRVector2 halfExtents = Mul(bounds, .5f);

    return
        std::abs(position.x) <= halfExtents.x &&
        std::abs(position.y) <= halfExtents.y;
}

static inline float GetCrossDirection(const UnityXRVector2& a, const UnityXRVector2& b)
{
    return a.x * b.y - a.y * b.x;
}

// See http://geomalgorithms.com/a03-_inclusion.html
static inline int WindingNumber(
    const UnityXRVector2& positionInPlaneSpace,
    const std::vector<UnityXRVector2>& boundaryInPlaneSpace)
{
    int windingNumber = 0;
    const UnityXRVector2& point = positionInPlaneSpace;
    const float zero = 0.f;
    for (size_t i = 0; i < boundaryInPlaneSpace.size(); ++i)
    {
        const size_t j = (i + 1) % boundaryInPlaneSpace.size();
        const UnityXRVector2& vi = boundaryInPlaneSpace[i];
        const UnityXRVector2& vj = boundaryInPlaneSpace[j];

        if (vi.y <= point.y)
        {
            if (vj.y > point.y)                                 // an upward crossing
                if (GetCrossDirection(Sub(vj, vi), Sub(point, vi)) < zero) // P left of  edge
                    ++windingNumber;
            // have  a valid up intersect
        }
        else
        {                                                       // y > P.y (no test needed)
            if (vj.y <= point.y)                                // a downward crossing
                if (GetCrossDirection(Sub(vj, vi), Sub(point, vi)) > zero) // P right of  edge
                    --windingNumber;
            // have  a valid down intersect
        }
    }

    return windingNumber;
}

static inline bool WithinPolygon(
    const UnityXRVector2& positionInPlaneSpace,
    const std::vector<UnityXRVector2>& boundaryInPlaneSpace)
{
    return WindingNumber(positionInPlaneSpace, boundaryInPlaneSpace) != 0;
}

std::vector<UnityXRRaycastHit> PlaneProvider::Raycast(const Ray& ray, UnityXRTrackableType hitFlags) const
{
    std::vector<UnityXRRaycastHit> hits;
    if ((hitFlags & kUnityXRTrackableTypePlanes) == kUnityXRTrackableTypeNone)
        return hits;

    const float eps = 1e-6f;

    const bool testWithinInfinity = hitFlags & kUnityXRTrackableTypePlaneWithinInfinity;
    const bool testWithinBounds = hitFlags & kUnityXRTrackableTypePlaneWithinBounds;
    const bool testWithinPolygon = hitFlags & kUnityXRTrackableTypePlaneWithinPolygon;

    std::lock_guard<std::mutex> lock(m_PlaneMutex);
    for (const auto iter : m_Planes)
    {
        const auto& plane = iter.second.plane;
        const auto& rotation = plane.pose.rotation;
        const auto invRotation = Inverse(rotation);
        const auto directionInPlaneSpace = Mul(invRotation, ray.direction);
        const float dDotN = directionInPlaneSpace.y;

        // If |dotN| <= eps, then ray is parallel to the plane.
        // If dotN > 0 then the hit is behind us.
        if (dDotN >= -eps)
            continue;

        const auto& center = plane.center;
        const auto originInPlaneSpace = Mul(invRotation, Sub(ray.origin, center));
        const float distance = -originInPlaneSpace.y / dDotN;

        const auto hitPositionPlaneSpace3d = Add(originInPlaneSpace, Mul(directionInPlaneSpace, distance));
        const UnityXRVector2 hitPositionPlaneSpace = {hitPositionPlaneSpace3d.x, hitPositionPlaneSpace3d.z};

        UnityXRTrackableType hitTeatureFlags = kUnityXRTrackableTypeNone;

        if (testWithinInfinity)
            hitTeatureFlags = static_cast<UnityXRTrackableType>(hitTeatureFlags | kUnityXRTrackableTypePlaneWithinInfinity);

        if (testWithinBounds && WithinBounds(hitPositionPlaneSpace, plane.bounds))
            hitTeatureFlags = static_cast<UnityXRTrackableType>(hitTeatureFlags | kUnityXRTrackableTypePlaneWithinBounds);

        if (testWithinPolygon)
        {
            const auto& boundaryPoints = iter.second.boundaryPoints;
            std::vector<UnityXRVector2> polygon2d(boundaryPoints.size());
            for (size_t i = 0; i < boundaryPoints.size(); ++i)
            {
                const auto& point = boundaryPoints[i];
                const auto pointInPlaneSpace = Mul(invRotation, Sub(point, center));
                polygon2d[i] = {pointInPlaneSpace.x, pointInPlaneSpace.z};
            }

            if (WithinPolygon(hitPositionPlaneSpace, polygon2d))
                hitTeatureFlags = static_cast<UnityXRTrackableType>(hitTeatureFlags | kUnityXRTrackableTypePlaneWithinPolygon);
        }

        if (hitTeatureFlags != kUnityXRTrackableTypeNone)
        {
            UnityXRRaycastHit hit;
            hit.trackableId = plane.id;
            hit.pose.position = Add(ray.origin, Mul(ray.direction, distance));
            hit.pose.rotation = rotation;
            hit.distance = distance;
            hit.hitType = hitTeatureFlags;
            hits.push_back(hit);
        }
    }

    return hits;
}
