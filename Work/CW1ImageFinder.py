
#H00493444: HQHEX REFLECT HIST

#!/usr/bin/python3
# Run this program, giving student IDs as arguments, e.g.:
#     python3 coursework1-mytasks.py H12345678
import sys
import hashlib
for studentid in sys.argv[1:]:
    h = hashlib.sha256(studentid.encode("ASCII") + b'-2025/26-Jan').digest()
    print(studentid + ':',
          ['HQ8', 'HQ16', 'HQDEC', 'HQHEX'][h[29] % 4],
          ['BLUR', 'MEDIAN', 'REFLECT', 'BRIGHT'][h[30] % 4],
          ['NORM', 'HIST', 'EDGE', 'COMP'][h[31] % 4],
          )
