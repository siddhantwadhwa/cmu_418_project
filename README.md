# CMU 15-418 Final Project
Final Project for 15-418 (Parallel Computer Architecture and Computing) at Carnegie Mellon University.


## Team Members ##
- Siddhant Wadhwa (swadhwa@andrew.cmu.edu)
- Druhin Sagar Goel (dsgoel@andrew.cmu.edu)


## Summary ##
A parallel C++ Computer vision library that uses SIMD vector instructions and takes advantage of the multiple cores and GPUs.
Think *A C++ Matlab for Computer Vision*!

## Background ##
Computer Vision algorithms most often involve repeated computation over all pixels in an image, which led us to believe that
the scope for speedup using parallel algorithm design techniques is enormous.

### Low-level functions ###
At the most basic level, a large number of computer vision alogrithms make excessive use of **convolution using image filters**. Image filtering requires
neghbourhood operations around every pixel, making it a prime target for parallelization, as shown in the following illustration:


![Image Convolution](https://developer.apple.com/library/ios/documentation/Performance/Conceptual/vImage/Art/kernel_convolution.jpg "Image Convolution")


How computers understand images is very different from how they are perceived by the human mind. In order to effeiciecntly store 
the required information about pixels, a large fraction of computer vision algorithms prefer to use feature descriptors over 
pixel intensities, which often take much lesser space to store than raw intensities and are faster to match and make use of.
Consider the following illustration in which, instead of storing raw image intenisities, like in (a), we decide to store only the
results of the pixel's intensity-based comprisons with its neighbours. Each pixel now only needs a few bits to store the requires
information:


![Binary features](./readme_data/binary_features.png "Binary features")






