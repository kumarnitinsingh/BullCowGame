// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);

    SetupGame();

    
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    
   
   
   if(bGameOver)
   {
       ClearScreen();
       SetupGame();
   }
   else//else checking player guess
   {
    ProcessGuess(Input);
   }
    
}

void UBullCowCartridge::SetupGame()
{

     //welcoming the player
     PrintLine(TEXT("Welcome to Bull Cows game!\nPowered by Nitin Kumar Singh"));
     HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num()-1)];
     Lives = HiddenWord.Len() ;
     bGameOver = false;
     PrintLine(TEXT("Guess the %i letter word!"),HiddenWord.Len());
     PrintLine(TEXT("You have %i lives "),Lives);
     PrintLine(TEXT("Type in your guess and then\nPress Enter to continue..."));
     PrintLine(TEXT("Hint:The word starts with %c and ends with %c"),HiddenWord[0],HiddenWord[HiddenWord.Len()-1]);
     //PrintLine(FString::Printf(TEXT("The HiddenWord is : %s"),*HiddenWord));//debug line
     //prompt player to guess
    
     
}

void UBullCowCartridge::EndGame()
{   
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
      
    if(Guess== HiddenWord)
    {
        PrintLine(TEXT("Congratulations You Win!"));
        EndGame();
        return;
    }    
    //check right no. of characters
    if(Guess.Len() != HiddenWord.Len())
        {
            PrintLine(TEXT("The hidden word is %i letters long"),HiddenWord.Len());
            PrintLine(TEXT("Sorry, try guessing again.\nYou have %i lives remaining"),Lives); 
            return;
        }

        //check if isogram
    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again!"));
        return;
    }

    // remove life
         --Lives;
        PrintLine(TEXT("Oops You lost a life!"));
          if(Lives<=0)
        {  ClearScreen();
           PrintLine(TEXT("You have no lives left"));
           PrintLine(TEXT("The hidden word was: %s"),*HiddenWord);
           EndGame();
           return;
        }
        
    //show players bulls and cows
   
   FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"),Score.Bulls,Score.Cows);
    PrintLine(TEXT("Guess again,you have %i lives left"),Lives);
        
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    //for each letter start at element [0] 
    //compare against the next letter until we reach[word.Len()-1]
    //if any letter are same return false
    for(int32 Index=0 ; Index < Word.Len(); Index++)
    {
        for(int32 Comparison = Index+1; Comparison < Word.Len(); Comparison++)
        {
            if(Word[Index] == Word[Comparison])
            {
                return false;
            }
        }

    }
    return true;
}

TArray<FString>UBullCowCartridge::GetValidWords(const TArray<FString>& WordList)const
{
     TArray<FString> ValidWords;
    for(FString Word : WordList)
    {
        if(Word.Len()>=4 && Word.Len() <= 8 && IsIsogram(Word))
        {  
              ValidWords.Emplace(Word);
        }    
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess)const
{
  FBullCowCount Count;
   //for every the index Guess is same as Hidden do Bullcount++
   //if not a bull was it a cow?if yes do cowcount++
   for(int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
   {
       if(Guess[GuessIndex] == HiddenWord[GuessIndex])
       {
           Count.Bulls++;
           continue;
       }
      for(int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
      {
          if(Guess[GuessIndex] == HiddenWord[HiddenIndex] )
          {
              Count.Cows++;
              break;
          }
      }
   }
   return Count;
}