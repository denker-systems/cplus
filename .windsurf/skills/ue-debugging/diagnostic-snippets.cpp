// ============================================================================
// UE 5.7 Diagnostic Code Snippets
// Copy and adapt these for debugging
// ============================================================================

// ----------------------------------------------------------------------------
// LOGGING SNIPPETS
// ----------------------------------------------------------------------------

// Basic log
UE_LOG(LogTemp, Log, TEXT("Message here"));

// Log with object name
UE_LOG(LogTemp, Log, TEXT("%s: Message"), *GetName());

// Log variable value
UE_LOG(LogTemp, Log, TEXT("Value = %d"), IntValue);
UE_LOG(LogTemp, Log, TEXT("Value = %f"), FloatValue);
UE_LOG(LogTemp, Log, TEXT("Value = %s"), *StringValue);
UE_LOG(LogTemp, Log, TEXT("Value = %s"), *FName.ToString());
UE_LOG(LogTemp, Log, TEXT("Bool = %s"), bValue ? TEXT("true") : TEXT("false"));

// Log pointer validity
UE_LOG(LogTemp, Log, TEXT("Pointer = %s"), Pointer ? TEXT("valid") : TEXT("null"));

// Log with function name
UE_LOG(LogTemp, Log, TEXT("%s::%hs called"), *GetName(), __FUNCTION__);

// Log array contents
for (int32 i = 0; i < Array.Num(); ++i)
{
    UE_LOG(LogTemp, Log, TEXT("  [%d] = %s"), i, *Array[i].ToString());
}

// Log TMap contents
for (const auto& Pair : MyMap)
{
    UE_LOG(LogTemp, Log, TEXT("  [%s] = %s"), *Pair.Key.ToString(), *Pair.Value.ToString());
}

// ----------------------------------------------------------------------------
// ASSERTION SNIPPETS
// ----------------------------------------------------------------------------

// Hard crash if false (development builds only)
check(Pointer != nullptr);
checkf(Value > 0, TEXT("Value must be positive, got %d"), Value);

// Soft assertion - logs error but continues (once per callsite)
if (!ensure(Pointer != nullptr))
{
    return;
}

// Soft assertion with message
if (!ensureMsgf(Value > 0, TEXT("Value must be positive, got %d"), Value))
{
    return DefaultValue;
}

// Always executes expression, asserts on result
verify(SomeFunction());

// ----------------------------------------------------------------------------
// NULL SAFETY SNIPPETS
// ----------------------------------------------------------------------------

// IsValid for UObjects (checks null AND pending kill)
if (IsValid(Actor))
{
    Actor->DoSomething();
}

// Early return pattern
if (!IsValid(Component))
{
    UE_LOG(LogTemp, Warning, TEXT("Component is invalid"));
    return;
}

// Null-safe chain
if (AActor* Owner = GetOwner())
{
    if (UActorComponent* Comp = Owner->FindComponentByClass<UActorComponent>())
    {
        Comp->DoSomething();
    }
}

// ----------------------------------------------------------------------------
// DEBUG DRAWING SNIPPETS
// ----------------------------------------------------------------------------

// Draw debug sphere
DrawDebugSphere(GetWorld(), Location, 50.0f, 12, FColor::Red, false, 2.0f);

// Draw debug line
DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f);

// Draw debug point
DrawDebugPoint(GetWorld(), Location, 10.0f, FColor::Blue, false, 2.0f);

// Draw debug string in world
DrawDebugString(GetWorld(), Location, TEXT("Debug"), nullptr, FColor::White, 2.0f);

// ----------------------------------------------------------------------------
// TIMING SNIPPETS
// ----------------------------------------------------------------------------

// Measure execution time
{
    SCOPE_CYCLE_COUNTER(STAT_MyFunction);
    // Code to measure
}

// Quick timer
double StartTime = FPlatformTime::Seconds();
// ... code ...
double EndTime = FPlatformTime::Seconds();
UE_LOG(LogTemp, Log, TEXT("Execution took %f ms"), (EndTime - StartTime) * 1000.0);

// ----------------------------------------------------------------------------
// OBJECT INTROSPECTION
// ----------------------------------------------------------------------------

// Get class name
UE_LOG(LogTemp, Log, TEXT("Class: %s"), *Actor->GetClass()->GetName());

// List all components
for (UActorComponent* Comp : Actor->GetComponents())
{
    UE_LOG(LogTemp, Log, TEXT("  Component: %s (%s)"), 
        *Comp->GetName(), *Comp->GetClass()->GetName());
}

// Check interface
if (Actor->Implements<UMyInterface>())
{
    IMyInterface::Execute_MyFunction(Actor);
}
