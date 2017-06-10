#include "ThostFtdcMdApi.h"

class MarketDataCollector : public CThostFtdcTraderSpi {
  
  public :  

    virtual void Release() {};
    virtual void Init() {};
    virtual int Join() {};
    virtual const char *GetTradingDay() {};
    virtual void RegisterFront(char *pszFrontAddress) {};
    virtual void RegisterNameServer(char *pszNsAddress) {};
    virtual void RegisterFensUserInfo(CThostFtdcFensUserInfoField * pFensUserInfo) {};
    virtual void RegisterSpi(CThostFtdcMdSpi *pSpi) {};
    virtual int SubscribeMarketData(char *ppInstrumentID[], int nCount) {};
    virtual int UnSubscribeMarketData(char *ppInstrumentID[], int nCount) {};
    virtual int SubscribeForQuoteRsp(char *ppInstrumentID[], int nCount) {};
    virtual int UnSubscribeForQuoteRsp(char *ppInstrumentID[], int nCount) {};
    virtual int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID) {};
    virtual int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID) {};

}
