// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate();

	//Tracks Sound Played
	bool OpenDoorSound = false;
	bool CloseDoorSound = true;

private:

	//door rotation values during rotation
	float InitialYaw;
	float CurrentYaw;

	// IMPORTANT: UPROPERTY(EditAnywhere) allows designers to edit values in-game

	//Allows us to set OpenAngle for individual doors in UE4 itself
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;

	//Timer Functions for Door Open/Close Continuation
	float DoorLastOpened = 0.f;

	//Close Delay When Door Opens
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = .5f;

	//Open Speed For Door
	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 0.8f;

	//Close Speed For Door
	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 2.f;

	//Mass To Open Door
	UPROPERTY(EditAnywhere)
	float MassToOpenDoors = 50.f;

	//Links our Pressure Plate to our OpenDoor Component
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	//Sound SFX
	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;
};
