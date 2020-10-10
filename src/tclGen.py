# ====================================================
# II prover for Vitis HLS v1.0
#
# Written by Jianyi Cheng
# ====================================================

from __future__ import print_function
import os, fnmatch, datetime, sys, re, glob
import helper as helper

top = sys.argv[1]

buff = []
ftemp=helper.fileOpen("../env.tcl")
for line in ftemp:
	if "#" in line:
		buff.append(line[0:line.find("#")])
	else:
		buff.append(line)
ftemp.close()
for line in buff:
	if "IIP=" in line:
		iip = helper.findPath(line, buff).replace("\n","")
	if "CLANG=" in line:
		clang = helper.findPath(line, buff).replace("\n","")
	if "VHLS=" in line:
		vhls = helper.findPath(line, buff).replace("\n","")
	if "OPT=" in line:
		opt = helper.findPath(line, buff).replace("\n","")

hL = glob.glob(top+'/src/*.h*')
fL = glob.glob(top+'/src/*.c*')

syn = open(top+"/syn.tcl", "w")
cosim = open(top+"/cosim.tcl", "w")

buff = ""
tb = ""
buff = buff + "open_project -reset proj\nset_top "+top+"\nadd_files {"
for h in hL:
	buff = buff + os.path.relpath(h, top) + " "
for f in fL:
	if top+"_tb" not in f:
		buff = buff + os.path.relpath(f, top) + " "
	else:
		tb = os.path.relpath(f, top)
if tb == "":
	helper.warning("No test bench found for "+top)
	buff = buff + "}\n"
else:
	buff = buff + "}\nadd_files -tb {"+tb+"}\n"
buff = buff + "open_solution -reset \"soluion1\"\nset_part {xc7z020clg484-1}\ncreate_clock -period 10 -name default\n"

syn.write(buff)
syn.write("csynth_design\nexit\n")

cosim.write(buff)
cosim.write("csim_design\ncsynth_design\ncosim_design -O -rtl vhdl\nexit\n")

syn.close()
cosim.close()
print("tcl files generated successfully.")
