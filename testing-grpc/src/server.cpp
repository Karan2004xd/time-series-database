#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include "../build/proto/hello.grpc.pb.h"

class ProcessingImpl : public ProcessingService::Service {

  ::grpc::Status computeSum(::grpc::ServerContext* context, const ::Point* request, ::Numeric* response) override {
    std::cout << "Called!" << std::endl;
    response->set_value(request->x() + request->y() + request->z());
    return grpc::Status::OK;
  }
};

int main() {
  ProcessingImpl service;
  grpc::ServerBuilder builder;
  builder.AddListeningPort("0.0.0.0:9999", grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  server->Wait();
  return 0;
}
