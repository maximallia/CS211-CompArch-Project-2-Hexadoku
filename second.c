#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

//int gridRow, gridCol; // input grid index from findEmpty()
// doesn't work with test5, for cannot recall original grid index during recursion

//bool checkFlag(int flag);//flag<1 thus OK!, else: failed! used in all checks
//bool checkRow(int** mat, int num);//true = OK for row!, false= FAIL
//bool checkCol(int** mat, int num); //true = OK for col!, false= FAIL
//bool checkGrid(int** mat, int num);//true = OK for grid! false=FAIL
bool checkAll(int** mat, int gridRow, int gridCol, int num);// compilation of 3 function above
bool checkValid(int** mat, int num);//complete all checks, return result to solveHex
bool findEmpty(int** mat, int* gridRow, int* gridCol);//sets address for row and col, true=empty found!,false=hex complete! inputs grid index
bool solveHex(int** mat);// recursion function, solves Hex
void printHex(int** mat);
void freeHex(int** mat);

bool findEmpty(int** mat, int* gridRow, int* gridCol){
  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 16; j++){
      if( mat[i][j] == -1 ){
	*gridRow = i;//used as input grid index
	*gridCol = j;
	return true;
      }
    }
  }
  return false;
}

/*bool checkFlag(int flag){
  if(flag < 1){
    return true;
  }
  return false;
  }*/ //cannot use return 1 and 0, for pointer comparison problem

bool checkAll(int** mat,int gridRow, int gridCol, int num){
  for(int col = 0; col < 16; col++){
    if(mat[gridRow][col] == num ){ //CHECK ROW
      return false; // duplicate found
    }
  }
  for(int row = 0; row < 16; row++){
    if(mat[row][gridCol] == num ){ // CHECK COL
      return false; // duplicate found
    }
  }
  //CHECK BOX
  //if row = 0, startrow = 0, thus 0-0 = 0
  //if row = 6, startrow = 4, thus 6-2 = 4
  //if row = 11, startrow = 8, thus 11-3 = 8
  //if row = 8, startrow = 8, thus 8-0 = 8

  int rowStart = gridRow - (gridRow%4); //solution to above
  int colStart = gridCol - (gridCol%4); //same as row

  //if row=3, startrow = 0, thus 0+3;
  //if row=4, startrow = 4, thus 4+0;
  //if row= 10, startrow = 8, thus 8+2;
  for(int i = 0; i < 4; i++){ //solution
    for(int j = 0; j < 4; j++){//same as above
      if( mat[rowStart+i][colStart + j] == num ){
	return false; //duplicate found
      }
    }
  }
  return true;// no duplicates found!!!!
}

bool checkHex(int** mat){ // to check for faulty hex problem
  int flag = 0; // used to count of appearances
  int flag2 = 0;
  int flag3 = 0;
  for(int num = 0; num < 16; num ++){
    for(int i = 0; i < 16; i++){
      for(int j = 0; j < 16; j++){
	if(mat[i][j] == num) flag++; //rowcheck
      }
      if(flag > 1 ) return false; //tried flag>1, but all become no-soln. do not understand why... 
      //fixed: due to wrong index of hex
      flag = 0;// restart in next row/col
    }
    for(int i = 0; i < 16; i++){
      for(int j = 0; j < 16; j++){
	if(mat[j][i] == num) flag2++; //colcheck
      }
      if(flag2 > 1) return false;
      flag2 = 0;
    }
    //BOX CHECK
    for(int i = 0; i <= 12; i+= 4){ //check each grid, need two loops for each grid and two loops for row and col, thus 4 loops
      for(int j = 0; j<= 12; j += 4){
	for(int k = 0; k < 4; k++){
	  for(int l = 0; l< 4; l++){
	    if(mat[k+i][l+j] == num) flag3++;
	  }
	}
	if( flag3 > 1) return false;//tried flag>1, but all become no-soln., reason unknown...
	//fixed: due to wrong index of hex
	flag3 = 0;// restart in next box
      }
    }
  }
  return true;
}

bool solveHex(int** mat){
  int gridRow, gridCol;
  if(!checkHex(mat)) return false;//check for no solution hex
  if(findEmpty(mat, &gridRow, &gridCol)){ // location address row and col from empty grid
    for(int inputNum = 0; inputNum < 16; inputNum++){
      if( checkAll(mat, gridRow, gridCol, inputNum)){// all passed, cont.
	mat[gridRow][gridCol] = inputNum;
	if(solveHex(mat)) return true;//number tried worked
	else{
	  mat[gridRow][gridCol] = -1;
	}
      }
    }
    return false; //all numbers tried none worked
  }
  else{
    return true; // full hex, no recursion needed
  }
}

void printHex(int** mat){
  for(int i = 0; i< 16; i++){
    for(int j = 0; j< 16; j++){
      printf("%X\t", mat[i][j]);
    }
    printf("\n");
  }
}

void freeHex(int** mat){
  for(int i = 0; i< 16; i++){
    free(mat[i]);
  }
  free(mat);
}

int main(int argc, char** argv){
  FILE* fp = fopen(argv[1], "r");
  if(fp == NULL){
    printf("ERROR");
    return 0;
  }
  int row = 16;
  int col = 16;
  char fileNum;
  int inputNum;
  int** mat = malloc(row * sizeof(int*));
  for(int i = 0; i< row; i++){
    mat[i] = malloc(col * sizeof(int));
    for(int j = 0; j < col; j++){
      fscanf(fp, "%c\t", &fileNum);
      if( fileNum != '-'){
        inputNum = fileNum - '0';
	if( inputNum > 9){//fileNum was A-F
	  int temp = inputNum - 7; //make A-F into 10-15
	  inputNum = temp;
	}
      }
      else{
	inputNum = -1;//make '-' into -1
      }
      mat[i][j] = inputNum;
      //printf("%d ", mat[i][j]);
    }
    //printf("\n");
  }
  //printHex(mat);//ERROR FOUND! PRINT ALL E's (fixed error in char conversion)
  
  if(solveHex(mat)){
    printHex(mat);
  }
  else{
    printf("no-solution");
  }
  freeHex(mat);
  fclose(fp);
  return 0;
}
