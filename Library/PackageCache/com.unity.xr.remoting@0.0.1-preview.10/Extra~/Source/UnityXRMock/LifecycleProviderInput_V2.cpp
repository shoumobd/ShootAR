#include "LifecycleProviderInput_V2.h"

using namespace UnityXRInput_V2;

const UnityXRInternalInputDeviceId kInputDeviceId = 0;

LifecycleProviderInput_V2::LifecycleProviderInput_V2()
    : m_Initialized(false)
{
}

LifecycleProviderInput_V2::~LifecycleProviderInput_V2()
{
    if (m_Initialized)
        ShutdownImpl();
}

UnitySubsystemErrorCode UNITY_INTERFACE_API LifecycleProviderInput_V2::Initialize(IUnitySubsystem* subsystem)
{
	InputProviderV2::Construct();

    if (m_Initialized)
    {
        return kUnitySubsystemErrorCodeFailure;
    }

    auto* xrInputSubsystem = static_cast<UnityXRInput_V2::IUnityXRInputSubsystem*>(subsystem);
    if (nullptr == xrInputSubsystem)
    {
        return kUnitySubsystemErrorCodeFailure;
    }

	xrInputSubsystem->RegisterProvider(InputProviderV2::GetInstance());
	InputProviderV2::GetInstance()->Subsystem = xrInputSubsystem;
    m_Initialized = true;
    return kUnitySubsystemErrorCodeSuccess;
}

void UNITY_INTERFACE_API LifecycleProviderInput_V2::Shutdown(IUnitySubsystem* /*subsystem*/)
{
    ShutdownImpl();
}

UnitySubsystemErrorCode UNITY_INTERFACE_API LifecycleProviderInput_V2::Start(IUnitySubsystem* subsystem)
{
    auto* xrInputSubsystem = static_cast<UnityXRInput_V2::IUnityXRInputSubsystem*>(subsystem);
    xrInputSubsystem->DeviceConnected(kInputDeviceId);
    return kUnitySubsystemErrorCodeSuccess;
}

void UNITY_INTERFACE_API LifecycleProviderInput_V2::Stop(IUnitySubsystem* subsystem)
{
	auto* xrInputSubsystem = static_cast<UnityXRInput_V2::IUnityXRInputSubsystem*>(subsystem);
    xrInputSubsystem->DeviceDisconnected(kInputDeviceId);
}

void LifecycleProviderInput_V2::ShutdownImpl()
{
	m_Initialized = false;
}
