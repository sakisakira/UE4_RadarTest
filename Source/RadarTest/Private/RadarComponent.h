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
	  FVector2D Resolution;

	UFUNCTION(BlueprintCallable, Category = "Radar")
	  bool DistanceAndPower(float& Distance, float& Power);

 private:	
	float ExtractDistance(const TArray<FLinearColor>& Colors) const;
	float ExtractPower(const TArray<FLinearColor>& Colors) const;
	TArray<FVector> ExtraceLocations(const TArray<FLinearColor>& Colors) const;
	TArray<float> ExtracePowers(const TArray<FLinearColor>& Colors) const;
};
