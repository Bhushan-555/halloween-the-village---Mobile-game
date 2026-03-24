// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "RandomHorrorSound.generated.h"

UCLASS()
class HELLOWEEN_1_API ARandomHorrorSound : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARandomHorrorSound();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayRandomSound();

	FTimerHandle SoundTimer;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USoundBase *> HorrorSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinDelay = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDelay = 20.0f;
};
