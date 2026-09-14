FROM jenkins/inbound-agent:latest

USER root

RUN apt-get update && apt-get install -y build-essential cmake pkg-config mingw-w64 xz-utils wget \
	    libx11-dev libxrender-dev libfreetype-dev libwayland-dev wayland-protocols libxkbcommon-dev libcairo-dev libdbus-1-dev libgl-dev libvulkan-dev

RUN mkdir -p /usr/watcom && wget -q -O /tmp/ow-snapshot.tar.xz https://github.com/open-watcom/open-watcom-v2/releases/download/Current-build/ow-snapshot.tar.xz
RUN tar xJf /tmp/ow-snapshot.tar.xz -C /usr/watcom && rm -f /tmp/ow-snapshot.tar.xz

ENV PATH=/usr/watcom/binl64:$PATH
ENV WATCOM=/usr/watcom
ENV INCLUDE=/usr/watcom/h

USER jenkins
