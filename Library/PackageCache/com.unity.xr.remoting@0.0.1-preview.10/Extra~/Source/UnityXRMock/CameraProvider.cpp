#include <cstdint>
#include <cmath>
#include <cstring>

#include "IUnityRenderingExtensions.h"
#include "CameraProvider.h"
#include "UnityMath.h"
#include "Flags.h"
#include "InputProvider.h"

typedef int(UNITY_INTERFACE_API *UnitySetLightEstimationCallback)(bool);
static UnitySetLightEstimationCallback gSetLightEstimationCallback;

extern "C"
{
    struct CameraFrameData
    {
        int eventType;
        int eventSubType;

        int64_t timeStampNs;
        float averageBrightness;
        float averageTemperature;

        UnityXRMatrix4x4 projectionMatrix;
        UnityXRMatrix4x4 displayMatrix;

        char cameraFramePropertyFlags;
    };

    UNITY_INTERFACE_EXPORT void UnityXRMock_setLightEstimation(UnitySetLightEstimationCallback unityFunctionPointer)
    {
        gSetLightEstimationCallback = unityFunctionPointer;
    }

    // Obselete TODO: Use individual methods
    UNITY_INTERFACE_EXPORT void UnityXRMock_setCameraFrameData(unsigned char* data)
    {
        const CameraFrameData* cameraFrameData = reinterpret_cast<CameraFrameData*>(data);

        UnityXRCameraFrame frameData;
        frameData.averageBrightness = cameraFrameData->averageBrightness;
        frameData.averageColorTemperature = cameraFrameData->averageTemperature;
        frameData.displayMatrix = cameraFrameData->displayMatrix;
        frameData.projectionMatrix = cameraFrameData->projectionMatrix;
        frameData.providedFields = (UnityXRCameraFramePropertyFlags)cameraFrameData->cameraFramePropertyFlags;
        frameData.timestampNs = cameraFrameData->timeStampNs;

        if (CameraProvider::GetInstance() != NULL)
        {
            CameraProvider::GetInstance()->UpdateFrameData(frameData);
        }
    }

    void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityXRMock_setDisplayMatrix(
        UnityXRMatrix4x4 displayMatrix, bool hasValue)
    {
        if (CameraProvider::GetInstance())
            CameraProvider::GetInstance()->SetDisplayMatrix(displayMatrix, hasValue);
    }

    void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityXRMock_setProjectionMatrix(
        UnityXRMatrix4x4 projectionMatrix, UnityXRMatrix4x4 inverseProjectionMatrix, bool hasValue)
    {
        if (CameraProvider::GetInstance())
            CameraProvider::GetInstance()->SetProjectionMatrix(projectionMatrix, inverseProjectionMatrix, hasValue);
    }

    void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityXRMock_setAverageBrightness(float averageBrightness, bool hasValue)
    {
        if (CameraProvider::GetInstance())
            CameraProvider::GetInstance()->SetAverageBrightness(averageBrightness, hasValue);
    }

    void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityXRMock_setAverageColorTemperature(float colorTemperature, bool hasValue)
    {
        if (CameraProvider::GetInstance())
            CameraProvider::GetInstance()->SetAverageColorTemperature(colorTemperature, hasValue);
    }

    static uint8_t* imageData;
    UNITY_INTERFACE_EXPORT void SetTextureUpdateData(unsigned char* data, int size)
    {
        if (imageData == nullptr)
        {
            imageData = new unsigned char[size];
        }

        for (int i = 0; i < size; i++)
        {
            imageData[i] = data[i];
        }
    }

    void TextureUpdateCallback(int eventID, void* data)
    {
        auto event = static_cast<UnityRenderingExtEventType>(eventID);

        if (event == kUnityRenderingExtEventUpdateTextureBegin)
        {
            auto params = reinterpret_cast<UnityRenderingExtTextureUpdateParams*>(data);

            params->texData = imageData;
        }
        else if (event == kUnityRenderingExtEventUpdateTextureEnd)
        {
            // UpdateTextureEnd: Free up the temporary memory.
            //auto params = reinterpret_cast<UnityRenderingExtTextureUpdateParams*>(data);
            //delete[] reinterpret_cast<uint32_t*>(params->texData);
        }
    }

    UnityRenderingEventAndData UNITY_INTERFACE_EXPORT GetTextureUpdateCallback()
    {
        return TextureUpdateCallback;
    }
}

