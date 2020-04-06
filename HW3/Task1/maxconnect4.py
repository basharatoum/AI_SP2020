#!/usr/bin/env python

# Written by Chris Conly based on C++
# code provided by Dr. Vassilis Athitsos
# Written to be Python 2.4 compatible for omega

import sys
from MaxConnect4Game import *

def oneMoveGame(currentGame):
    if currentGame.pieceCount == 42:    # Is the board full already?
        print 'BOARD FULL\n\nGame Over!\n'
        sys.exit(0)

    currentGame.aiPlay() 

    print 'Game state after move:'
    currentGame.printGameBoard()

    currentGame.countScore()
    print('Score: Player 1 = %d, Player 2 = %d\n' % (currentGame.player1Score, currentGame.player2Score))

    currentGame.printGameBoardToFile()
    currentGame.gameFile.close()


def interactiveGame(currentGame):
    if currentGame.pieceCount == 42:    # Is the board full already?
        currentGame.countScore()
        if currentGame.Computer == 1:
            print ('Scores:   Computer - %d      Player - %d \n' % (currentGame.player1Score,currentGame.player2Score))
        else:
            print ('Scores:   Computer - %d      Player - %d \n' % (currentGame.player2Score,currentGame.player1Score))
        print 'BOARD FULL\n\nGame Over!\n'
        sys.exit(0)
    
    
    if currentGame.currentTurn == currentGame.Computer:
        currentGame.aiPlay() 
        print 'Game state after move:'
        currentGame.printGameBoard()
        computerFile = 'computer.txt'
        try:
            computerFile = open(computerFile, 'w')
        except:
            sys.exit('Error opening output file.')
        currentGame.printGameBoardToFileS(computerFile)

    else:
        playerMove = input ('Player turn, choose a column from 1-7 :  ')

        print len(currentGame.gameBoard[0])
        if (not(playerMove >=1 and playerMove<=7)):
            print "Invalid input"
            interactiveGame(currentGame)
        if currentGame.gameBoard[0][playerMove-1] !=0:
            print "Invalid input"
            interactiveGame(currentGame)
        else:
            currentGame.playPiece(playerMove-1)
            currentGame.pieceCount +=1
            currentGame.currentTurn = currentGame.currentTurn%2 + 1
            currentGame.printGameBoard()
            humanFile = 'human.txt'
            try:
                humanFile = open(humanFile, 'w')
            except:
                sys.exit('Error opening output file.')
            currentGame.printGameBoardToFileS(humanFile)

    interactiveGame(currentGame)
    


def main(argv):
    # Make sure we have enough command-line arguments
    if len(argv) != 5:
        print 'Four command-line arguments are needed:'
        print('Usage: %s interactive [input_file] [computer-next/human-next] [depth]' % argv[0])
        print('or: %s one-move [input_file] [output_file] [depth]' % argv[0])
        sys.exit(2)

    game_mode, inFile = argv[1:3]

    if not game_mode == 'interactive' and not game_mode == 'one-move':
        print('%s is an unrecognized game mode' % game_mode)
        sys.exit(2)

    currentGame = maxConnect4Game() # Create a game

    # Try to open the input file
    try:
        currentGame.gameFile = open(inFile, 'r')
    except IOError:
        sys.exit("\nError opening input file.\nCheck file name.\n")

    # Read the initial game state from the file and save in a 2D list
    file_lines = currentGame.gameFile.readlines()
    currentGame.gameBoard = [[int(char) for char in line[0:7]] for line in file_lines[0:-1]]
    currentGame.currentTurn = int(file_lines[-1][0])
    currentGame.gameFile.close()
    
    # Get to know who is who
    if argv[3] == 'computer-next':
        currentGame.Computer = currentGame.currentTurn
        currentGame.Person = currentGame.currentTurn%2 + 1
    elif argv[3] == 'human-next':
        currentGame.Person = currentGame.currentTurn
        currentGame.Computer = currentGame.currentTurn%2 + 1
    else:
        currentGame.Computer = currentGame.currentTurn
        currentGame.Person = currentGame.currentTurn%2 + 1
    # Get the depth of search
    currentGame.depthLimit = int(argv[4])

    print '\nMaxConnect-4 game\n'
    print 'Game state before move:'
    currentGame.printGameBoard()

    # Update a few game variables based on initial state and print the score
    currentGame.checkPieceCount()
    currentGame.countScore()
    print('Score: Player 1 = %d, Player 2 = %d\n' % (currentGame.player1Score, currentGame.player2Score))

    if game_mode == 'interactive':
        interactiveGame(currentGame) # Be sure to pass whatever else you need from the command line
    else: # game_mode == 'one-move'
        # Set up the output file
        outFile = argv[3]
        try:
            currentGame.gameFile = open(outFile, 'w')
        except:
            sys.exit('Error opening output file.')
        oneMoveGame(currentGame) # Be sure to pass any other arguments from the command line you might need.


if __name__ == '__main__':
    main(sys.argv)



