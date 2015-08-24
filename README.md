# CppFileToStringExperiments
A simple set of experiments to see if there is a performance difference in different ways of reading files to memory

This was inspired by this Stackoverflow question:
http://stackoverflow.com/questions/32169936/optimal-way-of-reading-a-complete-file-to-a-string-using-fstream

This should be easy to build on any C++14 and most C++11 you can either use CMake to produce make or project files,
or you can simply compile both source files with the same command or project.

This size of the file tested is in bytes accross the first row of results. Each row of results has nummbers
indicating the milliseconds to read the file 10 times. 10 is hardly foolproof, but still shows us some
interesting trends. To change the test iterations change the variable TestIterations in main.cpp

In my dozen or so runs of this it seems whatever test runs first suffers about a performance penalty. This is likely
caused by memory allocation. Memory is freed from my program as far as the code I have written, but the compilers
surely realized that freed memory could be re-used and do not give it back to the OS until the execution ends.

TellSeekRead is clearly a minimalistic and efficient way to read files, but it seems too verbose for the
unitiated. It appears that anything using std::getline is somewhat optimized. Whatever checks it is doing have a
negligible cost, but the line by line input and lack of optimization for whole file slurping hurt it. Something
is happening with those ifstream_iterators to cause them to run slower than in linear time. I suspect that, on my
system at least, that the construction of the stringstream has some cost or skipping straight to the read buffer
is skipping some kind of optimization the ifstream uses, perhaps respecting blocksize or CPU cache size, regardless
why the Rdbuf methods are categorically faster than istream_iterators but slower than everything else.

Here is a more direct summary:

  Fastest - TellSeekRead - This has the system provide an easy to get the size and reads the file in one go.

  Faster - Getline Appending and Eof - The checking of chars does not seem to impose any cost.

  Fast - RdbufMove and Rdbuf -  The std::move seems to make no difference in release.

  Slow - Iterator - Something is wrong with this on.


I tried this on GCC 4.9.2, with a mobile i7 on a RAID1 of SSDs pulling about 1GB/sec formatted as EXT4 and
got these numbers:


    Doing 1000 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average
    RdbufMove           3           4           5           715         8156        1776
    Iterator            3           4           5           2429        29190       6326
    Rdbuf               3           3           3           220         8122        1670
    TellSeekRead        3           2           2           37          401         89
    Appending           3           2           2           214         2847        613
    Eof                 3           2           2           216         2800        604
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

    Doing 1000 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average
    Iterator            3           3           5           2874        29400       6457
    Eof                 3           2           2           217         2783        601
    Rdbuf               3           3           3           220         8119        1669
    RdbufMove           3           3           3           222         8116        1669
    TellSeekRead        3           2           2           36          397         88
    Appending           3           2           2           214         2771        598
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

    Doing 1000 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average
    TellSeekRead        3           3           3           36          397         88
    RdbufMove           3           2           3           223         4266        899
    Appending           3           3           3           217         2772        599
    Eof                 3           3           2           222         2803        606
    Iterator            3           4           5           2436        29461       6381
    Rdbuf               3           2           3           218         8087        1662
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A
