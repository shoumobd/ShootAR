#pragma once
#include "IUnityXRSession.deprecated.h"
#include "XRProvider.h"

class SessionProvider : public XRProvider<SessionProvider, IUnityXRSessionProvider>
{
public:
	SessionProvider();
    ~SessionProvider();

	UnitySubsystemErrorCode RegisterAsCProvider(UnitySubsystemHandle handle, IUnityXRSessionInterface* sessionInterface);

	void SetTrackingState(int state);

private:
	virtual UnityXRTrackingState UNITY_INTERFACE_API GetTrackingState();

	virtual void UNITY_INTERFACE_API BeginFrame();

	virtual void UNITY_INTERFACE_API BeforeRender();

	virtual void UNITY_INTERFACE_API ApplicationPaused();

	virtual void UNITY_INTERFACE_API ApplicationResumed();

	static UnityXRTrackingState UNITY_INTERFACE_API StaticGetTrackingState(UnitySubsystemHandle handle, void* userData);
	static void UNITY_INTERFACE_API StaticBeginFrame(UnitySubsystemHandle handle, void* userData);
	static void UNITY_INTERFACE_API StaticBeforeRender(UnitySubsystemHandle handle, void* userData);
	static void UNITY_INTERFACE_API StaticApplicationPaused(UnitySubsystemHandle handle, void* userData);
	static void UNITY_INTERFACE_API StaticApplicationResumed(UnitySubsystemHandle handle, void* userData);

	UnityXRTrackingState trackingState;

	IUnityXRSessionInterface* m_CInterface = nullptr;
};
