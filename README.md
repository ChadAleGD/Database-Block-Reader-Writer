# Database-Block-Reader-Writer
C++ Block Read and Writer

A demonstration of a file reader and writer that functions in block format. Use cases for a prototype such as this includes things like databases and file storage. Allows for faster lookup and lower overhead cost without decreasing functionality speed. I added a metadata container to better track and template data to keep sizes of types consistent across each block, as well as store key information about the block and keeping it clean and compact at the start of each block. 

### Usage
This program can be ran through a terminal using the g++ compiler. The following can be done using:
`g++ driver.cpp -o driver`
then
`./driver` or `driver.exe`