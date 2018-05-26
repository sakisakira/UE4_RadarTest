#include "RadarComponent.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"

URadarComponent::URadarComponent(void) : Super()
{
	bCaptureEveryFrame = true;
}

bool URadarComponent::LocationsAndPowers(
	TArray<FVector2D>& Locations,	TArray<float>& Powers) {
	if (HCount < 10) HCount = 10;
	if (HFoV < 10.f) HFoV = 10.f;
	if (HFoV > 170.f) HFoV = 170.f;
	
	TextureTarget = NewObject<UTextureRenderTarget2D>(this);
	TextureTarget->InitAutoFormat(HCount, 1);
	FOVAngle = HFoV;

	auto RadarPPMaterial = LoadObject<UMaterial>(this,
		TEXT("Material'/Game/RelativeLocation_Mat.RelativeLocation_Mat'"));
	AddOrUpdateBlendable(RadarPPMaterial);

  CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
  CaptureScene();

  FReadSurfaceDataFlags ReadPixelFlags(RCM_MinMax);
  FTextureRenderTargetResource* RTResource
    = TextureTarget->GameThread_GetRenderTargetResource();

  TArray<FLinearColor> Colors;
  const bool Result 
	  = RTResource->ReadLinearColorPixels(Colors, ReadPixelFlags);

  if (!Result || Colors.Num() != HCount) {
    return false;
  } else {
    Locations = ExtractLocations(Colors);
    Powers = ExtractPowers(Colors);
	return true;
  }
}

TArray<FVector2D> URadarComponent::ExtractLocations(const TArray<FLinearColor>& Colors) const {
	TArray<FVector2D> Locations;
	const float RaiseValue = 10000.f;
	Locations.SetNum(HCount);
	for (int HIndex = 0; HIndex < HCount; HIndex++) {
		Locations[HIndex].X = Colors[HIndex].G - RaiseValue;
		Locations[HIndex].Y = Colors[HIndex].R - RaiseValue;
	}
	return Locations;
}

TArray<float> URadarComponent::ExtractPowers(const TArray<FLinearColor>& Colors) const {
  TArray<float> Powers;
  Powers.Reserve(Colors.Num());
  for (auto& Color : Colors) {
    Powers.Add(Color.A);
  }
  return Powers;
}

