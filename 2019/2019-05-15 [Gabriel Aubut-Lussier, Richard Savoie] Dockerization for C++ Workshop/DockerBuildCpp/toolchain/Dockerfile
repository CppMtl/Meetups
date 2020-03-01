FROM gcc
RUN apt-get update \
	&& apt-get install -yq \
			cmake \
			ninja-build \
	&& rm -rf /var/lib/apt/lists/*

ADD build.sh /build.sh
ENTRYPOINT ["/build.sh"]
