pipeline {
	agent {
		label "built-in"
	}
	stages {
		stage("Build document") {
			steps {
				sh("doxygen")
				sh("rm -rf /var/www/milsko-doxygen")
				sh("mv doxygen/html /var/www/milsko-doxygen")
			}
		}
		stage("Build") {
			parallel {
				stage("Build for Linux 64-bit") {
					steps {
						sh("./Makefile.pl --enable-opengl --enable-vulkan --without-vulkan-string-helper")
						sh("make clean")
						sh("make -j4")
					}
				}
				stage("Build for Windows 32-bit") {
					steps {
						sh("./Makefile.pl --enable-opengl --enable-stb-truetype --disable-freetype2 --cross --target=Windows --host=i686-w64-mingw32")
						sh("make clean")
						sh("make -j4")
					}
				}
				stage("Build for Windows 64-bit") {
					steps {
						sh("./Makefile.pl --enable-opengl --enable-stb-truetype --disable-freetype2 --cross --target=Windows --host=x86_64-w64-mingw32")
						sh("make clean")
						sh("make -j4")
					}
				}
			}
		}
	}
}
