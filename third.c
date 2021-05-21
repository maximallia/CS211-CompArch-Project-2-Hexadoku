#include<stdio.h>
#include<stdlib.h>
//all ** for returning matrix
void printMat(double** mat, int row, int col);
double** transpose(double** mat, int row, int col);// create nxm tMat from mxn mat(swtich r with c)
double** multiply(double** mat, int row, int col, double** tMat, int tRow, int tCol); //multiply mat with tMat to create nxn matrix
double** inverse(double** mat, int row, int col);//need nxn matrix to inverse
double** augment(double** mat, int row, int col);//create the I identity of new mat from multiply, used in inverse
void freeMat(double** mat, int row);


void printMat(double** mat, int row, int col){
  for(int i = 0; i < row; i++){
    for(int j = 0; j < col; j++){
      printf("%lf ", mat[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

double** transpose(double** mat, int row, int col){
  //int tRow = col;
  //int tCol = row;
  if(mat == NULL){
    printf("error");
    return NULL;
  }
  double** tMat = malloc(col * sizeof(double*)) ;//tRow=col (create tRow)  
  for(int i = 0; i < col; i++){
    tMat[i] = malloc( row * sizeof(double)); //tCol = row (create col in each tRow)
    for( int j = 0; j < row; j++){
      tMat[i][j] = mat[j][i]; // tMat = switched row and col of mat
    }
  }
  return tMat;
}

double** multiply(double** mat, int row, int col, double** tMat, int row1, int col1){
  //mMat[row][col1]
  //if( col != tRow) return NULL; created from transpose, can't be NULL
  /*double** mMat = malloc(row * sizeof(double*)); //mMat's row created, a rowxcol1 matrix
  for(int i = 0; i < row; i++){
    mMat[i] = malloc(col1 * sizeof(double)); // mMat's col created
    for(int j = 0; j < col1; j++){
      for(int k = 0; k < row1; k++){
	mMat[i][j]+= mat[i][k] * tMat[k][j];
      }
    }
    }*/ ///CAUSED LOSING PRECISION DURING MULTIPLICATION, do not know why

  double** mMat = malloc(row * sizeof(double*));
  for(int i = 0; i < row; i++){
    mMat[i] = malloc(col1 * sizeof(double));
    for(int j = 0; j < col1; j++){
      mMat[i][j] = 0;
    }
  }
  for( int i = 0 ; i< row; i++){
    for(int j = 0; j < col1; j++){
      for(int  k = 0; k < row1; k++){
	mMat[i][j] += mat[i][k] * tMat[k][j];// this way, no lose of precision
      }
    }
  }

  return mMat;
}

double** augment(double** mMat, int row, int col){

  double** augMat = malloc( row* sizeof(double*)); // augMat has same row as mMAt
  for(int i = 0; i < row; i++){
    augMat[i] = malloc(2 * col* sizeof(double));// augMat has twice the col for identity I
    for(int j = 0; j < col * 2; j++){
      if( j >= col){// reach end mMAt col
	if( i == (j-col)){ // j-col would equal to i, forming diagonal 1
	  augMat[i][j] = 1;
	}
	else{
	  augMat[i][j] = 0;// input 0 into identity mat
	}
      }
      else{
	augMat[i][j] = mMat[i][j];//have yet reach identity block col
      }
    }
  }
  return augMat;
}


double** inverse(double** mMat, int row, int col){ // change augMat = mMat
  double constant;
  int augCol = 2 * col;

  //triangle, thus j<=i creating lower triangle
  for(int i = 0; i < row; i++){
    for( int j = 0; j <= i; j++){ // stop at before reaching 
      if( i == j ){//need the diag. 1's they are pivot
	constant = mMat[i][j];
	for(int nextCol = j; nextCol < augCol; nextCol++){
	  mMat[i][nextCol] /= constant; // creating pivot, and changing mat and id
	  //idMat[i][nextCol] /= constant;
	}
      }
      else if(mMat[i][j] != 0 ){ // col below pivot != 0, i!=j
	// mat[0][0] [1][1] [2][2] is pivot, use j as pivotRow in loop
	constant = mMat[i][j];  //since pivot is 1, * pivot row with constant
	for(int nextCol = 0; nextCol < augCol; nextCol++){//row (-) c*pivotRow
	  mMat[i][nextCol] -= constant * mMat[j][nextCol];//make pivot 0 and rest (-) cRow
	  //idMat[i][nextCol] -= constant * idMat[j][nextCol];
	}
      }
    }
  }

  //upper triangle, pivot line formed, thus j=col-1, i=row-1, i&j--
  for(int i = row - 1; i >= 0; i--){
    for(int j = col - 1; j >= i ; j--){
      if(i!=j && mMat[i][j] !=0 ){ // i=j is already 1
	constant = mMat[i][j]; //because pivot is 1, * pivot row with constant
	for(int nextCol = 0; nextCol < augCol; nextCol++){
	  mMat[i][nextCol] -= constant * mMat[j][nextCol];
	  //idMat[i][nextCol] -= constant * idMat[j][nextCol];
	}
      }
    }
  }
  //elimination complete, have inverse in right block, while left is identity I
  //input inverse mat into iMat

  //id matrix
  double** iMat = malloc(row * sizeof(double*));                                                           
  for(int i = 0; i < row; i++){                                                                         
    iMat[i] = malloc(col * sizeof(double));                                                               
    for(int j = 0; j < col; j++){
      iMat[i][j] = mMat[i][j+ col];
    }
  }
  return iMat;
}


void freeMat(double** mat, int row) {
  for(int i=0;i < row ;++i){
    free(mat[i]);
  }
  free(mat);
}

int main(int argc, char** argv){
  //need make two files into matrix to input val into xMat and yMat
  //double** trainMat, testMat, xMat, yMat, wMat;
  //xMat is train, yMat house $, wMat result from inverse*transpose
  FILE* fp = fopen(argv[1], "r");

  if(fp == NULL){// || fp2 == NULL){
    printf("ERROR\n");
    return 0;
  }

  //create matrix of trainfile
  int row;
  int col;//need to remember: input 1, last val is $$$
  fscanf(fp, "%d\n", &col);
  col++; //need one extra col for $$$
  fscanf(fp, "%d\n", &row);
  double** trainMat = malloc(row * sizeof(double*));
  for(int i = 0; i < row; i++){
    trainMat[i] = malloc(col * sizeof(double));
    for(int j = 0; j < col; j++){
      fscanf(fp, "%lf,", &trainMat[i][j]);
    }
  }
  /*printf("trainMat\n");
  printMat(trainMat, row, col); //check READ
  printf("\n");*/

  //create xMat, remember [i][0] is 1
  double** xMat = malloc(row * sizeof(double*));
  for(int i = 0; i < row; i++){
    xMat[i] = malloc(col * sizeof(double));
    xMat[i][0] = 1;
    for(int j = 1; j < col; j++){// no need to change col, for +1 and no $$$
      xMat[i][j] = trainMat[i][j - 1]; // trainMat doesnt have +1
    }
  }
  /*printf("xMat\n");
  printMat(xMat, row, col); //check xMat input
  printf("\n");*/


  //create yMat, only last col of trainMat
  double** yMat = malloc(row * sizeof(double*));
  for(int i = 0; i < row; i++){
    yMat[i] = malloc(sizeof(double)); // only one col
    yMat[i][0] = trainMat[i][col - 1];// input $$$
  }
  /*printf("yMat\n");
  printMat(yMat, row, 1); //check yMAt input
  printf("\n");*/

  //freeMat(trainMat, row);

  FILE* fp2 = fopen(argv[2], "r");
  //create testMat
  int rowTest;
  fscanf(fp2, "%d\n", &rowTest);
  int colTest = col - 1; // because testfile doesnt have $$$, using x to predict $$$                                              

  double** testMat = malloc(rowTest * sizeof(double*));
  for(int i = 0; i < rowTest; i++){
    testMat[i] = malloc(colTest * sizeof(double));
    for(int j = 0; j < colTest; j++){
      fscanf(fp2, "%lf,", &testMat[i][j]);
    }
  }

  /*printf("testMat\n");
  printMat(testMat, rowTest, colTest); // check READ                                                        
  printf("\n");*/

  //W = (Xt*X)^-1 * Xt * Y
  //printf("%d\t%d\n", row, col);
  //printf("\n");
  //Xt
  double** tMat = transpose(xMat, row, col); // mxn -> nXm
  /*printf("tMat\n");
  printMat(tMat, col, row);
  printf("\n");*/

  //Xt*X
  double** mMat = multiply(tMat, col, row, xMat, row, col);// mxn X nxm = mxm; from here col=row
  /*printf("mMAt\n");
  printMat(mMat, col, col);
  printf("\n");*/

  //(Xt*X)^-1
  double** augMat = augment(mMat, col, col); //mx(2*m); function auto increase col*2
  /*printf("augMat\n");
  printMat(augMat, col, col*2);
  printf("\n");*/

  double** iMat = inverse(augMat, col, col);
  /*printf("iMat\n");
  printMat(iMat, col, col);
  printf("\n");*/

  //(Xt*X)^-1 * Xt
  //double** iXMat = multiply(iMat, row, row, tMat, col, row); FAIL!!! 
  //in order for final wMat multi to work need tMat*xMat, not xMat*tMat
  double** iXMat = multiply(iMat, col, col, tMat, col, row);
  /*printf("iXMat\n");
  printMat(iXMat, col, row);
  printf("\n");*/

  //W = (Xt*X)^-1 * Xt * Y
  double** wMat = multiply(iXMat, col, row, yMat, row, 1);
  /*printf("wMat\n");
  printMat(wMat, col , 1);
  printf("\n");*/


  //printf("\n");

  //  wMat = W
  // X*W = Y
  //double** pPrice = multiply() doesnt work, testMat row != wMat col
  //cannot multiply through function
  //PROBLEM: testfile only has x col, but wMat has x+1 row due to 1
  //SOLN.: wMat[0][0] + w1x0 + w2x1+ ... + wNx(N-1)

  double pPrice = 0;// output is in int
  //printf("%d\t%d\n", rowTest, colTest); // check row,col

  for(int i = 0; i < rowTest; i++){ //number of rows from testMat
    for(int j = 0; j < colTest; j++){ // testCol == wMat rows
      if( j == 0) pPrice += wMat[0][0];//SOLUTION!!!
      pPrice += wMat[j+1][0] * testMat[i][j];
    }
    printf("%0.0lf\n", pPrice);
    pPrice = 0;
  }
  
  //free memory, need freeMat function
  freeMat(wMat, col);
  freeMat(iXMat, col);
  freeMat(iMat, col);
  freeMat(augMat, col);
  freeMat(mMat, col);
  freeMat(tMat, col);
  freeMat(xMat, row);
  freeMat(yMat, row);
  freeMat(trainMat, row);
  freeMat(testMat, rowTest);
  fclose(fp);
  fclose(fp2);

  return 0;

}
