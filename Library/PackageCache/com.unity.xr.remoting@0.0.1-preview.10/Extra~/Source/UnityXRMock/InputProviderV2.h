#pragma once
#include "XRProvider.h"
#include "IUnityXRInput_V2.h"
#include "IUnityXRInputCommon.h"
#include "UnityMath.h"

class InputProviderV2 : public XRProvider<InputProviderV2, UnityXRInput_V2::IUnityXRInputProvider>
{
public:
	InputProviderV2();

	void SetPose(const UnityXRPose& pose, const UnityXRMatrix4x4& transform);
	const UnityXRPose& GetPose() const { return m_LastPose; } 
	const UnityXRMatrix4x4& GetTransform() const { return m_Transform; }

	UnityXRInput_V2::IUnityXRInputSubsystem* Subsystem;

private:
    /// Fill in your connected device information here when requested.  Used to create customized device states.
    ///
    /// @param deviceId The Id of the device that needs a definition filled
    /// @param deviceDefinition The definition to be filled by the plugin
    void UNITY_INTERFACE_API FillDeviceDefinition(UnityXRInput_V2::UnityXRInternalInputDeviceId deviceId, UnityXRInput_V2::IUnityXRInputDeviceDefinition* const deviceDefinition) final;
    
    /// Called by Unity when it needs a current device snapshot
    ///
    /// @param deviceId The Id of the device whose state is requested.
    /// @param deviceState The customized DeviceState to fill in.  The indices within this state match the order those input features were added from FillDeviceDefinition
    bool UNITY_INTERFACE_API UpdateDeviceState(UnityXRInput_V2::UnityXRInternalInputDeviceId deviceId, UnityXRInput_V2::IUnityXRInputDeviceState* const deviceState) final;
    
    /// Simple, generic method callback to inform the plugin or individual devices of events occurring within unity
    ///
    /// @param eventType the type of event being sent
    /// @param deviceId the Id of the device that should handle the event.  Will be kUnityInvalidXRInputFeatureIndex if event is for all devices.
    virtual bool UNITY_INTERFACE_API HandleEvent(UnityXRInput_V2::UnityXRInputEventType eventType, UnityXRInput_V2::UnityXRInternalInputDeviceId deviceId, void* buffer, unsigned int size)
    {
        return false;
    }
    
    unsigned int m_PositionFeatureIndex = static_cast<unsigned int>(-1);

    unsigned int m_RotationFeatureIndex = static_cast<unsigned int>(-1);

	UnityXRPose m_LastPose = kIdentityPose;
    
    UnityXRMatrix4x4 m_Transform;
};
