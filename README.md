# sqs-bulk-load

[![Build Status](https://travis-ci.org/smmckay/sqs-bulk-load.svg?branch=master)](https://travis-ci.org/smmckay/sqs-bulk-load)

Load a big pile of messages from a file to SQS.

### Building

You will need cmake 3.5+, a compiler supporting C++11 (gcc and clang are tested), zlib, libcurl, openssl (on Linux) and
boost. On Ubuntu Trusty and Xenial the correct library packages are libboost-all-dev, zlib1g-dev, libssl-dev, and
libcurl4-openssl-dev.

### Running

Not much to say, you need a file with messages (one per line), and an SQS queue to put messages into. --help goes into more 
detail. AWS credentials are sourced from the default provider chain, so if awscli works without auth options this should too.
