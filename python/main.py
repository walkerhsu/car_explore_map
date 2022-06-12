import node

import maze as mz
import BT
import score
import mazeUI
import interface
import time
import serial
import threading

import numpy as np
import pandas
import time
import sys
import os


def main():
    # TODO : Initialize necessary variables
    startNode , rows , columns = 10 , 3 , 4
    total_route = ""
    # startNode , rows , columns = setMap()
    maze = mz.Maze(startNode , rows , columns)
    total_route = read(maze)
    print("\ncar_visited_route : {}".format(total_route))
    UI = mazeUI.MazeUI(startNode , rows , columns)
    UI.drawMap(total_route)
    startNode = maze.startNode
    loop = int(input("search times : "))
    for i in range(loop):
        endNode , route = maze.BFStwoNodes(startNode)
        startNode = endNode
        bt.SerialWrite(route)

    
def read(maze):
    # point = score.Scoreboard("data/UID.csv", "team_3")
    total_route = ""
    print("Go!!")
    bt.SerialWrite("Go!!"+'\n')
    while True:
        # DirString = input()
        # if DirString == "END" :
        #     return total_route
        # else:
        #     maze.setDirection(DirString)
        #     maze.updateStack()
        #     next_move = maze.nextDirection()
        #     if next_move != "" :
        #         print("Next move : {} (0 : North , 1 : East , 2 : South , 3 : West) \n".format(next_move))
        #         total_route += next_move
        #     elif next_move == "" :
        #         print("NextMovement error")
        #         return total_route
        #     if maze.END == 1 :
        #         print("End of tracking !! ")
        #         return total_route
        if bt.waiting():
            DString = bt.SerialReadString() #North , East , South , West
            DirString = ""
            DirString = DString[0:]
            
            # for i in range(len(DirString)):
            #     print("DirString[{}] = {}.".format(i , DirString[i]))
            if DirString == "END" :
                return total_route
            elif DirString == "Block":
                print("fuck")
            else:
                print(DirString)
                maze.setDirection(DirString)
                maze.updateStack()
                next_move = maze.nextDirection()
                if next_move != "" :
                    print("Next move : {} (0 : North , 1 : East , 2 : South , 3 : West) \n".format(next_move))
                    total_route += next_move
                elif next_move == "" :
                    print("NextMovement error")
                    break
                if maze.END == 1 :
                        print("End of tracking !! ")
                        bt.SerialWrite(next_move + "\n")
                        return total_route
            bt.SerialWrite(next_move)
            # bt.SerialWrite(next_move + "\n")
def setMap():
    startNode = int(input("Please input the startNode : "))
    rows = int(input("Please input the rows : "))
    columns = int(input("Please input the columns : "))
    return startNode , rows , columns
# def write():
#     while True:
#         msgWrite = input()
#         if msgWrite == "exit": sys.exit()
#         bt.SerialWrite(msgWrite + "\n")

if __name__ == '__main__':
    bt = BT.bluetooth("/dev/tty.035-SerialPort") 
    while not bt.is_open(): pass
    print("BT Connected!")

    # readThread = threading.Thread(target=read)
    # readThread.daemon = True
    # readThread.start()

    main()

