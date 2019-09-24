#include <cstring>
#include "InputProviderV1.h"

void InputProviderV1::SetPose(const UnityXRPose& pose, const UnityXRMatrix4x4& transform)
{
	m_LastPose = pose;
	m_Transform = transform;
}

InputProviderV1::InputProviderV1()
	: m_Transform(Identity())
{ }

void UNITY_INTERFACE_API InputProviderV1::FillDeviceDefinition(UnityXRInput_V1::UnityXRInternalInputDeviceId deviceId, 
	UnityXRInput_V1::IUnityXRInputDeviceDefinition* const deviceDefinition)
{
	deviceDefinition->SetName("XRMockDevice");
	deviceDefinition->SetRole(UnityXRInput_V1::kUnityXRInputDeviceRoleGeneric);
	m_PoseFeatureIndex = deviceDefinition->AddFeature("Pose", UnityXRInput_V1::kUnityXRInputFeatureUsageColorCameraPose, UnityXRInput_V1::kUnityXRInputFeatureTypePose);
}

bool UNITY_INTERFACE_API InputProviderV1::UpdateDeviceState(UnityXRInput_V1::UnityXRInternalInputDeviceId deviceId, 
	UnityXRInput_V1::IUnityXRInputDeviceState* const deviceState)
{
	deviceState->SetPoseValue(m_PoseFeatureIndex, m_LastPose);
	return false;
}
