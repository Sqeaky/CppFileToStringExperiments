# CppFileToStringExperiments
A simple set of experiments to see if there is a performance difference in different ways of reading files to memory

This was inspired by this Stackoverflow question:
http://stackoverflow.com/questions/32169936/optimal-way-of-reading-a-complete-file-to-a-string-using-fstream

This should be easy to build on any C++14 and most C++11 you can either use CMake to produce make or project files,
or you can simply compile both source files with the same command or project.

This size of the file tested is in bytes accross the first row of results. Each row of results has nummbers
indicating the milliseconds to read the file 10 times. 10 is hardly foolproof, but still shows us some
interesting trends. To change the test iterations change the variable TestIterations in main.cpp

I tried this on GCC 4.9.2, with a mobile i7 on a RAID1 of SSDs pulling about 1GB/sec formatted as EXT4 and
got these numbers:

Debug numbers:

    Name / Size         100         512         1024        1000000     10000000    100000000   Average     
    Rdbuf               0           0           0           11          125         1018        192         
    Iterator            0           0           0           708         7150        71268       13187       
    Appending           0           0           0           4           47          686         122         
    Eof                 0           0           0           4           46          685         122         
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

    Name / Size         100         512         1024        1000000     10000000    100000000   Average     
    Rdbuf               0           0           0           11          123         1017        191         
    Appending           0           0           0           4           46          676         121         
    Eof                 0           0           0           4           47          688         123         
    Iterator            0           0           0           707         7120        71044       13145       
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A


Release Numbers:

    Name / Size         100         512         1024        1000000     10000000    100000000   Average     
    Appending           0           0           0           13          90          690         132         
    Eof                 0           0           0           4           46          677         121         
    Rdbuf               0           0           0           4           69          967         173         
    Iterator            0           0           0           38          403         4272        785         
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

    Name / Size         100         512         1024        1000000     10000000    100000000   Average     
    Eof                 0           0           0           13          93          691         132         
    Rdbuf               0           0           0           4           68          957         171         
    Iterator            0           0           0           38          403         4252        782         
    Appending           0           0           0           3           43          692         123         
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

In my dozen or so runs of this it seems whatever test runs first suffers about a 10ms penalty. This is likely 
caused by memory allocation. Memory is freed from my program as far as the code I have written, but the compilers
surely realized that could be re-used and did not give it back to the OS.

It appears that anything using std::getline is highly optimized. Whatever checks it is doing have a negligible 
cost. While something is happening with those ifstream_iterators to cause them to run slower than in linear time.
I suspect that on my system at least that the construction of the stringstream has some cost or skipping straight
to the read buffer is skipping some kind of optimization the ifstream uses, perhaps respecting blocksize or CPU
cache size.


Here I upped the iteration count to 500 and dropped of the largest file read. The penalty for running first grows
to around 100% but still doesn't cause either Eof or Appending to fail to the other test, but whichever of those
two runs first is faster. The gaps between the tests increased in general.

    Doing 500 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average     
    Appending           2           2           2           659         4404        1013        
    Eof                 2           2           2           173         4386        913         
    Iterator            2           3           4           1923        23201       5026        
    Rdbuf               2           2           2           175         5970        1230        
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

    Doing 500 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average     
    Rdbuf               2           2           3           555         5897        1291        
    Iterator            2           3           4           1914        23012       4987        
    Eof                 2           2           2           175         2114        459         
    Appending           2           2           2           177         2095        455         
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

    Doing 500 iterations of the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average     
    Appending           2           2           2           651         4415        1014        
    Rdbuf               2           2           2           183         5988        1235        
    Iterator            2           3           4           1916        23180       5021        
    Eof                 2           2           2           176         2088        454         
    To Prevent Really smart compilers from optimizing anything out here is a random char of the intermediary output: A

    Doing 500 iteration sof the file read in each benchmark.
    Name / Size         100         512         1024        1000000     10000000    Average     
    Eof                 2           2           2           649         4453        1021        
    Iterator            2           3           4           1927        23180       5023        
    Rdbuf               3           2           2           177         6031        1243        
    Appending           3           2           2           178         2090        455         
