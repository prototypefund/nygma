# NYGMA: Not Your GrandMa's Applicance ( for Network Packet Processing )

the code you find here is probably horrible. the is no excuse to that. you're
help is needed ...

## RIOT ( Rapid Indexing of Network Traffic )

features:

- [ ] index existing pcap's inplace
    - [ ] ip addresses ( v4 & v6 )
    - [ ] tcp/udp ports
- [ ] query index & reassemble pcap
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

magenbluten < mb [ at ] 64k.by > | <https://64k.by>
