//
//  XR-SDK-ARKit.m
//  XR-SDK-ARKit
//
//  Created by Lukasz Pasek on 11/21/17.
//  Copyright © 2017 Unity. All rights reserved.
//
#include "IUnityInterface.h"
#include "UnityXRTypes.h"
#include "CameraProvider.h"
#include "PlaneProvider.h"
#include "InputProvider.h"
#include "InputProviderV1.h"
#include "SessionProvider.h"
#include "RaycastProvider.h"
#include "DepthProvider.h"
#include "ReferencePointProvider.h"

#include "LifecycleProviderInput_V1.h"
#include "LifecycleProviderInput_V2.h"

class LifecycleProviderCamera : public IUnityLifecycleProvider
{
public:
    UnitySubsystemErrorCode SetUnityInterfaceAndRegister(IUnityXRCameraInterface* cStyleInterface, const char* subsystemId)
    {
        m_UnityInterface = cStyleInterface;

        UnityLifecycleProvider provider;
        std::memset(&provider, 0, sizeof(provider));

        provider.userData = this;
        provider.Initialize = &StaticInitialize;
        provider.Shutdown = &StaticShutdown;
        provider.Start = &StaticStart;
        provider.Stop = &StaticStop;

        return cStyleInterface->RegisterLifecycleProvider("UnityXRMock", subsystemId, &provider);
    }

private:
    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticInitialize(UnitySubsystemHandle handle, void* userData)
    {
        LifecycleProviderCamera* thiz = static_cast<LifecycleProviderCamera*>(userData);
        if (thiz == nullptr)
            return kUnitySubsystemErrorCodeInvalidArguments;

        CameraProvider::Construct();
        return CameraProvider::GetInstance()->RegisterAsCProvider(handle, thiz->m_UnityInterface);
    }

    static void UNITY_INTERFACE_API StaticShutdown(UnitySubsystemHandle handle, void* userData)
    {
        LifecycleProviderCamera* thiz = static_cast<LifecycleProviderCamera*>(userData);
        if (thiz == nullptr)
            return;

        thiz->Shutdown(nullptr);
    }

    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticStart(UnitySubsystemHandle handle, void* userData)
    {
        return kUnitySubsystemErrorCodeSuccess;
    }

    static void UNITY_INTERFACE_API StaticStop(UnitySubsystemHandle handle, void* userData)
    {

    }

    UnitySubsystemErrorCode UNITY_INTERFACE_API Initialize(IUnitySubsystem* inst) final
    {
        CameraProvider::Construct();
        
        // Register with Unity
        auto* camera = static_cast<IUnityXRCameraSubsystem*>(inst);
        camera->RegisterCameraProvider(CameraProvider::GetInstance()); 
        

        return kUnitySubsystemErrorCodeSuccess;
    }
    
    UnitySubsystemErrorCode UNITY_INTERFACE_API Start(IUnitySubsystem* inst) final
    {
        return kUnitySubsystemErrorCodeSuccess;
    }
    
    void UNITY_INTERFACE_API Stop(IUnitySubsystem* inst) final
    {
    }
    
    void UNITY_INTERFACE_API Shutdown(IUnitySubsystem* inst) final
    {
        CameraProvider::Destroy();
    }

private:
    CameraProvider m_CameraProvider;
    IUnityXRCameraInterface* m_UnityInterface;
};

class LifecycleProviderPlane : public IUnityLifecycleProvider
{
public:
    UnitySubsystemErrorCode SetUnityInterfaceAndRegister(IUnityXRPlaneInterface* cStyleInterface, const char* subsystemId)
    {
        m_UnityInterface = cStyleInterface;

        UnityLifecycleProvider provider;
        std::memset(&provider, 0, sizeof(provider));

        provider.userData = this;
        provider.Initialize = &StaticInitialize;
        provider.Shutdown = &StaticShutdown;
        provider.Start = &StaticStart;
        provider.Stop = &StaticStop;

        return cStyleInterface->RegisterLifecycleProvider("UnityXRMock", subsystemId, &provider);
    }

private:
    IUnityXRPlaneInterface* m_UnityInterface;

    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticInitialize(UnitySubsystemHandle handle, void* userData)
    {
        LifecycleProviderPlane* thiz = static_cast<LifecycleProviderPlane*>(userData);
        if (thiz == nullptr)
            return kUnitySubsystemErrorCodeInvalidArguments;

        PlaneProvider::Construct();
        return PlaneProvider::GetInstance()->RegisterAsCProvider(handle, thiz->m_UnityInterface);
    }

