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

# echo "======================= Run tests ========================"
# cd ../tests && ../build/bin/session_test

echo "================== Starting the server ==================="
cd ..
./build/bin/nuke_server ./server_config_test1

# echo "================ Get test coverage report ================"
# if [ -d "build_coverage" ]; then
#   echo "Deleting the old build_coverage folder......"
#   rm -r build_coverage
# fi
# mkdir build_coverage
# cd build_coverage
# cmake -DCMAKE_BUILD_TYPE=Coverage ..
# make coverage
