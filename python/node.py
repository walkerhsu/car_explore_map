import math
from cmath import nan
from enum import IntEnum

# You can get the enumeration based on integer value, or make comparison
# ex: d = Direction(0), then d would be Direction.NORTH
# ex: print(Direction.SOUTH == 0) should return False
class Direction(IntEnum):
    NORTH = 0
    EAST = 1
    SOUTH  = 2
    WEST  = 3

# Construct class Node and its member functions
# You may add more member functions to meet your needs
class Node:
    def __init__(self , rows , columns ,  index=0):
        self.row = rows
        self.column = columns
        self.index = index
        self.Successors = []

    def setSuccessor(self , i , avail):
        # print(avail)
        # print(avail == '1')
        if avail=='1' : 
            nextIndex = self.setNextIndex(i)
            print(nextIndex)
            if nextIndex<0 or nextIndex>=(self.column*self.row):
                self.Successors.append([nan , nan][:])
                # print("For Node {}, successor {} is out of bounds.".format(self.index , nextIndex) )
            else:
                self.Successors.append([nextIndex , Direction(i)][:])
                # print("For Node {}, a successor {} is set.".format(self.index, self.Successors[-1]))
        elif avail=='0':
            pass
        else:
            print("error : Avail not bool!" )  
        return 



    def setNextIndex(self , i):
        if i==0:
            return self.index-1
        if i==1:
            return self.index-self.row
        if i==2:
            return self.index+1
        if i==3:
            return self.index+self.row


    def getSuccessors(self):
        return self.Successors

    def getSuccessorNumbers(self):
        return len(self.Successors)

    def getIndex(self):
        return self.index

    def checkIfEnd(self):
        return (self.getSuccessorNumbers() == 1)

    
    def isSuccessor(self, nd):
        for succ in self.Successors:
            if succ[0] == nd: 
                return True
        return False

