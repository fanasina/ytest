# FTEST_C
C library like `gtest / gmock`  like, but works mainly with functions 

## env
linux

## test examples
`test/is_good.c`

## compile and run
```
make
./launch_is_good_m
```
or
```
chmod +x compile.sh
./compile.sh 
./launch_is_good_c
```
## some compile options
### if need debug print
`./compile "-D DEBUG=1"`
### if need gdb
`./compile "-g"`
### if need prompt googletest like
`./compile "-D HK"`

We can combine these options, for example: `./compile "-D DEBUG=1 -D HK -g"`

## launch options if using run_all_tests_args(argc, argv);
By default `./launch_is_good_{c,m}` is on 1 thread but we can add some options to run tests in parallel, or chanche colors, to disable progress bar, ..., to print help

### help
`./launch_is_good_c -h`
```
usage: ./launch_is_good_c [OPTIONS] [<ARGS>] 

  or : ./launch_is_good_c [OPTIONS]=[<ARGS>]

OPTIONS
	 -h, --help 
		print help, options variables

	 -p <NB>, --parallel <NB>, -p=<NB>, --parallel=<NB>
		by default the program ran in sequantial all test, 
		if this option is set, the program run tests on NB threads.
		Each thread pull up one test out the list of all test not yet executed,
		and execute it, until the list is empty 

	 -t <unit>, --time <unit>, -t=<unit>, --time=<unit>  
		by default unit is millisecons ms, the other of unit are choices are second (or s), and nanosecond (or ns)
		ex: -t ns or -t=nanosecond or --time=n to set nanosecond unit

	 -u , --unicolour
		by default, the result is colored, if you choice this option, it prints with default color

	 -r , --remove
		if the option parallel is choosen the result on each thread is record in separate files,
		this option remove the file logs of each thread after all tests.

	 -s <file>, --savelog <file>, -s=file, --savelog=file
		this option save the global ordered result in 'file',
		  

	 -n=<NUM1>,<NUM2> <NUM3>... ,--numtests=<NUM1>,<NUM2>...
		this option allow to execute only the selected numbers of tests (in the order in file test)
		ex: -n=0,6,3 8 to execute   the tests 0,3,6,8 (if the number is less than the count of all tests)

	 -l=<NAME1>,<NAME2> <NAME3>... ,--listests=<NAME1>,<NAME2>...<NAMEn>
		this option allow to execute only the selected name of tests. It allows empty name by using '-l=,'
		ex:  -l=name0,,name2 : execute only (if they exist): TEST(name0),TEST(),TEST(name2)

	 -b <BPRGSS>, --bar_progress <BPRGSS>, -b=BPRGSS, --bar_progress=BPRGSS. Example: -b="#_c"
		this option change progression bar if it is active. The first character ('#') fills the bar
		the second char ('_') fills the other part of bar. the bar is colored if the 3rd char is 'c' and not if different.
		by default the progress bar is active and the option is -b="  c", if need not colored, we can put -b="  n" option.

	 -z=<option> 
		this option is to set option=0,
		for example, -z=progress is to not load progress bar, it is need if we want to redirect (pipe) the result to file.
		other option: -z=log_parallel (to avoid logs not ordered when parallel tests which is loged by default)
```

for example:
```
./launch_is_good_c -p 4 -r -u
```
launch the tests on 4 threads, using unicolor(black&white), and remove logs when all tests are done.



