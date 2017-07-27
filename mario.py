#program that prints a double sided pyramid, like that in super mario

def main():
    printPyramids(getNumFromUser(),2);

def getNumFromUser():
    while True:
        n = input("Enter a non-negative integer < 24: ")
        if (int(n) < 24) and (int(n) > 0):
            break
    return int(n)

def printPyramids(height,gap):
    space = ' '
    brick = '#'
    initialSpace = height - 1
    initialBrick = 1
    
    for i in range(height):
        printSection(space, initialSpace)
        printSection(brick, initialBrick)
        printSection(space, gap)
        printSection(brick, initialBrick)
        print()
        initialSpace -= 1
        initialBrick += 1

def printSection(content,duration):
    for i in range(duration):
        print(content, end='')
        
if __name__ == '__main__':
    main()