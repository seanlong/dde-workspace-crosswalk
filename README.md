dde-workspace-crosswalk
=======================
This a prototype for running Deepin desktop apps(https://github.com/linuxdeepin/dde-workspace) on Crosswalk(https://github.com/seanlong/crosswalk/tree/deepin).

Known issues:

1. Currently the native object pointer is used as ID in RP, this will lead memory leak.
We may need to make host-client synchronization in RP & EP.

2. drag & drop is not work.

3. "dock" is not work as the native GTK+ cario usage.
