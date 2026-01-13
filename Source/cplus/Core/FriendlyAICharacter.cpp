#include "FriendlyAICharacter.h"

AFriendlyAICharacter::AFriendlyAICharacter()
{
	// Set team to friendly
	Team = EAITeam::Friendly;

	// TeamByte for scoring (team 0 = friendly)
	TeamByte = 0;
}
