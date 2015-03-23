/***
    This project performs a naive matrix multiplication.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

// two 24 x 24 matrices = 576 ints = 6 * 384 bytes; this constant is
// defined in the Manager, to make sure the CPU and DFE are in sync
const int inSize = 256;

void print_matrix(int *mat, int size) {
  for(int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j)
      printf("%d ", mat[i * inSize + j]);
    printf("\n");
  }
}


void matrix_mult(int *a, int *b, int *res) {
  for(int i = 0; i < inSize; ++i)
    for (int j = 0; j < inSize; ++j)
      for (int k = 0; k < inSize; k++)
        res[i * inSize + j]  += a[i * inSize + k] * b[k * inSize + j ];
}


void transpose_matrix(int *mat, int size) {

  for (int i = 0 ; i < size; i++)
    for (int j = i; j < size; j++) {
      int t = mat[i * size + j];
      mat[i * size + j] = mat[j * size + i];
      mat[j * size + i] = t;
    }
}


int main(void)
{

  int overhead = inSize*10+2;

  int *i1 = malloc (sizeof(int) * inSize * (inSize+overhead));
  int *i2 = malloc (sizeof(int) * inSize * (inSize+overhead));
  int *i3 = malloc (sizeof(int) * inSize * (inSize+overhead));
  int *i4 = malloc (sizeof(int) * inSize * (inSize+overhead));
  float *i1_bgs = malloc (sizeof(float) * inSize * inSize);
  float *i1_bgs_positive = malloc (sizeof(float) * inSize * inSize);  
  float *i1_norm = malloc (sizeof(float) * inSize * inSize);

  float *i2_bgs = malloc (sizeof(float) * inSize * inSize);
  float *i2_bgs_positive = malloc (sizeof(float) * inSize * inSize);
  float *i2_norm = malloc (sizeof(float) * inSize * inSize);

  float *i3_bgs = malloc (sizeof(float) * inSize * inSize);
  float *i3_bgs_positive = malloc (sizeof(float) * inSize * inSize);
  float *i3_norm = malloc (sizeof(float) * inSize * inSize);

  float *i4_bgs = malloc (sizeof(float) * inSize * inSize);
  float *i4_bgs_positive = malloc (sizeof(float) * inSize * inSize);
  float *i4_norm = malloc (sizeof(float) * inSize * inSize);


  int *expected = malloc (sizeof(int) * inSize * inSize);
  float *mean1 = malloc (sizeof(float) * (inSize));
  float *mean2 = malloc (sizeof(float) * (inSize));
  float *mean3 = malloc (sizeof(float) * (inSize));
  float *mean4 = malloc (sizeof(float) * (inSize));
  float *i1_bgs_newMin = malloc(sizeof(float) * 4); 
  float *i1_pos_newMax = malloc(sizeof(float) * 4);

  float *i2_bgs_newMin = malloc(sizeof(float) * 4);
  float *i2_pos_newMax = malloc(sizeof(float) * 4);

  float *i3_bgs_newMin = malloc(sizeof(float) * 4);
  float *i3_pos_newMax = malloc(sizeof(float) * 4);

  float *i4_bgs_newMin = malloc(sizeof(float) * 4);
  float *i4_pos_newMax = malloc(sizeof(float) * 4);

  float *i6 = malloc (sizeof(float) * inSize * inSize);
  float *i8 = malloc (sizeof(float) * inSize * inSize);
  float *theta_y = malloc (sizeof(float) * inSize * inSize);
  float *dphi_y = malloc (sizeof(float) * inSize * inSize);
  float *G = malloc(sizeof(float) * inSize * inSize *2);
  memset(i1, 0, sizeof(int) * inSize * (inSize+overhead));
  memset(i2, 0, sizeof(int) * inSize * (inSize+overhead));
  memset(i3, 0, sizeof(int) * inSize * (inSize+overhead));
  memset(i4, 0, sizeof(int) * inSize * (inSize+overhead));
  memset(i1_bgs, 0, sizeof(float) * inSize * inSize);
  memset(i1_bgs_positive, 0, sizeof(float) * inSize * inSize);
  memset(i1_norm, 0, sizeof(float) * inSize * inSize);
  
  memset(i2_bgs, 0, sizeof(float) * inSize * inSize);
  memset(i2_bgs_positive, 0, sizeof(float) * inSize * inSize);
  memset(i2_norm, 0, sizeof(float) * inSize * inSize);

  memset(i3_bgs, 0, sizeof(float) * inSize * inSize);
  memset(i3_bgs_positive, 0, sizeof(float) * inSize * inSize);
  memset(i3_norm, 0, sizeof(float) * inSize * inSize);

  memset(i4_bgs, 0, sizeof(float) * inSize * inSize);
  memset(i4_bgs_positive, 0, sizeof(float) * inSize * inSize);
  memset(i4_norm, 0, sizeof(float) * inSize * inSize);


  memset(mean1, 0, sizeof(float) * (inSize));
  memset(mean2, 0, sizeof(float) * (inSize));
  memset(mean3, 0, sizeof(float) * (inSize));
  memset(mean4, 0, sizeof(float) * (inSize));
  memset(i1_bgs_newMin, 0, sizeof(float) * 4);
  memset(i1_pos_newMax, 0, sizeof(float) * 4);

  memset(i2_bgs_newMin, 0, sizeof(float) * 4);
  memset(i2_pos_newMax, 0, sizeof(float) * 4);

  memset(i3_bgs_newMin, 0, sizeof(float) * 4);
  memset(i3_pos_newMax, 0, sizeof(float) * 4);

  memset(i4_bgs_newMin, 0, sizeof(float) * 4);
  memset(i4_pos_newMax, 0, sizeof(float) * 4);

  memset(expected, 0, sizeof(int) * inSize  * inSize);

  // if you want deterministic random numbers, comment out the line below
  srand(time(NULL));

  for(int i = 0; i < inSize; ++i) {
    for (int j = 0; j < inSize; ++j) {
      i1[i * inSize + j] = -rand() % 100+1;
      i2[i * inSize + j] = -rand() % 100;
      i3[i * inSize + j] = -rand() % 100;
      i4[i * inSize + j] = -rand() % 100;
    }
  }
  
  /*
  printf("Matrix i1:\n");
  print_matrix(i1, inSize);

  printf("\nMatrix i2:\n");
  print_matrix(i2, inSize);

  printf("\nMatrix i3:\n");
  print_matrix(i3, inSize);

  printf("\nMatrix i4:\n");
  print_matrix(i4, inSize);
  */
  printf("Running on FPGA. \n");
  

  //invoke FPGA function
  /*
  QPI(inSize * (inSize+overhead) , i1, i2, i3, i4, 
      dphi_y,
      i1_bgs, 
      //i1_bgs_newMin, 
      i1_bgs_positive,
      i1_norm,
      //i1_pos_newMax,
      i2_bgs_positive,
      i2_norm,
      i6,
      //i8,
      //mean1, mean2, mean3, mean4
      theta_y
      );
   */
  //QPI for assembly
  QPI(inSize * (inSize+overhead), i1, i2, i3, i4);
    
  //QPI(1640, i1, i2, i3, i4, dphi_y, i1_bgs_positive, i1_norm, i2_bgs_positive, i2_norm, i6, i8, theta_y);

  /*
  for (int i = 0; i < inSize; i++) {
      printf("mean1[%d] = %f\n", i, mean1[i]);
      printf("mean2[%d] = %f\n", i, mean2[i]);
      printf("mean3[%d] = %f\n", i, mean3[i]);
      printf("mean4[%d] = %f\n", i, mean4[i]);
}


  printf("\nbackground subtraction image1: \n\n");
  for(int i = 0; i < inSize; ++i) {
    for (int j = 0; j < inSize; ++j)
      printf("%f ", i1_bgs[i * inSize + j]);
    printf("\n");
  }



  ////compare CPU min and FPGA min  
  float CPU_min = 0;
  for (int i = 0; i < inSize; ++i) {
    for (int j = 0; j < inSize; ++j) {
  if (i1_bgs[i*inSize+j] < CPU_min) CPU_min = i1_bgs[i*inSize+j];
    }
  }

  printf("\nig_bgs_newMin = %f \n", i1_bgs_newMin[3]);
  printf("\nCPU_min = %f \n\n", CPU_min);


  printf("\nbackground subtraction image1 positive: \n\n");
  for(int i = 0; i < inSize; ++i) {
    for (int j = 0; j < inSize; ++j)
      printf("%f ", i1_bgs_positive[i * inSize + j]);
    printf("\n");
  }

  float CPU_max_1 = 0;
  for (int i = 0; i < inSize; ++i) {
    for (int j = 0; j < inSize; ++j) {
        if (i1_bgs_positive[i*inSize+j] > CPU_max_1) CPU_max_1 = i1_bgs_positive[i*inSize+j];
    }
  }

  printf("\ni1_pos_newMax = %f \n", i1_pos_newMax[3]);
  printf("\nCPU_max_1 = %f \n\n", CPU_max_1);

  printf("\nimage1 normalization: \n\n");
  for(int i = 0; i < inSize; ++i) {
    for (int j = 0; j < inSize; ++j)
      printf("%f ", i1_norm[i * inSize + j]);
    printf("\n");
  }

  printf("\nbackground subtraction image2 positive: \n\n");
  for(int i = 0; i < inSize; ++i) {
    for (int j = 0; j < inSize; ++j)
      printf("%f ", i2_bgs_positive[i * inSize + j]);
    printf("\n");
  }

  float CPU_max_2 = 0;
  for (int i = 0; i < inSize; ++i) {
    for (int j = 0; j < inSize; ++j) {
        if (i2_bgs_positive[i*inSize+j] > CPU_max_2) CPU_max_2 = i2_bgs_positive[i*inSize+j];
    }
  }

  printf("\ni2_pos_newMax = %f \n", i2_pos_newMax[3]);
  printf("\nCPU_max_2 = %f \n\n", CPU_max_2);

  printf("\nimage2 normalization: \n\n");
  for(int i = 0; i < inSize; ++i) {
    for (int j = 0; j < inSize; ++j)
      printf("%f ", i2_norm[i * inSize + j]);
    printf("\n");
  }

  printf("\ni6: \n\n");
  for(int i = 0; i < inSize; ++i) {
    for (int j = 0; j < inSize; ++j)
      printf("%f ", i6[i * inSize + j]);
    printf("\n");
  }

  printf("\ni8: \n\n");
  for(int i = 0; i < inSize; ++i) {
    for (int j = 0; j < inSize; ++j)
      printf("%f ", i8[i * inSize + j]);
    printf("\n");
  }

  printf("\ntheta_y: \n\n");
  for(int i = 0; i < inSize; ++i) {
    for (int j = 0; j < inSize; ++j)
      printf("%f ", theta_y[i * inSize + j]);
    printf("\n");
  }

  printf("\ndphi_y: \n\n");
  for(int i = 0; i < inSize; ++i) {
    for (int j = 0; j < inSize; ++j)
      printf("%f ", dphi_y[i * inSize + j]);
    printf("\n");
  }
  */

  printf("G:\n\n");
  for(int i = 0; i < inSize; ++i) {
    for (int j = 0; j < inSize*2 ; j+=2)
      printf("G[%d,%d] = %f + i %f   ", i, j/2, G[i * inSize + j], G[i * inSize+j+1]);
    printf("\n");
  }
  printf("Shutting down.\n");

  return 0;
}


