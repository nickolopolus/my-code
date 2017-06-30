//*************************************************************
// Title: credit.c
// Author: Nick Larson
// Date: 06/27/2017
// Description: A program that determines the type and validity
// of a user supplied credit card number. Compatible with all
// Visa, Mastercard and Amex card numbers.
//*************************************************************

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

string getCardNumber();
int validateCardNumber(string);
int validUserInput(string, int);
string getCardType(string);

int main(void)
{
  string cardNumber = getCardNumber();
  int validFlag = validateCardNumber(cardNumber);

  if(validFlag){
    printf("%s\n", getCardType(cardNumber));
  } else {
    printf("INVALID\n"); 
  }
  
  return 0;

}

//supplies card number from user
string getCardNumber()
{
  string cardNumber = "";

  do{

    printf("Enter card number: ");
    cardNumber = get_string();
  
  }while(validUserInput(cardNumber, strlen(cardNumber)) < 1);

  return cardNumber;
}

//validates string card number is numeric
int validUserInput(string userInput, int inputLength)
{
  int validFlag = 1;

  for(int i = 0; i < inputLength; i++)
  {
    if(!isdigit(userInput[i])){
      validFlag = 0;
    }
  }

  return validFlag;
}

//performs card number validation
// 1. Multiply every other digit by 2, starting with 2nd to last number
// 2. Add the digits of each result
// 3. Add sum to sum of remaining, non-multiplied numbers
// 4. If evenly divisible by 10, card is valid  
int validateCardNumber(string cardNumber)
{
  int cardIsValid = 0;
  int sum = 0;
  int numDigits = strlen(cardNumber);

  //steps 1. and 2. above
  for(int i = numDigits - 2; i > -1; i -= 2)
  {
    int num = (((int)(cardNumber[i] - '0') * 2));
    
    if(num > 9){
      sum += num - 10;
      sum++;
    } else {
      sum += num;
    }
    
  }

  //step 3. above
  for(int j = numDigits - 1; j > -1; j -= 2)
  {
    int num = ((int) (cardNumber[j] - '0'));
    sum += num;
  }

  //step 4. above
  if(sum % 10 == 0){
    cardIsValid = 1;
  }

  return cardIsValid;  
}

//given card number, determines card type
// Visa = 16 or 13 digits, begins with 4
// Mastercard = 16 digits, begins with 5
// Amex = 15 digits, begins with 3
string getCardType(string cardNumber)
{
  string cardType = "INVALID";
  int numDigits = strlen(cardNumber);

  if(((numDigits == 16) || (numDigits == 13)) && cardNumber[0] == '4'){

    cardType = "VISA";

  } else if((numDigits == 16) && cardNumber[0] == '5'){

    switch(cardNumber[1]){
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
        cardType = "MASTERCARD";
    }

  } else if((numDigits == 15) && cardNumber[0] == '3'){

    switch(cardNumber[1]){
      case '4':
      case '7':
        cardType = "AMEX";
    }

  } 

  return cardType;
}

