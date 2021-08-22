user=$(if $(shell id -u),$(shell id -u),9001)
group=$(if $(shell id -g),$(shell id -g),1000)
ii-prover=/workspace
vhls=/tools/Xilinx/2020.2
th=1

# Build iiProver
build-docker: test-docker
	docker run -it -v $(shell pwd):/workspace -v $(vhls):$(vhls) ii-prover8:latest /bin/bash \
	-c "make build"
	echo "iiProver has been installed successfully!"

# Build docker container
test-docker:
	(cd Docker; docker build --build-arg UID=$(user) --build-arg GID=$(group) --build-arg VHLS_PATH=$(vhls) . --tag ii-prover8)

# Enter docker container
shell:
	docker run -it -v $(shell pwd):/workspace -v $(vhls):$(vhls) ii-prover8:latest /bin/bash

# Build LLVM and ii-prover
build: sync
	./scripts/build-ii-prover.sh

# Sync and update submodules
sync:
	git submodule sync
	git submodule update --init --recursive

clean: 
	rm -rf $(ii-prover)/llvm/build
	rm -rf $(ii-prover)/src/build
