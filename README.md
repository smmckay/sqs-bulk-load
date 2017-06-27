# sqs-bulk-load

[![Build Status](https://travis-ci.org/smmckay/sqs-bulk-load.svg?branch=master)](https://travis-ci.org/smmckay/sqs-bulk-load)

Load a big pile of messages from a file to SQS.

### Building

You will need cmake 3.5+, a compiler supporting C++11 (gcc and clang are tested), zlib, libcurl, openssl (on Linux) and
boost. On Ubuntu Trusty the correct library packages are libboost-all-dev, zlib1g-dev, libssl-dev, and
libcurl4-openssl-dev.
