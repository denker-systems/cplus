#include "HostileAICharacter.h"

AHostileAICharacter::AHostileAICharacter()
{
	// Set team to hostile
	Team = EAITeam::Hostile;

	// TeamByte for scoring (team 1 = hostile)
	TeamByte = 1;
}
