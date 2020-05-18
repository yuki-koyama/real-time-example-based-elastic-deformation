## !! Important Notice !! ##

This repository has not been maintained for long time and should have many build issues with recent environments. We are aware that this repository should

- Use CMake (or other alternatives) to manage the project (instead of using Xcode, which is only for macOS and generates many compatibility issues),
- Use pointers to external libraries (instead of using pre-compiled binaries, which cause many environemental issues), and
- Update/reduce external dependencies.

Unfortunately, we do not have enough time for updating this repository for now; pull requests are welcome.

---

# Overview #
This is an implementation of the paper:

> Yuki Koyama, Kenshi Takayama, Nobuyuki Umetani, and Takeo Igarashi. 2012. Real-time example-based elastic deformation. In Proceedings of the ACM SIGGRAPH/Eurographics Symposium on Computer Animation (SCA '12). Eurographics Association, Aire-la-Ville, Switzerland, Switzerland, 19-24.

# Developers #
Implemented by Kenshi Takayama and Yuki Koyama, and currently maintained by Yuki Koyama.

[koyama@is.s.u-tokyo.ac.jp](mailto:koyama@is.s.u-tokyo.ac.jp)

# Project Page #
[http://koyama.xyz/project/ExampleBasedShapeMatching/](http://koyama.xyz/project/ExampleBasedShapeMatching/)

# Note #
* The source codes (except for third party ones) are licensed under the MIT license.
* This implementation is slightly modified from the version of making the demos and recording the timings for the paper.
* It is not widely tested.
* This source code was written by using Xcode and Mac OS X with x86. Other environments are not supported. (But the source code is written in standard C++, so porting might not be so difficult.)
* If you want to build and use it on other OS, please achieve it by yourself.

# How to Build #
This source code can be built using Xcode (tested on Xcode 4.3.2, 6.1.1, and 9.0.1).

`Real-Time_Example-Based_Elastic_Deformation.xcodeproj` is the project file for Xcode.

# How to Use #
Please see this example of use:

[http://www.youtube.com/watch?v=7Pe-RPLMeDI](http://www.youtube.com/watch?v=7Pe-RPLMeDI)

If you want to learn more, please read the source codes.
