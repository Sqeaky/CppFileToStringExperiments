# CppFileToStringExperiments
A simple set of experiments to see if there is a performance difference in different ways of reading files to memory

This was inspired by this Stackoverflow question:
http://stackoverflow.com/questions/32169936/optimal-way-of-reading-a-complete-file-to-a-string-using-fstream

This should be easy to build on any C++14 and most C++11 you can either use CMake to produce make or project files,
or you can simply compile both source files with the same command or project.

This size of the file tested is in bytes accross the first row of results. Each row of results has nummbers
indicating the milliseconds to read the file 10 times. 10 is hardly foolproof, but still shows us some
interesting trends. To change the test iterations change the variable TestIterations in main.cpp

In my dozen or so runs of this it seems whatever test runs first suffers about a 100% performance penalty. This is likely
caused by memory allocation. Memory is freed from my program as far as the code I have written, but the compilers
surely realized that could be re-used and did not give it back to the OS.

It appears that anything using std::getline is highly optimized. Whatever checks it is doing have a negligible 
cost. While something is happening with those ifstream_iterators to cause them to run slower than in linear time.
I suspect that on my system at least that the construction of the stringstream has some cost or skipping straight
to the read buffer is skipping some kind of optimization the ifstream uses, perhaps respecting blocksize or CPU
cache size.

Here is a more direct summary:

  Fastest - Appending and Eof -  The reading chars does not seem to impose any cost.
  Faster - RdbufMove and Rdbuf -  The std::move seems to make no difference in release.
  Slow - Iterator - Something is wrong with this on.


I tried this on GCC 4.9.2, with a mobile i7 on a RAID1 of SSDs pulling about 1GB/sec formatted as EXT4 and
got these numbers:

    Doing 1000 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average
    Iterator            3           5           6           3123        28965       6420
    RdbufMove           3           3           3           220         4273        900
    Eof                 3           2           2           225         2809        608
    Rdbuf               3           3           3           240         7893        1628
    Appending           3           2           2           217         2795        603
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

    Doing 1000 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average
    Appending           3           3           3           834         5994        1367
    Iterator            3           4           5           2479        28953       6288
    Rdbuf               3           3           3           222         7807        1607
    Eof                 3           2           2           217         2824        609
    RdbufMove           3           3           3           220         7798        1605
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

    Doing 1000 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average
    RdbufMove           3           3           3           709         7902        1724
    Rdbuf               3           3           3           228         4253        898
    Iterator            3           4           5           2485        25530       5605
    Appending           3           3           3           220         2843        614
    Eof                 3           2           2           224         2841        614
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A
