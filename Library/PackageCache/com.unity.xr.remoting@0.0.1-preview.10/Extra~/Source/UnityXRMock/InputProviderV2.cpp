#include <cstring>
#include "InputProviderV2.h"

InputProviderV2::InputProviderV2()
	: m_Transform(Identity())
{ }

void InputProviderV2::SetPose(const UnityXRPose& pose, const UnityXRMatrix4x4& transform)
{
	m_LastPose = pose;
	m_Transform = transform;
}

void UNITY_INTERFACE_API InputProviderV2::FillDeviceDefinition(UnityXRInput_V2::UnityXRInternalInputDeviceId deviceId,
	UnityXRInput_V2::IUnityXRInputDeviceDefinition* const deviceDefinition)
{
	deviceDefinition->SetName("XRMockDevice");
	deviceDefinition->SetRole(UnityXRInput_V2::kUnityXRInputDeviceRoleGeneric);
	m_PositionFeatureIndex = deviceDefinition->AddFeatureWithUsage("Device - Position", UnityXRInput_V2::kUnityXRInputFeatureTypeAxis3D, UnityXRInput_V2::kUnityXRInputFeatureUsageColorCameraPosition);
	m_RotationFeatureIndex = deviceDefinition->AddFeatureWithUsage("Device - Rotation", UnityXRInput_V2::kUnityXRInputFeatureTypeRotation, UnityXRInput_V2::kUnityXRInputFeatureUsageColorCameraRotation);
}

bool UNITY_INTERFACE_API InputProviderV2::UpdateDeviceState(UnityXRInput_V2::UnityXRInternalInputDeviceId deviceId, UnityXRInput_V2::IUnityXRInputDeviceState* const deviceState)
{
	deviceState->SetAxis3DValue(m_PositionFeatureIndex, m_LastPose.position);
	deviceState->SetRotationValue(m_RotationFeatureIndex, m_LastPose.rotation);
	return false;
}
