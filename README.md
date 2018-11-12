[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
[![image](https://img.shields.io/pypi/v/aeron-python.svg)](https://pypi.org/project/aeron-python/)

### Introduction

This repository provides unofficial Python bindings for Aeron and Aeron archive client. Providing minimalistic, pythonic interface 
for sending and receiving messages through Aeron. 

### Installation and usage

For installation use [pipenv](https://docs.pipenv.org):
```
$ pipenv install aeronpy
```

or pip:
```
$ pip install aeronpy
```

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
    
* To build source package:
    ```
    $ cmake --build . --target build-package
    ```    
    
* To build a [wheel](https://www.python.org/dev/peps/pep-0427/):
    ```
    $ cmake --build . --target build-wheel
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

* **`CnC file not created:` when creating `Context`**

    This indicates that Aeron driver is not currently running in your system. You can either checkout Aeron from it's [official github repository](https://github.com/real-logic/aeron) and build is by yourself or use appropriate pre-built package available [here](https://bintray.com/lukaszlaszko/aeron/aeron-driver#files).
    
    Once downloaded and unpacked run:
    ```
    $ <unpack_directory>/bin/aeron-driver
    ```        

* **macOS Mojave & Anaconda**

    It appears that there are some problems with this sort of setup at this moment. An attempt to load shared object module linked against Anaconda 5.3 results with following exception being thrown:
    ```
    Fatal Python error: PyThreadState_Get: no current thread
    ```
    during a call to [`PyModule_Create`](https://docs.python.org/3.6/c-api/module.html#c.PyModule_Create). 

    This is resolve this issue use non-Anaconda, standard python distribution from [python.org](https://www.python.org/downloads/release/python-367/).
      
      
### License

Copyright 2018 Fairtide Pte. Ltd.

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
