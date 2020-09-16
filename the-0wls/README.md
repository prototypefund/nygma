# gotham

this is very much in progress.

  - tetch: regexp indexing using [nygma](https://github.com/stackless-goto/nygma)
  - the-0wls: ad-hoc user interface using imgui

![three-penny-imgui](https://64k.by/assets/nygma.png)

## dependencies

compiler and buildsystem

  - clang10
  - tup

must be distribution / manually provided:

  - boost
      - this is a dependency of intel's hyperscan
  - glfw
  - vulkan

runtime dependencies:

  - tshark

all other dependencies are inlined as git submodules

## support & blame-game

this project is part of [~stackless-goto/nygma]( https://github.com/stackless-goto/nygma )

magenbluten < mb [ at ] 64k.by > :: <https://64k.by>
