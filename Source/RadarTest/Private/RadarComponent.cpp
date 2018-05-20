#include "RadarComponent.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"

URadarComponent::URadarComponent(void) : Super()
{
	bCaptureEveryFrame = true;
}

bool URadarComponent::DistanceAndPower(float& Distance, float& Power)
{
	if (Resolution.X < 10) Resolution.X = 10;
	if (Resolution.Y < 10) Resolution.Y = 10;

  TextureTarget = NewObject<UTextureRenderTarget2D>(this);
  TextureTarget->InitAutoFormat(Resolution.X, Resolution.Y);

  auto RadarPPMaterial = LoadObject<UMaterial>(this, TEXT("Material'/Game/Radar_Mat.Radar_Mat'"));
  AddOrUpdateBlendable(RadarPPMaterial);

  CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
  CaptureScene();

  FReadSurfaceDataFlags ReadPixelFlags(RCM_MinMax);
  FTextureRenderTargetResource* RTResource
    = TextureTarget->GameThread_GetRenderTargetResource();

  TArray<FLinearColor> Colors;
  const bool Result 
	  = RTResource->ReadLinearColorPixels(Colors, ReadPixelFlags);
  TArray<FFloat16Color> depths;

  if (!Result || Colors.Num() == 0) {
    return false;
  } else {
    Distance = ExtractDistance(Colors);
    Power = ExtractPower(Colors);
	return true;
  }
}

float URadarComponent::ExtractDistance(const TArray<FLinearColor>& Colors) const {
  float TotalDistance = 0.f;
  for (auto& Color : Colors)
    TotalDistance += Color.R;
  return TotalDistance / Colors.Num();
}

float URadarComponent::ExtractPower(const TArray<FLinearColor>& Colors) const {
  float TotalPower = 0.f;
  for (auto& Color : Colors)
    TotalPower += Color.B;
  return TotalPower / Colors.Num();
}

TArray<FVector> URadarComponent::ExtraceLocations(const TArray<FLinearColor>& Colors) const {
  //////////////////// working
}

TArray<float> URadarComponent::ExtractPowers(const TArray<FLinearColor>& Colors) const {
  TArray<float> Powers;
  Powers.Reserve(Colors.Num());
  for (auto& Color : Colors) {
    Powers.Add(Color.B);
  }
  return Powers;
}
