#include "MenuGameMode.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

AMenuGameMode::AMenuGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/MainMenu/MenuCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
		DefaultPawnClass = PlayerPawnBPClass.Class;

	for (int i = 0; i < 4; i++)
		playerConnected.Add(false);

	teamList.Add(FColor::Blue);
	teamList.Add(FColor::Red);
	teamList.Add(FColor::Orange);
	teamList.Add(FColor::Green);
	teamList.Add(FColor::Purple);
	teamList.Add(FColor::MakeRandomColor());

	nicknameList.Add("Player 1");
	nicknameList.Add("Story Mode");
	nicknameList.Add("Player 2");
	nicknameList.Add("Winner");
	nicknameList.Add("Loser");
	nicknameList.Add("Floriqn");
	nicknameList.Add("Dr3quxis");
	nicknameList.Add("Jqrod");
	nicknameList.Add("R?mi");
	nicknameList.Add("LesPetitsProgrq;;eurs");
	nicknameList.Add("Leon");
	nicknameList.Add("Superwoman");
	nicknameList.Add(" . ");
	nicknameList.Add("=)");
	nicknameList.Add(")=");
	nicknameList.Add("Chocolate");

	OnGoToSelection.AddDynamic(this, &AMenuGameMode::ResetSelectionMenu);
}

void AMenuGameMode::StartPlay()
{
	Super::StartPlay();

	UGameplayStatics::SetForceDisableSplitscreen(GetWorld(), true);

	// Save them in memory
	for (float f = 0; f < 4 * 270.f; f += 270.f)
	{
		FVector newLoc = FVector(firstPlayerLocation.X, firstPlayerLocation.Y + f, firstPlayerLocation.Z);
		playerFlags.Add(newLoc);
	}
}

void AMenuGameMode::SetPlayer(APlayerController* controller)
{
	int id = UGameplayStatics::GetPlayerControllerID(controller);
	playerConnected[id] = true;
	connectedCount++;

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString("Player number ") + FString::FromInt(id));
	bool prev = true;
	goodConnectionOrder = true;
	for (bool& connected : playerConnected)
	{
		if (goodConnectionOrder && !prev && connected)
		{
			goodConnectionOrder = false;
			break;
		}
		prev = connected;
	}

	SpawnControllerAtPlayerStart(controller);
	OnPlayerJoin.Broadcast(id);
}

void AMenuGameMode::SpawnControllerAtPlayerStart(APlayerController* controller)
{
	if (playerFlags.Num() > 0)
	{
		// Set the current transform with the player start transform
		FVector currentStart = playerFlags[UGameplayStatics::GetPlayerControllerID(controller)];
		controller->AcknowledgedPawn->SetActorLocation(currentStart);
	}
}

void AMenuGameMode::ResetSelectionMenu()
{
	for (bool& connect : playerConnected)
		connect = false;

	connectedCount = 0;
	goodConnectionOrder = false;

	// Remove player spawn
}

void AMenuGameMode::PlayGame()
{
	// Reset player count
	for (int i = 3; i >= 0; i--)
		UGameplayStatics::RemovePlayer(UGameplayStatics::GetPlayerController(GetWorld(), i), true);

	UGameplayStatics::SetForceDisableSplitscreen(GetWorld(), false);
	UGameplayStatics::OpenLevel(GetWorld(), "ThirdPersonExampleMap");

}