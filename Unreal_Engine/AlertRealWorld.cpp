// Fill out your copyright notice in the Description page of Project Settings.


#include "AlertsRealWorld.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "Components/TextBlock.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "TimerManager.h"
#include "Components/Border.h"

float TimeSinceLastCalled = 0.0f;


void UAlertsRealWorld::NativeConstruct(){
	Super::NativeConstruct();
	FString output = "";
	AlertText->SetText(FText::FromString(output));
}

void UAlertsRealWorld::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	TimeSinceLastCalled += InDeltaTime;

	if (TimeSinceLastCalled >= 0.5f)
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
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseContent);

			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				FString Message = JsonObject->GetStringField(TEXT("message"));

				if (JsonObject->HasField(TEXT("topic")))
				{
					FString Topic = JsonObject->GetStringField(TEXT("topic"));
					UE_LOG(LogTemp, Log, TEXT("Topic: %s"), *Topic);
				
					if (Topic == "sensor/sound")
					{
						FString output = "There is noice around you";
						AlertText->SetText(FText::FromString(output));
						GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UAlertsRealWorld::DelayFunction, 2.0f, false);
					}
				}
				else
				{
					FString output = "";
					AlertText->SetText(FText::FromString(output));
				}

			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("HTTP Request Faileed"));
		}
	});

	Request->ProcessRequest();
}

void UAlertsRealWorld::DelayFunction()
{
	UE_LOG(LogTemp, Error, TEXT("two second delay"));
}
