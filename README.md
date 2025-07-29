# FNetShark
Multithreaded read only file server that can only hold a max limit of ~256 MB worth of data.

## How to build
1) Download python3
2) Run vcpkgInstall.py file. 
3) Run the build.py file.
4) Compile the generated solution and run the server.

### TODO
1) Remove the max limit of 256MB by implementing a better memory allocator on heap for loading files upfront.
2) Include the number of bytes in the data packets that you send so the client can reserve all data upfront.
3) Investigate the ideal size of data packets to send in terms of performance.
