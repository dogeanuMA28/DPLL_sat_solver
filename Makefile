build:
	gcc -g -O1 -O2 -O3 sat_solver.c -o sat_solver

run:
	./sat_solver $(INPUT) $(OUTPUT)
