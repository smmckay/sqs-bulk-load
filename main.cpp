#include <aws/core/Aws.h>
#include <aws/sqs/SQSClient.h>
#include <aws/sqs/model/SendMessageRequest.h>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>
#include <boost/thread/thread.hpp>
#include <fstream>

#include "tg_joiner.h"

namespace po = boost::program_options;

int main(int argc, const char *const *argv) {
    int worker_count;
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

    boost::asio::io_service io_service;
    boost::thread_group thread_group;
    long message_count = 0;
    {
        tg_joiner joiner(thread_group);
        boost::asio::io_service::work work(io_service);

        for (int i = 0; i < worker_count; i++) {
            thread_group.create_thread([&io_service]() { io_service.run(); });
        }

        std::ifstream infile(msg_filename);
        while (infile.good()) {
            std::shared_ptr<std::string> line = std::make_shared<std::string>();
            std::getline(infile, *line);
            io_service.post([line, &queue_url, &sqs]() {
                Aws::SQS::Model::SendMessageRequest sm_req;
                sm_req.SetMessageBody((*line).c_str());
                sm_req.SetQueueUrl(queue_url.c_str());
                sqs.SendMessage(sm_req);
            });

            if (++message_count % 1000 == 0) {
                std::cout << "Read " << message_count << " messages" << std::endl;
            }
        }

        std::cout << "Read " << message_count << " messages" << std::endl;
    }

    Aws::ShutdownAPI(options);
    return 0;
}