struct LightEstimationPayload
{
    bool enabled = true;
};

void CameraProvider::SetProjectionMatrix(
    const UnityXRMatrix4x4& projectionMatrix, const UnityXRMatrix4x4& inverseProjectionMatrix, bool hasValue)
{
    m_LatestFrameData.projectionMatrix = projectionMatrix;
    m_LatestFrameData.providedFields = SetFlag(m_LatestFrameData.providedFields, kUnityXRCameraFramePropertiesProjectionMatrix, hasValue);

    m_InverseProjectionMatrix = inverseProjectionMatrix;
    m_HasInverseProjectionMatrix = true;
}

void CameraProvider::SetDisplayMatrix(
    const UnityXRMatrix4x4& displayMatrix,
    bool hasValue)
{
    m_LatestFrameData.displayMatrix = displayMatrix;
    m_LatestFrameData.providedFields = SetFlag(m_LatestFrameData.providedFields, kUnityXRCameraFramePropertiesDisplayMatrix, hasValue);
}

void CameraProvider::SetAverageBrightness(float averageBrightness, bool hasValue)
{
    m_LatestFrameData.averageBrightness = averageBrightness;
    m_LatestFrameData.providedFields = SetFlag(m_LatestFrameData.providedFields, kUnityXRCameraFramePropertiesAverageBrightness, hasValue);
}

void CameraProvider::SetAverageColorTemperature(float averageColorTemperature, bool hasValue)
{
    m_LatestFrameData.averageColorTemperature = averageColorTemperature;
    m_LatestFrameData.providedFields = SetFlag(m_LatestFrameData.providedFields, kUnityXRCameraFramePropertiesAverageColorTemperature, hasValue);
}

void CameraProvider::UpdateFrameData(UnityXRCameraFrame frame)
{
    m_LatestFrameData = frame;
}

void UNITY_INTERFACE_API CameraProvider::SetLightEstimationRequested(bool enable)
{
    if (gSetLightEstimationCallback != nullptr)
        gSetLightEstimationCallback(enable);

    m_LightEstimationRequested = enable;
}

bool UNITY_INTERFACE_API CameraProvider::GetFrame(const UnityXRCameraParams& paramsIn, UnityXRCameraFrame* frameOut)
{
    m_CameraParams = paramsIn;
    m_HasCameraParameters = true;
    *frameOut = m_LatestFrameData;

	auto* descriptors = &frameOut->textureDescriptors[0];
	descriptors->format = UnityRenderingExtTextureFormat::kUnityRenderingExtFormatR8G8B8A8_UNorm;
	descriptors->width = 4;
	descriptors->height = 4;
	descriptors->nativeId = 0;
	strncpy(descriptors->name, "TEMP", kUnityXRStringSize);

	frameOut->numTextures = 0;

    // Don't provide light estimation if not requested
    if (!m_LightEstimationRequested)
    {
        frameOut->providedFields = RemoveFlag(frameOut->providedFields, kUnityXRCameraFramePropertiesAverageBrightness);
        frameOut->providedFields = RemoveFlag(frameOut->providedFields, kUnityXRCameraFramePropertiesAverageColorTemperature);
    }

    return true;
}

bool UNITY_INTERFACE_API CameraProvider::GetShaderName(char(&shaderName)[kUnityXRStringSize])
{
	/*
	if (CameraProvider* provider = CameraProvider::GetInstance())
	{
#if PLATFORM_WINDOWS
		strncpy_s(shaderName, kUnityXRStringSize, "Unlit/Texture", kUnityXRStringSize);
#else
		strncpy(shaderName, "Unlit/Texture", kUnityXRStringSize);
#endif
		return true;
	}*/

    return false;
}

