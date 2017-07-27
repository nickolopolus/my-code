
def main():
    
    cardNumber = getCardNumber()
    valid = validateCardNumber(cardNumber)
    
    if(valid):
        print(getCardType(cardNumber))
    else:
        print("INVALID")

def getCardNumber():
    
    cardNumber = ""

    while True:
        cardNumber = input("Enter card number: ")
        if validUserInput(cardNumber, len(cardNumber)):
            break

    return cardNumber

def validUserInput(userInput, length):
    
    valid = True

    for i in range(length):
        if not userInput[i].isdigit():
            valid = False
            break;
            
    return valid
    

#performs card number validation
# 1. Multiply every other digit by 2, starting with 2nd to last number
# 2. Add the digits of each result
# 3. Add sum to sum of remaining, non-multiplied numbers
# 4. If evenly divisible by 10, card is valid  
def validateCardNumber(cardNumber):
    
    cardIsValid = False
    sum = 0
    numDigits = len(cardNumber)
    
    # steps 1. and 2. above
    i = numDigits - 2
    while i > -1:
        
        num = int(cardNumber[i]) * 2
        
        if num > 9:
            sum += num - 10
            sum += 1
        else:
            sum += num
        
        i -= 2
    
    # step 3. above
    i = numDigits - 1
    while i > -1:
        sum += int(cardNumber[i])
        i -= 2
        
    # step 4. above
    if sum % 10 == 0:
        return True
    else:
        return False
    
def getCardType(cardNumber):
    
    cardType = "INVALID"
    
    numDigits = len(cardNumber)
    
    if (numDigits is 16 or numDigits is 13) and int(cardNumber[0]) is 4:
        cardType = "VISA"
    elif numDigits is 16 and cardNumber[0] is 5 and int(cardNumber[1]) in range(0,6):
        cardType = "MASTERCARD"
    elif numDigits is 15 and cardNumber[0] is 3 and int(cardNumber[1]) in [4,7]:
        cardType = "AMEX"
        
    return cardType
    
if __name__ == "__main__":
    main()