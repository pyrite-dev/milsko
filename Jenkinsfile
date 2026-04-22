pipeline {
	agent {
		label "built-in"
	}
	stages {
		stage("Build document") {
			when {
				branch "master"
			}
			agent {
				label "built-in"
			}
			steps {
				sh("rm -rf include/Mw/LowLevel/Wayland")
				sh("doxygen")
				sh("rm -rf /var/www/milsko-doxygen")
				sh("mv doxygen/html /var/www/milsko-doxygen")
			}
			post {
				always {
					notifyDiscord()
				}
			}
		}
		stage("Build") {
			parallel {
				stage("Build for Linux 64-bit") {
					agent {
						label "built-in"
					}
					steps {
						sh("rm -f *.so")
						sh("./configure --enable-opengl --enable-vulkan --without-vulkan-string-helper")
						sh("make clean")
						sh("make -j4")
						sh("mv src/libMw.so libMw64.so")
						archiveArtifacts("libMw64.so")
					}
				}
				stage("Build for Windows 32-bit") {
					agent {
						label "built-in"
					}
					steps {
						sh("rm -f *.dll *.dll.a")
						sh("./configure --enable-opengl --enable-stb-truetype --disable-freetype2 --cross --target=Windows --host=i686-w64-mingw32")
						sh("make clean")
						sh("make -j4")
						sh("mv src/Mw.dll Mw32.dll")
						sh("mv src/libMw.dll.a libMw32.dll.a")
						archiveArtifacts("Mw32.dll,libMw32.dll.a")
					}
				}
				stage("Build for Windows 64-bit") {
					agent {
						label "built-in"
					}
					steps {
						sh("rm -f *.dll *.dll.a")
						sh("./configure --enable-opengl --enable-stb-truetype --disable-freetype2 --cross --target=Windows --host=x86_64-w64-mingw32")
						sh("make clean")
						sh("make -j4")
						sh("mv src/Mw.dll Mw64.dll")
						sh("mv src/libMw.dll.a libMw64.dll.a")
						archiveArtifacts("Mw64.dll,libMw64.dll.a")
					}
				}
				stage("Build for Windows 32-bit (MSVC)") {
					agent {
						label "2012r2"
					}
					steps {
						bat("del /s *.dll *.lib")
						bat("nmake -f NTMakefile clean")
						bat("nmake -f NTMakefile")
						bat("move /y src\\Mw.dll MwMSVC32.dll")
						bat("move /y src\\Mw.lib MwMSVC32.lib")
						archiveArtifacts("MwMSVC32.dll,MwMSVC32.lib")
					}
				}
			}
			post {
				always {
					notifyDiscord()
				}
			}
		}
	}
}
