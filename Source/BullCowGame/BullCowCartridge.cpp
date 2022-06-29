// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWords.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    FBullCowCounts Count;
    Isograms = GetValidWords(Words); 
    SetupGame();


}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
        
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else 
    {
        ProcessGuess(Input);
    } 

}   

void UBullCowCartridge::SetupGame()
{
    
    PrintLine(TEXT("Hello There! Welcome to our Bulls and Cows game"));
    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num() - 1)];
    Lives = HiddenWord.Len()*2;
    bGameOver = false; 
    PrintLine(TEXT("You have %i lives"), Lives);
    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("Type your guess and Please press enter to continue")); //Prompt player to play
      //debug line
    PrintLine((TEXT("The Hidden word is: %s.")), *HiddenWord);
     
    
}
void UBullCowCartridge::EndGame()
{   
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("Good! You win!"));
        EndGame();
        return;
    }

    //check if right # of characters
    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The Hidden word is %i letters long"), HiddenWord.Len());
        PrintLine(TEXT("Try again! \nYou have %i Lives"), Lives);
        return;
    }


    //check if isogram
    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters,guess again"));
        return;
    }

    //remove the life
    PrintLine(TEXT("You have lost a life"));
    --Lives;

    // check if the left lives are more than 0
    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("Game Over!"));
        PrintLine(TEXT("\nThe hidden word was %s:"), *HiddenWord);
        EndGame();
        return;
    } 

    // show the player the Bulls and Cows

    FBullCowCounts Score = GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const 
{
    for (int32 i = 0; i < Word.Len(); i++)
    {
        for(int32 j = i+1; j < Word.Len(); j++)
        {
            if(Word[i] == Word[j])
            {
                return false;
            }
        }
    }
    
    return true;   
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if(Word.Len() >= 4 && Word.Len() <= 8)
        {
            if(IsIsogram(Word))
            {
                ValidWords.Emplace(Word);
            }
        }
    }
 
    return ValidWords;
}

FBullCowCounts UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCounts Counts;

    //if the index of the guess is same as the index of hidden word BullCount++
    //if not a bull, was it a cow? if yes cowcount ++

    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if(Guess[i] == HiddenWord[i])
        {
            Counts.Bulls++;
            continue;
        }

        for(int32 j = 0; j < HiddenWord.Len(); j++ )
        {
            if(Guess[i] == HiddenWord[j])
            {
                Counts.Cows++;
                break;
            }
        }
        //hello
    }

    return Counts;
}
