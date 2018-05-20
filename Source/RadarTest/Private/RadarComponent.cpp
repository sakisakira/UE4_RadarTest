#include "RadarComponent.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"

URadarComponent::URadarComponent()
{

}

bool URadarComponent::DistanceAndPower(float& Distance, float& Power)
{
  TextureTarget = NewObject<UTextureRenderTarget2D>(this);
  TextureTarget->InitAutoFormat(16, 16);

  CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
//  CaptureSource = ESceneCaptureSource::SCS_SceneDepth;
  CaptureScene();

  FReadSurfaceDataFlags ReadPixelFlags(RCM_UNorm);
  FTextureRenderTargetResource* RTResource
    = TextureTarget->GameThread_GetRenderTargetResource();

  TArray<FColor> Colors;
  const bool Result = RTResource->ReadPixels(Colors);

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

