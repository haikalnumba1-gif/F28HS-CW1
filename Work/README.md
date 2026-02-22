Author: Haikal Hafiz
Task Entry: H00493444: HQHEX REFLECT HIST

To compile and build program using terminal:
>> CMD make

To remove built program:
>> CMD: make clean

To run program:
>> CMD: sudo ./process FILEIN FILEOUT

( FILEIN ) 
- Mandatory - 
The input filename.
Must already exist.

( FILEOUT ) 
- Mandatory - 
The output filename. 
If exists: Overried file.
If not exists: Create file.

To convert hqhex to ppm viewable image:
>> CMD: python3 hqconvert -f PPM FILEIN FILEOUT

( FILEIN ) 
- Mandatory - 
The input filename.
Must already exist.

( FILEOUT ) 
- Mandatory - 
The output filename. 
If exists: Overried file.
If not exists: Create file.