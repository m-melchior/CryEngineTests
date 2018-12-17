# CryEngineTests
This repository contains test applications for getting a deeper understanding of the architecutre and algorithms used in the [CryEngine](https://www.cryengine.com/) as well as doing performance tests and comparisons to other coding standards and examine the effectiveness. The source code of the engine can be found in their [github repository](https://github.com/CRYTEK/CRYENGINE).

For more information on the project and the experience with the CryEngine, check the project log on [my blog](https://mmelchior.wordpress.com/category/projects/dune-ii-3d/)

Each of the .cpp files is an application of its own and can be compiled / run without any further dependencies. The idea is to extract the algorithms in question and work on them exclusively without any external dependencies or unrelated code.

* assert.cpp contains the functional chain of the assert method used in the CryEngine [LinuxLauncher](https://github.com/CRYTEK/CRYENGINE/tree/release/Code/Launcher/LinuxLauncher)
* cry_strcpy_performance is a simple performance / binary size test of 100k calls of the cry_strcpy implemented in [CryString](https://github.com/CRYTEK/CRYENGINE/tree/release/Code/CryEngine/CryCommon/CryString)
* strcpy_performance is the same as in cry_strcpy, but using the native strcpy. Note that the calls are not being done in a loop but as 100k lines to see the result in size of binary of the inline cry_strcpy and the native strpy.
* unlikely_vs_normal_if.cpp is a performance test of the [\_\_builtin_expect](https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html) used in CryEngine implementation of [Assert](https://github.com/CRYTEK/CRYENGINE/tree/release/Code/CryEngine/CryCommon/CryCore/Assert). For more information about this, please check the blog post talking about [black magic if then else](https://mmelchior.wordpress.com/2018/12/17/dune-ii-in-3d-part-6-cryengine-going-in/).

