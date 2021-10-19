I put filtered images and my own convolution kernels in the experiments folder.

When the program convolve, I get the maximum value of either sum of positive weights and the sum of the negative weights. Using the absolute value of maximum value as a scale factor. I need to make sure the scale factor is equal to 1 if the maximum value is equal to 0. I clamp both the scale factor and final pixel value between 0 and 255.

For boundary condition, I use reflection. I reflect pixels index beyond the boundary.

## Basic Requirements

##### original vs pulse.filt

The image blurred by pulse filter

| ![squares](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\images\squares.png) | ![squares_pulse](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\experiments\squares_pulse.png) |
| :----------------------------------------------------------: | :----------------------------------------------------------: |

##### original vs tent.filt

The image blurred by tent filter

| ![squares](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\images\squares.png) | ![squares_tent](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\experiments\squares_tent.png) |
| :----------------------------------------------------------: | :----------------------------------------------------------: |

##### original vs hp.filt

hp filter draw the outline of origin image

| ![squares](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\images\squares.png) | ![squares_hp](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\experiments\squares_hp.png) |
| :----------------------------------------------------------: | :----------------------------------------------------------: |

##### original vs sobol-horiz.filt

sobol-horiz filter compress the rectangle to a line and rotate it

| ![squares](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\images\squares.png) | ![squares_sobol_horiz](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\experiments\squares_sobol_horiz.png) |
| :----------------------------------------------------------: | :----------------------------------------------------------: |

##### original vs sobol-vert.filt

sobol-vertfilter compress the rectangle to a line

| ![squares](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\images\squares.png) | ![squares_sobol_vert](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\experiments\squares_sobol_vert.png) |
| :----------------------------------------------------------: | :----------------------------------------------------------: |

##### original vs new_sharpen.filt

new_sharpen filter make the outline more clear

| ![waves](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\images\waves.png) | ![waves_new_sharpen](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\experiments\waves_new_sharpen.png) |
| :----------------------------------------------------------: | :----------------------------------------------------------: |

##### original vs new_outline .filt

new_outline draw the outline of original image

| ![waves](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\images\waves.png) | ![waves_new_outline](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\experiments\waves_new_outline.png) |
| :----------------------------------------------------------: | :----------------------------------------------------------: |



## Advanced Extension

##### original vs Gaussian filter [sigma = 2]

greater sigma make original image blurrier

| ![rhino](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\images\rhino.png) | ![rhino_gaussian_2](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\experiments\rhino_gaussian_2.png) |
| :----------------------------------------------------------: | :----------------------------------------------------------: |

##### original vs Gaussian filter [sigma = 4]

greater sigma make original image blurrier

| ![rhino](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\images\rhino.png) | ![rhino_gaussian_4](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\experiments\rhino_gaussian_4.png) |
| :----------------------------------------------------------: | :----------------------------------------------------------: |



##### original vs Gaussian filter [theta = 0, sigma = 4, periods = 4]

It's hard to identify the original image

| ![Lena](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\images\Lena.png) | ![Lena_sobol_0_4_4](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\experiments\Lena_sobol_0_4_4.png) |
| :----------------------------------------------------------: | :----------------------------------------------------------: |

##### original vs Gaussian filter [theta = 45, sigma = 4, periods = 8]

we can see some outlines from upper right to bottom left

| ![Lena](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\images\Lena.png) | ![Lena_sobol_45_4_8](C:\Xicheng\MyLife\College\Clemson\2021-2022FALL\Computer Graphics Image\Assignments\CPSC6041-Computer-Graphics-Images\HW4\experiments\Lena_sobol_45_4_8.png) |
| :----------------------------------------------------------: | :----------------------------------------------------------: |