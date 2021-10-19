When program convolve, I get maximum value of either sum of positive weights and sum of the negative weights. Using the absolute value of maximum value as scale factor. I need to make sure scale factor equal to 1 if the maximum value equal to 0. I clamp both the scale factor and final pixel value between 0 and 255.

For boundary condition, I use reflection. I reflect pixels index beyond the boundary.

