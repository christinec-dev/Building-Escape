// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	//set absolute values for door rotation upon game start
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	// Protection Against Null Pointers
	if(!PressurePlate){
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressureplate set!"), *GetOwner()->GetName());
	}

	//Sets the actor to our default pawn so we can open the door
	ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Opening the door to x degrees upon game start
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Calls OpenDoor function if the Actor steps into pressure plate zone
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpen))
	{
		//Opens Door In Trigger Zone
		OpenDoor(DeltaTime);

		//Sets Timer Count Seconds Door Is Open
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	} 
	else {
		//Close Door After A Few Seconds
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay) {
			CloseDoor(DeltaTime);
		}
	}
}

// Opens the door 
void UOpenDoor::OpenDoor(float DeltaTime)
{
	//Gets current location/rotation value of door for console
	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());

	//Gets set yaw(opening degree) value for door for console
	UE_LOG(LogTemp, Warning, TEXT("Yaw is %f"), GetOwner()->GetActorRotation().Yaw);

	//updates current door rotation to rotate (DeltaTime * DoorOpenSpeed will solve fps crash problems)
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;

	//rotates the door x degrees during x seconds
	GetOwner()->SetActorRotation(DoorRotation);
}

// Opens the door 
void UOpenDoor::CloseDoor(float DeltaTime)
{
	//updates current door rotation to rotate shut much faster than it opens
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;

	//rotates the door x degrees during x seconds
	GetOwner()->SetActorRotation(DoorRotation);
}

