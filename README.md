# jsolve
### Overview
##### What is the goal?

My goal is to implement an LP solver that can solve all the NETLIB models.

##### Why do this?

1. To increase my understanding of the Simplex method 
2. To increase my understanding of commercial LP solvers
3. To test out modern C++ features

##### How?

I am gradually adding complexity to the implementation while working through "Linear Programming" (Vanderbei, 2020). I have also tried to make the code match the mathematical notation in the text as much as possible (at the cost of performance) to make it easier to understand.

### Status

My process has been:
1. &#9745; Standard primal algorithm (i.e. full dictionary)
2. &#9745; Two phase standard primal algorithm (i.e. handle infeasible starting bases)
3. &#9745; Revised primal algorithm (i.e. matrix based approach, Gaussian elimination)
4. &#9745; Revised dual algorithm (i.e. matrix based approach, Gaussian elimination)
5. &#9745; Two phase revised algorithm (i.e. combine the above to handle infeasible starting bases)
6. &#9745; Replace Gaussian elimination with LU factorisation
7. &#9745; Re-use an LU factorisation between iterations (using the eta-matrix method)
8. General simplex algorithm (i.e. handle bounded variables in pivoting)

Implementing (8) is a significant undertaking and is not covered in detail in many references.

The current implementation features:
- Two-phase (dual then primal) revised simplex method
- Dantzig's 'largest coefficient' full pricing method
- Variable bounds as explicit constraints
- Dense matrix implementation
- LU factorisation (via Doolittle's method) to avoid explicit matrix inverses
- Re-uses the LU factorisation between iterations using an eta-matrix FTRAN/BTRAN
- Periodically re-calculates the LU factorisation (every 50 iterations seems to work well)
- Parallelised matrix multiplication with std::execution

Potential improvements include:
- Change to a sparse matrix representation
- Implement other pricing methods like steepest edge or Devex
- Implement a basic scaling mechanism
- Implement a basic pre-solve
- Threshold/Markowitz pivoting in the LU factorisation
- Use std::mdspan to avoid copying in pivoting
- Use std::ranges and std::string_view in the MPS parser

### Using jsolve
#### Building and Testing

jsolve requires a c++20 compatible compiler such as:
- g++ 11.3
- MSVC 17.4.3
- clang++ 14.0 (can't compile tests, no std::source_location support)

The solver can be built and tested by running:
```
$ git clone https://github.com/jaxor24/jsolve.git
$ cd jsolve
$ cmake . & make
$ ctest . --verbose
```

#### Running

To run jsolve, specify a logging level and point it at an mps file at the command line:
```
$ jsolve_app.exe --log <trace|debug|info> --mps <path to mps file>
```
You should get an output like this:
```
(Start) Running jsolve
(Start) Reading MPS file "C:\\afiro.mps"
(End) Reading MPS file "C:\\afiro.mps" (0 ms)
Model: AFIRO (MIN, 27 constraints, 32 variables)
(Start) Solving
Starting basis is primal and dual infeasible, starting phase 1 with dummy objective
It      1 Obj       0.0000 DInf:       0.0000 PInf:     -44.0000
It      2 Obj     -44.0000 DInf:       0.0000 PInf:     -23.7160
It      3 Obj     -44.0000 DInf:       0.0000 PInf:    -906.3230
It      4 Obj     -44.0000 DInf:       0.0000 PInf:    -590.8320
It      5 Obj     -44.0000 DInf:       0.0000 PInf:      -0.0000
Restoring objective for phase 2
It      6 Obj       0.0000 DInf:      -1.8000 PInf:      -0.0000
It      7 Obj       0.0000 DInf:      -1.8000 PInf:      -0.0000
It      8 Obj       0.0000 DInf:      -2.5953 PInf:      -0.0000
It      9 Obj       0.0000 DInf:      -6.7046 PInf:      -0.0000
It     10 Obj       0.0000 DInf:      -6.3046 PInf:      -0.0000
It     11 Obj       0.0000 DInf:      -5.9930 PInf:      -0.0000
It     12 Obj       0.0000 DInf:     -18.4877 PInf:      -0.0000
It     13 Obj     290.1573 DInf:      -1.0194 PInf:      -0.0000
It     14 Obj     455.9615 DInf:      -0.4362 PInf:      -0.0000
It     15 Obj     455.9615 DInf:      -1.3962 PInf:      -0.0000
It     16 Obj     455.9615 DInf:      -0.5087 PInf:      -0.0000
It     17 Obj     455.9615 DInf:      -0.3448 PInf:      -0.0000
It     18 Obj     464.7531 DInf:      -0.0000 PInf:      -0.0000
Objective = -464.75 (18 iterations)
(End) Solving (6 ms)
(End) Running jsolve (7 ms)
```
#### Dependencies
I have tried to keep these to a minimum (and implement all the numerical code myself):
- [catch2](https://github.com/catchorg/Catch2) for unit testing
- [spdlog](https://github.com/gabime/spdlog) for logging
- [google benchmark](https://github.com/google/benchmark) for profiling and benchmarking
- [commandline](https://schneegans.github.io/tutorials/2019/08/06/commandline) for a the CLI

### Results
I am using the classic [Netlib](https://netlib.org/) set of models to measure progress.

| #  | Name     | Rows | Cols  | Non-zeros | Optimal     | jsolve (Iterations) | jsolve (Seconds) |
| -- | -------- | ---- | ----- | --------- | ----------- | ------------------- | ---------------- |
| 4  | AFIRO    | 28   | 32    | 88        | \-464.75    | 18                  | 0.02             |
| 57 | SC50B    | 51   | 48    | 119       | \-70        | 62                  | 0.05             |
| 56 | SC50A    | 51   | 48    | 131       | \-64.58     | 53                  | 0.04             |
| 54 | SC105    | 106  | 103   | 281       | \-52.2      | 129                 | 0.31             |
| 42 | KB2      | 44   | 41    | 291       | \-1749.9    | 115                 | 1.06             |
| 3  | ADLITTLE | 57   | 97    | 465       | 225494.96   | 129                 | 0.15             |
| 85 | STOCFOR1 | 118  | 111   | 474       | \-41131.98  | 179                 | 0.65             |
| 10 | BLEND    | 75   | 83    | 521       | \-30.81     | 289                 | 0.55             |
| 55 | SC205    | 206  | 203   | 552       | \-52.2      | 293                 | 2.37             |
| 59 | SCAGR7   | 130  | 140   | 553       | \-2331389.3 | 249                 | 1.15             |
| 73 | SHARE2B  | 97   | 79    | 730       | \-415.73    | 164                 | 0.26             |
| 53 | RECIPE   | 92   | 180   | 752       | \-266.62    | 98                  | 0.89             |
| 88 | VTP.BASE | 199  | 203   | 914       | 129831.46   | 450                 | 5.60             |
| 43 | LOTFI    | 154  | 308   | 1086      | \-25.26     | 385                 | 2.76             |
| 72 | SHARE1B  | 118  | 225   | 1182      | \-76589.32  | 608                 | 2.87             |
| 14 | BOEING2  | 167  | 143   | 1339      | \-315.02    | mps ranges          |                  |
| 15 | BORE3D   | 234  | 315   | 1525      | 1373.08     | 304                 | 6.56             |
| 63 | SCORPION | 389  | 358   | 1708      | 1878.12     | 378                 | 14.4             |
| 17 | CAPRI    | 272  | 353   | 1786      | 2690.01     | 497                 | 17.8             |
| 58 | SCAGR25  | 472  | 500   | 2029      | \-14753433  | 1327                | 81.1             |
| 68 | SCTAP1   | 301  | 480   | 2052      | 1412.25     | 670                 | 10.8             |
| 16 | BRANDY   | 221  | 249   | 2150      | 1518.51     | 821                 | 11.5             |
| 41 | ISRAEL   | 175  | 142   | 2358      | \-896644.82 | 334                 | 1.13             |
| 26 | ETAMACRO | 401  | 688   | 2489      | \-755.72    | 2735                | 369              |
| 5  | AGG      | 489  | 163   | 2541      | \-35991767  | 158                 | 5.54             |
| 60 | SCFXM1   | 331  | 457   | 2612      | 18416.76    | 563                 | 18.4             |
| 40 | GROW7    | 141  | 301   | 2633      | \-47787812  | 317                 | 11.2             |
| 8  | BANDM    | 306  | 472   | 2659      | \-158.63    | 1490                | 51.9             |
| 28 | FINNIS   | 498  | 614   | 2714      | 172790.97   | 1493                | 98.7             |
| 25 | E226     | 224  | 282   | 2767      | \-11.64     | 810                 | 8.89             |
| 83 | STANDATA | 360  | 1075  | 3038      | 1257.7      | 100                 | 4.81             |
| 65 | SCSD1    | 78   | 760   | 3148      | 8.67        | 148                 | 7.68             |
| 35 | GFRD-PNC | 617  | 1092  | 3467      | 6902236     | 681                 | 270              |
| 9  | BEACONFD | 174  | 262   | 3476      | 33592.49    | 181                 | 1.71             |
| 84 | STANDMPS | 468  | 1075  | 3686      | 1406.02     | 267                 | 33.2             |
| 82 | STAIR    | 357  | 467   | 3857      | \-251.27    | 968                 | 41.1             |
| 13 | BOEING1  | 351  | 384   | 3865      | \-335.21    | mps ranges          |                  |
| 64 | SCRS8    | 491  | 1169  | 4029      | 904.3       | 1559                | 142              |
| 46 | MODSZK1  | 688  | 1620  | 4158      | 320.62      | timeout             |                  |
| 22 | DEGEN2   | 445  | 534   | 4449      | \-1435.18   | 4928                | 263              |
| 6  | AGG2     | 517  | 302   | 4515      | \-20239252  | 155                 | 7.63             |
| 87 | TUFF     | 334  | 587   | 4523      | 0.29        | error               |                  |
| 7  | AGG3     | 517  | 302   | 4531      | 10312115.9  | 170                 | 7.85             |
| 71 | SEBA     | 516  | 1028  | 4874      | 15711.6     | mps ranges          |                  |
| 74 | SHELL    | 537  | 1775  | 4900      | 1208825346  | timeout             |                  |
| 33 | FORPLAN  | 162  | 421   | 4916      | \-664.22    | mps spaces          |                  |
| 50 | PILOT4   | 411  | 1000  | 5145      | \-2581.14   | mps bounds          |                  |
| 61 | SCFXM2   | 661  | 914   | 5229      | 36660.26    | 1312                | 162              |
| 38 | GROW15   | 301  | 645   | 5665      | \-106870941 | wrong soln          |                  |
| 66 | SCSD6    | 148  | 1350  | 5666      | 50.5        | 417                 | 6.02             |
| 76 | SHIP04S  | 403  | 1458  | 5810      | 1798714.7   | 389                 | 24.4             |
| 48 | PEROLD   | 626  | 1376  | 6026      | \-9380.76   | timeout             |                  |
| 11 | BNL1     | 644  | 1175  | 6129      | 1977.63     | error               |                  |
| 27 | FFFFF800 | 525  | 854   | 6235      | 555679.61   | error               |                  |
| 34 | GANGES   | 1310 | 1681  | 7021      | \-109586.36 | timeout             |                  |
| 62 | SCFXM3   | 991  | 1371  | 7846      | 54901.25    | timeout             |                  |
| 69 | SCTAP2   | 1091 | 1880  | 8124      | 1724.81     | 779                 | 421              |
| 39 | GROW22   | 441  | 946   | 8318      | \-160834336 | 1310                | 1047             |
| 75 | SHIP04L  | 403  | 2118  | 8450      | 1793324.54  | 381                 | 30.3             |
| 81 | SIERRA   | 1228 | 2036  | 9252      | 15394362.2  | error               |                  |
| 86 | STOCFOR2 | 2158 | 2031  | 9492      | \-39024.41  | timeout             |                  |
| 78 | SHIP08S  | 779  | 2387  | 9501      | 1920098.21  | 655                 | 222              |
| 44 | MAROS    | 847  | 1443  | 10006     | \-58063.74  | timeout             |                  |
| 70 | SCTAP3   | 1481 | 2480  | 10734     | 1424        | timeout             |                  |
| 30 | FIT1P    | 628  | 1677  | 10894     | 9146.38     | timeout             |                  |
| 80 | SHIP12S  | 1152 | 2763  | 10941     | 1489236.13  | timeout             |                  |
| 1  | 25FV47   | 822  | 1571  | 11127     | 5501.85     | timeout             |                  |
| 67 | SCSD8    | 398  | 2750  | 11334     | 905         | 2431                | 232              |
| 52 | PILOTNOV | 976  | 2172  | 13129     | \-4497.28   | error               |                  |
| 47 | NESM     | 663  | 2923  | 13988     | 14076073    | mps ranges          |                  |
| 19 | CZPROB   | 930  | 3523  | 14173     | 2185196.7   | timeout             |                  |
| 29 | FIT1D    | 25   | 1026  | 14430     | \-9146.38   | 1333                | 238              |
| 12 | BNL2     | 2325 | 3489  | 16124     | 1811.24     | error               |                  |
| 77 | SHIP08L  | 779  | 4283  | 17085     | 1909055.21  | 684                 | 341              |
| 18 | CYCLE    | 1904 | 2857  | 21322     | \-5.23      | timeout             |                  |
| 79 | SHIP12L  | 1152 | 5427  | 21597     | 1470187.92  | timeout             |                  |
| 23 | DEGEN3   | 1504 | 1818  | 26230     | \-987.29    | timeout             |                  |
| 2  | 80BAU3B  | 2263 | 9799  | 29063     | 987232.16   | mps ub < 0          |                  |
| 36 | GREENBEA | 2393 | 5405  | 31499     | \-72462406  | mps fixed           |                  |
| 37 | GREENBEB | 2393 | 5405  | 31499     | \-4302147.6 | timeout             |                  |
| 20 | D2Q06C   | 2172 | 5167  | 35674     | 122784.24   | timeout             |                  |
| 90 | WOODW    | 1099 | 8405  | 37478     | 1.3         | timeout             |                  |
| 24 | DFL001   | 6072 | 12230 | 41873     | 11266400    | timeout             |                  |
| 49 | PILOT    | 1442 | 3652  | 43220     | \-557.4     | timeout             |                  |
| 21 | D6CUBE   | 416  | 6184  | 43888     | 315.49      | 1711                | 364              |
| 32 | FIT2P    | 3001 | 13525 | 60784     | 68464.29    | timeout             |                  |
| 89 | WOOD1P   | 245  | 2594  | 70216     | 1.44        | error               |                  |
| 51 | PILOT87  | 2031 | 4883  | 73804     | 301.71      |                     |                  |
| 31 | FIT2D    | 26   | 10500 | 138018    | \-68464.29  | timeout             |                  |
| 45 | MAROS-R7 | 3137 | 9408  | 151120    | 1497185.17  | timeout             |                  |


### References

There are many excellent references available. I have used these heavily:

- Vanderbei, *Linear Programming*, 2020
- Chvatal, *Linear Programming*, 1983
- Bradley, *Applied Mathematical Programming*, 1977
- Nocedal, *Numerical Optimization*, 2006
- Koberstein, *The Dual Simplex Method, Techniques for a fast and stable implementation*, 2005

Others include:
- Kipp, *Large Scale Linear and Integer Optimization*, 1999
- Michael Saunders, *CME338 Large-Scale Numerical Optimization*, 2019, Stanford University Course Notes
- Robert Bixby, *Implementing The Simplex Method: The Initial Basis*, 1991

Robert Bixby's 2015 talk, "Solving Linear Programs: The Dual Simplex Algorithm", also provides some great insights.
