sudo apt-get update
sudo apt-get install clang cmake gcc g++ llvm z3 

# install Boogie
wget https://packages.microsoft.com/config/ubuntu/18.04/packages-microsoft-prod.deb -O packages-microsoft-prod.deb
sudo dpkg -i packages-microsoft-prod.deb
sudo apt-get update; \
  sudo apt-get install -y apt-transport-https && \
  sudo apt-get update && \
  sudo apt-get install -y dotnet-sdk-3.1
sudo apt-get update; \
  sudo apt-get install -y apt-transport-https && \
  sudo apt-get update && \
  sudo apt-get install -y aspnetcore-runtime-3.1
dotnet tool install --global boogie

. env.tcl 

# install llvm-6.0
git clone http://llvm.org/git/llvm.git --branch release_60 --depth 1
#cd llvm/tools                                             
#git clone http://llvm.org/git/clang.git --branch release_60 --depth 1
#cd ..                                                                                   
cd llvm
mkdir _build                                                                                            
cd _build                                                                                               
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DLLVM_INSTALL_UTILS=ON -DLLVM_TARGETS_TO_BUILD="X86" -DCMAKE_INSTALL_
PREFIX=../../llvm-6.0                                                                                   
make -j4
make install 

# install boogie generator
cd ../../src
mkdir _build
cd _build
cmake .. -DLLVM_ROOT=../llvm-6.0                                                                  
make

echo "Installation done."