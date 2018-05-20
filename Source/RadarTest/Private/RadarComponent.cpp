#include "RadarComponent.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"

URadarComponent::URadarComponent(void) : Super()
{
	bCaptureEveryFrame = false;
}

bool URadarComponent::DistanceAndPower(float& Distance, float& Power)
{
	if (Resolution.X < 10) Resolution.X = 10;
	if (Resolution.Y < 10) Resolution.Y = 10;

  TextureTarget = NewObject<UTextureRenderTarget2D>(this);
  TextureTarget->InitAutoFormat(Resolution.X, Resolution.Y);

  TextureTarget->OverrideFormat = EPixelFormat::PF_FloatRGBA;
  TextureTarget->UpdateResourceImmediate(true);

  CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
//  CaptureSource = ESceneCaptureSource::SCS_SceneDepth;
  CaptureScene();

  FReadSurfaceDataFlags ReadPixelFlags(RCM_UNorm);
  FTextureRenderTargetResource* RTResource
    = TextureTarget->GameThread_GetRenderTargetResource();

  TArray<FColor> Colors;
  const bool Result = RTResource->ReadPixels(Colors,
	  ReadPixelFlags);

  if (!Result || Colors.Num() == 0) {
    return false;
  } else {
    Distance = ExtractDistance(Colors);
    Power = ExtractPower(Colors);
	return true;
  }
}

float URadarComponent::ExtractDistance(const TArray<FColor>& Colors) {
  float TotalDistance = 0.f;
  for (auto& Color : Colors)
    TotalDistance += Color.R;
  return TotalDistance / Colors.Num();
}

float URadarComponent::ExtractPower(const TArray<FColor>& Colors) {
  float TotalPower = 0.f;
  for (auto& Color : Colors)
    TotalPower += Color.B;
  return TotalPower / Colors.Num();
}

