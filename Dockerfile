FROM gcc:9.3.0

RUN apt-get update && apt-get install -y \ 
    cmake \
    python3-pip \
    graphviz \
    doxygen \
    plantuml \
    astyle \
    gcovr \
    && rm -rf /var/lib/apt/lists/*

ENV CEMB_UTILS_DEV_ENV=1
# Needed to run doxygen properly
ENV PLANTUML_JAR_PATH=/usr/share/plantuml/plantuml.jar

ENTRYPOINT [ "/bin/bash" , "-c" ]
