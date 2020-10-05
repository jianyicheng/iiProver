#!make
include ../env.tcl

BENCH = vecTrans

default: test

test:
	cd ../src/_build; make
	@$(OPT) -load ../src/_build/boogieGen/libboogieGen.so -boogieGen $(name)/proj/soluion1/.autopilot/db/a.o.3.bc -S > /dev/null  || (echo "Error: syntax check failed."; exit 3)   
	boogie $(name)/output.bpl

sim:
	@gcc $(name)/src/*.c* || (echo "Error: syntax check failed."; exit 3)
	@rm a.out
	@python ../src/tclGen.py $(name) || (echo "Error: generate synthesis script failed."; exit 3)
	@cd $(name); $(VHLS) cosim.tcl || (echo "Error: Vitis HLS synthesis failed."; exit 3)

verify:
	@gcc $(name)/src/*.c* || (echo "Error: syntax check failed."; exit 3)
	@rm a.out
	@python ../src/tclGen.py $(name) || (echo "Error: generate synthesis script failed."; exit 3)
	@cd $(name); $(VHLS) syn.tcl || (echo "Error: Vitis HLS synthesis failed."; exit 3)
	@$(OPT) -load ../src/_build/boogieGen/libboogieGen.so -boogieGen $(name)/proj/soluion1/.autopilot/db/$(name).bc -S > /dev/null
	boogie $(name)/output.bpl

clean:
	for dir in $(BENCH); do \
		rm -rf $$dir/proj;\
		rm -rf $$dir/*.tcl; \
	done
