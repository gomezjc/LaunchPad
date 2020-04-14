// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LP_Panel.generated.h"

UCLASS()
class LAUNCHPAD_API ALP_Panel : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* InteractZoneCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* BaseComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* ButtonComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UTextRenderComponent * TextRenderer;

protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	bool bCanShowMessage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	bool bIsButtonOn;
	
public:	
	// Sets default values for this actor's properties
	ALP_Panel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Platform")
	void BP_ChangeButtonState();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ChangeButtonState();

	UFUNCTION(BlueprintCallable, Category = "Getter")
	bool GetButtonState() { return bIsButtonOn; }

	UFUNCTION(BlueprintCallable, Category = "Getter")
	bool GetCanShowMessage() { return bCanShowMessage; }

	UFUNCTION()
	void OnPlayerBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerEndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