    static void UNITY_INTERFACE_API StaticShutdown(UnitySubsystemHandle handle, void* userData)
    {
        LifecycleProviderPlane* thiz = static_cast<LifecycleProviderPlane*>(userData);
        if (thiz == nullptr)
            return;

        thiz->Shutdown(nullptr);
    }

    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticStart(UnitySubsystemHandle handle, void* userData)
    {
        return kUnitySubsystemErrorCodeSuccess;
    }

    static void UNITY_INTERFACE_API StaticStop(UnitySubsystemHandle handle, void* userData)
    {

    }

    UnitySubsystemErrorCode UNITY_INTERFACE_API Initialize(IUnitySubsystem* inst) final
    {
        PlaneProvider::Construct();

        // Register with Unity
        auto* camera = static_cast<IUnityXRPlaneSubsystem*>(inst);
        camera->RegisterPlaneProvider(PlaneProvider::GetInstance());


        return kUnitySubsystemErrorCodeSuccess;
    }

    UnitySubsystemErrorCode UNITY_INTERFACE_API Start(IUnitySubsystem* inst) final
    {
        return kUnitySubsystemErrorCodeSuccess;
    }

    void UNITY_INTERFACE_API Stop(IUnitySubsystem* inst) final
    {
    }

    void UNITY_INTERFACE_API Shutdown(IUnitySubsystem* inst) final
    {
        PlaneProvider::Destroy();
    }
};

class LifecycleProviderSession : public IUnityLifecycleProvider
{
public:
    UnitySubsystemErrorCode SetUnityInterfaceAndRegister(IUnityXRSessionInterface* cStyleInterface, const char* subsystemId)
    {
        m_UnityInterface = cStyleInterface;

        UnityLifecycleProvider provider;
        std::memset(&provider, 0, sizeof(provider));

        provider.userData = this;
        provider.Initialize = &StaticInitialize;
        provider.Shutdown = &StaticShutdown;
        provider.Start = &StaticStart;
        provider.Stop = &StaticStop;

        return cStyleInterface->RegisterLifecycleProvider("UnityXRMock", subsystemId, &provider);
    }

private:
    IUnityXRSessionInterface* m_UnityInterface;

    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticInitialize(UnitySubsystemHandle handle, void* userData)
    {
        LifecycleProviderSession* thiz = static_cast<LifecycleProviderSession*>(userData);
        if (thiz == nullptr)
            return kUnitySubsystemErrorCodeInvalidArguments;

        SessionProvider::Construct();
        return SessionProvider::GetInstance()->RegisterAsCProvider(handle, thiz->m_UnityInterface);
    }

    static void UNITY_INTERFACE_API StaticShutdown(UnitySubsystemHandle handle, void* userData)
    {
        LifecycleProviderSession* thiz = static_cast<LifecycleProviderSession*>(userData);
        if (thiz == nullptr)
            return;

        thiz->Shutdown(nullptr);
    }

    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticStart(UnitySubsystemHandle handle, void* userData)
    {
        return kUnitySubsystemErrorCodeSuccess;
    }

    static void UNITY_INTERFACE_API StaticStop(UnitySubsystemHandle handle, void* userData)
    {

    }

    UnitySubsystemErrorCode UNITY_INTERFACE_API Initialize(IUnitySubsystem* inst) final
    {
        SessionProvider::Construct();

        // Register with Unity
        auto* session = static_cast<IUnityXRSessionSubsystem*>(inst);
        session->RegisterSessionProvider(SessionProvider::GetInstance());


        return kUnitySubsystemErrorCodeSuccess;
    }

    UnitySubsystemErrorCode UNITY_INTERFACE_API Start(IUnitySubsystem* inst) final
    {
        return kUnitySubsystemErrorCodeSuccess;
    }

