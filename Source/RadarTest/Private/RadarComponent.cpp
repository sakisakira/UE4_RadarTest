#include "RadarComponent.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"

URadarComponent::URadarComponent(void) : Super()
{
	bCaptureEveryFrame = true;
}

bool URadarComponent::LocationsAndPowers(
	TArray<FVector2D>& Locations,	TArray<float>& Powers)
{
	if (HCount < 10) HCount = 10;
	if (HFoV < 10.f) HFoV = 10.f;
	if (HFoV > 170.f) HFoV = 170.f;
	
  TextureTarget = NewObject<UTextureRenderTarget2D>(this);
  TextureTarget->InitAutoFormat(HCount, 1);
  FOVAngle = HFoV;

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
	Locations.SetNum(HCount);
	for (int HIndex = 0; HIndex < HCount; HIndex++) {
		const float Distance = Colors[HIndex].R;
		Locations[HIndex] = LocationOf(HIndex, Distance);
	}
	return Locations;
}

TArray<float> URadarComponent::ExtractPowers(const TArray<FLinearColor>& Colors) const {
  TArray<float> Powers;
  Powers.Reserve(Colors.Num());
  for (auto& Color : Colors) {
    Powers.Add(Color.B);
  }
  return Powers;
}

FVector2D URadarComponent::LocationOf(const int HIndex, const float Distance) const {
	const float ang = float(atan(2 * tan(HFoV * PI / 180 / 2) * (HIndex + 0.5) / HCount));
	return FVector2D(Distance * cosf(ang), Distance * sinf(ang));
}
