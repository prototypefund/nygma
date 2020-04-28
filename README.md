# NYGMA

... here be dragons -- prototype ahead ...

project `nygma` is organized as a set of ( mostly ) header only cxx libraries and an example
console application `ny`.

instead of boring you to death the obligatory animated gif:

## ny ( basic command line demonstration tool for indexing and reassembling pcaps )

```
```

## libnygma ( base functionality and dealing with network packets )

  - [x] rss hashing in software ( using toeplitz hashing )
  - [x] lightweight packet dissector
  - [x] lightweight dns dissector

## libriot ( indexing and index compression using SIMD based integer compression )

Rapid Indexing of Network Traffic ( or data thereof ).

features & current status:

  - [x] indexing of existing pcap's inplace
      - [x] provide indexing method for ipv4 addresses
      - [x] provide indexing method for ipv6 addresses
      - [x] provide indexing method for ports
      - [x] index compression using SIMD bitpacking/streamvbyte
      - [ ] provide indexing for *IOC*s ( regexp that is basically )
  - [x] provide means of querying the index
  - [x] reassemble a pcap given an index query ( and the original pcap )
  - [ ] lightweight query engine supporting SIMD accelerated union and intersection ops
  - [ ] ... fun ...

## libcapture ( TBD )

full packet capture ( or caching ) and compressed capture storage format ( ccap ).

  - [ ] combined full packet capture / caching & indexing on commodity ( server ) hardware
  - [ ] a storage format suited for network packets and indexing
      - [ ] full 64bit timestamps in nanosecond resolution
      - [ ] block oriented and optimized for indexing
      - [ ] integer compression for internal block offsets ...
      - [ ] stores capture port for each packet

## support & blame-game

the code you find here is probably horrible. there is no excuse to that. therefore, your help is
needed ...

magenbluten < mb [ at ] 64k.by > :: <https://64k.by>

## funding

![bmbf](https://64k.by/assets/bmbf-gefoerdert.jpg)
