# libunclassified

cxx utility library for stuff that didn't find a place elsewhere.

## overview

| source           | function                                                                      |
|------------------|-------------------------------------------------------------------------------|
| `bytestring.hxx` | non-owning `std::byte*`, `std::size_t` wrapper                                |
| `nanolog.hxx`    | asynchronous low-latency logging                                              |
| `nanomsg.hxx`    | ipc using `ring.hxx`                                                          |
| `femtolog.hxx`   | asynchronous logging ( active object pattern )                                |
| `bencode.hxx`    | non-allocating [bencode](https://en.wikipedia.org/wiki/Bencode) encode&decode |
| `format.hxx`     | lightweight `timestamp`, `itoa` and `dtoa` ( used in nanolog )                |
| `ring.hxx`       | multi-producer, multi-consumer yolo ringbuffer                                |

## support & blame-game

magenbluten < mb [at] 64k.by > :: <https://64k.by>
