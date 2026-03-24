#include "EnemySpawnZone.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

AEnemySpawnZone::AEnemySpawnZone()
{
    PrimaryActorTick.bCanEverTick = false;

    SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
    RootComponent = SpawnBox;

    SpawnBox->SetBoxExtent(FVector(300.f, 300.f, 200.f));

    SpawnBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawnZone::OnOverlapBegin);
    SpawnBox->OnComponentEndOverlap.AddDynamic(this, &AEnemySpawnZone::OnOverlapEnd);

    PlayerRef = nullptr;
}

void AEnemySpawnZone::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemySpawnZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        PlayerRef = OtherActor;
        StartSpawning();

        // Distance cleanup timer
        GetWorld()->GetTimerManager().SetTimer(
            DistanceTimer,
            this,
            &AEnemySpawnZone::CheckDistanceAndCleanup,
            1.0f,
            true
        );
    }
}

void AEnemySpawnZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        StopSpawning();
        GetWorld()->GetTimerManager().ClearTimer(DistanceTimer);
    }
}

void AEnemySpawnZone::StartSpawning()
{
    SpawnEnemies();
}

void AEnemySpawnZone::StopSpawning()
{
    GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
}

void AEnemySpawnZone::SpawnEnemies()
{
    if (!PlayerRef || EnemyClasses.Num() == 0) return;

    // CLEANUP invalid (destroy enemys)
    SpawnedEnemies.RemoveAll([](AActor* Enemy)
        {
            return Enemy == nullptr;
        });

    //COUNT near player
    int32 CurrentCount = 0;

    for (AActor* Enemy : SpawnedEnemies)
    {
        if (!Enemy) continue;

        float Dist = FVector::Distance(
            Enemy->GetActorLocation(),
            PlayerRef->GetActorLocation()
        );

        if (Dist <= DestroyDistance)
        {
            CurrentCount++;
        }
    }

    // ❌ LIMIT HIT → spawn stop
    if (CurrentCount >= MaxEnemiesNearPlayer)
    {
        // retry after delay
        float RetryTime = FMath::RandRange(MinSpawnInterval, MaxSpawnInterval);

        GetWorld()->GetTimerManager().SetTimer(
            SpawnTimer,
            this,
            &AEnemySpawnZone::SpawnEnemies,
            RetryTime,
            false
        );
        return;
    }

    // 🔥 how many enemy spawn
    int32 Count = FMath::RandRange(MinEnemiesPerSpawn, MaxEnemiesPerSpawn);

    for (int32 i = 0; i < Count; i++)
    {
        if (SpawnedEnemies.Num() >= MaxEnemiesNearPlayer) break;

        int32 Index = FMath::RandRange(0, EnemyClasses.Num() - 1);
        TSubclassOf<AActor> EnemyClass = EnemyClasses[Index];

        float Distance = FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);

        FVector Dir = FVector(
            FMath::RandRange(-1.f, 1.f),
            FMath::RandRange(-1.f, 1.f),
            0.f
        ).GetSafeNormal();

        FVector SpawnLoc = PlayerRef->GetActorLocation() + Dir * Distance;

        AActor* NewEnemy = GetWorld()->SpawnActor<AActor>(
            EnemyClass,
            SpawnLoc,
            FRotator::ZeroRotator
        );

        if (NewEnemy)
        {
            SpawnedEnemies.Add(NewEnemy);
        }
    }

    // next spawn
    float NextTime = FMath::RandRange(MinSpawnInterval, MaxSpawnInterval);

    GetWorld()->GetTimerManager().SetTimer(
        SpawnTimer,
        this,
        &AEnemySpawnZone::SpawnEnemies,
        NextTime,
        false
    );
}

void AEnemySpawnZone::CheckDistanceAndCleanup()
{
    if (!PlayerRef) return;

    for (int32 i = SpawnedEnemies.Num() - 1; i >= 0; i--)
    {
        AActor* Enemy = SpawnedEnemies[i];

        if (!Enemy)
        {
            SpawnedEnemies.RemoveAt(i);
            continue;
        }

        float Dist = FVector::Distance(
            Enemy->GetActorLocation(),
            PlayerRef->GetActorLocation()
        );

        if (Dist > DestroyDistance)
        {
            Enemy->Destroy();
            SpawnedEnemies.RemoveAt(i);
        }
    }
}