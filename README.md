# Integrating the HOOPS Native Platform with Android

This sample Android Project contains simple functional examples that demonstrate HOOPS Native SDKs interoperating with Android Applications. This is achieved through the Java Native Interface which allows Android Java code to invoke Native Library Methods.

## Training Recording
[![HOOPS Training - Android](http://img.youtube.com/vi/FAxJRnXsXX0/1.jpg)](https://youtu.be/FAxJRnXsXX0 "HOOPS Training - Android")

These examples assume basic knowledge and familiarity with Android Studio, developing Android Applications, and writing Native Code (C++ in our case). The below resources are a good jumping off point if you are new to this.
 - [Create your first Android Project](https://developer.android.com/training/basics/firstapp/creating-project)
 - [What is HOOPS Native Platform?](https://docs.techsoft3d.com/hps/latest/build/general/technical_overview.html)
 - [The Java Native Interface](https://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/intro.html#wp725)

## Project Dependencies
- Android Studio (Built on 4.1.1 w/ Gradle 6.5)
  - Native Development Kit (NDK) * using v18.1 which requires manual install.
  - CMAKE 3.10.2
- HOOPS Visualize for Android
  - Request and evaluation from [www.techsoft3d.com](www.techsoft3d.com)
  - Download package from [developer.techsoft3d.com](developer.techsoft3d.com)-> HOOPS Products -> HPS API -> Mobile -> Android
  - On your system, set HPS_VISUALIZE_INSTALL_DIR to the extracted download location containing /bin, /include, etc.
  - If you are integrating HOOPS Exchange as well. Download it from Developer Zone and set the environment variable HEXCHANGE_INSTALL_DIR respectively as well.


Click for [help](https://developer.android.com/studio/projects/install-ndk#specific-version) installing NDK and CMAKE through Android Studio

Note the section [Install a specific Version of the NDK](https://developer.android.com/studio/projects/install-ndk#specific-version)

You may also require a HOOPS Product License if running this Project after 1/1/2021. To generate a new 60 Day license for free, sign up for an evaluation at https://www.techsoft3d.com.


## Setup and Build
With the proper dependencies installed, the project should build cleanly out of the box.

Gradle and CMake Build steps are executed with relative path dependencies so be aware if moving around.

By default ABI's will be built for the following: 'armeabi-v7a', 'arm64-v8a', 'x86', 'x86_64'.

The CMAKE CMakeLists.txt is located at app/src/main/cpp and targets HPS_VISUALIZE_INSTALL_DIR, as well as a relative dependency to `sip/` in the root directory.

## Navigating the File Structure
Android Projects come with a LOT of overhead, try to ignore the mass of files and folders and focus on those below:

```
Visualize_Training
│   README.md - You are here X
└───app
│   └───src
│       └───main
│           └───cpp
│           │   UserMobileSurface.h - Declarations for UserMobileSurface.cpp (Useful comments in file header regarding JNI)
│           │   UserMobileSurface.cpp - HOOPS Visualize Native Code that will be invoked in MobileSurfaceActivity.java
|           └───java/com/techsoft3d/hps/virtual_training
│           │   MobileSurfaceActivity.java - Main entrypoint for App
│           │   AndroidUserMobileSurfaceView.java - Autogenerated JNI glue code.
│       └───assets - Contains 3D CAD Data that is loaded to virtual SC Card.
│       └───res - Contains XML Layouts and Static resources like icons.
└───sip
    │   sip.py - Java Native Interface Generator
```


## Implement HOOPS Native Code

The following three files from the Tree above are involved in writing and executing HOOPS Native Code.
- UserMobileSurface.cpp - High level Visualize Operations built open MobileSurface & App
- AndroidUserMobileSurfaceView.java - Autogenerated Java Class w/ Native Methods defined in UserMobileSurface.cpp
- MobileSurfaceActivity.java - Main Android Activity, get user input, load data, create instance of AndroidUserMobileSurfaceView.

To add, or remove functionality - you must modify UserMobileSurface.cpp/h and trigger a CMake Build (included in Gradle Build). This will run a script located at `sip/sip.py`
which is responsible for the Auto-generated `AndroidUserMobileSurfaceView.java` file as well as `src/main/cpp/JNI/AndroidUserMobileSurfaceViewJNI.cpp`. The auto-generated code wraps the C++ functionality into a Java Class that can be instantiated and used in your Android Application.

## Using HOOPS Exchange.
If you would like to use the HOOPS Exchange libraries as well for access to more CAD Filetypes or advanced Data Translation embedded in your Application, you can use the USING_EXCHANGE variable when building via NDK.
