# FYDP51

Build process (use the button that has tilda to get ' between pkg to end of opencv) = g++ detection_with_images.cpp -o detect `pkg-config --cflags --libs opencv` -lsqlite3 -std=c++11