# NYGMA

Not Your GrandMa's Applicance for Network Packet Processing.

the code you find here is probably horrible. there is no excuse to that. therefore, your help is
needed ...

`nygma` is organized as a set of ( mostly ) header only cxx libraries.

## libnygma ( base functionality and dealing with pcaps )

- [ ] rss hashing in software ( using toeplitz hashing )
- [ ] lightweight packet dissector
- [ ] permissive dns dissector

## libriot ( indexing and index compression using SIMD based integer compression )

Rapid Indexing of Network Traffic ( or Data thereof ).

features & current status:

- [x] indexing of existing pcap's inplace
    - [x] provide indexing method for ipv4 addresses
    - [x] provide indexing method for ipv6 addresses
    - [x] provide indexing method for ports
- [x] provide means of querying the index
- [x] re-assemble a pcap given an index query ( and the original pcap )
- [ ] lightweight query engine supporting SIMD accelerated union and intersection ops
- [ ] ... fun ...

## libcapture

full packet capture ( or caching ) and compressed capture storage format ( ccap ).

- [ ] combined full packet capture / caching & indexing
- [ ] a storage format suited for network packets and indexing
    - [ ] full 64bit timestamps in nanosecond resolution
    - [ ] block oriented and optimized for indexing
    - [ ] integer compression for internal block offsets ...
    - [ ] stores capture port for each packet

## support & blame-game

magenbluten < mb [ at ] 64k.by > :: <https://64k.by>
