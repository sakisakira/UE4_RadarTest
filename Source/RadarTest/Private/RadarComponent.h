#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "RadarComponent.generated.h"

UCLASS()
class URadarComponent : public USceneCaptureComponent2D
{
	GENERATED_BODY()
	
public:
	URadarComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
		FVector2D Resolution;

	UFUNCTION(BlueprintCallable, Category = "Radar")
		float Distance();
	
	
};
