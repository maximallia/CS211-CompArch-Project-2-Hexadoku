# CS214-Project-2-Hexadoku

Summary:

Part 1: Hexadoku (50 points)

A Hexadoku is defined as solved when all of its cells in the 16x16 grid are filled with numbers with each cell satisfying the constraints above. A Hexadoku is defined as unsolvable when there is no configuration where all the cells can be filled without breaking the constraints. A Hexadoku will start partially completed with some numbers placed in the 16x16 grid. This will allow the solver to determine where to place new numbers in order to find the solution for the given Hexadoku. For this part, we will only be looking at Hexadoku grids with one unique solution.

Part 2: Extra Credit (25 points)

If a solution is found, then you are done. Otherwise, if no moves can be taken and the board has not yet been filled, then you must backtrack to the state where you guessed the value of cell A and either guess a new value for cell A or perhaps choose a new cell altogether. This is a simple backtracking algorithm. There are many algorithms to solve these complex Hexadoku. You can implement any algorithm that produces a solution.

Part 3: One-Shot Learning (50 points)

Given a house, we know the attributes of the house (i.e., x1, x2, x3, x4). However, we don’t know the weights for these attributes: w0, w1, w2, w3 and w4. The goal of the machine learning algorithm in our context is to learn the weights for the attributes of the house from lots of training data. Your main task in this part to implement a program to read the training data and learn the weights for each of the attributes. 
