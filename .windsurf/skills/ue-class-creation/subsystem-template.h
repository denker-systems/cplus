#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "[SubsystemName]Subsystem.generated.h"

// Forward declarations
class U[DataType];

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOn[EventName], [ParamType], [ParamName]);

/**
 * [SubsystemName] Subsystem
 * 
 * Global manager for [functionality description].
 * 
 * Access via: GetGameInstance()->GetSubsystem<U[SubsystemName]Subsystem>()
 * 
 * @see [RelatedClass1]
 * @see [RelatedClass2]
 */
UCLASS()
class CPLUS_API U[SubsystemName]Subsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // USubsystem interface
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }

    // Public API
    
    /**
     * [Method description]
     * 
     * @param [ParamName] [Param description]
     * @return [Return description]
     */
    UFUNCTION(BlueprintCallable, Category = "[Category]")
    bool [MethodName]([ParamType] [ParamName]);
    
    /** Get all active [items] */
    UFUNCTION(BlueprintPure, Category = "[Category]")
    TArray<[ItemType]> GetAll[Items]() const;

    // Events
    
    /** Broadcast when [event description] */
    UPROPERTY(BlueprintAssignable, Category = "[Category]")
    FOn[EventName] On[EventName];

private:
    // Internal state
    
    /** Map of active [items], keyed by [key description] */
    UPROPERTY()
    TMap<FName, [ValueType]> Active[Items];
    
    /** Cached data */
    UPROPERTY()
    TArray<TObjectPtr<U[DataType]>> Cached[Data];
};
