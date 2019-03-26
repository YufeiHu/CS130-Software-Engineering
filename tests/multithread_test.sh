#!/bin/bash

../build/bin/nuke_server ../server_config_test1 &

pid_server=$!
echo "A server starts with PID: $pid_server"
sleep 1

response1=$(echo -e "GET /echo/message HTTP/1.1" | nc -q 1 localhost 8080)
response2=$(echo -e "GET /img/abc HTTP/1.1\r\n\r\n" | nc -q 1 localhost 8080)

ans1=$""
ans2=$"HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 48\r\n\r\n<html><body><h1>Not Found</h1></body></html>\r\n\r"

diff <(echo "$response1") <(echo -e "$ans1")
result1=$?
diff <(echo "$response2") <(echo -e "$ans2")
result2=$?

kill -9 "$pid_server"

if [ $result1 != 0 ] 
then
  echo "Failed with incomplete requests"
  exit 1
fi
if [ $result2 != 0 ]
then
  echo "Failed with complete requests"
  exit 1
fi

echo "Success"
exit 0
