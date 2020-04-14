// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/LP_LaunchPlatform.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LaunchPadCharacter.h"

// Sets default values
ALP_LaunchPlatform::ALP_LaunchPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRootcomponent"));
	RootComponent = CustomRootComponent;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(CustomRootComponent);

	PlatformCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("MainColliderComponent"));
	PlatformCollision->SetupAttachment(CustomRootComponent);
	PlatformCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	PlatformCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PlatformCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	PlatformCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	PlatformCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	bIsPlatformOn = false;
}

// Called when the game starts or when spawned
void ALP_LaunchPlatform::BeginPlay()
{
	Super::BeginPlay();
	PlatformCollision->OnComponentBeginOverlap.AddDynamic(this, &ALP_LaunchPlatform::CheckPlayerEnter);
}

// Called every frame
void ALP_LaunchPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALP_LaunchPlatform::CheckPlayerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		ALaunchPadCharacter* OverlapCharacter = Cast<ALaunchPadCharacter>(OtherActor);

		if (IsValid(OverlapCharacter) && bIsPlatformOn)
		{
			OverlapCharacter->LaunchPlayer(SweepResult.Normal * -1);
		}
	}
}

void ALP_LaunchPlatform::ChangePlatformState()
{
	bIsPlatformOn = !bIsPlatformOn;
	BP_ChangePlatformState();
}


