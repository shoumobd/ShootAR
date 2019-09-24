#pragma once
#include "XRProvider.h"
#include "IUnityXRInput_V1.h"
#include "IUnityXRInputCommon.h"
#include "UnityMath.h"

class InputProviderV1 : public XRProvider<InputProviderV1, UnityXRInput_V1::IUnityXRInputProvider>
{
public:
	InputProviderV1();

	void SetPose(const UnityXRPose& pose, const UnityXRMatrix4x4& transform);
	const UnityXRPose& GetPose() const { return m_LastPose; } 
	const UnityXRMatrix4x4& GetTransform() const { return m_Transform; }

	UnityXRInput_V1::IUnityXRInputSubsystem* Subsystem;

private:
	/// Fill in your connected device information here when requested.  Used to create customized device states.
	///
	/// @param deviceId The Id of the device that needs a definition filled
	/// @param deviceDefinition The definition to be filled by the plugin
	virtual void UNITY_INTERFACE_API FillDeviceDefinition(UnityXRInput_V1::UnityXRInternalInputDeviceId deviceId,
		UnityXRInput_V1::IUnityXRInputDeviceDefinition* const deviceDefinition) override;
	/// Called by Unity when it needs a current device snapshot
	///
	/// @param deviceId The Id of the device whose state is requested.
	/// @param deviceState The customized DeviceState to fill in.  The indices within this state match the order those input features were added from FillDeviceDefinition
	virtual bool UNITY_INTERFACE_API UpdateDeviceState(UnityXRInput_V1::UnityXRInternalInputDeviceId deviceId, 
		UnityXRInput_V1::IUnityXRInputDeviceState* const deviceState) override;

	unsigned int m_PoseFeatureIndex = static_cast<unsigned int>(-1);

	UnityXRPose m_LastPose = kIdentityPose;

	UnityXRMatrix4x4 m_Transform;
};
