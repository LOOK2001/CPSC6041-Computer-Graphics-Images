I put filtered images and my own convolution kernels in experiments folder.

When program convolve, I get maximum value of either sum of positive weights and sum of the negative weights. Using the absolute value of maximum value as scale factor. I need to make sure scale factor equal to 1 if the maximum value equal to 0. I clamp both the scale factor and final pixel value between 0 and 255.

For boundary condition, I use reflection. I reflect pixels index beyond the boundary.

## Basic Requirements

##### original vs pulse.filt

##### original vs tent.filt

##### original vs hp.filt

##### original vs sobol-horiz.filt

##### original vs sobol-vert.filt

##### original vs new_sharpen.filt

##### original vs new_outline .filt

## Advanced Extension

##### original vs Gaussian filter [sigma = 4]

##### original vs Gaussian filter [sigma = 12]

##### original vs Gaussian filter [theta = 0, sigma = 8, periods = 4]

##### original vs Gaussian filter [theta = 45, sigma = 8, periods = 8]