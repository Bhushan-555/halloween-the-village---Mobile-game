#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnZone.generated.h"

class UBoxComponent;

UCLASS()
class HELLOWEEN_1_API AEnemySpawnZone : public AActor
{
    GENERATED_BODY()

public:
    AEnemySpawnZone();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    UBoxComponent* SpawnBox;

    //Multiple enemy types
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TArray<TSubclassOf<AActor>> EnemyClasses;

    //Spawn count per wave
    UPROPERTY(EditAnywhere, Category = "Spawn")
    int32 MinEnemiesPerSpawn = 1;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    int32 MaxEnemiesPerSpawn = 2;

    // Spawn interval
    UPROPERTY(EditAnywhere, Category = "Spawn")
    float MinSpawnInterval = 2.f;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    float MaxSpawnInterval = 4.f;

    // Spawn distance
    UPROPERTY(EditAnywhere, Category = "Spawn")
    float MinSpawnDistance = 300.f;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    float MaxSpawnDistance = 800.f;

    // Destroy distance
    UPROPERTY(EditAnywhere, Category = "Spawn")
    float DestroyDistance = 1500.f;

    //MAX enemies near player (IMPORTANT)
    UPROPERTY(EditAnywhere, Category = "Spawn")
    int32 MaxEnemiesNearPlayer = 8;

    // Runtime
    AActor* PlayerRef;

    UPROPERTY()
    TArray<AActor*> SpawnedEnemies;

    FTimerHandle SpawnTimer;
    FTimerHandle DistanceTimer;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void SpawnEnemies();
    void StartSpawning();
    void StopSpawning();

    void CheckDistanceAndCleanup();
};