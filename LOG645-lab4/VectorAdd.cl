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
__kernel void VectorAdd(__global float* matrix, int nb_col, int nb_ligne, float h, float td, __global float* matrix_sortie,  int iNumElements)
{
    // get index into global data array
    int iGID = get_global_id(0);

    // bound check (equivalent to the limit on a 'for' loop for standard/serial C code
    if (iGID >= iNumElements)
    {   
        return; 
    }
    
    // add the vector elements
	int x, y;
	x = iGID % nb_col;
	y = iGID / nb_col;
	if (x == 0 || x == nb_col-1 || y == 0 || y == nb_ligne-1)
	{
		return;
	}
	matrix_sortie[iGID] = ((1-(4*td)/(h*h))*matrix[iGID])+((td/(h*h))*(matrix[iGID+nb_col]+matrix[iGID-nb_col]+matrix[iGID+1]+matrix[iGID-1]));
}