**********************************************************
* README V1.0
*
* Quantitative Phase Image Processing (QPI)
* Author: Junyi Xie
*         University of Hong Kong
*         Dept. of Electrical and Electronic Engineering         
* 
* Special Thanks to 
*         Custom Computing Group
*         Imperial College London
*
*
**********************************************************
  This is the maxeler FPGA accelerated version of Quantitative Phase Image Processing.
  Image size:
    256 * 256
  
  Current notices:
    Fixed point number is (30, 15) format now.
    Precision loss in critical modules.
  
  Current Clock Frequency:
    150MHz

  Current throughputs on FPGA:
    120000 images in ~67 seconds
  
  CPU version on E5 2640 6 cores, 6 threads, HT turned off:
    120000 images in ~420 seconds
