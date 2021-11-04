//Copyright Christine Coomans 2021

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

//Allows us to use out parameters - useless
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	SetupInputComponent();
}

//Game Input
void UGrabber::SetupInputComponent() 
{
	//InputComponent To Allow Player To Interact
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		//"Grab" Must Be Same As Input In Project Settings
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} 
}

//Game Physics
void UGrabber::FindPhysicsHandle() 
{
	//Checks for Physics Handle Component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	//Crash Protection
	if(!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Found On : %s"), *GetOwner()->GetName());

	} 
}

//Press Right Mouse Button
void UGrabber::Grab() 
{
	//Only Log Collision In Reach When Key Clicked
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	//If We Grab Something, Attach Physics Handle
	if (ActorHit) 
	{
		if (!PhysicsHandle) {return;}
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab, 
			NAME_None, 
			GetPlayersReach()
		);
	}
}

//Release Right Mouse Button
void UGrabber::Release() 
{
	//Removes Physics Handle
	if (!PhysicsHandle) {return;}
	PhysicsHandle->ReleaseComponent();
}


// Called Every Frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//If Physics Handle Is Attached, Then Set Target Location
	if (!PhysicsHandle) {return;}
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
}

// Line Tracing
FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;
}

// Gets Player ViewPoint
FVector UGrabber::GetPlayersWorldPos() const 
{
	//Gets Player ViewPoint
	FVector PlayerViewPointLocation; 
	FRotator PlayerViewPointRotation; 

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);

	//We use Ray-Cast (Line Tracing) - To Help Indicate Object Contact
	return PlayerViewPointLocation;
}

// Gets Player ReachPoint
FVector UGrabber::GetPlayersReach() const {
	FVector PlayerViewPointLocation; 
	FRotator PlayerViewPointRotation; 

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);

	//We use Ray-Cast (Line Tracing) - To Help Indicate Object Contact
	return PlayerViewPointLocation * PlayerViewPointRotation.Vector() * Reach;
}
