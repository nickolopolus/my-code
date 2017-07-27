import sys

def main(argv):
    
    #verify usage
    if len(sys.argv) < 2:
        print("Usage: /vigenere.py key")
        exit(1)
    
    printCipher(input("plaintext: "), argv[1])
    


def printCipher(plaintext, keyword):
    
    textLen = len(plaintext)
    keyLen = len(keyword)
    keyIndex = 0
    
    #solve for wrapping around key
    while keyLen < textLen:
        keyword *= 2
        keyLen = len(keyword)
    
    print("ciphertext: ", end="")
    
    for i in range(textLen):
        
        if plaintext[i].isalpha():
            
            #get numeric value of key
            key = ord(keyword[keyIndex].upper()) - 65
            
            #check if wrapping to beginning of alphabet is required
            if chr(ord(plaintext[i]) + key).upper() > 'Z':
                print(chr(ord(plaintext[i]) + key - 26), end="")
            else:
                print(chr(ord(plaintext[i]) + key), end="")
            
            keyIndex += 1
        
        else:
            print(plaintext[i], end="")
            
    print()
    
if __name__ == "__main__":
    main(sys.argv)