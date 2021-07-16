FROM gcc:9.3.0

RUN apt-get update && apt-get install -y \ 
    cmake \
    python3-pip \
    graphviz \
    doxygen \
    git \
    && rm -rf /var/lib/apt/lists/*

RUN pip3 install gcovr

ENV C_EMB_UTILS_DEV_ENV=1

ENTRYPOINT [ "/bin/bash", "-c" ]
