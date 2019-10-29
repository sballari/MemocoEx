\ENCODING=ISO-8859-1
\Problem name: 

Minimize
 obj: 9 xa1 + 6 xa2 + 5 xa3 + 7 xb1 + 4 xb2 + 9 xb3 + 4 xc1 + 6 xc2 + 3 xc3
      + 50 ta1 + 50 ta2 + 50 ta3 + 50 tb1 + 50 tb2 + 50 tb3 + 50 tc1 + 50 tc2
      + 50 tc3 + 15 T5 + 0 x20 + 0 x21 + 0 x22 + 0 x23 + 0 x24 + 0 x25 + 0 x26
      + 0 x27 + 0 x28 + 0 x29 + 0 x30 + 0 x31 + 0 x32 + 0 x33 + 0 x34 + 0 x35
      + 0 x36 + 0 x37 + 0 x38
Subject To
 c1:  xa1 + xa2 + xa3 <= 7000
 c2:  xb1 + xb2 + xb3 <= 6000
 c3:  xc1 + xc2 + xc3 <= 4000
 c4:  xa1 + xb1 + xc1 >= 8000
 c5:  xa2 + xb2 + xc2 >= 5000
 c6:  xa3 + xb3 + xc3 >= 4000
 c7:  xa1 - 8000 ta1 <= 0
 c8:  xa2 - 8000 ta2 <= 0
 c9:  xa3 - 8000 ta3 <= 0
 c10: xb1 - 8000 tb1 <= 0
 c11: xb2 - 8000 tb2 <= 0
 c12: xb3 - 8000 tb3 <= 0
 c13: xc1 - 8000 tc1 <= 0
 c14: xc2 - 8000 tc2 <= 0
 c15: xc3 - 8000 tc3 <= 0
 c16: xa1 <= 10000
 c17: xa2 <= 10000
 c18: xa3 <= 10000
 c19: xb1 <= 10000
 c20: xb2 <= 10000
 c21: xb3 <= 10000
 c22: xc1 <= 10000
 c23: xc2 <= 10000
 c24: xc3 <= 10000
 c25: ta1 + ta2 + ta3 + tb1 + tb2 + tb3 + tc1 + tc2 + tc3 - T5 <= 4
Bounds
 0 <= ta1 <= 1
 0 <= ta2 <= 1
 0 <= ta3 <= 1
 0 <= tb1 <= 1
 0 <= tb2 <= 1
 0 <= tb3 <= 1
 0 <= tc1 <= 1
 0 <= tc2 <= 1
 0 <= tc3 <= 1
 0 <= T5 <= 1
Binaries
 ta1  ta2  ta3  tb1  tb2  tb3  tc1  tc2  tc3  T5 
End
