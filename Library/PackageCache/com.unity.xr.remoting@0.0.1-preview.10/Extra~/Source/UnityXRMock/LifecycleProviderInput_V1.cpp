#include "LifecycleProviderInput_V1.h"

using namespace UnityXRInput_V1;

const UnityXRInternalInputDeviceId kInputDeviceId = 0;

LifecycleProviderInput_V1::LifecycleProviderInput_V1()
    : m_Initialized(false)
{
}

LifecycleProviderInput_V1::~LifecycleProviderInput_V1()
{
    if (m_Initialized)
        ShutdownImpl();
}

UnitySubsystemErrorCode UNITY_INTERFACE_API LifecycleProviderInput_V1::Initialize(IUnitySubsystem* subsystem)
{
	InputProviderV1::Construct();

    if (m_Initialized)
    {
        return kUnitySubsystemErrorCodeFailure;
    }

    IUnityXRInputSubsystem* xrInputSubsystem = static_cast<IUnityXRInputSubsystem*>(subsystem);
    if (nullptr == xrInputSubsystem)
    {
        return kUnitySubsystemErrorCodeFailure;
    }

	xrInputSubsystem->RegisterProvider(InputProviderV1::GetInstance());
	InputProviderV1::GetInstance()->Subsystem = xrInputSubsystem;
    m_Initialized = true;
    return kUnitySubsystemErrorCodeSuccess;
}

void UNITY_INTERFACE_API LifecycleProviderInput_V1::Shutdown(IUnitySubsystem* /*subsystem*/)
{
    ShutdownImpl();
}

UnitySubsystemErrorCode UNITY_INTERFACE_API LifecycleProviderInput_V1::Start(IUnitySubsystem* subsystem)
{
    IUnityXRInputSubsystem* xrInputSubsystem = static_cast<IUnityXRInputSubsystem*>(subsystem);
    xrInputSubsystem->DeviceConnected(kInputDeviceId);
    return kUnitySubsystemErrorCodeSuccess;
}

void UNITY_INTERFACE_API LifecycleProviderInput_V1::Stop(IUnitySubsystem* subsystem)
{
    IUnityXRInputSubsystem* xrInputSubsystem = static_cast<IUnityXRInputSubsystem*>(subsystem);
    xrInputSubsystem->DeviceDisconnected(kInputDeviceId);
}

void LifecycleProviderInput_V1::ShutdownImpl()
{
}
