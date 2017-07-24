/*
 *    Copyright 2017 Steve McKay
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 */

#include <aws/core/Aws.h>
#include <aws/sqs/SQSClient.h>
#include <aws/sqs/model/SendMessageBatchRequest.h>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>
#include <boost/thread/thread.hpp>
#include <fstream>

#include "tg_joiner.h"

namespace po = boost::program_options;

int main(int argc, const char *const *argv) {
    unsigned int worker_count;
    std::string queue_url, msg_filename, region;

    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("threads", po::value(&worker_count)->default_value(boost::thread::hardware_concurrency()),
             "SQS worker thread count")
            ("msg-file", po::value(&msg_filename)->required(), "Path to message file")
            ("region", po::value(&region)->default_value("us-east-1"), "AWS region to use")
            ("queue-url", po::value(&queue_url)->required(), "URL of SQS queue to load");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    try {
        po::notify(vm);
    } catch (const po::error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }


    Aws::SDKOptions options;
    options.httpOptions.installSigPipeHandler = true;
    Aws::InitAPI(options);

    Aws::Client::ClientConfiguration client_config;
    client_config.region = region.c_str();
    Aws::SQS::SQSClient sqs(client_config);

    boost::asio::io_service io_service(worker_count);
    boost::thread_group thread_group;
    long message_count = 0;
    std::atomic_uint_fast64_t sent;
    {
        tg_joiner joiner(thread_group);
        boost::asio::io_service::work work(io_service);

        for (int i = 0; i < worker_count; i++) {
            thread_group.create_thread([&io_service]() { io_service.run(); });
        }

        std::ifstream infile(msg_filename);
        do {
            auto lines = std::make_shared<std::vector<std::string>>();
            while (infile.good() && lines->size() < 10) {
                std::string line;
                std::getline(infile, line);
                if (line.length() == 0 || line.at(0) == '#') {
                    continue;
                }
                lines->push_back(std::move(line));
            }

            if (lines->empty()) {
                break;
            }

            io_service.post([lines, &queue_url, &sqs, &sent]() {
                Aws::SQS::Model::SendMessageBatchRequest smb_req;
                smb_req.SetQueueUrl(queue_url.c_str());
                for (int i = 0; i < lines->size(); ++i) {
                    auto&& line = lines->at(i);
                    std::ostringstream id_stream;
                    id_stream << i;

                    Aws::SQS::Model::SendMessageBatchRequestEntry entry;
                    entry.SetId(id_stream.str().c_str());
                    entry.SetMessageBody(line.c_str());
                    smb_req.AddEntries(std::move(entry));
                }

                auto result = sqs.SendMessageBatch(smb_req);
                if (!result.IsSuccess()) {
                    auto err = result.GetError();
                    std::cerr << "Send message batch failed, code " << static_cast<int>(err.GetResponseCode()) << ": " << err.GetMessage() << std::endl;
                    for (auto&& line : *lines) {
                        std::cerr << "Message was " << line.c_str() << std::endl;
                    }
                } else {
                    auto v = sent.fetch_add(lines->size(), std::memory_order_relaxed) + lines->size();
                    if (v % 1000 == 0) {
                        std::cout << "Sent " << v << " messages" << std::endl;
                    }
                }
            });

            message_count += lines->size();
            if (message_count % 1000 == 0) {
                std::cout << "Read " << message_count << " messages" << std::endl;
            }
        } while (infile.good());

        if (message_count % 1000 != 0) {
            std::cout << "Read " << message_count << " messages" << std::endl;
        }
    }

    auto v = sent.load(std::memory_order_relaxed);
    if (v % 1000 != 0) {
        std::cout << "Sent " << v << " messages" << std::endl;
    }

    Aws::ShutdownAPI(options);
    return 0;
}
