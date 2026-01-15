#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "[InterfaceName].generated.h"

/**
 * [InterfaceName] Interface
 * 
 * Implement this interface on actors that [capability description].
 * 
 * @see [RelatedClass1]
 * @see [RelatedClass2]
 */
UINTERFACE(MinimalAPI, Blueprintable)
class U[InterfaceName] : public UInterface
{
    GENERATED_BODY()
};

class CPLUS_API I[InterfaceName]
{
    GENERATED_BODY()

public:
    /**
     * [Method description]
     * 
     * @param [ParamName] [Param description]
     * @return [Return description]
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "[Category]")
    [ReturnType] [MethodName]([ParamType] [ParamName]);
    
    /**
     * Check if [condition description]
     * 
     * @param [ParamName] [Param description]
     * @return True if [condition]
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "[Category]")
    bool Can[Action]([ParamType] [ParamName]) const;
};

// Implementation note:
// In implementing class, add: , public I[InterfaceName]
// Override methods as: virtual [ReturnType] [MethodName]_Implementation([ParamType] [ParamName]) override;
