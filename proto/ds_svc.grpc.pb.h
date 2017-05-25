// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: ds_svc.proto
#ifndef GRPC_ds_5fsvc_2eproto__INCLUDED
#define GRPC_ds_5fsvc_2eproto__INCLUDED

#include "ds_svc.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/proto_utils.h>
#include <grpc++/impl/codegen/rpc_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/status.h>
#include <grpc++/impl/codegen/stub_options.h>
#include <grpc++/impl/codegen/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class RpcService;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace diststore {

class DsService final {
 public:
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status PutFile(::grpc::ClientContext* context, const ::diststore::PutFileRequest& request, ::diststore::PutFileReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::diststore::PutFileReply>> AsyncPutFile(::grpc::ClientContext* context, const ::diststore::PutFileRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::diststore::PutFileReply>>(AsyncPutFileRaw(context, request, cq));
    }
    virtual ::grpc::Status GetFile(::grpc::ClientContext* context, const ::diststore::GetFileRequest& request, ::diststore::GetFileReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::diststore::GetFileReply>> AsyncGetFile(::grpc::ClientContext* context, const ::diststore::GetFileRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::diststore::GetFileReply>>(AsyncGetFileRaw(context, request, cq));
    }
    virtual ::grpc::Status CheckHealth(::grpc::ClientContext* context, const ::diststore::CheckHealthRequest& request, ::diststore::CheckHealthReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::diststore::CheckHealthReply>> AsyncCheckHealth(::grpc::ClientContext* context, const ::diststore::CheckHealthRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::diststore::CheckHealthReply>>(AsyncCheckHealthRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientReaderInterface< ::diststore::HelpReply>> Help(::grpc::ClientContext* context, const ::diststore::HelpRequest& request) {
      return std::unique_ptr< ::grpc::ClientReaderInterface< ::diststore::HelpReply>>(HelpRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::diststore::HelpReply>> AsyncHelp(::grpc::ClientContext* context, const ::diststore::HelpRequest& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::diststore::HelpReply>>(AsyncHelpRaw(context, request, cq, tag));
    }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::diststore::PutFileReply>* AsyncPutFileRaw(::grpc::ClientContext* context, const ::diststore::PutFileRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::diststore::GetFileReply>* AsyncGetFileRaw(::grpc::ClientContext* context, const ::diststore::GetFileRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::diststore::CheckHealthReply>* AsyncCheckHealthRaw(::grpc::ClientContext* context, const ::diststore::CheckHealthRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientReaderInterface< ::diststore::HelpReply>* HelpRaw(::grpc::ClientContext* context, const ::diststore::HelpRequest& request) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::diststore::HelpReply>* AsyncHelpRaw(::grpc::ClientContext* context, const ::diststore::HelpRequest& request, ::grpc::CompletionQueue* cq, void* tag) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status PutFile(::grpc::ClientContext* context, const ::diststore::PutFileRequest& request, ::diststore::PutFileReply* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::diststore::PutFileReply>> AsyncPutFile(::grpc::ClientContext* context, const ::diststore::PutFileRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::diststore::PutFileReply>>(AsyncPutFileRaw(context, request, cq));
    }
    ::grpc::Status GetFile(::grpc::ClientContext* context, const ::diststore::GetFileRequest& request, ::diststore::GetFileReply* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::diststore::GetFileReply>> AsyncGetFile(::grpc::ClientContext* context, const ::diststore::GetFileRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::diststore::GetFileReply>>(AsyncGetFileRaw(context, request, cq));
    }
    ::grpc::Status CheckHealth(::grpc::ClientContext* context, const ::diststore::CheckHealthRequest& request, ::diststore::CheckHealthReply* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::diststore::CheckHealthReply>> AsyncCheckHealth(::grpc::ClientContext* context, const ::diststore::CheckHealthRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::diststore::CheckHealthReply>>(AsyncCheckHealthRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientReader< ::diststore::HelpReply>> Help(::grpc::ClientContext* context, const ::diststore::HelpRequest& request) {
      return std::unique_ptr< ::grpc::ClientReader< ::diststore::HelpReply>>(HelpRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::diststore::HelpReply>> AsyncHelp(::grpc::ClientContext* context, const ::diststore::HelpRequest& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::diststore::HelpReply>>(AsyncHelpRaw(context, request, cq, tag));
    }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    ::grpc::ClientAsyncResponseReader< ::diststore::PutFileReply>* AsyncPutFileRaw(::grpc::ClientContext* context, const ::diststore::PutFileRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::diststore::GetFileReply>* AsyncGetFileRaw(::grpc::ClientContext* context, const ::diststore::GetFileRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::diststore::CheckHealthReply>* AsyncCheckHealthRaw(::grpc::ClientContext* context, const ::diststore::CheckHealthRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientReader< ::diststore::HelpReply>* HelpRaw(::grpc::ClientContext* context, const ::diststore::HelpRequest& request) override;
    ::grpc::ClientAsyncReader< ::diststore::HelpReply>* AsyncHelpRaw(::grpc::ClientContext* context, const ::diststore::HelpRequest& request, ::grpc::CompletionQueue* cq, void* tag) override;
    const ::grpc::RpcMethod rpcmethod_PutFile_;
    const ::grpc::RpcMethod rpcmethod_GetFile_;
    const ::grpc::RpcMethod rpcmethod_CheckHealth_;
    const ::grpc::RpcMethod rpcmethod_Help_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status PutFile(::grpc::ServerContext* context, const ::diststore::PutFileRequest* request, ::diststore::PutFileReply* response);
    virtual ::grpc::Status GetFile(::grpc::ServerContext* context, const ::diststore::GetFileRequest* request, ::diststore::GetFileReply* response);
    virtual ::grpc::Status CheckHealth(::grpc::ServerContext* context, const ::diststore::CheckHealthRequest* request, ::diststore::CheckHealthReply* response);
    virtual ::grpc::Status Help(::grpc::ServerContext* context, const ::diststore::HelpRequest* request, ::grpc::ServerWriter< ::diststore::HelpReply>* writer);
  };
  template <class BaseClass>
  class WithAsyncMethod_PutFile : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_PutFile() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_PutFile() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status PutFile(::grpc::ServerContext* context, const ::diststore::PutFileRequest* request, ::diststore::PutFileReply* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestPutFile(::grpc::ServerContext* context, ::diststore::PutFileRequest* request, ::grpc::ServerAsyncResponseWriter< ::diststore::PutFileReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_GetFile : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_GetFile() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_GetFile() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetFile(::grpc::ServerContext* context, const ::diststore::GetFileRequest* request, ::diststore::GetFileReply* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGetFile(::grpc::ServerContext* context, ::diststore::GetFileRequest* request, ::grpc::ServerAsyncResponseWriter< ::diststore::GetFileReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_CheckHealth : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_CheckHealth() {
      ::grpc::Service::MarkMethodAsync(2);
    }
    ~WithAsyncMethod_CheckHealth() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status CheckHealth(::grpc::ServerContext* context, const ::diststore::CheckHealthRequest* request, ::diststore::CheckHealthReply* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestCheckHealth(::grpc::ServerContext* context, ::diststore::CheckHealthRequest* request, ::grpc::ServerAsyncResponseWriter< ::diststore::CheckHealthReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(2, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_Help : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_Help() {
      ::grpc::Service::MarkMethodAsync(3);
    }
    ~WithAsyncMethod_Help() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Help(::grpc::ServerContext* context, const ::diststore::HelpRequest* request, ::grpc::ServerWriter< ::diststore::HelpReply>* writer) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestHelp(::grpc::ServerContext* context, ::diststore::HelpRequest* request, ::grpc::ServerAsyncWriter< ::diststore::HelpReply>* writer, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncServerStreaming(3, context, request, writer, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_PutFile<WithAsyncMethod_GetFile<WithAsyncMethod_CheckHealth<WithAsyncMethod_Help<Service > > > > AsyncService;
  template <class BaseClass>
  class WithGenericMethod_PutFile : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_PutFile() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_PutFile() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status PutFile(::grpc::ServerContext* context, const ::diststore::PutFileRequest* request, ::diststore::PutFileReply* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_GetFile : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_GetFile() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_GetFile() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetFile(::grpc::ServerContext* context, const ::diststore::GetFileRequest* request, ::diststore::GetFileReply* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_CheckHealth : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_CheckHealth() {
      ::grpc::Service::MarkMethodGeneric(2);
    }
    ~WithGenericMethod_CheckHealth() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status CheckHealth(::grpc::ServerContext* context, const ::diststore::CheckHealthRequest* request, ::diststore::CheckHealthReply* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_Help : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_Help() {
      ::grpc::Service::MarkMethodGeneric(3);
    }
    ~WithGenericMethod_Help() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Help(::grpc::ServerContext* context, const ::diststore::HelpRequest* request, ::grpc::ServerWriter< ::diststore::HelpReply>* writer) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_PutFile : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_PutFile() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::StreamedUnaryHandler< ::diststore::PutFileRequest, ::diststore::PutFileReply>(std::bind(&WithStreamedUnaryMethod_PutFile<BaseClass>::StreamedPutFile, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_PutFile() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status PutFile(::grpc::ServerContext* context, const ::diststore::PutFileRequest* request, ::diststore::PutFileReply* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedPutFile(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::diststore::PutFileRequest,::diststore::PutFileReply>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_GetFile : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_GetFile() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::StreamedUnaryHandler< ::diststore::GetFileRequest, ::diststore::GetFileReply>(std::bind(&WithStreamedUnaryMethod_GetFile<BaseClass>::StreamedGetFile, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_GetFile() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status GetFile(::grpc::ServerContext* context, const ::diststore::GetFileRequest* request, ::diststore::GetFileReply* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedGetFile(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::diststore::GetFileRequest,::diststore::GetFileReply>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_CheckHealth : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_CheckHealth() {
      ::grpc::Service::MarkMethodStreamed(2,
        new ::grpc::StreamedUnaryHandler< ::diststore::CheckHealthRequest, ::diststore::CheckHealthReply>(std::bind(&WithStreamedUnaryMethod_CheckHealth<BaseClass>::StreamedCheckHealth, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_CheckHealth() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status CheckHealth(::grpc::ServerContext* context, const ::diststore::CheckHealthRequest* request, ::diststore::CheckHealthReply* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedCheckHealth(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::diststore::CheckHealthRequest,::diststore::CheckHealthReply>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_PutFile<WithStreamedUnaryMethod_GetFile<WithStreamedUnaryMethod_CheckHealth<Service > > > StreamedUnaryService;
  template <class BaseClass>
  class WithSplitStreamingMethod_Help : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithSplitStreamingMethod_Help() {
      ::grpc::Service::MarkMethodStreamed(3,
        new ::grpc::SplitServerStreamingHandler< ::diststore::HelpRequest, ::diststore::HelpReply>(std::bind(&WithSplitStreamingMethod_Help<BaseClass>::StreamedHelp, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithSplitStreamingMethod_Help() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status Help(::grpc::ServerContext* context, const ::diststore::HelpRequest* request, ::grpc::ServerWriter< ::diststore::HelpReply>* writer) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with split streamed
    virtual ::grpc::Status StreamedHelp(::grpc::ServerContext* context, ::grpc::ServerSplitStreamer< ::diststore::HelpRequest,::diststore::HelpReply>* server_split_streamer) = 0;
  };
  typedef WithSplitStreamingMethod_Help<Service > SplitStreamedService;
  typedef WithStreamedUnaryMethod_PutFile<WithStreamedUnaryMethod_GetFile<WithStreamedUnaryMethod_CheckHealth<WithSplitStreamingMethod_Help<Service > > > > StreamedService;
};

}  // namespace diststore


#endif  // GRPC_ds_5fsvc_2eproto__INCLUDED
