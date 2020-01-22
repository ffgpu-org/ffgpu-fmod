# ffgpu-fmod

## Description
The functional model (a.k.a C-model) of FFGPU.

ffgpu-fmod is a SystemC model. It can communicate to ffgpu-qemu through DBus IPC.
The Operating Systerm(Linux) sees a pcie device in ffgpu-qemu and any operations on that device are pass to ffgpu-fmod through DBus.

ffgpu-fmod implements a full functional GPU.

## The Purposes of ffgpu-fmod
- As a golden model to RTL Hardware Model.
- As a virtual device implementation for Software Development. (Linux kernel driver/Mesa3d Gallium driver)
- Define a software to hardware protocol for OpenGL API.

## Software Dependencies
- SystemC (SystemC should be built with C++11)
>  We use systemc-2.3.3 and configure with:
>
>  CXXFLAGS=-stdc++-11 ./configure --prefix=/usr/local --with-unix-layout --enable-pthreads
- gtk-3.0
- glib

## Build
	mkdir build && cd build
	PKG_CONFIG_PATH=/usr/local/lib/pkgconfig cmake -DFFGPU_QEMU_DBUS_XML=[the path of org.ffgpu.qemu.xml] .. && make

FFGPU_QEMU_DBUS_XML should be pointed to the qemu dbus interface xml file.
