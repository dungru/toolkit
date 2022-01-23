FROM debian:buster

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get -qqy update && \
	apt-get install -qqy --no-install-recommends curl \
	python3 python3-distro ssh git cmake gcc g++ debhelper \
    devscripts gnupg zlib1g-dev libz3-4 libxml2-dev libz3-dev \
    libtool-bin build-essential autoconf libtool pkg-config python3-pip python3-setuptools python3-dev libssl-dev wget

