#include "./sdk/ThostFtdcTraderApi.h"
#include "./sdk/ChildThostFtdcTraderApi.h"

int main() {
  CThostFtdcTraderApi * api = CThostFtdcTraderApi::CreateFtdcTraderApi("/Users/dreamboad/Projects/libuv-service/data");
  MarketDataCollector mdCollector(api);

  api->RegisterSpi(&mdCollector);
  api->RegisterFront("tcp://127.0.0.1:17001");
  THOST_TE_RESUME_TYPE one;
  api->SubscribePrivateTopic(one);
  api->SubscribePublicTopic(one);
  api->Init();
  api->Join();
  return 1;
}


