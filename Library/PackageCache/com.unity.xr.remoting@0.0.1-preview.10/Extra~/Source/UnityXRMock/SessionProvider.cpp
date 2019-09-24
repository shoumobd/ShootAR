#include <cstring>
#include "SessionProvider.h"

extern "C"
{
	UNITY_INTERFACE_EXPORT void UnityXRMock_setTrackingState(int trackingState)
	{
		SessionProvider::GetInstance()->SetTrackingState(trackingState);
	}
}

SessionProvider::SessionProvider()
{
}

SessionProvider::~SessionProvider()
{
}

void SessionProvider::SetTrackingState(int state)
{
	trackingState = (UnityXRTrackingState)state;
}

UnityXRTrackingState UNITY_INTERFACE_API SessionProvider::GetTrackingState()
{
	return trackingState;
}

void UNITY_INTERFACE_API SessionProvider::BeginFrame()
{

}

void UNITY_INTERFACE_API SessionProvider::BeforeRender()
{

}

void UNITY_INTERFACE_API SessionProvider::ApplicationPaused()
{

}

void UNITY_INTERFACE_API SessionProvider::ApplicationResumed()
{

}

UnitySubsystemErrorCode SessionProvider::RegisterAsCProvider(UnitySubsystemHandle handle, IUnityXRSessionInterface* sessionInterface)
{
	m_CInterface = sessionInterface;
	UnityXRSessionProvider provider = {};
	provider.userData = this;
	provider.GetTrackingState = &StaticGetTrackingState;
	provider.ApplicationPaused = &StaticApplicationPaused;
	provider.ApplicationResumed = &StaticApplicationResumed;
	provider.BeforeRender = &StaticBeforeRender;
	provider.BeginFrame = &StaticBeginFrame;
	return sessionInterface->RegisterSessionProvider(handle, &provider);
}

UnityXRTrackingState UNITY_INTERFACE_API SessionProvider::StaticGetTrackingState(UnitySubsystemHandle handle, void* userData)
{
	auto thiz = static_cast<SessionProvider*>(userData);
	if (thiz == nullptr)
		return kUnityXRTrackingStateUnknown;

	return thiz->GetTrackingState();
}

void UNITY_INTERFACE_API SessionProvider::StaticBeginFrame(UnitySubsystemHandle handle, void* userData)
{
	if (SessionProvider* provider = SessionProvider::GetInstance())
	{
		provider->BeginFrame();
	}
}

void UNITY_INTERFACE_API SessionProvider::StaticBeforeRender(UnitySubsystemHandle handle, void* userData)
{
	if (SessionProvider* provider = SessionProvider::GetInstance())
	{
		provider->BeforeRender();
	}
}

void UNITY_INTERFACE_API SessionProvider::StaticApplicationPaused(UnitySubsystemHandle handle, void* userData)
{
	if (SessionProvider* provider = SessionProvider::GetInstance())
	{
		provider->ApplicationPaused();
	}
}

void UNITY_INTERFACE_API SessionProvider::StaticApplicationResumed(UnitySubsystemHandle handle, void* userData)
{
	if (SessionProvider* provider = SessionProvider::GetInstance())
	{
		provider->ApplicationResumed();
	}
}
