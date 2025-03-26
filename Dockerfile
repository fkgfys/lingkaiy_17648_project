FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y build-essential libmicrohttpd-dev pkgconf

WORKDIR /app

COPY vapi.c .

RUN gcc vapi.c -o vapi $(pkg-config --cflags --libs libmicrohttpd)

EXPOSE 8080

CMD ["./vapi"]