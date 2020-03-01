FROM alpine:latest
RUN apk add --no-cache libstdc++ libc6-compat
COPY bin/hellocpp /bin/
ENTRYPOINT /bin/hellocpp
