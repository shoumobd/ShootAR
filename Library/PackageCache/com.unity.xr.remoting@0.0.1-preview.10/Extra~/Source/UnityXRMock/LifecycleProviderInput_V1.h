#pragma once

#include "InputProviderV1.h"

class LifecycleProviderInput_V1 : public IUnityLifecycleProvider
{
public:
    LifecycleProviderInput_V1();
    ~LifecycleProviderInput_V1();

    virtual UnitySubsystemErrorCode UNITY_INTERFACE_API Initialize(IUnitySubsystem* subsystem) override;
    virtual void UNITY_INTERFACE_API Shutdown(IUnitySubsystem* subsystem) override;

    virtual UnitySubsystemErrorCode UNITY_INTERFACE_API Start(IUnitySubsystem* subsystem) override;
    virtual void UNITY_INTERFACE_API Stop(IUnitySubsystem* subsystem) override;

private:
    void ShutdownImpl();

    InputProviderV1 m_InputProvider;
    bool m_Initialized;
};
