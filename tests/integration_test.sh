#!/bin/bash

../build/bin/nuke_server ../server_config_test1 &
pid_server_1=$!
echo "A server starts with PID: $pid_server_1"
sleep 1

../build/bin/nuke_server ../server_config_test2 &
pid_server_2=$!
echo "A server starts with PID: $pid_server_2"
sleep 1

response1=$(echo -e "GET /\r\n\r\n" | nc -q 1 localhost 8080)
response3=$(echo -e "GET / HTTP/2.0\r\n\r\n" | nc -q 1 localhost 8080)
response4=$(echo -e "GET / HTTP/1.1\r\n\r\n" | nc -q 1 localhost 8080)
response5=$(echo -e "GET /echo HTTP/1.1\r\n\r\n" | nc -q 1 localhost 8080)
response6=$(echo -e "GET /pic HTTP/1.1\r\n\r\n" | nc -q 1 localhost 8080)
response7=$(echo -e "GET /img/abc HTTP/1.1\r\n\r\n" | nc -q 1 localhost 8080)
response8=$(echo -e "GET /img/index.abc HTTP/1.1\r\n\r\n" | nc -q 1 localhost 8080)
response9=$(echo -e "GET /proxy/echo/adlsjfal HTTP/1.1\r\n\r\n" | nc -q 1 localhost 8080)
response10=$(echo -e "GET /health_check_request HTTP/1.1\r\n\r\n" | nc -q 1 localhost 8080)

ans9=$?

ans1=$"HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nContent-Length: 23\r\n\r\nError: bad request!\r\n\r"
ans3=$"HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nContent-Length: 23\r\n\r\nError: bad request!\r\n\r"
ans4=$"HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 48\r\n\r\n<html><body><h1>Not Found</h1></body></html>\r\n\r"
ans5=$"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 23\r\n\r\nGET /echo HTTP/1.1\r\n\r"
ans6=$"HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 48\r\n\r\n<html><body><h1>Not Found</h1></body></html>\r\n\r"
ans7=$"HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 48\r\n\r\n<html><body><h1>Not Found</h1></body></html>\r\n\r"
ans8=$"HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 49\r\n\r\nError: cannot open the file in the given URL!\r\n\r"
ans10=$"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 6\r\n\r\n200 OK"

diff <(echo "$response1") <(echo -e "$ans1")
result1=$?
diff <(echo "$response3") <(echo -e "$ans3")
result3=$?
diff <(echo "$response4") <(echo -e "$ans4")
result4=$?
diff <(echo "$response5") <(echo -e "$ans5")
result5=$?
diff <(echo "$response6") <(echo -e "$ans6")
result6=$?
diff <(echo "$response7") <(echo -e "$ans7")
result7=$?
diff <(echo "$response8") <(echo -e "$ans8")
result8=$?
result9=$ans9
diff <(echo "$response10") <(echo -e "$ans10")
result10=$?

kill -9 "$pid_server_1"
kill -9 "$pid_server_2"

if [ $result1 != 0 ]
then
    echo "Failed with GET /\r\n\r\n"
    exit 1
fi
if [ $result3 != 0 ]
then
    echo "Failed with GET / HTTP/2.0\r\n\r\n"
    exit 1
fi
if [ $result4 != 0 ]
then
    echo "Failed with GET / HTTP/1.1\r\n\r\n"
    exit 1
fi
if [ $result5 != 0 ]
then
    echo "Failed with GET /echo HTTP/1.1\r\n\r\n"
    exit 1
fi
if [ $result6 != 0 ]
then
    echo "Failed with GET /pic HTTP/1.1\r\n\r\n"
    exit 1
fi
if [ $result7 != 0 ]
then
    echo "Failed with GET /img/abc HTTP/1.1\r\n\r\n"
    exit 1
fi
if [ $result8 != 0 ]
then
    echo "Failed with GET /img/index.abc HTTP/1.1\r\n\r\n"
    exit 1
fi
if [ $result9 != 0 ]
then
	echo "Failed with GET /proxy HTTP/1.1\r\n\r\n"
	exit 1
fi
if [ $result10 != 0 ]
then
    echo "Failed with GET /health_check_request HTTP/1.1\r\n\r\n"
    exit 1
fi

echo "Success"
exit 0
