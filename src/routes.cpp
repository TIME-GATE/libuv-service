#include "./sdk/ThostFtdcMdApi.h"

int main() {
  CThostFtdcMdApi * api = CThostFtdcMdApi::CreateFtdcMdApi("/Users/dreamboad/Projects/libuv-service/data", true);
  MarketDataCollector mdCollector(api);

  api->RegisterSpi(&mdCollector);
  api->RegisterFront("tcp://127.0.0.1:17001");
  api->Init();
  api->Join();
  return 1;
}


