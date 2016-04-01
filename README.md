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


We plan to include support for the popular feature descriptors in use today. Like image convolution with filters, **feature descriptor
computation and matching** can benefit greatly with parallelization due to independent and repeated computations over pixels.

The 2 of the above functions are only a small set of examples of the functions we plan to optimize, using parallel algorithm design techniques.


### High-level Computer Vision pipelines ###
*Need to add descriptions and illustrations here*
- **Corner detection**
- **Image classification**
- **Object detection**
- **Image alignment**
- **3D reconstruction from motion**
- **Image stitching**



## The Challenge ##



## Resources ##
- CMU's 16-385 : Undergrad level Computer Vision course
- http://www.engr.colostate.edu/~hj/conferences/47.pdf
- http://algo.yonsei.ac.kr/international_JNL/APCVsystem98Kim.pdf
- http://grid.cs.gsu.edu/~tcpp/curriculum/sites/default/files/Teaching%20Parallel%20Programming%20Using%20Computer%20Vision%20and%20Image%20Processing%20Algorithms(paper).pdf


## Goals and Deliverables ##
### Plan to Achieve ###

### Hope to Achieve ###


## Platform Choice ##
We have chosen to write our library in C++:
- for the low-level system control it offers
- to utilize compatible GPU and multi-core libraries and modules such as OpenMP, CUDA, SSE and AVX instructions
- due to its high speed and lower overheads

## Schedule ##

| Week ending | Target                                                                                       |
|-------------|----------------------------------------------------------------------------------------------|
| April 9th   | Finish writing low-level functions with parallel-computing optimizations                     |
| April 16th  | Finish writing sequential version of high-level CV pipelines                                 |
| April 23rd  | Optimize and add-in parallel-computing support for high-level pipelines                      |
| April 30th  | If running on schedule, then extend support for 3D point clouds                              |
| May 7th     | Compile speedup and project reports and refactor code, and prepare for the the presentation! |
