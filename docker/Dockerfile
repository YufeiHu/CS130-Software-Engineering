### Build/test container ###
FROM nuke-server:base as builder

COPY . /usr/src/project
WORKDIR /usr/src/project/build

RUN cmake ..
RUN make
RUN ctest --output-on_failure

WORKDIR /usr/src/project/
RUN if [ ! -f "static/meme/meme.db" ]; then sqlite3 ./static/meme/meme.db "CREATE TABLE meme_info (meme_id INTEGER PRIMARY KEY, top_text TEXT, bottom_text TEXT, image_name TEXT, image_url TEXT);" else echo "sqlite3 database already exists"; fi

### Deploy container ###
FROM ubuntu:latest as deploy

# TODO(!): Copy server output binary to "."
COPY --from=builder /usr/src/project/build/bin/nuke_server .
COPY --from=builder /usr/src/project/server_config .
COPY --from=builder /usr/src/project/static .

RUN apt-get update && apt-get install -y libsqlite3-dev # apt-util warning
RUN mkdir static
RUN mv app static
RUN mv img static
RUN mv text static
RUN mv meme static

# TODO(!): Expose some port(s)
EXPOSE 80

# Use ENTRYPOINT to specify the binary name
# TODO(!): Update with real server name
ENTRYPOINT ["./nuke_server"]

# Use CMD to specify arguments to ENTRYPOINT
# TODO(!): Update with real server args
CMD ["./server_config"]
