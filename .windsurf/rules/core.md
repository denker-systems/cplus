---
trigger: always_on
description: Core workspace rules for cplus UE 5.7 C++ project
---

# Core Rules

<project_context>
- Project: cplus (Unreal Engine 5.7 C++)
- Type: Multi-variant game (Shooter + Horror)
- Core System: Quest System with components and subsystem
- Repository: github.com/denker-systems/cplus
</project_context>

<ai_behavior>
- Communicate with user in Swedish
- Write all code, comments, documentation in English
- Be concise and direct - avoid verbose explanations
- Read code before proposing changes - never speculate
- Implement changes rather than only suggesting them
- Use parallel tool calls when actions are independent
</ai_behavior>

<development_approach>
- Always do minimal implementations first
- Use event-driven patterns over polling (avoid Tick)
- Prefer composition over inheritance (Component pattern)
- Use Subsystems for global managers
- Use DataAssets for data-driven design
- Use Delegates for event communication
- Cache expensive operations
</development_approach>

<code_quality>
- Add defensive null checks with IsValid() before pointer use
- Use early returns to reduce nesting
- Keep functions focused on single responsibility
- Use check/ensure for assertions
- Follow const correctness
</code_quality>

<investigation_before_action>
Never speculate about code you have not opened. If referencing a specific file, read it before answering. Give grounded, hallucination-free answers based on actual code inspection.
</investigation_before_action>

<tool_usage>
- Use @ue-class-creation skill when creating new UE classes
- Use @ue-debugging skill for systematic debugging
- Use @ue-code-review skill for code review
- Use @ue-testing skill for test procedures
- Use Context7 MCP for library documentation lookups
</tool_usage>