    void UNITY_INTERFACE_API Stop(IUnitySubsystem* inst) final
    {
    }

    void UNITY_INTERFACE_API Shutdown(IUnitySubsystem* inst) final
    {
        SessionProvider::Destroy();
    }
};

class LifecycleProviderReferencePoint : public IUnityLifecycleProvider
{
public:
    UnitySubsystemErrorCode SetUnityInterfaceAndRegister(IUnityXRReferencePointInterface* cStyleInterface, const char* subsystemId)
    {
        m_UnityInterface = cStyleInterface;

        UnityLifecycleProvider provider;
        std::memset(&provider, 0, sizeof(provider));

        provider.userData = this;
        provider.Initialize = &StaticInitialize;
        provider.Shutdown = &StaticShutdown;
        provider.Start = &StaticStart;
        provider.Stop = &StaticStop;

        return cStyleInterface->RegisterLifecycleProvider("UnityXRMock", subsystemId, &provider);
    }

private:
    IUnityXRReferencePointInterface* m_UnityInterface;

    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticInitialize(UnitySubsystemHandle handle, void* userData)
    {
        LifecycleProviderReferencePoint* thiz = static_cast<LifecycleProviderReferencePoint*>(userData);
        if (thiz == nullptr)
            return kUnitySubsystemErrorCodeInvalidArguments;

        ReferencePointProvider::Construct();
        return ReferencePointProvider::GetInstance()->RegisterAsCProvider(handle, thiz->m_UnityInterface);
    }

    static void UNITY_INTERFACE_API StaticShutdown(UnitySubsystemHandle handle, void* userData)
    {
        LifecycleProviderReferencePoint* thiz = static_cast<LifecycleProviderReferencePoint*>(userData);
        if (thiz == nullptr)
            return;

        thiz->Shutdown(nullptr);
    }

    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticStart(UnitySubsystemHandle handle, void* userData)
    {
        return kUnitySubsystemErrorCodeSuccess;
    }

    static void UNITY_INTERFACE_API StaticStop(UnitySubsystemHandle handle, void* userData)
    {

    }

    UnitySubsystemErrorCode UNITY_INTERFACE_API Initialize(IUnitySubsystem* inst) final
    {
        ReferencePointProvider::Construct();

        // Register with Unity
        auto* camera = static_cast<IUnityXRReferencePointSubsystem*>(inst);
        camera->RegisterReferencePointProvider(ReferencePointProvider::GetInstance());


        return kUnitySubsystemErrorCodeSuccess;
    }

    UnitySubsystemErrorCode UNITY_INTERFACE_API Start(IUnitySubsystem* inst) final
    {
        return kUnitySubsystemErrorCodeSuccess;
    }

    void UNITY_INTERFACE_API Stop(IUnitySubsystem* inst) final
    {
    }

    void UNITY_INTERFACE_API Shutdown(IUnitySubsystem* inst) final
    {
        ReferencePointProvider::Destroy();
    }
};

class LifecycleProviderDepth : public IUnityLifecycleProvider
{
public:
    UnitySubsystemErrorCode SetUnityInterfaceAndRegister(IUnityXRDepthInterface* cStyleInterface, const char* subsystemId)
    {
        m_UnityInterface = cStyleInterface;

        UnityLifecycleProvider provider;
        std::memset(&provider, 0, sizeof(provider));

        provider.userData = this;
        provider.Initialize = &StaticInitialize;
        provider.Shutdown = &StaticShutdown;
        provider.Start = &StaticStart;
        provider.Stop = &StaticStop;

        return cStyleInterface->RegisterLifecycleProvider("UnityXRMock", subsystemId, &provider);
    }

private:
    IUnityXRDepthInterface* m_UnityInterface;

    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticInitialize(UnitySubsystemHandle handle, void* userData)
    {
        LifecycleProviderDepth* thiz = static_cast<LifecycleProviderDepth*>(userData);
        if (thiz == nullptr)
            return kUnitySubsystemErrorCodeInvalidArguments;

        DepthProvider::Construct();
        return DepthProvider::GetInstance()->RegisterAsCProvider(handle, thiz->m_UnityInterface);
    }

