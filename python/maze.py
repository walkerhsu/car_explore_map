import node 
import numpy as np
import csv
import pandas
import math
import time

from enum import IntEnum

# You can get the enumeration based on integer value, or make comparison
# ex: d = Direction(0), then d would be Direction.NORTH
# ex: print(Direction.SOUTH == 0) should return False
class Direction(IntEnum):
    NORTH = 0
    EAST = 1
    SOUTH  = 2
    WEST  = 3

class Maze:
    def __init__(self , start , row , column):
        # TODO : read file and implement a data structure you like
		# For example, when parsing raw_data, you may create several Node objects.  
		# Then you can store these objects into self.nodes.  
		# Finally, add to nd_dictionary by {key(index): value(corresponding node)}
        #self.raw_data = pandas.read_csv(filepath).values #returned as two-dimensional data structure with labeled axes.
        self.rows = row
        self.columns = column
        self.startNode = start
        self.currentIndex = self.startNode
        self.nd_dict = dict()
        self.stack = []
        self.record = []
        self.successors = []
        self.nextChoice = 0
        self.END = 0
        self.setNode()

    def setNode(self):
        for i in range (self.columns):
            for j in range (self.rows):
                index = (i*self.rows)+j
                tmpNode = node.Node(self.rows , self.columns ,index)
                self.nd_dict[index] = tmpNode


    def setDirection(self , DirString):
        self.record.append(self.currentIndex)
        self.nextChoice=0
        for i in range(len(DirString)):
            self.nd_dict[self.currentIndex].setSuccessor(i , DirString[i])
        return

    def updateStack(self):
        curNode = self.nd_dict[self.currentIndex]
        curNodeIndex = curNode.getIndex()
        tmpSuccessor = curNode.getSuccessors()
        for suc in tmpSuccessor:
            if self.notInRecord(suc[0]):
                self.stack.append([curNodeIndex , suc[0] , suc[1]][:])
                self.nextChoice += 1
        # print(self.stack)

    def nextDirection(self):
        nextMovement = ""
        if self.nextChoice == 0:
            return self.back(nextMovement)

        elif self.nextChoice >= 1 and self.nextChoice<=3:
            nextMovement += self.nextMovement()
            return nextMovement
        else:
            print("Successors number incorrect !! ")
            return ""

    def back(self , nextMovement ):
        while(self.checkRepeat()):
            self.stack.pop(-1)
        nextMovement += self.backward(nextMovement)
        
        if(len(self.stack)!=0):
            nextMovement += self.nextMovement()
        return nextMovement

    def backward(self , nextMovement):
        if len(self.stack)==0:
            print("end of tracking , back to startNode...")
            nextMovement = self.BFS(self.currentIndex , self.startNode)
            self.currentIndex = self.startNode
            self.END = 1
            for i in range(self.rows*self.columns):
                print("Node {} , successors = {}.".format(i , self.nd_dict[i].getSuccessors()))
            return nextMovement
            
        elif len(self.stack)!=0:
            nextMovement += self.backToCrossing(self.currentIndex)
            # print(nextMovement)
            return nextMovement

    def checkRepeat(self):
        if len(self.stack) == 0:
            return False
        elif self.notInRecord(self.stack[-1][1]):
            return False
        else :
            return True
            

    def nextMovement(self):
        nextMovement = self.stack.pop(-1)
        # print(nextMovement)
        movement = self.DirectionToStr(nextMovement[2])
        self.setCurrentIndex(nextMovement[1])
        return movement

    def backToCrossing(self , currentIndex):
        print("last crossing : {}".format(self.stack[-1][0]))
        totalMovement = self.BFS(currentIndex , self.stack[-1][0])
        self.currentIndex = self.stack[-1][0]
        return totalMovement

        
    def DirReverse(self , dir):
        if dir >=4 or dir<0 :
            return 4
        else :
            return (dir+2)%4

    def getNextIndex(self , i):
        index = self.currentIndex
        if i==0:
            index -= 1
        if i==1:
            index -= self.rows
        if i==2:
            index += 1
        if i==3:
            index += self.rows
        return index

    def DirectionToStr(self , dir):
        for i in range(4):
            if dir==i:
                return str(i)

    def setCurrentIndex(self , nextIndex):
        self.currentIndex = nextIndex
        print("next index : {}".format(self.currentIndex))
    
    def notInRecord(self , nextIndex):
        for i in self.record:
            if i==nextIndex:
                return False
        return True

    def indexInMap(self , index):
        if index>=0 and index<(self.rows * self.columns):
            return True
        else:
            return False

    def BFS(self, nd_from, nd_to):
        print(nd_from , nd_to)
        cnt=0
        queue = [[nd_from,0]]
        nodeList = []
        DirectionList = []
        parentList = []
        records = [nd_from]
        
        while len(queue)!=0:
            tmplist = queue.pop(0)[:]
            index = tmplist[0]
            tmpNode = self.nd_dict[index]
            print(tmplist)
             
            for suc in tmpNode.getSuccessors():
                inRecord=False
                for record in records :
                    if record == suc[0]:
                        inRecord=True
                        break
                if not inRecord :
                    if suc[0] == nd_to:
                        path = self.DirectionToStr(suc[1])
                        while index!=nd_from:
                            for j in range(cnt):
                                if nodeList[j]==index:
                                    index = parentList[j]
                                    path += DirectionList[j]
                                    cnt = j
                                    break
                        return path[::-1]
                    else :
                        queue.append(suc)
                        nodeList.append(suc[0])
                        DirectionList.append(self.DirectionToStr(suc[1]))
                        parentList.append(index)
                        records.append(suc[0])
                        cnt+=1
