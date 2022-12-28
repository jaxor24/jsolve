# jsolve
### Overview
My goal is to implement an LP solver of increasing complexity, and test out modern C++ features.

There are many variations of the algorithm. My plan is:

1. &#9745; Standard primal algorithm (i.e. full dictionary)
2. &#9745; Two phase standard primal algorithm (i.e. handle infeasible starting bases)
3. &#9745; Revised primal algorithm (i.e. matrix based approach, Gaussian elimination)
4. &#9745; Revised dual algorithm (i.e. matrix based approach, Gaussian elimination)
5. &#9745; Two phase revised algorithm (i.e. combine the above to handle infeasible starting bases)
6. &#9745; Replace Gaussian elimination with LU factorisation
7. Re-use (update) LU factorisation between iterations
8. General simplex algorithm (i.e. handle bounded variables)

I have just completed (6) above.

There are many excellent references available. I have used these heavily so far:
- Vanderbei, *Linear Programming*, 2014
- Chvatal, *Linear Programming*, 1983
- Winston, *Operations Research*, 2004
- Bradley, *Applied Mathematical Programming*, 1977

Robert Bixby's 2015 talk, "Solving Linear Programs: The Dual Simplex Algorithm", also provides some great context and insights.

### New C++ Features
Used:
- `std::source_location` to locate MPS files
- `std::ranges` for the range version of functions in `<algorithm>`
- `std::filesystem` to interact with MPS files
- `std::execution` used to parallelize matrix multiplication

Want to use:
- `std::mdspan` to avoid copies within the simplex pivots


### Results

I am using the classic netlib model suite to test my progress.

