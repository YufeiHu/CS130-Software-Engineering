#!/bin/bash

../build/bin/nuke_server ../server_config_test1 &
pid_server=$!
echo "A server starts with PID: $pid_server"
sleep 1

# send the first create POST request
response1=$(echo -e "POST /meme/create/form HTTP/1.1\r\nHost: localhost:8080\r\nConnection: keep-alive\r\nContent-Length: 36\r\nCache-Control: max-age=0\r\nUpgrade-Insecure-Requests: 1\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nimage=index.png&top=toptest&bottom=bottomtest\r\n\r\n" | nc -q 1 localhost 8080) &
response2=$(echo -e "POST /meme/create/form HTTP/1.1\r\nHost: localhost:8080\r\nConnection: keep-alive\r\nContent-Length: 36\r\nCache-Control: max-age=0\r\nUpgrade-Insecure-Requests: 1\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nimage=index.jpg&top=longerlongertoptest&bottom=longerlongerbottomtest\r\n\r\n" | nc -q 1 localhost 8080)
sleep 1
response3=$(echo -e "GET /meme/list/message HTTP/1.1\r\n\r\n" | nc -q 1 localhost 8080)

# two situations can randomly occur 
ans3_opt1=$"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 373\r\n\r\n<html><head><title>Meme List</title></head><p> Total Number of Memes: 2</p><table border=\"1\"><tr><th>Meme ID</th><th>Fig Template</th><th>Top Text</th><th>Bottom Text</th></tr><tr><td>0</td><td>index.png</td><td>toptest</td><td>bottomtest\r\n\r\n\r\n</td></tr><tr><td>1</td><td>index.jpg</td><td>longerlongertoptest</td><td>longerlongerbottomtest\r\n\r\n\r\n</td></tr></table><br></html>"
ans3_opt2=$"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 373\r\n\r\n<html><head><title>Meme List</title></head><p> Total Number of Memes: 2</p><table border=\"1\"><tr><th>Meme ID</th><th>Fig Template</th><th>Top Text</th><th>Bottom Text</th></tr><tr><td>0</td><td>index.jpg</td><td>longerlongertoptest</td><td>longerlongerbottomtest\r\n\r\n\r\n</td></tr><tr><td>1</td><td>index.png</td><td>toptest</td><td>bottomtest\r\n\r\n\r\n</td></tr></table><br></html>"
ans3_substr=$"Content-Length: 48"
sleep 1

# echo "*************************************************"
# echo "$response3"

# echo "===compare with opt 1=="
# diff <(echo "$response3") <(echo -e "$ans3_opt1") # should have request
# result1=$?
# echo "===compare with opt 2=="
# diff <(echo "$response3") <(echo -e "$ans3_opt2") # should have request
# result2=$?

kill -9 "$pid_server"

# if [ $result1 != 0 ] && [ $result2 != 0 ]
# then
#     echo "Failed with incomplete requests"
#     exit 1
# fi

# compare with Content-Length
if [[ "$response3" != *"$ans3_substr"* ]]; then
  echo "Failed with incomplete requests"
  exit 1
fi

echo "Success"
exit 0
