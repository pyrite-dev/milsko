
Greetings - Welcome to the Milsko GUI Toolkit (Version pre-1.0) 

  This document contains a brief summary of the contents of this source 
distributions and building instructions for Milsko GUI Toolkit. 

                                  Requirements

  Milsko requires the Windows environment with GDI (so anything NT or 9x) or 
the Unix-like environment with X11 for runtime. 

  To build Milsko for Windows, you must have one of following compilers: 
  * Visual C++ 6.0 or newer 
  * Borland C++ 5.5 or newer 
  * Open Watcom 2.0 or newer 
  * MinGW-w64 

  and for Unix-like: 
  * GNU C Compiler 
  * Clang 


                                    Contents

            At the top level of this hierarchy there are seven directories:

            src -      Contains the source for Milsko library

            include -  Contains the headers for Milsko

            doc -      Contains the documentation for Milsko

            external - Contains the external dependency that Milsko uses

            resource - Contains the resources used by maintainers

            tools -    Contains the tools used by maintainers


                                Building Milsko

  Building Milsko depends on the platform you use, and the compiler you use. 

A. Visual C++
-------------

1) Run `nmake -f NTMakefile'. 

B. Borland C++
--------------

1) Run `make -f BorMakefile'. 

C. Open Watcom
--------------

1) Run `wmake -f WatMakefile'. 

D. MinGW-w64/GCC/Clang
----------------------

1) Determine if you need Vulkan and/or OpenGL. 

2) Run `make'. 


                               Makefile settings

          This section exists for people who chose D.

          TARGET        - Target environment, chosen by Makefile in
                          default settings.

          GCC           - Prefix for GCC toolchains. This does not
                          matter if you set CC manually.

          CC            - C compiler.

          DEBUG         - Debug build or not, 0 in default settings.

          VULKAN        - Build Vulkan widget or not.

          OPENGL        - Build OpenGL widget or not.

          USE_STB_IMAGE - Use stb_image or not.

          PREFIX        - Installation destination directory.


                                Makefile targets

          This section exists for people who chose D.

          lib      - Builds Milsko library

          examples - Builds examples

          all      - Builds lib and examples

          install  - Installs Milsko to PREFIX

 -- Nishi  (nishi@nishi.boats) 
