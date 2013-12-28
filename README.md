LobaFX_cpp
==========

C++ Version of LobaFX

Installation & Usage
==========

1. Under this directory: ./boot.sh
2. mkdir build and cd into it
3. Under build: ../configure
4. Under build: make 2>errlog
5. Under build: cd into src/netapps/lobafx
6. Under lobafx: make 2>errlog and cd ..
7. Under src: ./nox_core -v -i ptcp:8888 lobafx=<app_name>
At present, app_name could be h, l or hl.
8. Startup mininet in the virtual machine
