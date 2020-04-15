# NYGMA: Not Your GrandMa's Applicance ( for Network Packet Processing )

the code you find here is probably horrible. the is no excuse to that. you're
help is needed ...

## RIOT ( Rapid Indexing of Network Traffic )

features & current status:

- [x] indexing of existing pcap's inplace
    - [x] provide indexing method for ipv4 addresses
    - [x] provide indexing method for ipv6 addresses
    - [x] provide indexing method for ports
- [x] provide means of querying the index
- [x] re-assemble a pcap given an index query ( and the original pcap )
- [ ] lightweight query engine supporting SIMD accelerated union and intersection ops
- [ ] ... fun ...

## FPCC ( Full Packet Capture / Caching )

- [ ] combined full packet capture / caching & indexing
- [ ] custom storage format for compressed packet captures ( CCAP )

## CCAP ( Compressed Capture )

a storage format suited for network packets and indexing

- [ ] full 64bit timestamps in nanosecond resolution
- [ ] block oriented and optimized for indexing
- [ ] integer compression
- [ ] stores capture ports for each packet

## support & blame-game

magenbluten < mb [ at ] 64k.by > :: <https://64k.by>
