#include <cstring>
#include "InputProvider.h"
#include "InputProviderV1.h"
#include "InputProviderV2.h"

extern "C"
{
	UNITY_INTERFACE_EXPORT void UnityXRMock_connectDevice(int id)
	{
		if (InputProviderV1::GetInstance() != nullptr)
		{
			InputProviderV1::GetInstance()->Subsystem->DeviceConnected(id);
		}
		else if (InputProviderV2::GetInstance() != nullptr)
		{
			auto* input = InputProviderV2::GetInstance();
			auto* subsystem = input->Subsystem;
			subsystem->DeviceConnected(id);
		}
		else
		{
			if (InputProvider* inputProvider = InputProvider::GetInstance())
			{
				inputProvider->InputInterface()->InputSubsystem_DeviceConnected(inputProvider->GetSubsystemHandle(), 0);
			}
		}
	}

	UNITY_INTERFACE_EXPORT void UnityXRMock_disconnectDevice(int id)
	{
		if (InputProviderV1::GetInstance() != nullptr)
		{
			InputProviderV1::GetInstance()->Subsystem->DeviceDisconnected(id);
		}
		else if (InputProviderV2::GetInstance() != nullptr)
		{
			InputProviderV2::GetInstance()->Subsystem->DeviceDisconnected(id);
		}
		else if (auto inputProvider = InputProvider::GetInstance())
		{
			inputProvider->InputInterface()->InputSubsystem_DeviceDisconnected(inputProvider->GetSubsystemHandle(), 0);
		}
	}

	UNITY_INTERFACE_EXPORT void UnityXRMock_setPose(UnityXRPose pose, UnityXRMatrix4x4 transform)
	{
		if (InputProviderV1::GetInstance() != nullptr)
		{
			InputProviderV1::GetInstance()->SetPose(pose, transform);
		}
		else if (InputProviderV2::GetInstance() != nullptr)
		{
			InputProviderV2::GetInstance()->SetPose(pose, transform);
		}
		else if (auto inputProvider = InputProvider::GetInstance())
		{
			inputProvider->SetPose(pose, transform);
		}
	}
}

InputProvider::InputProvider(IUnityXRInputInterface* xrInputInterfacePtr)
	: m_InputInterface(xrInputInterfacePtr)
	, m_Transform(Identity())
{ }

void InputProvider::SetPose(const UnityXRPose& pose, const UnityXRMatrix4x4& transform)
{
	m_LastPose = pose;
	m_Transform = transform;
}

bool InputProvider::TryGetTransform(UnityXRMatrix4x4* transformOut)
{
    if (auto inputProvider = InputProviderV1::GetInstance())
    {
        *transformOut = inputProvider->GetTransform();
        return true;
    }
    else if (auto inputProvider = InputProviderV2::GetInstance())
    {
        *transformOut = inputProvider->GetTransform();
        return true;
    }
    else if (auto inputProvider = GetInstance())
    {
        *transformOut = inputProvider->GetTransform();
        return true;
    }
    else
    {
        return false;
    }
}

UnitySubsystemErrorCode UNITY_INTERFACE_API InputProvider::Initialize(UnitySubsystemHandle handle, void* xrInputInterfacePtr)
{
	UnityXRInputProvider inputProvider;
	inputProvider.userData = NULL;
	inputProvider.OnNewInputFrame = &InputProvider::OnNewInputFrame;
	inputProvider.FillDeviceDefinition = &InputProvider::FillDeviceDefinition;
	inputProvider.UpdateDeviceState = &InputProvider::UpdateDeviceState;
	inputProvider.HandleEvent = &InputProvider::HandleEvent;

	IUnityXRInputInterface* xrInputInterface = static_cast<IUnityXRInputInterface*>(xrInputInterfacePtr);

	InputProvider::Construct(xrInputInterface);
	xrInputInterface->RegisterInputProvider(handle, &inputProvider);

	InputProvider::GetInstance()->m_SubsystemHandle = handle;

	return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode UNITY_INTERFACE_API InputProvider::Start(UnitySubsystemHandle handle, void* xrInputInterfacePtr)
{
	IUnityXRInputInterface* xrInputInterface = static_cast<IUnityXRInputInterface*>(xrInputInterfacePtr);
	xrInputInterface->InputSubsystem_DeviceConnected(handle, 0);

	return kUnitySubsystemErrorCodeSuccess;
}

void UNITY_INTERFACE_API InputProvider::Stop(UnitySubsystemHandle handle, void* xrInputInterfacePtr)
{
	IUnityXRInputInterface* xrInputInterface = static_cast<IUnityXRInputInterface*>(xrInputInterfacePtr);
	xrInputInterface->InputSubsystem_DeviceDisconnected(handle, 0);
}

void UNITY_INTERFACE_API InputProvider::Shutdown(UnitySubsystemHandle handle, void* xrInputInterfacePtr)
{
	InputProvider::Destroy();
}

void UNITY_INTERFACE_API InputProvider::FillDeviceDefinition(UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId, UnityXRInputDeviceDefinitionHandle definitionHandle)
{
	if (InputProvider* inputProvider = InputProvider::GetInstance())
	{

		IUnityXRInputInterface* inputInterface = inputProvider->m_InputInterface;

		inputInterface->DeviceDefinition_SetName(definitionHandle, "XRMockDevice");
		inputInterface->DeviceDefinition_SetRole(definitionHandle, kUnityXRInputDeviceRoleGeneric);

		inputProvider->m_PositionFeatureIndex = inputInterface->DeviceDefinition_AddFeatureWithUsage(definitionHandle, "Device - Position", kUnityXRInputFeatureTypeAxis3D, kUnityXRInputFeatureUsageColorCameraPosition);
		inputProvider->m_RotationFeatureIndex = inputInterface->DeviceDefinition_AddFeatureWithUsage(definitionHandle, "Device - Rotation", kUnityXRInputFeatureTypeRotation, kUnityXRInputFeatureUsageColorCameraRotation);
	}
}

UnitySubsystemErrorCode UNITY_INTERFACE_API InputProvider::UpdateDeviceState(UnitySubsystemHandle handle, void* userData, UnityXRInternalInputDeviceId deviceId, UnityXRInputUpdateType updateType, UnityXRInputDeviceStateHandle stateHandle)
{
	if (InputProvider* inputProvider = InputProvider::GetInstance())
	{
		IUnityXRInputInterface* inputInterface = inputProvider->m_InputInterface;
		inputInterface->DeviceState_SetAxis3DValue(stateHandle, inputProvider->m_PositionFeatureIndex, inputProvider->GetPose().position);
		inputInterface->DeviceState_SetRotationValue(stateHandle, inputProvider->m_RotationFeatureIndex, inputProvider->GetPose().rotation);
		return kUnitySubsystemErrorCodeSuccess;
	}
	return kUnitySubsystemErrorCodeFailure;
}

UnitySubsystemErrorCode UNITY_INTERFACE_API InputProvider::HandleEvent(UnitySubsystemHandle handle, void* userData, UnityXRInputEventType eventType, UnityXRInternalInputDeviceId deviceId, void* buffer, unsigned int size)
{
	return kUnitySubsystemErrorCodeFailure;
}
