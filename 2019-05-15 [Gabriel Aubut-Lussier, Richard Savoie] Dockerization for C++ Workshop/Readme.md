# Workshop instructions

This workshop is intended to help you learn how to leverage Docker to run dockerized applications, dockerize your own application and dockerize your build toolchain. This document is a step by step guide meant to help you discover Docker progressively. It is accompanied by sample code that has a non-trivial structure in order to help progress further than what a classic Hello-World example would allow. At any point, feel free to explore something more in depth or to skip exercices. Good luck!


## 1. Getting started (~40 minutes)

By the end of this section, you will have installed Docker and learned how to use it through basic examples that leverage existing containers. This section can be completed prior to the workshop in order to maximize how far you can get during the activity.


### 1.1. Install Docker

Windows : https://hub.docker.com/editions/community/docker-ce-desktop-windows
macOS : https://hub.docker.com/editions/community/docker-ce-desktop-mac
Ubuntu : https://docs.docker.com/install/linux/docker-ce/ubuntu/
Fedora : https://docs.docker.com/install/linux/docker-ce/fedora/
CentOS : https://docs.docker.com/install/linux/docker-ce/centos/
Debian : https://docs.docker.com/install/linux/docker-ce/debian/


### 1.2. Run the official hello-world container

`docker run --rm hello-world`

Hint : Take a few moments to read the container's output.


### 1.3. Run the official bash container

`docker run --rm -it bash`


### 1.4. Find out the meaning of the arguments supplied to the `docker run` command

`docker help run`


### 1.5. Look up the online reference to find out more about the `docker run` command

https://docs.docker.com/engine/reference/commandline/run/


### 1.6. Follow the "Docker For Beginners - Linux" tutorial

https://training.play-with-docker.com/beginner-linux/


### 1.7. Conclusion

By now, you should have a basic understanding of how to use Docker to run applications by leveraging existing containers. You have also had a quick introduction to deriving new containers from existing ones. Now is a good time to explore the official Docker library and look for software you are already using without Docker. You can also explore unofficial images for a wider array of services.

Official images : https://hub.docker.com/search?image_filter=official&type=image
Programming languages images : https://hub.docker.com/search?type=image&category=languages
All images : https://hub.docker.com/search?type=image


## 2. Building a toolchain container

By the end of this section, you will understand how Docker makes it possibles to create an isolated toolchain to build a C++ software.


### 2.1. Take some time to read through the toolchain's Dockerfile

./DockerBuildCpp/toolchain/Dockerfile.


### 2.2. Familiarize with the Dockerfile reference

https://docs.docker.com/engine/reference/builder/


### 2.3. Build the sample toolchain

`docker build -t toolchain ./DockerBuildCpp/toolchain`


### 2.4. Use the toolchain to build sample code

`pushd DockerBuildCpp`
`docker run --rm -it -v $(pwd)/src:/Sources -v $(pwd)/Install:/Install -v $(pwd)/Build:/Build toolchain`
`popd`


### 2.5. Familiarize with the sample application's Dockerfile

./DockerBuildCpp/Install/Dockerfile


### 2.6. Dockerize the sample application

`docker build -t sample-app Install/`


### 2.7. Run the dockerized sample application

`docker run --rm -it sample-app`


### 2.8. Conclusion

By now, you have learned how a toolchain container can be created and used to compile applications. You should understand how this enables reproducible builds and how easy setting up a development environment can become. You have also seen how to dockerize a sample application built with that toolchain and then execute it.


## 3. Using a serious toolchain container : dockcross

By the end of this section, you will have become familiar with dockcross which is a toolchain container to cross-compile to different targets.


### 3.1. Download and run dockcross

`docker pull dockcross/linux-x64:latest`


### 3.2. Create the helper script meant to launch dockcross

```
docker run --rm dockcross/linux-x64:latest > dockcross
chmod +x dockcross
```


### 3.3. Use the toolchain to build sample code

```
./dockcross bash
cd DockerBuildCpp/Build
cmake ../src
make -j 4
exit
```


### 3.4. Run the application

```
./dockcross bash
./DockerBuildCpp/Build/hellocpp
exit
```


### 3.8. Conclusion

By now, you have learned how a toolchain container can be created and used to compile applications. You should understand how this enables reproducible builds and how easy setting up a development environment can become. You have also seen how to dockerize a sample application built with that toolchain and then execute it.


## 4. Cross-compiling to other systems

Docker toolchain containers are extremely useful to make reproducible builds throughout your git history. Cross-compiling from linux to other OSes is another way to make sure all of your builds can be dockerized. Let's try compiling a macOS binary.

In order to cross-compile to other systems, you may use the existing Linux and Windows cross-compilers from the dockcross project. Try compiling a Hello World application for Windows. In order to cross-compile for macOS, things are more complicated because of Apple's licenses. First, you need Apple hardware, and then you can try looking into [osxcross](https://github.com/tpoechtrager/osxcross.git).

## 5. More ideas

# Derive a new toolchain container from dockcross which includes dependencies of a fictive project (make sure one of those dependencies is not up to date). Then, create a feature branch on git to update said library and resolve any "breaking changes". Then merge back into master. Finally, you can experience a `git bisect` where the revisions before and after the dependency change compile without any complications.
# Lookup "Docker in docker". Try the following on a linux system : `docker run --rm -it -v /usr/bin/docker:/usr/bin/docker -v /var/run/docker.sock:/var/run/docker.sock bash`
# Have a look at Kubernetes
