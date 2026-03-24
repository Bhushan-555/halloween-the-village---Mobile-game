// Fill out your copyright notice in the Description page of Project Settings.

#include "RandomHorrorSound.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
ARandomHorrorSound::ARandomHorrorSound()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ARandomHorrorSound::BeginPlay()
{
	Super::BeginPlay();

	PlayRandomSound();
}

// Called every frame
void ARandomHorrorSound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARandomHorrorSound::PlayRandomSound()
{
	if (HorrorSounds.Num() == 0)
		return;

	int32 Index = FMath::RandRange(0, HorrorSounds.Num() - 1);

	UGameplayStatics::PlaySoundAtLocation(
		this,
		HorrorSounds[Index],
		GetActorLocation());

	float NextTime = FMath::RandRange(MinDelay, MaxDelay);

	GetWorldTimerManager().SetTimer(
		SoundTimer,
		this,
		&ARandomHorrorSound::PlayRandomSound,
		NextTime,
		false);
}