/*
 * Copyright 1993-2010 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */
 
 // OpenCL Kernel Function for element by element vector addition
__kernel void VectorAdd(__global float* matrix, __global int nb_col, __global int nb_ligne, __global float h, __global float td,  int iNumElements)
{
    // get index into global data array
    int iGID = get_global_id(0);

    // bound check (equivalent to the limit on a 'for' loop for standard/serial C code
    if (iGID >= iNumElements)
    {   
        return; 
    }
    
    // add the vector elements
	matrix[iGID+1+nb_col] = ((1-(4*td)/(h*h))*matrix[iGID+1+nb_col])+((td/(h*h))*(matrix[iGID+2+nb_col]+matrix[iGID+nb_col]+matrix[iGID+1+2*nb_col]+matrix[iGID+1]));
}