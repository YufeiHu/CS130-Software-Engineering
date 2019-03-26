### Build/test container ###
FROM nuke-server:base as builder

COPY . /usr/src/project

WORKDIR /usr/src/project/build
RUN cmake ..
RUN make

WORKDIR /usr/src/project/build_coverage
RUN cmake -DCMAKE_BUILD_TYPE=Coverage ..
RUN make coverage
