# Setup Instructions #

### System Requirements and Setup Instructions###

- a CUDA-capable GPU
- Mac OSX 10.9 or later
- the Clang compiler and toolchain installed using Xcode
    - The CUDA Toolkit requires that the native command-line tools are already installed on the system. To install those command-line tools, Xcode must be installed first.
    - Once Xcode is installed, the command-line tools can be installed by running the following command: `xcode-select --install`
- the NVIDIA CUDA Toolkit (download and install from the CUDA Download page)
    - Useful step-by-step instructions at : http://docs.nvidia.com/cuda/cuda-getting-started-guide-for-mac-os-x/
- Set up the required environment variables:

```
export PATH=/Developer/NVIDIA/CUDA-7.0/bin:$PATH
export DYLD_LIBRARY_PATH=/Developer/NVIDIA/CUDA-7.0/lib:$DYLD_LIBRARY_PATH
```


**Note:** To run CUDA applications in console mode on MacBook Pro with both an integrated GPU and a discrete GPU, use the following settings before dropping to console mode:

- Uncheck System Preferences > Energy Saver > Automatic Graphic Switch
- Drag the Computer sleep bar to Never in System Preferences > Energy Saver


### Setting up opencv 3 for mac ###

Follow the step-by-step instructions at http://tilomitra.com/opencv-on-mac-osx/


### Install PCL library ###

Follow instuctions on : http://pointclouds.org/downloads/macosx.html. First install the dependencies (boost, eigen, FLANN, VTK, Qhull and OpenNI), then the PCL library.






