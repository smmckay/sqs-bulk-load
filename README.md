# sqs-bulk-load

[![Build Status](https://travis-ci.org/smmckay/sqs-bulk-load.svg?branch=master)](https://travis-ci.org/smmckay/sqs-bulk-load)

Load a big pile of messages from a file to SQS.

### Building

You will need cmake 2.8+, a compiler supporting C++11 (gcc and clang are tested), zlib, libcurl, and boost. On Ubuntu
Trusty the correct library packages are libboost-all-dev, zlib1g-dev, and libcurl4-gnutls-dev.
