from ast import NodeTransformer
from pickle import TRUE
import turtle

from numpy import column_stack, row_stack

class MazeUI:
    def __init__(self , start , row , column):
        self.start = start
        self.rows = row
        self.columns = column
        self.nodeRecord = [start]
        self.window_height = 0
        self.window_width  = 0
        self.unit_height = 0
        self.unit_width  = 0
        self.circleRadius = 0
        self.index = start
        self.movementTable = []
        self.screen = turtle.Screen()
        self.pencil = turtle.Turtle()
        self.pencil.speed(10)
        self.pencil.shapesize(2 , 2 , 2)
        self.pencil.pensize(5)

    def drawMap(self , route):
        self.window_height = self.screen.window_height()
        self.window_width  = self.screen.window_width()
        self.unit_height = self.window_height / self.rows
        self.unit_width  = self.window_width / self.columns
        self.circleRadius = self.unit_width/4
        initialOffsetX = ( self.columns/2 - int(self.start/self.rows) - 1) * self.unit_width
        initialOffsetY = ( self.rows/2 - int(self.start%self.rows) - 1 ) * self.unit_height
        self.setTable()
        
        self.movePencil(self.unit_width/2 + initialOffsetX , (self.unit_height/2) + initialOffsetY)
        self.pencil.speed(3.5)
        self.drawRoute(route)
        self.screen.exitonclick()
    
    def drawRoute(self , route):
        for nextMove in route:
            self.pencil.dot(self.circleRadius)
            self.drawNextMove(int(nextMove))

    def checkNodeExist(self):
        for node in self.nodeRecord:
            if node == self.index:
                return True
        return False
            
    def drawNextMove(self , nextMove):
        self.setTurtle(self.movementTable[nextMove][0])
        self.pencil.goto(self.pencil.xcor() + self.movementTable[nextMove][1] ,
                         self.pencil.ycor() + self.movementTable[nextMove][2])
        # if nextMove == '0':
        #     # self.addRecord(-1)
        #     self.setTurtle(90)
        #     self.pencil.goto(self.pencil.xcor() , self.pencil.ycor() + self.unit_height )
        # elif nextMove == '1':
        #     # self.addRecord(-1*self.rows)
        #     self.setTurtle(0)
        #     self.pencil.goto(self.pencil.xcor() + self.unit_width , self.pencil.ycor())
        # elif nextMove == '2':
        #     # self.addRecord(1)
        #     self.setTurtle(270)
        #     self.pencil.goto(self.pencil.xcor() , self.pencil.ycor() - self.unit_height )            
        # elif nextMove == '3':
        #     # self.addRecord(self.rows)
        #     self.setTurtle(180)
        #     self.pencil.goto(self.pencil.xcor() - self.unit_width , self.pencil.ycor())

        self.pencil.hideturtle()

    def setTable(self):
        self.movementTable.append([90 , 0 , self.unit_height][:])
        self.movementTable.append([0 , self.unit_width , 0][:])
        self.movementTable.append([270 , 0 , -self.unit_height][:])
        self.movementTable.append([180 , -self.unit_width , 0][:])

    def movePencil(self , offsetX , offsetY):
        self.pencil.hideturtle()
        self.pencil.penup()
        self.pencil.goto(self.pencil.xcor() + offsetX , self.pencil.ycor() + offsetY)
        self.pencil.pendown()
        # print(self.pencil.xcor() , self.pencil.ycor())

    def addRecord(self , deltaNode) :
        self.nodeRecord.append(self.index+deltaNode)
        self.index += deltaNode

    def setTurtle(self , angle):
        self.pencil.showturtle()
        self.pencil.fillcolor("red")
        self.pencil.setheading(angle)
    
if __name__ == '__main__':
    mazeUI = MazeUI(0 , 3 , 4)
    mazeUI.drawMap("3332211100")
