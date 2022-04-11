#define SE_LIST_LENGTH 42

/*

typedef struct {
  unsigned short id;    //本代码中，为每一个资源（Resource）都定义了一个唯一的 id 。在se_types.h中定义。
  unsigned short base;  //这个什么含义不是很清楚？？
  unsigned short offset;//List元素在其所从属的上级数据结构中的地址偏移量。
  unsigned short type;  //为每一个Resource定义的一个唯一的id。在se_types.h中定义。
  Key key[3]; //？？？ 不清楚什么意思
} ListInfo;


typedef struct {
  unsigned short offset;
  short type;
} Key;

offsetof(type, member-designator)宏定义

C 库宏 offsetof(type, member-designator) 会生成一个类型为 size_t 的整型常量，它是一个结构成员相对于结构开头的字节偏移量。
成员是由 member-designator 给定的，结构的名称是在 type 中给定的。

type -- 这是一个 class 类型，其中，member-designator 是一个有效的成员指示器。
member-designator -- 这是一个 class 类型的成员指示器。


*/
ListInfo se_list_info[] = {
  /*------id-------------base-----------------List *BillingPeriod;在SE_BillingPeriodList_t中的偏移量--type---------Key key[3]:?????----*/
  {SE_BillingPeriodList, SE_SubscribableList, offsetof(SE_BillingPeriodList_t, BillingPeriod), SE_BillingPeriod, {{offsetof(SE_BillingPeriod_t, interval.start), XS_LONG}, {offsetof(SE_BillingPeriod_t, href), XS_ANY_URI}}},
  {SE_BillingReadingList, SE_List, offsetof(SE_BillingReadingList_t, BillingReading), SE_BillingReading, {{offsetof(SE_BillingReading_t, timePeriod.start), XS_LONG}, {offsetof(SE_BillingReading_t, consumptionBlock), XS_UBYTE}, {offsetof(SE_BillingReading_t, touTier), XS_UBYTE}}},
  {SE_BillingReadingSetList, SE_SubscribableList, offsetof(SE_BillingReadingSetList_t, BillingReadingSet), SE_BillingReadingSet, {{offsetof(SE_BillingReadingSet_t, timePeriod.start), -XS_LONG}, {offsetof(SE_BillingReadingSet_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_ConsumptionTariffIntervalList, SE_List, offsetof(SE_ConsumptionTariffIntervalList_t, ConsumptionTariffInterval), SE_ConsumptionTariffInterval, {{offsetof(SE_ConsumptionTariffInterval_t, startValue), XS_ULONG}}},
  {SE_CreditRegisterList, SE_List, offsetof(SE_CreditRegisterList_t, CreditRegister), SE_CreditRegister, {{offsetof(SE_CreditRegister_t, effectiveTime), -XS_LONG}, {offsetof(SE_CreditRegister_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_CustomerAccountList, SE_SubscribableList, offsetof(SE_CustomerAccountList_t, CustomerAccount), SE_CustomerAccount, {{offsetof(SE_CustomerAccount_t, customerName), xs_type(XS_STRING, 42)}, {offsetof(SE_CustomerAccount_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_CustomerAgreementList, SE_SubscribableList, offsetof(SE_CustomerAgreementList_t, CustomerAgreement), SE_CustomerAgreement, {{offsetof(SE_CustomerAgreement_t, serviceLocation), xs_type(XS_STRING, 42)}, {offsetof(SE_CustomerAgreement_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_DERControlList, SE_SubscribableList, offsetof(SE_DERControlList_t, DERControl), SE_DERControl, {{offsetof(SE_DERControl_t, interval.start), XS_LONG}, {offsetof(SE_DERControl_t, creationTime), -XS_LONG}, {offsetof(SE_DERControl_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_DERCurveList, SE_List, offsetof(SE_DERCurveList_t, DERCurve), SE_DERCurve, {{offsetof(SE_DERCurve_t, creationTime), -XS_LONG}, {offsetof(SE_DERCurve_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_DERList, SE_List, offsetof(SE_DERList_t, DER), SE_DER, {{offsetof(SE_DER_t, href), XS_ANY_URI}}},
  {SE_DERProgramList, SE_SubscribableList, offsetof(SE_DERProgramList_t, DERProgram), SE_DERProgram, {{offsetof(SE_DERProgram_t, primacy), XS_UBYTE}, {offsetof(SE_DERProgram_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_DemandResponseProgramList, SE_SubscribableList, offsetof(SE_DemandResponseProgramList_t, DemandResponseProgram), SE_DemandResponseProgram, {{offsetof(SE_DemandResponseProgram_t, primacy), XS_UBYTE}, {offsetof(SE_DemandResponseProgram_t, mRID), xs_type(XS_HEX_BINARY, 16)}}},
  {SE_EndDeviceControlList, SE_SubscribableList, offsetof(SE_EndDeviceControlList_t, EndDeviceControl), SE_EndDeviceControl, {{offsetof(SE_EndDeviceControl_t, interval.start), XS_LONG}, {offsetof(SE_EndDeviceControl_t, creationTime), -XS_LONG}, {offsetof(SE_EndDeviceControl_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_EndDeviceList, SE_SubscribableList, offsetof(SE_EndDeviceList_t, EndDevice), SE_EndDevice, {{offsetof(SE_EndDevice_t, sFDI), XS_ULONG}, {offsetof(SE_EndDevice_t, href), XS_ANY_URI}}},
  {SE_FlowReservationRequestList, SE_List, offsetof(SE_FlowReservationRequestList_t, FlowReservationRequest), SE_FlowReservationRequest, {{offsetof(SE_FlowReservationRequest_t, intervalRequested.start), XS_LONG}, {offsetof(SE_FlowReservationRequest_t, creationTime), -XS_LONG}, {offsetof(SE_FlowReservationRequest_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_FlowReservationResponseList, SE_SubscribableList, offsetof(SE_FlowReservationResponseList_t, FlowReservationResponse), SE_FlowReservationResponse, {{offsetof(SE_FlowReservationResponse_t, interval.start), XS_LONG}, {offsetof(SE_FlowReservationResponse_t, creationTime), -XS_LONG}, {offsetof(SE_FlowReservationResponse_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_FunctionSetAssignmentsList, SE_SubscribableList, offsetof(SE_FunctionSetAssignmentsList_t, FunctionSetAssignments), SE_FunctionSetAssignments, {{offsetof(SE_FunctionSetAssignments_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_HistoricalReadingList, SE_List, offsetof(SE_HistoricalReadingList_t, HistoricalReading), SE_HistoricalReading, {{offsetof(SE_HistoricalReading_t, description), xs_type(XS_STRING, 32)}, {offsetof(SE_HistoricalReading_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_IPAddrList, SE_List, offsetof(SE_IPAddrList_t, IPAddr), SE_IPAddr, {{offsetof(SE_IPAddr_t, address), xs_type(XS_HEX_BINARY, 16)}}},
  {SE_IPInterfaceList, SE_List, offsetof(SE_IPInterfaceList_t, IPInterface), SE_IPInterface, {{offsetof(SE_IPInterface_t, ifIndex), XS_UINT}}},
  {SE_LLInterfaceList, SE_List, offsetof(SE_LLInterfaceList_t, LLInterface), SE_LLInterface, {{offsetof(SE_LLInterface_t, EUI64), xs_type(XS_HEX_BINARY, 8)}}},
  {SE_LogEventList, SE_SubscribableList, offsetof(SE_LogEventList_t, LogEvent), SE_LogEvent, {{offsetof(SE_LogEvent_t, createdDateTime), XS_LONG}, {offsetof(SE_LogEvent_t, logEventID), XS_USHORT}}},
  {SE_MessagingProgramList, SE_SubscribableList, offsetof(SE_MessagingProgramList_t, MessagingProgram), SE_MessagingProgram, {{offsetof(SE_MessagingProgram_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_MeterReadingList, SE_SubscribableList, offsetof(SE_MeterReadingList_t, MeterReading), SE_MeterReading, {{offsetof(SE_MeterReading_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_MirrorUsagePointList, SE_List, offsetof(SE_MirrorUsagePointList_t, MirrorUsagePoint), SE_MirrorUsagePoint, {{offsetof(SE_MirrorUsagePoint_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_NotificationList, SE_List, offsetof(SE_NotificationList_t, Notification), SE_Notification, {{offsetof(SE_Notification_t, href), XS_ANY_URI}}},
  {SE_PrepaymentList, SE_SubscribableList, offsetof(SE_PrepaymentList_t, Prepayment), SE_Prepayment, {{offsetof(SE_Prepayment_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_ProjectionReadingList, SE_List, offsetof(SE_ProjectionReadingList_t, ProjectionReading), SE_ProjectionReading, {{offsetof(SE_ProjectionReading_t, description), xs_type(XS_STRING, 32)}, {offsetof(SE_ProjectionReading_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_RPLInstanceList, SE_List, offsetof(SE_RPLInstanceList_t, RPLInstance), SE_RPLInstance, {{offsetof(SE_RPLInstance_t, DODAGid), XS_UBYTE}, {offsetof(SE_RPLInstance_t, RPLInstanceID), XS_UBYTE}}},
  {SE_RPLSourceRoutesList, SE_List, offsetof(SE_RPLSourceRoutesList_t, RPLSourceRoutes), SE_RPLSourceRoutes, {{offsetof(SE_RPLSourceRoutes_t, SourceRoute), xs_type(XS_HEX_BINARY, 16)}, {offsetof(SE_RPLSourceRoutes_t, DestAddress), xs_type(XS_HEX_BINARY, 16)}}},
  {SE_RateComponentList, SE_List, offsetof(SE_RateComponentList_t, RateComponent), SE_RateComponent, {{offsetof(SE_RateComponent_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_ReadingList, SE_SubscribableList, offsetof(SE_ReadingList_t, Reading), SE_Reading, {{offsetof(SE_Reading_t, localID), xs_type(XS_HEX_BINARY, 2)}, {offsetof(SE_Reading_t, consumptionBlock), XS_UBYTE}, {offsetof(SE_Reading_t, touTier), XS_UBYTE}}},
  {SE_ReadingSetList, SE_SubscribableList, offsetof(SE_ReadingSetList_t, ReadingSet), SE_ReadingSet, {{offsetof(SE_ReadingSet_t, timePeriod.start), -XS_LONG}, {offsetof(SE_ReadingSet_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_ResponseList, SE_List, offsetof(SE_ResponseList_t, Response), SE_Response, {{offsetof(SE_Response_t, createdDateTime), -XS_LONG}, {offsetof(SE_Response_t, endDeviceLFDI), xs_type(XS_HEX_BINARY, 20)}}},
  {SE_ResponseSetList, SE_List, offsetof(SE_ResponseSetList_t, ResponseSet), SE_ResponseSet, {{offsetof(SE_ResponseSet_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_SubscriptionList, SE_List, offsetof(SE_SubscriptionList_t, Subscription), SE_Subscription, {{offsetof(SE_Subscription_t, href), XS_ANY_URI}}},
  {SE_SupplyInterruptionOverrideList, SE_List, offsetof(SE_SupplyInterruptionOverrideList_t, SupplyInterruptionOverride), SE_SupplyInterruptionOverride, {{offsetof(SE_SupplyInterruptionOverride_t, interval.start), XS_LONG}, {offsetof(SE_SupplyInterruptionOverride_t, interval.duration), XS_UINT}}},
  {SE_TargetReadingList, SE_List, offsetof(SE_TargetReadingList_t, TargetReading), SE_TargetReading, {{offsetof(SE_TargetReading_t, description), xs_type(XS_STRING, 32)}, {offsetof(SE_TargetReading_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_TariffProfileList, SE_SubscribableList, offsetof(SE_TariffProfileList_t, TariffProfile), SE_TariffProfile, {{offsetof(SE_TariffProfile_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_TextMessageList, SE_SubscribableList, offsetof(SE_TextMessageList_t, TextMessage), SE_TextMessage, {{offsetof(SE_TextMessage_t, interval.start), -XS_LONG}, {offsetof(SE_TextMessage_t, creationTime), -XS_LONG}, {offsetof(SE_TextMessage_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_TimeTariffIntervalList, SE_SubscribableList, offsetof(SE_TimeTariffIntervalList_t, TimeTariffInterval), SE_TimeTariffInterval, {{offsetof(SE_TimeTariffInterval_t, interval.start), XS_LONG}, {offsetof(SE_TimeTariffInterval_t, creationTime), -XS_LONG}, {offsetof(SE_TimeTariffInterval_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}},
  {SE_UsagePointList, SE_SubscribableList, offsetof(SE_UsagePointList_t, UsagePoint), SE_UsagePoint, {{offsetof(SE_UsagePoint_t, mRID), -xs_type(XS_HEX_BINARY, 16)}}}
};
