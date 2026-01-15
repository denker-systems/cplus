// ============================================================================
// UE 5.7 Automation Test Template
// Copy and adapt for new test classes
// ============================================================================

#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Engine/World.h"

// ----------------------------------------------------------------------------
// SIMPLE TEST (single function)
// ----------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    F[TestName]Test,
    "cplus.[System].[TestName]",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter
)

bool F[TestName]Test::RunTest(const FString& Parameters)
{
    // Arrange - Setup test conditions
    // ...
    
    // Act - Execute the code being tested
    // ...
    
    // Assert - Verify results
    TestTrue(TEXT("Description of expected condition"), bCondition);
    TestEqual(TEXT("Values should match"), ActualValue, ExpectedValue);
    TestNull(TEXT("Should be null"), Pointer);
    TestNotNull(TEXT("Should not be null"), Pointer);
    
    return true;
}

// ----------------------------------------------------------------------------
// COMPLEX TEST (with setup/teardown)
// ----------------------------------------------------------------------------

IMPLEMENT_COMPLEX_AUTOMATION_TEST(
    F[ComplexTestName]Test,
    "cplus.[System].[ComplexTestName]",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter
)

void F[ComplexTestName]Test::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    // Add test variations
    OutBeautifiedNames.Add(TEXT("Variation1"));
    OutTestCommands.Add(TEXT("Param1"));
    
    OutBeautifiedNames.Add(TEXT("Variation2"));
    OutTestCommands.Add(TEXT("Param2"));
}

bool F[ComplexTestName]Test::RunTest(const FString& Parameters)
{
    // Parameters contains the test command string
    if (Parameters == TEXT("Param1"))
    {
        // Test variation 1
    }
    else if (Parameters == TEXT("Param2"))
    {
        // Test variation 2
    }
    
    return true;
}

// ----------------------------------------------------------------------------
// LATENT TEST (async/multi-frame)
// ----------------------------------------------------------------------------

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FWaitForConditionCommand, TFunction<bool()>, Condition);

bool FWaitForConditionCommand::Update()
{
    return Condition();
}

// Usage in test:
// ADD_LATENT_AUTOMATION_COMMAND(FWaitForConditionCommand([this]() { return bConditionMet; }));

// ----------------------------------------------------------------------------
// COMMON ASSERTIONS
// ----------------------------------------------------------------------------

// Boolean checks
TestTrue(TEXT("Message"), bValue);
TestFalse(TEXT("Message"), bValue);

// Equality checks
TestEqual(TEXT("Message"), Actual, Expected);
TestNotEqual(TEXT("Message"), Actual, NotExpected);

// Null checks
TestNull(TEXT("Message"), Pointer);
TestNotNull(TEXT("Message"), Pointer);

// String checks
TestEqual(TEXT("Message"), ActualString, ExpectedString);

// Numeric comparisons
TestEqual(TEXT("Message"), ActualFloat, ExpectedFloat, Tolerance);

// Collection checks
TestEqual(TEXT("Array size"), Array.Num(), ExpectedSize);

// Custom failure
AddError(TEXT("Custom error message"));
AddWarning(TEXT("Custom warning message"));

// ----------------------------------------------------------------------------
// TEST UTILITIES
// ----------------------------------------------------------------------------

// Create test world
UWorld* TestWorld = UWorld::CreateWorld(EWorldType::Game, false);

// Create test object
UMyObject* TestObj = NewObject<UMyObject>();

// Create test actor (requires world)
AMyActor* TestActor = TestWorld->SpawnActor<AMyActor>();

// Cleanup
TestWorld->DestroyWorld(false);
