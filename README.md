# matching_engine
- input file name: market_data_file.txt
- no error handling, assuming input format 
- assume order id is int
- tested in gcc 4.8.3@centos7 with c++11 enabled

# compile
```
g++ main.cc orderbook.cc -std=c++11
```

# run
```
./a.out
```

# test result for given input: market_data_file.txt
```
Trade - id:0, qty:2, price:1025
Trade - id:1, qty:1, price:1025
=================
ASK
1075: 1(100000)
1050: 10(100003)
1025: 4(100007)

------------

1000: 9(100001) 1(100006)
975: 30(100002)
950:
BID:
=================
```
