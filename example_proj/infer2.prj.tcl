open_project -reset proj
add_files infer2.cpp
add_files -tb infer2.cpp
set_top example

open_solution -reset solution1
set_part "zynq"
create_clock -period "75MHz"

# Need to be placed before csynth_design
set ::LLVM_CUSTOM_CMD {$LLVM_CUSTOM_OPT -load ../example_llvm_passes/LLVMRenamer.so -mem2reg -analyzer -renamer $LLVM_CUSTOM_INPUT -o $LLVM_CUSTOM_OUPUT}

csim_design
csynth_design
cosim_design

exit
