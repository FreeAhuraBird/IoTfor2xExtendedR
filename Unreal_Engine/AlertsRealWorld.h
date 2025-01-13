// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AlertsRealWorld.generated.h"


UCLASS()
class IOTSYSTEM_API UAlertsRealWorld : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void RetriveAlert();
	void DelayFunction();
	FString ResponseContent;
	FString lastAlert;
	FTimerHandle  TimerHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UTextBlock* AlertText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UBorder* AlertBorder;
	
};
