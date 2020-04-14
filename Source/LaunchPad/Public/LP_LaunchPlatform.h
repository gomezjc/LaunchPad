// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LP_LaunchPlatform.generated.h"

UCLASS()
class LAUNCHPAD_API ALP_LaunchPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALP_LaunchPlatform();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USceneComponent* CustomRootComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Components")
	class UBoxComponent* PlatformCollision;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	class UStaticMeshComponent* MeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void CheckPlayerEnter(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
