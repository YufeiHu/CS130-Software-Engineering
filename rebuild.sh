echo "================= Creating the database =================="
if [ ! -f "static/meme/meme.db" ]; then
  echo "Creating a sqlite3 database......"
  sqlite3 ./static/meme/meme.db "CREATE TABLE meme_info (meme_id INTEGER PRIMARY KEY, top_text TEXT, bottom_text TEXT, image_name TEXT, image_url TEXT);"
else
  echo "sqlite3 database already exists"
fi

echo "================= Building source codes =================="
if [ -d "build" ]; then
  echo "Deleting the old build folder......"
  rm -r build
fi
mkdir build
cd build
cmake ..
make

echo "======================= Run tests ========================"
# cd ../tests && ../build/bin/config_parser_test
# cd ../tests && ../build/bin/session_test
# cd ../tests && ../build/bin/echo_handler_test
# cd ../tests && ../build/bin/file_handler_test
# cd ../tests && ../build/bin/bad_request_handler_test
# cd ../tests && ../build/bin/not_found_handler_test
# cd ../tests && ../build/bin/status_handler_test
# cd ../tests && ../build/bin/server_status_test
# cd ../tests && ../build/bin/request_handler_util_test
# cd ../tests && ../build/bin/meme_create_handler_test
# cd ../tests && ../build/bin/meme_list_handler_test
# cd ../tests && ../build/bin/meme_display_handler_test
# cd ../tests && ../build/bin/proxy_handler_test
cd ..

echo "================ Get test coverage report ================"
if [ -d "build_coverage" ]; then
  echo "Deleting the old build_coverage folder......"
  rm -r build_coverage
fi
mkdir build_coverage
cd build_coverage
cmake -DCMAKE_BUILD_TYPE=Coverage ..
make coverage
