// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Paths.h"
#include "FileHelper.h"


void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter, accepts the address of player input
{

    // display the congratulatory message when all the words have been guessed correctly.
    PrintLine(TEXT("Num Words: %i, Level: %i"), Isograms.Num(), Level);
    if (Level == Isograms.Num()) {
        PrintLine(TEXT("You have guessed all the words correctly.\nYou have won this game."));
        return;
    }

    // if the game is over then do ClearScreen() and SetupGame() for the game reset
    // else check  the PlayerGuess
    if (bGameOver) {
        ClearScreen();
        SetupGame();
        return;
    } 

    // process the uder input
    ProcessGuess(Input);

    // display the lose message and prompt the user to play again.
    if (!Lives) {
        LoseMessage();
        EndGame();
        return;
    }
}

void UBullCowCartridge::BeginPlay() {
    // start game
    Super::BeginPlay();

    // initialize the words
    TArray<FString> Words;
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordList/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    
    // get the isograms
    Isograms = GetWords(Words);

    // PrintLine(TEXT("Words path: %s"), *WordListPath);
    PrintLine(TEXT("The number of possible words are %i"), Words.Num());
    PrintLine(TEXT("The number of valid vords is: %i"), Isograms.Num());
    SetupGame();
}

bool UBullCowCartridge::IsIsogram(const FString& Input) const {

    // loop through every character in Input provided and check if the same character is repeated.
    int32 Index = 0;
    int32 Comparison = 0;

    // no need to do <= to Input.Len() because we do not need to proceed further than Input + 1,
    // we do not need to cause a overflow fault when we go beyond the length of the char array
    do {
        for (Comparison = Index + 1; Comparison < Input.Len(); Comparison++)
        {
            // compare the current index against the Index + 1 of the char Input;
            if (Input[Index] == Input[Comparison]) {
                return false;
            }
        }
        Index++;
    } while (Index < Input.Len());
    return true;
}

void UBullCowCartridge::NothingEnteredMessage() {
    PrintLine(TEXT("Please enter a %i letter word."), HWordLen);
}

void UBullCowCartridge::ProcessGuess(const FString& Input) {

    if (!IsIsogram(Input)) {
        PrintLine(TEXT("No repeating letters please. Guess again."));
        return;
    }

    // if nothing is entered, repeat the message
    if (Input == TEXT("")) {
        NothingEnteredMessage();
        return;
    } 

    // win the game if the word is guessed correctly    
    if (Input == HiddenWord) {
        WinMessage();
        EndGame();
        ++Level;
        return;
    } 

    if (Input.Len() != HiddenWord.Len()) {
        PrintLine(TEXT("The Hidden Word is %i characters long. Please Try again"), HiddenWord.Len()); // introduce the magic number.
    } else {
        PrintLine(TEXT("You have guessed the word incorrectly: "));
        PrintLine(FString::Printf(TEXT("Chances remaining: %i"), Lives));
        PrintLine(Input);

        // pre-decrement the chances
        --Lives;
    }

    // Show the Bulls and Cows
    FBullCowCount Count = GetBullCows(Input);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Count.Bulls, Count.Cows);
}

void UBullCowCartridge::LoseMessage() {
    PrintLine(TEXT("The hidden word was: %s."), *HiddenWord);  
    PrintLine(TEXT("You Lose! "));
    PrintLine(TEXT("Want to play again?"));
}

void UBullCowCartridge::WinMessage() {
    PrintLine(HiddenWord);
    PrintLine(TEXT("You have guessed the word correctly!"));
}

void UBullCowCartridge::SetupGame() {
    // get a random letter
    int32 RandNum = FMath::RandRange(0, Isograms.Num() - 1);
    // PrintLine(TEXT("Random Number: %i"), RandNum);
    HiddenWord = Isograms[RandNum];
    HWordLen = HiddenWord.Len();
    Lives = HiddenWord.Len();
    bGameOver = false;

    // PrintLine(TEXT("length of words: %i"), Isograms.Num());

    // print game information
    PrintLine(TEXT("Hi there! Welcome to the Bull Cow Game!"));
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Guess the %i letter word."), HWordLen);

    // sets the array index automatically
    PrintLine(TEXT("The first character of the word is: %c."), HiddenWord[0]);
    PrintLine(TEXT("The %i character of Hidden word is %c\n"), HWordLen, HiddenWord[HWordLen - 1]);
    PrintLine(TEXT("The hidden word is: %s"), *HiddenWord);

}

void UBullCowCartridge::EndGame() {
    // end the game
    bGameOver = true;
    PrintLine(TEXT("Press enter to continue..."));
}


TArray<FString> UBullCowCartridge::GetWords(const TArray<FString>& WordList) const {
    // create a new FString array
    TArray<FString> ValidWords = {};

    // loop through words
    int32 count = 0;
    for (FString word: WordList) {
        if (word.Len() >= 4 && word.Len() <= 8 && IsIsogram(word)) {
            ValidWords.Emplace(word);
            count++;
        }
    }
    return ValidWords;
}


FBullCowCount UBullCowCartridge::GetBullCows(const FString& Input) const {
    // instantiate the struct
    FBullCowCount Count;

    // for every index Guess is the same as the index Hidden, BullCount ++
    // if not a bull was it a cow? if cow CowCount ++
    for (int32 GuessIndex = 0; GuessIndex < Input.Len(); GuessIndex++) {
        if (Input[GuessIndex] == HiddenWord[GuessIndex]) {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++) {
            if (Input[GuessIndex] == HiddenWord[HiddenIndex]) {
                Count.Cows++;
                break;
            } 
        }
    }
    return Count;
}