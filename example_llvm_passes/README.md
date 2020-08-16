# Renamer Pass

Rename the arguments of all functions.

## Installation

### LLVM
- Build your own LLVM if not using Xilinx's specific intrinsics.
[Reference](http://www.llvm.org/docs/GettingStarted.html)

- Or try out with [Xilinx](https://www.xilinx.com/) Tools!
1. Set up Environment.
```
source /proj/xbuilds/{vivado_version}/installs/lin64/Vivado/HEAD/settings64.sh
source /proj/xbuilds/{vitis_version}/installs/lin64/Vitis/HEAD/settings64.sh
source setup_vitis_clang.sh
```

For example,
```
source /proj/xbuilds/2020.1_daily_latest/installs/lin64/Vivado/HEAD/settings64.sh
source /proj/xbuilds/2020.1_daily_latest/installs/lin64/Vitis/HEAD/settings64.sh
source setup_vitis_clang.sh
```

2. Check the Xilinx include files are put in the correct directory
InjectHLS-0423/xilinx-hls-include

## Compile the pass
```
make all
```

## Run the pass
```
opt -load ./LLVMRenamer.so -renamer *.bc
```
