// 


#include "Core/SmashGameInstance.h"

USmashGameInstance::USmashGameInstance()
{
	PlayerTeam.Add(1);
	PlayerTeam.Add(2);
	PlayerTeam.Add(3);
	PlayerTeam.Add(1);

	AliveArray.Add(true);
	AliveArray.Add(true);
	AliveArray.Add(false);
	AliveArray.Add(false);

	CPU.Add(false);
	CPU.Add(false);
	CPU.Add(false);
	CPU.Add(false);

	PlayerCharacters.Add(ESmashCharacter::Fighter);
	PlayerCharacters.Add(ESmashCharacter::Fighter);
	PlayerCharacters.Add(ESmashCharacter::Gunner);
	PlayerCharacters.Add(ESmashCharacter::Hats);
}
