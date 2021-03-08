FROM gcc:9.3.0

RUN apt-get update && apt-get upgrade -y && apt-get install -y cmake python3-pip

RUN pip3 install gcovr


