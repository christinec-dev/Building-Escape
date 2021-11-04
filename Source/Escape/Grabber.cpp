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

//Game Physics
void UGrabber::FindPhysicsHandle() {
	//Checks for Physics Handle Component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	//Crash Protection
	if(PhysicsHandle) {
		//Physics Handle Is Found
	} 
	else {
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Found On : %s"), *GetOwner()->GetName());
	}
}

//Game Input
void UGrabber::SetupInputComponent() {
	//InputComponent To Allow Player To Interact
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		//"Grab" Must Be Same As Input In Project Settings
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} 
}

//Press Right Mouse Button
void UGrabber::Grab() 
{
	UE_LOG(LogTemp, Error, TEXT("Pressed"));

	//Only Log Collision In Reach When Key Clicked
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	
	//***********************
	//Gets Player ViewPoint
	FVector PlayerViewPointLocation; 
	FRotator PlayerViewPointRotation; 
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);

	//We use Ray-Cast (Line Tracing) - To Help Indicate Object Contact
	FVector LineTraceEnd = PlayerViewPointLocation * PlayerViewPointRotation.Vector() * Reach;
	//**************************

	//If We Grab Something, Attach Physics Handle
	if (HitResult.GetActor()) {
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
	}
}

//Release Right Mouse Button
void UGrabber::Release() 
{
	UE_LOG(LogTemp, Error, TEXT("Released"));
	//Removes Physics Handle
	PhysicsHandle->ReleaseComponent();
}

// Called Every Frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//***********************
	//Gets Player ViewPoint
	FVector PlayerViewPointLocation; 
	FRotator PlayerViewPointRotation; 
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);

	//We use Ray-Cast (Line Tracing) - To Help Indicate Object Contact
	FVector LineTraceEnd = PlayerViewPointLocation * PlayerViewPointRotation.Vector() * Reach;
	//**************************

	//If Physics Handle Is Attached, Then Set Target Location
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

// Line Tracing
FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	//Gets Player ViewPoint
	FVector PlayerViewPointLocation; 
	FRotator PlayerViewPointRotation; 
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);

	//We use Ray-Cast (Line Tracing) - To Help Indicate Object Contact
	FVector LineTraceEnd = PlayerViewPointLocation * PlayerViewPointRotation.Vector() * Reach;
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	//Logging What Player Collides With
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Has Hit: %s"), *ActorHit->GetName());
	}

	return Hit;
}
