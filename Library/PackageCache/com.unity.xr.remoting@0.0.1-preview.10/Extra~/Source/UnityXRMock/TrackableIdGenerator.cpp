#include "IUnityInterface.h"
#include "UnityXRTrackable.h"

#ifdef __cplusplus
extern "C" {
#endif
    typedef UnityXRTrackableId(UNITY_INTERFACE_API * TrackableIdGenerator)();

    static TrackableIdGenerator s_TrackableIdGenerator = nullptr;

    void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityXRMock_setTrackableIdGenerator(TrackableIdGenerator generator)
    {
        s_TrackableIdGenerator = generator;
    }

    UnityXRTrackableId UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityXRMock_createTrackableId(UnityXRTrackableId trackableId)
    {
        return trackableId;
    }

#ifdef __cplusplus
} // extern "C"
#endif

UnityXRTrackableId GenerateTrackableId()
{
    if (s_TrackableIdGenerator)
        return s_TrackableIdGenerator();

    return UnityXRTrackableId{};
}
