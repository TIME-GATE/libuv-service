#include "routes.h"
#include "stdio.h"
/*
int main() {
  CThostFtdcTraderApi * api = CThostFtdcTraderApi::CreateFtdcTraderApi("/home/zhang/Projects/libuv-service/data/trader");
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
*/


char *Routes::getCThostFtdcTraderApi() {
      printf("start ...."); 
      CThostFtdcTraderApi * api = CThostFtdcTraderApi::CreateFtdcTraderApi("/home/zhang/Projects/libuv-service/data/trader");
      
      printf("create ....");
      MarketDataCollector mdCollector(api);
      
      printf("register ....");
      api->RegisterSpi(&mdCollector);
      
      printf("front ....");
      api->RegisterFront("tcp://127.0.0.1:17001");
      
      THOST_TE_RESUME_TYPE one;

      printf("SubscribePrivateTopic ....");
      api->SubscribePrivateTopic(one);
      
      printf("SubscribePublicTopic ....");
      api->SubscribePublicTopic(one);
      
      printf("Init ....");
      api->Init();
      
      printf("Join ....");
      api->Join();

      printf("CThostFtdcInputOrderActionField ....\n");
      CThostFtdcInputOrderActionField *two;
      
      printf("ReqOrderAction ....\n");
      CThostFtdcRspInfoField three;
      api->ReqOrderAction(two, 123);
       
      return "nihao";
}
