import node
import maze as mz
import BT
import score
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
    #point = score.Scoreboard("data/UID.csv", "team_3")
    #interf = interface.interface()
    # TODO : Initialize necessary variables
    read()
    print("Function End")
    
def read():
    # point = score.Scoreboard("data/UID.csv", "team_3")
    maze = mz.Maze("data/maze_8x6_2.csv")
    while True:
        # DirString = input()
        # if DirString == "END" :
        #     break
        # else:
        #     maze.setDirection(DirString)
        #     maze.updateStack()
        #     next_move = maze.nextDirection()
        #     if next_move != "" :
        #         print("Next move : {} (0 : North , 1 : East , 2 : South , 3 : West) \n".format(next_move))
        #     elif next_move == "" :
        #         break
        if bt.waiting():
            DirString = bt.SerialReadString() #North , East , South , West
            if DirString == "END" :
                break
            else:
                print(DirString)
                maze.setDirection(DirString)
                maze.updateStack()
                next_move = maze.nextDirection()
                if next_move != "" :
                    print("Next move : {} (0 : North , 1 : East , 2 : South , 3 : West) \n".format(next_move))
                elif next_move == "" :
                    break
                bt.SerialWrite(next_move+'\n')
            # UID = bt.SerialReadString()
            # print(UID)
            # point.add_UID(UID)
            # point.getCurrentScore()



def write():
    while True:
        msgWrite = input()
        if msgWrite == "exit": sys.exit()
        bt.SerialWrite(msgWrite + "\n")

if __name__ == '__main__':
    bt = BT.bluetooth("/dev/tty.038-SerialPort") 
    while not bt.is_open(): pass
    print("BT Connected!")

    # readThread = threading.Thread(target=read)
    # readThread.daemon = True
    # readThread.start()

    main()

