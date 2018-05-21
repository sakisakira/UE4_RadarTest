#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "RadarComponent.generated.h"

UCLASS(ClassGroup = Rendering, meta = (BlueprintSpawnableComponent))
class URadarComponent : public USceneCaptureComponent2D
{
	GENERATED_BODY()

 public:
	URadarComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
		int32 HCount = 120;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
		float HFoV = 120.f;

	UFUNCTION(BlueprintCallable, Category = "Radar")
	  bool LocationsAndPowers(TArray<FVector2D>& Locations, TArray<float>& Powers);

 private:	
	TArray<FVector2D> ExtractLocations(const TArray<FLinearColor>& Colors) const;
	TArray<float> ExtractPowers(const TArray<FLinearColor>& Colors) const;
	FVector2D LocationOf(const int HIndex, const float Distance) const;
};
