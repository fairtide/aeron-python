### Introduction

This repository provides unofficial Python bindings for Aeron and Aeron archive client. Providing minimalistic, pythonic interface 
for sending and receiving messages through Aeron. 

### Installation and usage



### Compilation and packaging

#### Requirements

To build, following prerequisites are necessary:
* Linux, OSX or Windows host
* Python 3.x with headers and development libraries
* Modern C++ 14 compatible compiler - clang 3.9+/gcc 7+/vcpp 2017+
* CMake 3.7 or newer
* make or ninja / visual studio
* Modern version of boost
* JDK 8+

#### Compilation 

##### cmake approach

* Clone this repository into a local directory:
    ```
    $ git clone 
    ```

* Create a build directory.
* From the build directory invoke project configuration:
    ```
    $ cmake <path to cloned repository>
    ```
* Invoke a build for all compilation targets:
    ```
    $ cmake --build . --target all
    ``` 
* Invoke unit tests with **ctest**:
    ```
    $ ctest --verbose
    ```

##### setup tools approach

It is possible to build and install this repository as a source package using setuptools:

Clone this repository into a local directory:
```
$ git clone ...
```

Run pip against newly cloned repository
```
$ python3 -m pip install <path to cloned repository>
```

In this way setup tools will automatically trigger cmake configuration, compilation and installation.

### Troubleshooting   