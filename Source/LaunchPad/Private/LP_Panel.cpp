// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/LP_Panel.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "LaunchPadCharacter.h"

// Sets default values
ALP_Panel::ALP_Panel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRootcomponent"));
	RootComponent = CustomRootComponent;

	BaseComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseComponent"));
	BaseComponent->SetupAttachment(CustomRootComponent);

	ButtonComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonComponent"));
	ButtonComponent->SetupAttachment(CustomRootComponent);

	InteractZoneCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ZoneCollider"));
	InteractZoneCollider->SetupAttachment(CustomRootComponent);
	InteractZoneCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractZoneCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	InteractZoneCollider->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	InteractZoneCollider->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	InteractZoneCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	TextRenderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextToInteract"));
	TextRenderer->SetupAttachment(CustomRootComponent);
	TextRenderer->SetText(FString("Press 'E' to Interact"));
	
	bIsButtonOn = false;
	bCanShowMessage = false;
}

// Called when the game starts or when spawned
void ALP_Panel::BeginPlay()
{
	Super::BeginPlay();
	InteractZoneCollider->OnComponentBeginOverlap.AddDynamic(this, &ALP_Panel::OnPlayerBeginOverlap);
	InteractZoneCollider->OnComponentEndOverlap.AddDynamic(this, &ALP_Panel::OnPlayerEndOverlap);
	TextRenderer->SetVisibility(false);
}

// Called every frame
void ALP_Panel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALP_Panel::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		ALaunchPadCharacter* OverlapCharacter = Cast<ALaunchPadCharacter>(OtherActor);

		if (IsValid(OverlapCharacter))
		{
			bCanShowMessage = true;
			TextRenderer->SetVisibility(true);
			OverlapCharacter->SetCanInteract(true);
		}
	}
}

void ALP_Panel::OnPlayerEndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor))
	{
		ALaunchPadCharacter* OverlapCharacter = Cast<ALaunchPadCharacter>(OtherActor);

		if (IsValid(OverlapCharacter))
		{
			TextRenderer->SetVisibility(false);
			bCanShowMessage = false;
			OverlapCharacter->SetCanInteract(false);
		}
	}
}

void ALP_Panel::ChangeButtonState()
{
	bIsButtonOn = !bIsButtonOn;
	BP_ChangeButtonState();
}