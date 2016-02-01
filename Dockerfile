FROM ubuntu:14.04
MAINTAINER Moisés J. <moisesjbc@gmail.com>
RUN apt-get update
RUN export DEBIAN_FRONTEND=noninteractive
COPY build_deps_ubuntu_14_04_x86_64.sh /var/tmp
RUN cd /var/tmp && ./build_deps_ubuntu_14_04_x86_64.sh -y
RUN sudo mv /var/tmp/third-party /opt
