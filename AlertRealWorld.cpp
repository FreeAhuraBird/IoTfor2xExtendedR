// Fill out your copyright notice in the Description page of Project Settings.


#include "AlertsRealWorld.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

float TimeSinceLastCalled = 0.0f;


void UAlertsRealWorld::NativeConstruct(){
	Super::NativeConstruct();

	//if (Border) Border->SetVisibility(ESlateVisibility::Collapsed);
}

void UAlertsRealWorld::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	TimeSinceLastCalled += InDeltaTime;

	if (TimeSinceLastCalled >= 3.0f)
	{
		RetriveAlert();
		TimeSinceLastCalled = 0.0f;
	}
}

void UAlertsRealWorld::RetriveAlert()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(TEXT("http://172.20.10.8:5000/api/alert"));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
		if (bWasSuccessful && Response.IsValid())
		{ 
			FString ResponseContent = Response->GetContentAsString();
			UE_LOG(LogTemp, Log, TEXT("Message from Flask: %s"), *ResponseContent);
			this->checkResponse(ResponseContent);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("HTTP Request Faileed"));
		}
	});

	Request->ProcessRequest();
}

void UAlertsRealWorld::checkResponse(const FString& newResponse)
{
	if (newResponse != lastAlert && newResponse != "No alert")
	{
		AlertText->SetText(FText::FromString(newResponse));
	}
	lastAlert = newResponse;
}
