// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullCowCount 
{
    int32 Bulls = 0;
    int32 Cows = 0;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
		virtual void BeginPlay() override;
		virtual void OnInput(const FString& Input) override;

		void SetupGame();
		void RestartPlay();
		void EndGame();
		void WinMessage();
		void LoseMessage();
		void NothingEnteredMessage();
		void ProcessGuess(const FString& Input);
		bool IsIsogram(const FString& Input) const;
		TArray<FString> GetWords(const TArray<FString>& WordList) const;
		FBullCowCount GetBullCows(const FString& Input) const;

	// Your declarations go below!
	private:
		FString HiddenWord;
		int32 Lives;
		int32 Level;
		bool bGameOver;
		int32 HWordLen;
		// int32 MaxWords;
		TArray<FString> Isograms;
};
