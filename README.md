# CppFileToStringExperiments
A simple set of experiments to see if there is a performance difference in different ways of reading files to memory

This was inspired by this Stackoverflow question:
http://stackoverflow.com/questions/32169936/optimal-way-of-reading-a-complete-file-to-a-string-using-fstream

This should be easy to build on any C++14 and most C++11 you can either use CMake to produce make or project files,
or you can simply compile both source files with the same command or project.

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

  Fastest - TellSeekRead and CTellSeekRead- These have the system provide an easy to get the size and reads the file in one go.

  Faster - Getline Appending and Eof - The checking of chars does not seem to impose any cost.

  Fast - RdbufMove and Rdbuf -  The std::move seems to make no difference in release.

  Slow - Iterator, BackInsertIterator and AssignIterator - Something is wrong with iterators and input streams. The work great in memory, but not here.


I tried this on GCC 4.9.2, with a mobile i7 on a RAID1 of SSDs pulling about 1GB/sec formatted as EXT4 and
got these numbers:


    Doing 1000 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average
    Appending           3           2           2           828         5616        1290
    RdbufMove           3           3           3           223         4235        893
    Eof                 3           2           2           222         2772        600
    TellSeekRead        3           3           2           105         1493        321
    Iterator            3           3           5           2429        27851       6058
    Rdbuf               3           3           3           219         6705        1386
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

    Doing 1000 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average
    Appending           5           4           4           844         5233        1218
    TellSeekRead        3           3           3           109         1508        325
    Eof                 4           3           2           217         2767        598
    Rdbuf               4           3           3           241         6761        1402
    RdbufMove           4           3           3           246         6732        1397
    Iterator            3           4           5           2429        27909       6070
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

    Doing 1000 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average
    Rdbuf               3           3           3           707         8182        1779
    TellSeekRead        2           2           3           111         1508        325
    Appending           3           2           2           215         2762        596
    Eof                 3           2           2           216         2783        601
    RdbufMove           3           2           3           218         8136        1672
    Iterator            3           3           5           2424        29112       6309
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

    Doing 1000 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average
    Eof                 3           2           3           871         5815        1338
    Rdbuf               3           3           3           225         4270        900
    Appending           3           2           2           220         2737        592
    TellSeekRead        3           3           2           108         1503        323
    RdbufMove           3           2           3           224         8153        1677
    Iterator            3           4           5           2697        30097       6561
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

    Doing 1000 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average
    TellSeekRead        5           3           3           107         1488        321
    Eof                 3           2           2           216         5788        1202
    Iterator            3           4           5           2432        29148       6318
    Rdbuf               3           3           3           225         8191        1685
    RdbufMove           3           3           3           220         8259        1697
    Appending           3           3           3           226         2767        600
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A
