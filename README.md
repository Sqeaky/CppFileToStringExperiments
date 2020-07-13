# CppFileToStringExperiments
A simple set of experiments to see if there is a performance difference in different ways of reading files to memory

This was inspired by this Stackoverflow question:
http://stackoverflow.com/questions/32169936/optimal-way-of-reading-a-complete-file-to-a-string-using-fstream

This should be easy to build on any C++14 compiler and most C++11 compileryou can either use CMake to produce make or
project files, or you can simply compile both source files with the same command or project.

This size of the file tested is in bytes accross the first row of results. Each row of results has nummbers
indicating the milliseconds to read the file 10 times. 10 is hardly foolproof, but still shows us some
interesting trends. To change the test iterations change the variable TestIterations in main.cpp

In my runs of this it seems whatever test runs first, Except TellSeekRead, suffers about a performance penalty.
This is likely caused by memory allocation. Memory is freed from my program as far as the code I have written,
but the compilers surely realized that freed memory could be re-used and do not give it back to the OS until
the execution ends.

TellSeekRead is clearly a minimalistic and efficient way to read files, but it seems a bit verbose for the
unitiated. CTellSeekRead is similar but seems slower on smalle files. It appears that anything using
std::getline is somewhat optimized. Whatever checks it is doing have a negligible cost, but the line
by line input and lack of optimization for whole file slurping hurt it. Something
is happening with those ifstream_iterators to cause them to run slower than in linear time. I suspect that, on my
system at least, that the construction of the stringstream has some cost or skipping straight to the read buffer
is skipping some kind of optimization the ifstream uses, perhaps respecting blocksize or CPU cache size, regardless
why the Rdbuf methods are categorically faster than istream_iterators but slower than everything else.

Here is a more direct summary:

  Fastest - TellSeekRead, and CTellSeekRead- These have the system provide an easy to get the size and reads the file in one go.

  Faster - Getline Appending, and Eof - The checking of chars does not seem to impose any cost, but these are slower a bulk at once copy of a known size.

  Fast - RdbufMove, and Rdbuf - The std::move seems to make no difference in release.

  Slow - Iterator, BackInsertIterator, and AssignIterator - Something is wrong with iterators and input streams. They work great in memory, but not here.

In 2020 I tried this on with Clang 10 and GCC 9.3 on Ubuntu with a Ryzen 9 3950x on a ZFS volume comprised of a mirror
of NVME disk which has a read block read speed of about 2.3GB/sec per Bonnie++. These largely line up with previous
results.

    $ clang++ --version | head -n1
    clang version 10.0.0-4ubuntu1
    $ ./CppFileToStringExperiments
    Doing 1000 iterations of the file read in each benchmark. Output is milliseconds, lower is better.
    Name / Size         100         512         1024        1000000     10000000    Average
    Eof                 8           7           3           565         10854       2287
    Appending           3           3           3           310         10924       2248
    BackInsertIterator  4           5           6           2489        33030       7106
    Iterator            3           5           6           2654        31986       6930
    RdbufMove           4           4           4           316         12407       2547
    Rdbuf               4           4           4           308         12443       2552
    AssignIterator      4           5           6           2911        35255       7636
    TellSeekRead        3           3           3           106         1136        250
    CTellSeekRead       3           3           3           110         1151        254
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

    $ g++ --version | head -n1
    g++ (Ubuntu 9.3.0-10ubuntu2) 9.3.0
    $ ./CppFileToStringExperiments
    Doing 1000 iterations of the file read in each benchmark. Output is milliseconds, lower is better.
    Name / Size         100         512         1024        1000000     10000000    Average
    Iterator            8           5           6           3138        35005       7632
    Rdbuf               4           4           4           315         7048        1475
    CTellSeekRead       3           3           3           80          1165        250
    BackInsertIterator  4           6           5           3121        34195       7466
    Eof                 3           3           3           314         10687       2202
    Appending           3           4           3           305         10573       2177
    RdbufMove           4           4           4           303         12134       2489
    AssignIterator      4           5           6           2658        38334       8201
    TellSeekRead        3           3           3           105         1178        258
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A


In 2015 I tried this on GCC 4.9.2, with a mobile i7 on a RAID1 of SSDs with a read speed (per bonnie++) of about 1GB/sec
formatted as EXT4 and got these numbers:

    Doing 1000 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average
    Eof                 3           2           2           835         6021        1372
    RdbufMove           3           3           3           224         4303        907
    BackInsertIterator  3           4           5           2697        27602       6062
    TellSeekRead        3           3           2           104         1490        320
    CTellSeekRead       4           4           4           108         1494        322
    Iterator            3           3           5           2419        28954       6276
    AssignIterator      3           4           5           2502        30140       6530
    Rdbuf               3           3           3           221         7878        1621
    Appending           3           2           2           223         2792        604
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

    Doing 1000 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average
    Iterator            3           4           6           2915        28883       6362
    CTellSeekRead       4           4           4           111         1549        334
    RdbufMove           3           3           3           225         4294        905
    BackInsertIterator  3           4           5           2748        27517       6055
    AssignIterator      4           4           5           2492        30082       6517
    TellSeekRead        3           2           2           104         1486        319
    Appending           3           2           2           216         2832        611
    Rdbuf               3           3           3           221         7774        1600
    Eof                 3           2           2           215         2816        607
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

    Doing 1000 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average
    Rdbuf               4           3           3           714         8055        1755
    Eof                 3           3           2           224         2744        595
    TellSeekRead        3           3           2           107         1539        330
    RdbufMove           3           3           3           221         7916        1629
    Iterator            3           4           5           2438        29119       6313
    CTellSeekRead       4           4           4           113         1533        331
    BackInsertIterator  4           4           5           2693        27213       5983
    AssignIterator      4           4           5           2496        28808       6263
    Appending           3           3           3           215         2852        615
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

    Doing 1000 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average
    Eof                 4           3           2           832         6073        1382
    RdbufMove           3           3           3           230         4310        909
    BackInsertIterator  4           4           6           2724        27629       6073
    TellSeekRead        3           3           3           108         1489        321
    Appending           3           3           3           224         2767        600
    CTellSeekRead       4           4           4           110         1501        324
    Iterator            3           4           5           2435        29203       6330
    Rdbuf               4           3           3           226         7806        1608
    AssignIterator      3           4           5           2508        30964       6696
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A
