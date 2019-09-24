#pragma once
#include "IUnityXRCamera.h"
#include "IUnityXRCamera.deprecated.h"
#include "XRProvider.h"
#include "Ray.h"

class CameraProvider : public XRProvider<CameraProvider, IUnityXRCameraProvider>
{
public:

	UnitySubsystemErrorCode RegisterAsCProvider(UnitySubsystemHandle handle, IUnityXRCameraInterface* cameraInterface);

	void UpdateFrameData(UnityXRCameraFrame frame);

	bool TryGetRay(float screenX, float screenY, Ray* rayOut) const;

	void SetDisplayMatrix(
        const UnityXRMatrix4x4& displayMatrix,
		bool hasValue);

    void SetProjectionMatrix(
        const UnityXRMatrix4x4& projectionMatrix,
        const UnityXRMatrix4x4& inverseProjectionMatrix,
		bool hasValue);

    void SetTransform(
        const UnityXRPose& pose, const UnityXRMatrix4x4& transform);

    void SetAverageBrightness(float averageBrightness, bool hasValue);

    void SetAverageColorTemperature(float averageColorTemperature, bool hasValue);

private:

    bool UNITY_INTERFACE_API GetFrame(const UnityXRCameraParams& paramsIn, UnityXRCameraFrame* frameOut) final;
    bool UNITY_INTERFACE_API GetShaderName(char(&shaderName)[kUnityXRStringSize]) final;
    void UNITY_INTERFACE_API SetLightEstimationRequested(bool enable) final;

	static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticGetFrame(UnitySubsystemHandle handle, void* userData, const UnityXRCameraParams* paramsIn, UnityXRCameraFrame* frameOut);
	static void UNITY_INTERFACE_API StaticSetLightEstimationRequested(UnitySubsystemHandle handle, void* userData, bool requested);
	static UnitySubsystemErrorCode UNITY_INTERFACE_API StaticGetShaderName(UnitySubsystemHandle handle, void* userData, char shaderName[kUnityXRStringSize]);

	UnityXRCameraFrame m_LatestFrameData;

	IUnityXRCameraInterface* m_CInterface = nullptr;

    UnityXRCameraParams m_CameraParams;

    UnityXRMatrix4x4 m_InverseProjectionMatrix;

    bool m_LightEstimationRequested = false;

    bool m_HasCameraParameters = false;

    bool m_HasInverseProjectionMatrix = false;

};
