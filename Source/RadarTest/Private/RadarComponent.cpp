#include "RadarComponent.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"

URadarComponent::URadarComponent()
{

}

bool URadarComponent::DistanceAndPower(float& Distance, float& Power)
{
  TextureTarget = NewObject<UTextureRenderTarget2D>(this);
  TextureTarget->InitAutoFormat(2, 16);

  CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
  CaptureScene();

  FReadSurfaceDataFlags ReadPixelFlags(RCM_UNorm);
  FTextureRenderTargetResource* RTResource
    = TextureTarget->GameThread_GetRenderTargetResource();

  TArray<FLinearColor> Colors;
  const bool Result = RTResource->ReadLinearColorPixels(Colors);

  if (!Result || Colors.Num() == 0) {
    return false;
  } else {
    Distance = ExtractDistance(Colors);
    Power = ExtractPower(Colors);
	return true;
  }
  return 0.0f;
}

float URadarComponent::ExtractDistance(const TArray<FLinearColor>& Colors) {
  float TotalDistance = 0.f;
  for (auto& Color : Colors)
    TotalDistance += Color.R;
  return TotalDistance / Colors.Num();
}

float URadarComponent::ExtractPower(const TArray<FLinearColor>& Colors) {
  float TotalDistance = 0.f;
  for (auto& Color : Colors)
    TotalDistance = Color.B;
  return TotalDistance / Colors.Num();
}

