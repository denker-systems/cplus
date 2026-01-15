#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "[ComponentName]Component.generated.h"

/**
 * [ComponentName] Component
 * 
 * [Brief description of what this component does]
 * 
 * Usage: Attach to [ActorType] to enable [functionality]
 * 
 * @see [RelatedComponent]
 * @see [RelatedSubsystem]
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPLUS_API U[ComponentName]Component : public UActorComponent
{
    GENERATED_BODY()

public:
    U[ComponentName]Component();

    // UActorComponent interface
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // Public API
    
    /** [Description of method] */
    UFUNCTION(BlueprintCallable, Category = "[Category]")
    void [MethodName]();
    
    /** [Description of getter] */
    UFUNCTION(BlueprintPure, Category = "[Category]")
    bool Is[State]() const;

    // Delegates
    
    /** Broadcast when [event description] */
    UPROPERTY(BlueprintAssignable, Category = "[Category]")
    F[EventName] On[EventName];

protected:
    // Protected methods
    void Internal[HelperMethod]();

private:
    // Properties
    
    /** [Property description] */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "[Category]", meta = (AllowPrivateAccess = "true"))
    [Type] [PropertyName];
    
    /** Cached reference */
    UPROPERTY()
    TObjectPtr<[OwnerType]> CachedOwner;
};