|    Name    |  Rows |  Cols | Non-zeros |  Bytes  | BR |       Optimal | Alg 2 (Iterations)         | Alg 5 (Iterations)    |
|:----------:|:-----:|:-----:|:---------:|:-------:|:--:|--------------:|--------------:|-----------:|
| 25FV47     | 822   | 1571  | 11127     | 70477   |    |       5501.85 |error|timeout|
| 80BAU3B    | 2263  | 9799  | 29063     | 298952  | B  |     987232.16 |mps lb|mps lb|
| ADLITTLE   | 57    | 97    | 465       | 3690    |    |     225494.96 |159|134|
| AFIRO      | 28    | 32    | 88        | 794     |    |       -464.75 |17|16|
| AGG        | 489   | 163   | 2541      | 21865   |    |  -35991767.29 |133|infeas|
| AGG2       | 517   | 302   | 4515      | 32552   |    |  -20239252.36 |160|infeas|
| AGG3       | 517   | 302   | 4531      | 32570   |    |   10312115.94 |173|infeas|
| BANDM      | 306   | 472   | 2659      | 19460   |    |       -158.63 |1066|timeout|
| BEACONFD   | 174   | 262   | 3476      | 17475   |    |      33592.49 |111|error|
| BLEND      | 75    | 83    | 521       | 3227    |    |        -30.81 |185|320|
| BNL1       | 644   | 1175  | 6129      | 42473   |    |       1977.63 |infeas|timeout|
| BNL2       | 2325  | 3489  | 16124     | 127145  |    |       1811.24 |error|timeout|
| BOEING1    | 351   | 384   | 3865      | 25315   | BR |       -335.21 |mps ranges|mps ranges|
| BOEING2    | 167   | 143   | 1339      | 8761    | BR |       -315.02 |mps ranges|mps ranges|
| BORE3D     | 234   | 315   | 1525      | 13160   | B  |       1373.08 |212|timeout|
| BRANDY     | 221   | 249   | 2150      | 14028   |    |       1518.51 |590|timeout|
| CAPRI      | 272   | 353   | 1786      | 15267   | B  |       2690.01 |566|timeout|
| CYCLE      | 1904  | 2857  | 21322     | 166648  | B  |         -5.23 |error|timeout|
| CZPROB     | 930   | 3523  | 14173     | 92202   | B  |    2185196.70 |6756|timeout|
| D2Q06C     | 2172  | 5167  | 35674     | 258038  |    |     122784.24 |error|timeout|
| D6CUBE     | 416   | 6184  | 43888     | 167633  | B  |        315.49 |timeout|timeout|
| DEGEN2     | 445   | 534   | 4449      | 24657   |    |      -1435.18 |timeout|timeout|
| DEGEN3     | 1504  | 1818  | 26230     | 130252  |    |       -987.29 |timeout|timeout|
| DFL001     | 6072  | 12230 | 41873     | 353192  | B  |   11266400.00 |todo|timeout|
| E226       | 224   | 282   | 2767      | 17749   |    |        -18.75 |mps error|mps error|
| ETAMACRO   | 401   | 688   | 2489      | 21915   | B  |       -755.72 |1370|timeout|
| FFFFF800   | 525   | 854   | 6235      | 39637   |    |     555679.61 |infeas|timeout|
| FINNIS     | 498   | 614   | 2714      | 23847   | B  |     172790.97 |1892|timeout|
| FIT1D      | 25    | 1026  | 14430     | 51734   | B  |      -9146.38 |1333|timeout|
| FIT1P      | 628   | 1677  | 10894     | 65116   | B  |       9146.38 |2467|timeout|
| FIT2D      | 26    | 10500 | 138018    | 482330  | B  |     -68464.29 |timeout|timeout|
| FIT2P      | 3001  | 13525 | 60784     | 439794  | B  |      68464.29 |timeout|timeout|
| FORPLAN    | 162   | 421   | 4916      | 25100   | BR |       -664.22 |mps error|mps error|
| GANGES     | 1310  | 1681  | 7021      | 60191   | B  |    -109586.36 |2082|timeout|
| GFRD-PNC   | 617   | 1092  | 3467      | 24476   | B  |    6902236.00 |error|timeout|
| GREENBEA   | 2393  | 5405  | 31499     | 235711  | B  |  -72462405.91 |mps fixed|mps fixed|
| GREENBEB   | 2393  | 5405  | 31499     | 235739  | B  |   -4302147.61 |todo|timeout|
| GROW15     | 301   | 645   | 5665      | 35041   | B  | -106870941.29 |mps error|mps error|
| GROW22     | 441   | 946   | 8318      | 50789   | B  | -160834336.48 |mps error|mps error|
| GROW7      | 141   | 301   | 2633      | 17043   | B  |  -47787811.82 |mps error|mps error|
| ISRAEL     | 175   | 142   | 2358      | 12109   |    |    -896644.82 |361|348|
| KB2        | 44    | 41    | 291       | 2526    | B  |      -1749.90 |144|175|
| LOTFI      | 154   | 308   | 1086      | 6718    |    |        -25.26 |308|291|
| MAROS      | 847   | 1443  | 10006     | 65906   | B  |     -58063.74 |error|timeout|
| MAROS-R7   | 3137  | 9408  | 151120    | 4812587 |    |    1497185.17 |todo|timeout|
| MODSZK1    | 688   | 1620  | 4158      | 40908   | B  |        320.62 |error|timeout|
| NESM       | 663   | 2923  | 13988     | 117828  | BR |   14076073.04 |mps ranges|mps ranges|
| PEROLD     | 626   | 1376  | 6026      | 47486   | B  |      -9380.76 |infeas|timeout|
| PILOT      | 1442  | 3652  | 43220     | 278593  | B  |       -557.40 |timeout|timeout|
| PILOT4     | 411   | 1000  | 5145      | 40936   | B  |      -2581.14 |mps bounds|mps bounds|
| PILOT87    | 2031  | 4883  | 73804     | 514192  | B  |        301.71 |mps bounds|mps bounds|
| PILOTNOV   | 976   | 2172  | 13129     | 89779   | B  |      -4497.28 |timeout|timeout|
| RECIPE     | 92    | 180   | 752       | 6210    | B  |       -266.62 |108|96|
| SC105      | 106   | 103   | 281       | 3307    |    |        -52.20 |115|120|
| SC205      | 206   | 203   | 552       | 6380    |    |        -52.20 |297|259|
| SC50A      | 51    | 48    | 131       | 1615    |    |        -64.58 |53|55|
| SC50B      | 51    | 48    | 119       | 1567    |    |        -70.00 |59|64|
| SCAGR25    | 472   | 500   | 2029      | 17406   |    |  -14753433.06 |1111|timeout|
| SCAGR7     | 130   | 140   | 553       | 4953    |    |   -2331389.25 |235|248|
| SCFXM1     | 331   | 457   | 2612      | 19078   |    |      18416.76 |581|timeout|
| SCFXM2     | 661   | 914   | 5229      | 37079   |    |      36660.26 |1356|timeout|
| SCFXM3     | 991   | 1371  | 7846      | 53828   |    |      54901.25 |1962|timeout|
| SCORPION   | 389   | 358   | 1708      | 12186   |    |       1878.12 |605|timeout|
| SCRS8      | 491   | 1169  | 4029      | 36760   |    |        904.30 |605|timeout|
| SCSD1      | 78    | 760   | 3148      | 17852   |    |          8.67 |error|150|
| SCSD6      | 148   | 1350  | 5666      | 32161   |    |         50.50 |960|452|
| SCSD8      | 398   | 2750  | 11334     | 65888   |    |        905.00 |error|timeout|
| SCTAP1     | 301   | 480   | 2052      | 14970   |    |       1412.25 |450|timeout|
| SCTAP2     | 1091  | 1880  | 8124      | 57479   |    |       1724.81 |1667|timeout|
| SCTAP3     | 1481  | 2480  | 10734     | 78688   |    |       1424.00 |error|timeout|
| SEBA       | 516   | 1028  | 4874      | 38627   | BR |      15711.60 |mps ranges|mps ranges|
| SHARE1B    | 118   | 225   | 1182      | 8380    |    |     -76589.32 |682|infeas|
| SHARE2B    | 97    | 79    | 730       | 4795    |    |       -415.73 |186|174|
| SHELL      | 537   | 1775  | 4900      | 38049   | B  | 1208825346.00 |1975|timeout|
| SHIP04L    | 403   | 2118  | 8450      | 57203   |    |    1793324.54 |667|timeout|
| SHIP04S    | 403   | 1458  | 5810      | 41257   |    |    1798714.70 |530|timeout|
| SHIP08L    | 779   | 4283  | 17085     | 117083  |    |    1909055.21 |1000|timeout|
| SHIP08S    | 779   | 2387  | 9501      | 70093   |    |    1920098.21 |789|timeout|
| SHIP12L    | 1152  | 5427  | 21597     | 146753  |    |    1470187.92 |1863|timeout|
| SHIP12S    | 1152  | 2763  | 10941     | 82527   |    |    1489236.13 |1404|timeout|
| SIERRA     | 1228  | 2036  | 9252      | 76627   | B  |   15394362.18 |infeas|timeout|
| STAIR      | 357   | 467   | 3857      | 27405   | B  |       -251.27 |mps bounds|mps bounds|
| STANDATA   | 360   | 1075  | 3038      | 26135   | B  |       1257.70 |106|110|
| STANDMPS   | 468   | 1075  | 3686      | 29839   | B  |       1406.02 |639|mps bounds|
| STOCFOR1   | 118   | 111   | 474       | 4247    |    |     -41131.98 |136|infeas|
| STOCFOR2   | 2158  | 2031  | 9492      | 79845   |    |     -39024.41 |1645|timeout|
| TUFF       | 334   | 587   | 4523      | 29439   | B  |          0.29 |714|timeout|
| VTP.BASE   | 199   | 203   | 914       | 8175    | B  |     129831.46 |mps bounds|mps bounds|
| WOOD1P     | 245   | 2594  | 70216     | 328905  |    |          1.44 |919|timeout|
| WOODW      | 1099  | 8405  | 37478     | 240063  |    |          1.30 |2252|timeout|