========================================================================================================================
                                               CODING EXERCISE
========================================================================================================================
Deliverables:
	source code: 
				src/coding_exercise.c
				src/coding_exercise.h
				src/build_command.sh
	prebuilt image:
				prebuilt_image/coding_exercise.dat
	design:
				design_specifications.pdf
	output:
				output/test1.out
				output/test2.out
				output/test3.out
				output/test4.out
				output/test5.out
				output/test6.out
	test_file:
				test/test1.bin
				test/test2.bin
				test/test3.bin
				test/test4.bin
				test/test5.bin
				test/test6.bin
	README.txt


Instructions to build:
	1. copy the src directory to a UNIX enviroment with a gcc compiler.
	2. copy the  "test/test[x].bin" (x - 1,2,3,4,5,6) to the src directory.
	3. run the build_command script with test input file name and test output file name as parameters- for e.g. "./build_command.sh test1.bin test1.out".
	4. the output file is generated as "src/test1.out".


additional information:
	1. This source code has been compiled on UNIX machine.
	2. GNU based gcc compiler is needed.
	3. The output folder has a prebuilt image for the source code.

steps to use the prebuilt image:
	1. copy the "prebuilt_image/coding_exercise.dat" file to a unix envoriment.
	2. rename the "coding_exercise.dat" to "coding_exercise.exe" file.
	3. copy the "test/test[x].bin" (x - 1,2,3,4,5,6) to the same level as the image file.
	4. run the command "./coding_exercise test1.bin test1.out".
	5. output will be generated and saved to file "test1.out".