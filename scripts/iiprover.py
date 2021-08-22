# ====================================================
# II Prover for Vitis HLS v1.0
#
# Written by Jianyi Cheng
# ====================================================

from __future__ import print_function
import os, fnmatch, datetime, sys, re, glob
import helper as helper

top = sys.argv[1]
II_MAX = int(sys.argv[2]) if len(sys.argv) > 2 else 100
II_MIN = int(sys.argv[3]) if len(sys.argv) > 3 else 1
mode = sys.argv[4] if len(sys.argv) > 4 else "lite"

if top == "-":
	helper.error("Please specify the name of the benchmark. e.g.\nmake name=vecTrans")
	assert(0)

print(" ==================================================== ")
print(" II prover for Vitis HLS v1.0 ")
print(" Written by Jianyi Cheng, Imperial College London ")
print(" "+str(datetime.datetime.now().strftime("%D %T")))
print(" ==================================================== ")
print("Proving "+top+" from II = "+str(II_MIN)+" to "+str(II_MAX)+"...")
sys.stdout.flush()

# set up the environments
ftemp=helper.fileOpen("../env.tcl")
buff = []
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

# generate the scripts
hL = glob.glob(top+'/src/*.h*')
fL = glob.glob(top+'/src/*.c*')
iterFile = ""
iterFileName = ""
for f in fL:
	ftemp = open(f)
	buff = ""
	for line in ftemp:
		buff += line
	ftemp.close()
	buff = helper.removeComment(buff)
	buff1 = buff.replace(" ", "").replace("\t", "")
	if "#pragmaHLSPIPELINE" in buff1:	# JC: to be changed to clang in the future
		if iterFile != "" or buff1.count("#pragmaHLSPIPELINE") != 1:
			helper.error("Multiple pipelines found - now only support the analysis for a single loop.")
			assert(0)
		else:
			iterFile = f
			iterFileName = os.path.relpath(os.path.splitext(f)[0]+"_"+os.path.splitext(f)[1], top)
			ftemp = open(top+"/"+iterFileName, "w")
			ftemp.write(buff)
			ftemp.close()

def iiProverExit():
	os.system("rm -f "+top+"/"+iterFileName)
	os.system("rm -f "+top+"/iiprover.tcl")
	os.system("rm -f "+top+"/iiprover_.tcl")
	os.system("rm -f "+top+"/output.bpl")
	os.system("rm -f boogie.log")

tcl = open(top+"/iiprover.tcl", "w")
tcl_ = open(top+"/iiprover_.tcl", "w")
buff = ""
buff = buff + "open_project -reset proj\nset_top "+top+"\nadd_files {"
for h in hL:
	buff = buff + os.path.relpath(h, top) + " "
for f in fL:
	if top+"_tb" not in f:
		if iterFile != f:
			buff = buff + os.path.relpath(f, top) + " "
		else:
			buff = buff + iterFileName + " "
	else:
		tb = os.path.relpath(f, top)
buff = buff + "}\n"
buff = buff + "open_solution \"soluion1\"\nset_part {xc7z020clg484-1}\ncreate_clock -period 10 -name default\n"
tcl.write(buff)
if mode == "lite":
	tcl.write("csynth_design\nexit\n")
else:
	tcl.write("config_bind -effort high\ncsynth_design\nexit\n")
tcl.close()
tcl_.write(buff)
tcl_.write("config_bind -effort high\ncsynth_design\nexit\n")
tcl_.close()
print("preprocess successfully.")

# now do preprocess
done = False
ii = II_MIN
while ii < II_MAX:
	os.system("echo \"Synthesising II = "+str(ii)+"...\"")
	buff = []
	ftemp = open(top+"/"+iterFileName)
	for line in ftemp:
		lineTemp = line.replace(" ", "").replace("\t", "")
		if "#pragmaHLSPIPELINE" in lineTemp:
			buff.append("#pragma HLS PIPELINE II="+str(ii)+"\n")
		else:
			buff.append(line)
	ftemp.close()
	ftemp = open(top+"/"+iterFileName, "w")
	for line in buff:
		ftemp.write(line)
	ftemp.close()
	os.system("cd "+top+";"+vhls+" iiprover.tcl > /dev/null")
	os.system(opt+" -load ../src/_build/boogieGen/libboogieGen.so -boogieGen "+top+"/proj/soluion1/.autopilot/db/a.o.3.bc -S > /dev/null")
	os.system("boogie "+top+"/output.bpl > boogie.log")
	buff = ""
	ftemp = open("boogie.log")
	for line in ftemp:
		buff = buff + line
	ftemp.close()
	if "This assertion might not hold" not in buff and "Error" not in buff:
		done = True
	elif "Error" in buff and "This assertion might not hold" not in buff:
		helper.error("Syntax error found in Boogie output")
	if done:
		os.system("echo \"Proof succeeded.\"")
		break;
	os.system("echo \"Proof failed.\"")
	ii = ii + 1

if ii == II_MAX:
	print("Cannot find II smaller than "+str(II_MAX))
else:
	print("Found the optimal II = "+str(ii))

if mode == "lite":
	os.system("echo \"Prove the schedule with resource sharing...\"")
	os.system("cd "+top+";"+vhls+" iiprover.tcl_ > /dev/null")
	os.system(opt+" -load ../src/_build/boogieGen/libboogieGen.so -boogieGen "+top+"/proj/soluion1/.autopilot/db/a.o.3.bc -S > /dev/null")
	os.system("boogie "+top+"/output.bpl > boogie.log")
	done = False
	buff = ""
	ftemp = open("boogie.log")
	for line in ftemp:
		buff = buff + line
	ftemp.close()
	if "This assertion might not hold" not in buff and "Error" not in buff:
		done = True
	elif "Error" in buff and "This assertion might not hold" not in buff:
		helper.error("Syntax error found in Boogie output")
		
if done == False and mode == "lite":
	helper.error("Mismatched schedule with resource sharing.")
	assert(0)

iiProverExit()




