Author: Haikal Hafiz
Task Entry: H00493444: HQHEX REFLECT HIST

HQHEX Data:
1. width - Integer number of columns.
2. height - Integer number of rows.
3. nvalues - Integer number of values per pixel. Always 3 (RGB).
Image data follows header.

Pixel Data:
1. Each pixel is 3 unsigned 16-bit value.
2. Each pixel is stored as 12 hexadecimal digits (A-F may be upper/lower case).
3. Maximum individual RGB value: 65535
4. Read left to right, top to bottom.


Example of a HQHEX file:
HQHEX 400 300 3 ffff ffff ffff...More