==================================================
How Pragma and Attribute Encoded in Xilinx LLVM IR
==================================================

.. contents::
   :local:

.. _loop:

Loop Related
============

Encoded as Metadata: "!llvm.loop" on the br instruction of loop back edge.

NOTE: Please run LLVM -loop-simplify pass to canonicalize the loop before adding
the Metadata.

Loop pipeline
-------------

Pragma options: II, rewind, off

.. code-block:: llvm
  
  !{!"llvm.loop.pipeline.enable", i32 II, i1 isRewind}

* II: -1, means default "II" value. 0, means "off" option presented. > 0, means "II" value from user set
* isRewind: true for is rewind. false for is not rewind.

Loop trip count
---------------

Pragma options: avg, min, max

.. code-block:: llvm
  
  !{!"llvm.loop.tripcount", i32 min, i32 max, i32 avg}

* min: maximum number of loop iterations, 32 bit int literal
* max: minimum number of loop iterations, 32 bit int literal
* avg: average number of loop iterations, 32 bit int literal

Loop unroll
-----------

Pragma options: factor, skip_exit_check

* factor = 0:
.. code-block:: llvm
  
  !{!"llvm.loop.unroll.full"}

* factor = 1:
.. code-block:: llvm
  
  !{!"llvm.loop.unroll.disable"}

* factor > 1:
.. code-block:: llvm
  
  !{!"llvm.loop.unroll.count", i32 factor}

* factor > 1 and skip_exit_check = 1:
.. code-block:: llvm
  
  !{!"llvm.loop.unroll.withoutcheck", i32 factor}

Loop flatten
------------

Pragma options: off

.. code-block:: llvm
  
  !{!"llvm.loop.flatten.enable", i1 isEnabled}

* isEnabled: true for is enabled. false for is not enabled(off).

Parallel loop
-------------

Internal attribute. Equivalent to no inter dependency in a loop.

.. code-block:: llvm
  
  !{!"reflow.parallel.loop"}

.. _function:

Function Related
================

Encoded as Function attribute.

Function pipeline
-----------------

Pragma options: II, enable_flush, rewind, off

.. code-block:: llvm
  
  "fpga.static.pipeline"="i32_II.i1_enable_flush"

* i32_II: II, 32 bit int literal
* i1_enable_flush: 1 for enable flush. 0 for not enabling flush.

Array dimension
---------------

To record the outer most(left most) dimension.

.. code-block:: llvm
  
  "fpga.decayed.dim.hint"=i64_dim"

* i64_dim: outer most dimension. 64 bit unsigned literal.

.. _variable:

Variable Related
================

Encoded as `llvm.sideeffect` intrinsic with operand bundle.

Array partition
---------------

Pragma options: dim, factor, type

.. code-block:: llvm
  
  call void @llvm.sideeffect() [ "xlx_array_partition"(%variable, i32 type, i32 factor, i32 dim) ]

* type: 0 for cyclic, 1 for block, 2 for complete

Resource
--------

Pragma options: core, memory_style, latency, metadata, port_map

memory core
^^^^^^^^^^^^

.. code-block:: llvm
  
  call void @llvm.sideeffect()  [ "xlx_bind_storage"(%variable, i32 666, i32 memory_impl, i32 lateny) ]

* 666 needs to be hard coded. It represents memory core.
* memory_impl: returns by Xilinx's platform API -> memory_impl map.
* lateny: 32 bits int literal. Default latency is "-1"(BackEnd will choose the optimal one).

Dependence
----------

Pragma options: class, dependent, direction, distance, type, variable

.. code-block:: llvm
  
  call @llvm.sideeffect() #1 [ "fpga.dependence"([100 x i32]* %variable, i32 class, i32 compel, i32 direction, i32 distance, i32 type)

* class :  1 for array, 2 for pointer, 0 for no class option.
* compel:  1 for dependent option, 0 for no dependent option.
* direction: 0 for RAW, 1 for WAR, 2 for WAW, -1 for no direction option.
* distance : the inter-iteration distance for array access. 32 bit int literal
* type: 0 for intra, 1 for inter.
