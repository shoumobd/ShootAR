#pragma once
#include "XRProvider.h"
#include "IUnityXRInput.h"
#include "IUnityXRInputCommon.h"
#include "UnityMath.h"

class InputProvider : public Singleton<InputProvider>
{
public:

	InputProvider(IUnityXRInputInterface* xrInputInterfacePtr);

	void SetPose(const UnityXRPose& pose, const UnityXRMatrix4x4& transform);
	const UnityXRPose& GetPose() const { return m_LastPose; } 
	const UnityXRMatrix4x4& GetTransform() const { return m_Transform; }
    static bool TryGetTransform(UnityXRMatrix4x4* transformOut);
	IUnityXRInputInterface* InputInterface() { return m_InputInterface;	}
	UnitySubsystemHandle GetSubsystemHandle() const { return m_SubsystemHandle; }

public:

	static UnitySubsystemErrorCode UNITY_INTERFACE_API Initialize(UnitySubsystemHandle handle, void* xrInputInterfacePtr);
	static UnitySubsystemErrorCode UNITY_INTERFACE_API Start(UnitySubsystemHandle handle, void* xrInputInterfacePtr);
	static void UNITY_INTERFACE_API Stop(UnitySubsystemHandle handle, void* xrInputInterfacePtr);
	static void UNITY_INTERFACE_API Shutdown(UnitySubsystemHandle handle, void* xrInputInterfacePtr);

	static void UNITY_INTERFACE_API OnNewInputFrame(UnitySubsystemHandle handle, void* userData) {}
	static void UNITY_INTERFACE_API FillDeviceDefinition(UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId, UnityXRInputDeviceDefinitionHandle definitionHandle);
	static UnitySubsystemErrorCode UNITY_INTERFACE_API UpdateDeviceState(UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId, UnityXRInputUpdateType updateType, UnityXRInputDeviceStateHandle stateHandle);
	static UnitySubsystemErrorCode UNITY_INTERFACE_API HandleEvent(UnitySubsystemHandle handle, void* userData, UnityXRInputEventType eventType, UnityXRInternalInputDeviceId deviceId, void* buffer, unsigned int size);

private:

	UnitySubsystemHandle m_SubsystemHandle;

	IUnityXRInputInterface * const m_InputInterface;

	unsigned int m_PositionFeatureIndex = static_cast<unsigned int>(-1);

	unsigned int m_RotationFeatureIndex = static_cast<unsigned int>(-1);

	UnityXRPose m_LastPose = kIdentityPose;

	UnityXRMatrix4x4 m_Transform;
};
