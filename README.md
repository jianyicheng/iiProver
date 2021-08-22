# IIProver

This repo includes an LLVM pass called iiProver that proves that it is safe to use a certain II with the commercial Vitis HLS tool from Xilinx. 

## Requirements

[Vitis HLS 2020.2](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/introductionvitishls.html)

[Docker](https://docker-curriculum.com) (Recommended)

## Build IIProver

1. Clone the source

```shell
git clone --recursive git@github.com:JianyiCheng/iiProver.git
cd iiProver
```

2. Build the Docker image

```shell
# Check if your Vitis HLS can be found:
ls $YOUR_VHLS_DIR
# You should see the following...
#     DocNav  Vitis  Vivado  xic

# Build docker image by specify your directory of Vitis HLS. 
make build-docker vhls=$YOUR_VHLS_DIR
```

If you do not want to use Docker, you need to manually install dependencies for LLVM and [Boogie](https://github.com/boogie-org/boogie), and export the required environemntal variables manually as illustrated at the end of `Docker/Dockerfile`.

3. Build iiProver
```shell
make build
```

## Try IIProver

Enter the Docker container if you are using Docker:
```shell
make shell
```

To prove a given II works for a loop, run `iiprover`:
```shell
cd examples/top
iiprover top
```

To automatically find the optimal II for a loop, run `iifinder`:
```shell
cd examples/top
iifinder top
```

To verify the results using co-simulation in Vitis HLS, run `vhls-cosim`:
```shell
cd examples/top
vhls-cosim top
```

## Publication

If you use iiProver in your research, please cite [our FPL2021 paper](https://jianyicheng.github.io/papers/ChengFPL21.pdf)

```
@inproceedings{cheng-fpl2021,
 author = {Cheng, Jianyi and Wickerson, John and Constantinides, George A.},
 title = {Exploiting the Correlation between Dependence Distance and Latency in Loop Pipelining for HLS},
 booktitle = {2021 31th International Conference on Field-Programmable Logic and Applications (FPL)},
 year = {2021},
 address = {},
 pages = {},
 numpages = {},
 doi = {},
 publisher = {IEEE},
}
```