UnitySubsystemErrorCode CameraProvider::RegisterAsCProvider(UnitySubsystemHandle handle, IUnityXRCameraInterface* cameraInterface)
{
    m_CInterface = cameraInterface;
    UnityXRCameraProvider provider = {};
    provider.userData = this;
    provider.GetFrame = &StaticGetFrame;
    provider.GetShaderName = &StaticGetShaderName;
    provider.SetLightEstimationRequested = &StaticSetLightEstimationRequested;
    return cameraInterface->RegisterCameraProvider(handle, &provider);
}

UnitySubsystemErrorCode UNITY_INTERFACE_API CameraProvider::StaticGetFrame(UnitySubsystemHandle handle, void* userData, const UnityXRCameraParams * paramsIn, UnityXRCameraFrame * frameOut)
{
    if (CameraProvider* provider = CameraProvider::GetInstance())
    {
        return provider->GetFrame(*paramsIn, frameOut) ? kUnitySubsystemErrorCodeSuccess : kUnitySubsystemErrorCodeFailure;
    }

    return kUnitySubsystemErrorCodeFailure;
}

void UNITY_INTERFACE_API CameraProvider::StaticSetLightEstimationRequested(UnitySubsystemHandle handle, void* userData, bool requested)
{
    if (CameraProvider* provider = CameraProvider::GetInstance())
    {
        provider->SetLightEstimationRequested(requested);
    }
}

UnitySubsystemErrorCode UNITY_INTERFACE_API CameraProvider::StaticGetShaderName(UnitySubsystemHandle handle, void* userData, char shaderName[kUnityXRStringSize])
{
	/*
    if (CameraProvider* provider = CameraProvider::GetInstance())
    {
#if PLATFORM_WINDOWS
        strncpy_s(shaderName, kUnityXRStringSize, "Unlit/Texture", kUnityXRStringSize);
#else
        strncpy(shaderName, "Unlit/Texture", kUnityXRStringSize);
#endif
        return kUnitySubsystemErrorCodeSuccess;
    }
	*/
    return kUnitySubsystemErrorCodeFailure;
}

bool CameraProvider::TryGetRay(float screenX, float screenY, Ray* rayOut) const
{
    if (!m_HasInverseProjectionMatrix || !m_HasCameraParameters)
        return false;

    UnityXRMatrix4x4 transform;
    if (!InputProvider::TryGetTransform(&transform))
        return false;

    const UnityXRVector3 screenPoint = {1.f - screenX * 2.f, 1.f - screenY * 2.f, .95f};
    UnityXRVector3 pointOnPlane = {};
    const UnityXRVector3 cameraPosition =
    {
        transform.columns[3].x,
        transform.columns[3].y,
        transform.columns[3].z
    };

    auto clipToWorld = Mul(transform, m_InverseProjectionMatrix);
    if (!Mul(clipToWorld, screenPoint, &pointOnPlane))
        return false;

    auto rayDirection = Sub(pointOnPlane, cameraPosition);

    auto cameraForward = UnityXRVector3
    {
        transform.columns[2].x,
        transform.columns[2].y,
        transform.columns[2].z
    };

    float distToPlane = Dot(rayDirection, cameraForward);
    if (std::abs(distToPlane) < 1.0e-6f)
        return false;

    const bool isPerspective =
        clipToWorld.columns[0].w != 0.f ||
        clipToWorld.columns[1].w != 0.f ||
        clipToWorld.columns[2].w != 0.f ||
        clipToWorld.columns[3].w != 1.f;

    if (isPerspective)
    {
        rayDirection = Mul(rayDirection, m_CameraParams.zNear / distToPlane);
        rayOut->direction = Normalize(rayDirection);
        rayOut->origin = Add(cameraPosition, rayDirection);
    }
    else
    {
        rayOut->direction = cameraForward;
        rayOut->origin = Sub(pointOnPlane, Mul(cameraForward, (distToPlane - m_CameraParams.zNear)));
    }

    return true;
}