    static void UNITY_INTERFACE_API StaticShutdown(UnitySubsystemHandle handle, void* userData)
    {
        LifecycleProviderDepth* thiz = static_cast<LifecycleProviderDepth*>(userData);
        if (thiz == nullptr)
            return;

        thiz->Shutdown(nullptr);
    }

    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticStart(UnitySubsystemHandle handle, void* userData)
    {
        return kUnitySubsystemErrorCodeSuccess;
    }

    static void UNITY_INTERFACE_API StaticStop(UnitySubsystemHandle handle, void* userData)
    {

    }

    UnitySubsystemErrorCode UNITY_INTERFACE_API Initialize(IUnitySubsystem* inst) final
    {
        DepthProvider::Construct();

        // Register with Unity
        auto* camera = static_cast<IUnityXRDepthSubsystem*>(inst);
        camera->RegisterDepthProvider(DepthProvider::GetInstance());


        return kUnitySubsystemErrorCodeSuccess;
    }

    UnitySubsystemErrorCode UNITY_INTERFACE_API Start(IUnitySubsystem* inst) final
    {
        return kUnitySubsystemErrorCodeSuccess;
    }

    void UNITY_INTERFACE_API Stop(IUnitySubsystem* inst) final
    {
    }

    void UNITY_INTERFACE_API Shutdown(IUnitySubsystem* inst) final
    {
        DepthProvider::Destroy();
    }
};

class LifecycleProviderRaycast : public IUnityLifecycleProvider
{
public:
    UnitySubsystemErrorCode SetUnityInterfaceAndRegister(IUnityXRRaycastInterface* cStyleInterface, const char* subsystemId)
    {
        m_UnityInterface = cStyleInterface;

        UnityLifecycleProvider provider;
        std::memset(&provider, 0, sizeof(provider));

        provider.userData = this;
        provider.Initialize = &StaticInitialize;
        provider.Shutdown = &StaticShutdown;
        provider.Start = &StaticStart;
        provider.Stop = &StaticStop;

        return cStyleInterface->RegisterLifecycleProvider("UnityXRMock", subsystemId, &provider);
    }

private:
    IUnityXRRaycastInterface* m_UnityInterface;

    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticInitialize(UnitySubsystemHandle handle, void* userData)
    {
        LifecycleProviderRaycast* thiz = static_cast<LifecycleProviderRaycast*>(userData);
        if (thiz == nullptr)
            return kUnitySubsystemErrorCodeInvalidArguments;

        RaycastProvider::Construct();
        return RaycastProvider::GetInstance()->RegisterAsCProvider(handle, thiz->m_UnityInterface);
    }

    static void UNITY_INTERFACE_API StaticShutdown(UnitySubsystemHandle handle, void* userData)
    {
        LifecycleProviderRaycast* thiz = static_cast<LifecycleProviderRaycast*>(userData);
        if (thiz == nullptr)
            return;

        thiz->Shutdown(nullptr);
    }

    static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticStart(UnitySubsystemHandle handle, void* userData)
    {
        return kUnitySubsystemErrorCodeSuccess;
    }

    static void UNITY_INTERFACE_API StaticStop(UnitySubsystemHandle handle, void* userData)
    {

    }

    UnitySubsystemErrorCode UNITY_INTERFACE_API Initialize(IUnitySubsystem* inst) final
    {
        RaycastProvider::Construct();

        // Register with Unity
        auto* camera = static_cast<IUnityXRRaycastSubsystem*>(inst);
        camera->RegisterRaycastProvider(RaycastProvider::GetInstance());


        return kUnitySubsystemErrorCodeSuccess;
    }

    UnitySubsystemErrorCode UNITY_INTERFACE_API Start(IUnitySubsystem* inst) final
    {
        return kUnitySubsystemErrorCodeSuccess;
    }

    void UNITY_INTERFACE_API Stop(IUnitySubsystem* inst) final
    {
    }

    void UNITY_INTERFACE_API Shutdown(IUnitySubsystem* inst) final
    {
        RaycastProvider::Destroy();
    }
};

