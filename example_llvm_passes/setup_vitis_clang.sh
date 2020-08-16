if [ -n "${XILINX_VITIS}" ]; then
  if [ -n "${PATH}" ]; then
    export PATH=$XILINX_VITIS/lnx64/tools/clang-3.9-csynth/bin:$PATH
  else
    export PATH=$XILINX_VITIS/lnx64/tools/clang-3.9-csynth/bin
  fi

  if [ -n "${LD_LIBRARY_PATH}" ]; then
    export LD_LIBRARY_PATH=$XILINX_VITIS/lnx64/tools/clang-3.9-csynth/lib:$LD_LIBRARY_PATH
  else
    export LD_LIBRARY_PATH=$XILINX_VITIS/lnx64/tools/clang-3.9-csynth/lib
  fi

  if [ -n "${LIBRARY_PATH}" ]; then
    export LIBRARY_PATH=$XILINX_VITIS/lnx64/tools/clang-3.9-csynth/lib:$LIBRARY_PATH
  else
    export LIBRARY_PATH=$XILINX_VITIS/lnx64/tools/clang-3.9-csynth/lib
  fi

  if [ -n "${CPATH}" ]; then
    export CPATH=$(pwd)/../xilinx-llvm-include:$CPATH
  else
    export CPATH=$(pwd)/../xilinx-llvm-include
  fi
fi

if [ -n "${XILINX_VIVADO}" ]; then
  if [ -n "${PATH}" ]; then
    export PATH=$XILINX_VIVADO/tps/lnx64/binutils-2.26/bin:$XILINX_VIVADO/tps/lnx64/gcc-6.2.0/bin:$PATH
  else
    export PATH=$XILINX_VIVADO/tps/lnx64/binutils-2.26/bin:$XILINX_VIVADO/tps/lnx64/gcc-6.2.0/bin
  fi

  if [ -n "${LD_LIBRARY_PATH}" ]; then
    export LD_LIBRARY_PATH=$XILINX_VIVADO/tps/lnx64/binutils-2.26/lib:$XILINX_VIVADO/tps/lnx64/gcc-6.2.0/lib:$XILINX_VIVADO/tps/lnx64/gcc-6.2.0/lib64:$LD_LIBRARY_PATH
  else
    export LD_LIBRARY_PATH=$XILINX_VIVADO/tps/lnx64/binutils-2.26/lib:$XILINX_VIVADO/tps/lnx64/gcc-6.2.0/lib:$XILINX_VIVADO/tps/lnx64/gcc-6.2.0/lib64
  fi

  if [ -n "${LIBRARY_PATH}" ]; then
    export LIBRARY_PATH=$XILINX_VIVADO/tps/lnx64/binutils-2.26/lib:$XILINX_VIVADO/tps/lnx64/gcc-6.2.0/lib:$XILINX_VIVADO/tps/lnx64/gcc-6.2.0/lib64:$LIBRARY_PATH
  else
    export LIBRARY_PATH=$XILINX_VIVADO/tps/lnx64/binutils-2.26/lib:$XILINX_VIVADO/tps/lnx64/gcc-6.2.0/lib:$XILINX_VIVADO/tps/lnx64/gcc-6.2.0/lib64
  fi

  if [ -n "${CPATH}" ]; then
    export CPATH=$XILINX_VIVADO/tps/lnx64/binutils-2.26/include:$XILINX_VIVADO/tps/lnx64/gcc-6.2.0/include:$CPATH
  else
    export CPATH=$XILINX_VIVADO/tps/lnx64/binutils-2.26/include:$XILINX_VIVADO/tps/lnx64/gcc-6.2.0/include
  fi

  if [ -n "${MANPATH}" ]; then
    export MANPATH=$XILINX_VIVADO/tps/lnx64/binutils-2.26/share/man:$XILINX_VIVADO/tps/lnx64/gcc-6.2.0/share/man:$MANPATH
  else
    export MANPATH=$XILINX_VIVADO/tps/lnx64/binutils-2.26/share/man:$XILINX_VIVADO/tps/lnx64/gcc-6.2.0/share/man
  fi
fi
