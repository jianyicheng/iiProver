#!make
include ../env.tcl

BENCH = vecTrans loopCond dist_itr quard top
max = 100
min = 1
mode = lite
name = -

default: full

test: # automatically determine an optimal II for the loop
	cd ../src/_build; make
	@$(OPT) -load ../src/_build/boogieGen/libboogieGen.so -boogieGen $(name)/proj/soluion1/.autopilot/db/a.o.3.bc -S > /dev/null  || (echo "Error: syntax check failed."; exit 3)   
	boogie $(name)/output.bpl

full: preprocess
	@python ../src/iiProver.py $(name) $(max) $(min) $(mode) 2>&1 | tee iiProver.log || (echo "Error: iiProver check failed."; exit 3)

cosim: preprocess # verify a schedule is correct by co-simulation
	@python ../src/tclGen.py $(name) || (echo "Error: generate synthesis script failed."; exit 3)
	@cd $(name); $(VHLS) cosim.tcl || (echo "Error: Vitis HLS synthesis failed."; exit 3)
	rm $(name)/syn.tcl $(name)/cosim.tcl

verify: preprocess # verify a schedule is correct by Boogie
	@python ../src/tclGen.py $(name) || (echo "Error: generate synthesis script failed."; exit 3)
	@cd $(name); $(VHLS) syn.tcl || (echo "Error: Vitis HLS synthesis failed."; exit 3)
	@$(OPT) -load ../src/_build/boogieGen/libboogieGen.so -boogieGen $(name)/proj/soluion1/.autopilot/db/a.o.3.bc -S > /dev/null  || (echo "Error: Boogie gen failed."; exit 3)   
	boogie $(name)/output.bpl
	rm $(name)/syn.tcl $(name)/cosim.tcl $(name)/output.bpl 

preprocess:
	@g++ $(name)/src/*.c* || (echo "Error: syntax check failed."; exit 3)
	@rm a.out

clean:
	for dir in $(BENCH); do \
		rm -rf $$dir/proj;\
		rm -f $$dir/*.tcl; \
		rm -f $$dir/vitis_hls.log; \
		rm -f $$dir/output.bpl; \
	done