static LifecycleProviderCamera s_LifecycleProviderCamera;
static LifecycleProviderPlane s_LifecycleProviderPlane;
static LifecycleProviderInput_V1 s_LifecycleProviderInput_V1;
static LifecycleProviderInput_V2 s_LifecycleProviderInput_V2;
static LifecycleProviderSession s_LifecycleProviderSession;
static LifecycleProviderReferencePoint s_LifecycleProviderReferencePoint;
static LifecycleProviderDepth s_LifecycleProviderDepth;
static LifecycleProviderRaycast s_LifecycleProviderRaycast;

template<typename T_LifecycleProvider, typename T_DeprecatedInterface>
UnitySubsystemErrorCode RegisterLifecycleProviderOnDeprecatedInterface(IUnityInterfaces* unityInterfaces, T_LifecycleProvider& lifecycleProvider, const char* subsystemId)
{
    T_DeprecatedInterface* deprecatedInterface = unityInterfaces->Get<T_DeprecatedInterface>();
    if (deprecatedInterface == nullptr)
        return kUnitySubsystemErrorCodeFailure;

    return deprecatedInterface->RegisterLifecycleProvider("UnityXRMock", subsystemId, &lifecycleProvider) ? kUnitySubsystemErrorCodeSuccess : kUnitySubsystemErrorCodeFailure;
}

template<typename T_LifecycleProvider, typename T_UpdatedInterface, typename T_DeprecatedInterface>
UnitySubsystemErrorCode RegisterLifecycleProvider(IUnityInterfaces* unityInterfaces, T_LifecycleProvider& lifecycleProvider, const char* subsystemId)
{
    T_UpdatedInterface* updatedInterface = unityInterfaces->Get<T_UpdatedInterface>();
    if (updatedInterface == nullptr)
        return RegisterLifecycleProviderOnDeprecatedInterface<T_LifecycleProvider, T_DeprecatedInterface>(unityInterfaces, lifecycleProvider, subsystemId);

    return lifecycleProvider.SetUnityInterfaceAndRegister(updatedInterface, subsystemId);
}

#define REGISTER_LIFECYCLE_PROVIDER(SubsystemName) \
do \
{ \
    UnitySubsystemErrorCode registerErrorCode = RegisterLifecycleProvider \
        <LifecycleProvider##SubsystemName, IUnityXR##SubsystemName##Interface, IUnityXR##SubsystemName##Interface_Deprecated> \
        (unityInterfaces, s_LifecycleProvider##SubsystemName, "UnityXRMock-"#SubsystemName); \
} \
while (false)

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
UnityPluginLoad(IUnityInterfaces* unityInterfaces)
{
    REGISTER_LIFECYCLE_PROVIDER(Camera);
    REGISTER_LIFECYCLE_PROVIDER(Plane);
    REGISTER_LIFECYCLE_PROVIDER(ReferencePoint);
    REGISTER_LIFECYCLE_PROVIDER(Depth);
    REGISTER_LIFECYCLE_PROVIDER(Raycast);
    REGISTER_LIFECYCLE_PROVIDER(Session);

    if (auto* xrInputInterface = unityInterfaces->Get<IUnityXRInputInterface>())
    {
        inputVersion = 0;
        UnityLifecycleProvider lifecycleProvider;
        lifecycleProvider.userData = xrInputInterface;
        lifecycleProvider.Initialize = &InputProvider::Initialize;
        lifecycleProvider.Start = &InputProvider::Start;
        lifecycleProvider.Stop = &InputProvider::Stop;
        lifecycleProvider.Shutdown = &InputProvider::Shutdown;

        xrInputInterface->RegisterLifecycleProvider("UnityXRMock", "UnityXRMock-Input", &lifecycleProvider);
    }
    else if (auto* xrInputInterface = unityInterfaces->Get<UnityXRInput_V1::IUnityXRInputInterface>())
    {
        inputVersion = 1;
        xrInputInterface->RegisterLifecycleProvider("UnityXRMock", "UnityXRMock-Input", &s_LifecycleProviderInput_V1);
    }
    else if (auto* xrInputInterface = unityInterfaces->Get<UnityXRInput_V2::IUnityXRInputInterface>())
    {
        inputVersion = 2;
        xrInputInterface->RegisterLifecycleProvider("UnityXRMock", "UnityXRMock-Input", &s_LifecycleProviderInput_V2);
    }

}
