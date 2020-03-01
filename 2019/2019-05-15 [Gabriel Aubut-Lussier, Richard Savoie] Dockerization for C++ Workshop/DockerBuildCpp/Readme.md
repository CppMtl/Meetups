# Installation

1. Install Docker
1. `git clone https://github.com/Dalzhim/DockerBuildCpp.git`
1. `cd DockerBuildCpp`
1. Create the Docker container that holds the toolchain : `docker build -t toolchain toolchain`
1. Execute the toolchain container to build the Hello World application : `docker run --rm -it -v $(pwd)/src:/Sources -v $(pwd)/Build:/Build -v $(pwd)/Install:/Install --user $(id -u):$(id -g) toolchain`
1. Execute the Hello World application : `docker run --rm -it -v $(pwd)/Install:/Install --user $(id -u):$(id -g) --workdir /Install/bin --entrypoint ./hellocpp toolchain`

# Troubleshooting

* If the `docker build -t toolchain toolchain` command generates permission errors, make sure you followed Docker's instructions to enable the use of the `docker` command without using `sudo`. Otherwise, use `sudo docker […]` everywhere.
