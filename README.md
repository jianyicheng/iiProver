# IIProver

This repo includes an LLVM pass called iiProver that proves that it is safe to use a certain II with the commercial Vitis HLS tool from Xilinx. 

## Requirements

[Vitis HLS 2020.2](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/introductionvitishls.html)

[Docker](https://docker-curriculum.com)

## Build with Docker

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

3. Build iiProver
```shell
make build
```

## Try IIProver

TODO

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