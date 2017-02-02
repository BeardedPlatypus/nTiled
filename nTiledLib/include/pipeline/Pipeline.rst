.. _Pipeline:

Pipeline
========

The Pipeline module is the heart of `nTiled`, it contains all rendering
functionality, including both Tiled and Clustered shading.

Because the module itself is as big as it is, it has been subdivided in
the following sections

.. toctree::
    :maxdepth: 1

    pipeline/Interfaces
    pipeline/Forward
    pipeline/Deferred
    pipeline/LightManagement
    pipeline/PipelineUtil
    pipeline/Debug

Interfaces contains the Interfaces from which Forward and Deferred inherit.
Forward contains all functionality relating to the Forward Pipeline, and
deferred contains all the functionality relating to the Deferred Pipeline.
LightManagement contains all functionality related to Tiled, Clustered and
Hierarchical shading.
Finally Debug contains the Debug functionality used to display additional
information on top of the rendered scene.

