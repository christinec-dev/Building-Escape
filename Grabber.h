#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Arm Reach Length
	float Reach = 100.f;

	UPROPERTY();
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	
	UPROPERTY();
	UInputComponent* InputComponent = nullptr;

	void Grab();
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();

	//Return Objects Within Character Reach
	FHitResult GetFirstPhysicsBodyInReach() const;

	//Return LineTraceEnd
	FVector GetPlayersReach() const;

	//Player Position
	FVector GetPlayersWorldPos() const;
};
