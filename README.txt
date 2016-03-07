**********************************************************
* README V1.2
*
* Quantitative Phase Image Processing (QPI)
* Author: Junyi Xie
*         Dept. of Electrical and Electronics Engineering         
*         University of Hong Kong
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
    This public version has no SVM classifier and is implemented on Xilinx Virtex-6 FPGA.
    Up-to-data version is implemented on Altera Stratix-V FPGA with SVM classifier.
    Fixed point number is (25, 15) format now on Altera FPGA.
    Nvidia CUDA code of QPI algorithm will be uploaded to git repository later.

  Current Clock Frequency:
    150MHz

  Current throughputs on Xilinx V-6 FPGA:
    32-bit float input:
      120000 images in ~67 seconds
    16-bit signed integer input:
      120000 images in ~60 seconds
  
  CPU version on E5 2640 6 cores, 6 threads, HT turned off:
    120000 images in ~420 seconds
  
  Reference published paper:
  "Accelerated cell imaging and classification on FPGAs for quantitative-phase asymmetric-detection time-stretch optical microscopy",
  IEEE FPT 2015
  <http://ieeexplore.ieee.org/xpl/articleDetails.jsp?reload=true&arnumber=7393123>
