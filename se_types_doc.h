/** @defgroup se_types 2030.5 Types */

/** @defgroup se_typedefs Typedefs 
    @ingroup se_types */

/** @defgroup UInt48 UInt48
    @ingroup se_typedefs

    Unsigned integer, max inclusive 281474976710655 (2^48-1)

    @{ */

/** @typedef SE_UInt48_t

    Unsigned integer, max inclusive 281474976710655 (2^48-1)

*/

/** @} */

/** @defgroup UInt8 UInt8
    @ingroup se_typedefs

    Unsigned integer, max inclusive 255 (2^8-1)

    @{ */

/** @typedef SE_UInt8_t

    Unsigned integer, max inclusive 255 (2^8-1)

*/

/** @} */

/** @defgroup PhaseCode PhaseCode
    @ingroup UInt8

    0 = Not Applicable (default, if not specified)

    32 = Phase C (and S2)

    33 = Phase CN (and S2N)

    40 = Phase CA

    64 = Phase B

    65 = Phase BN

    66 = Phase BC

    128 = Phase A (and S1)

    129 = Phase AN (and S1N)

    132 = Phase AB

    224 = Phase ABC

    All other values reserved.

    @{ */

/** @typedef SE_PhaseCode_t

    0 = Not Applicable (default, if not specified)

    32 = Phase C (and S2)

    33 = Phase CN (and S2N)

    40 = Phase CA

    64 = Phase B

    65 = Phase BN

    66 = Phase BC

    128 = Phase A (and S1)

    129 = Phase AN (and S1N)

    132 = Phase AB

    224 = Phase ABC

    All other values reserved.

*/

/** @} */

/** @defgroup KindType KindType
    @ingroup UInt8

    0 = Not Applicable (default, if not specified)

    3 = Currency

    8 = Demand

    12 = Energy

    37 = Power

    All other values reserved.

    @{ */

/** @typedef SE_KindType_t

    0 = Not Applicable (default, if not specified)

    3 = Currency

    8 = Demand

    12 = Energy

    37 = Power

    All other values reserved.

*/

/** @} */

/** @defgroup UInt32 UInt32
    @ingroup se_typedefs

    Unsigned integer, max inclusive 4294967295 (2^32-1)

    @{ */

/** @typedef SE_UInt32_t

    Unsigned integer, max inclusive 4294967295 (2^32-1)

*/

/** @} */

/** @defgroup FlowDirectionType FlowDirectionType
    @ingroup UInt8

    0 = Not Applicable (default, if not specified)

    1 = Forward (delivered to customer)

    19 = Reverse (received from customer)

    All other values reserved.

    @{ */

/** @typedef SE_FlowDirectionType_t

    0 = Not Applicable (default, if not specified)

    1 = Forward (delivered to customer)

    19 = Reverse (received from customer)

    All other values reserved.

*/

/** @} */

/** @defgroup DataQualifierType DataQualifierType
    @ingroup UInt8

    0 = Not Applicable (default, if not specified)

    2 = Average

    8 = Maximum

    9 = Minimum

    12 = Normal

    29 = Standard Deviation of a Population (typically indicated by a lower case sigma)

    30 = Standard Deviation of a Sample Drawn from a Population (typically indicated by a lower case 's')

    All other values reserved.

    @{ */

/** @typedef SE_DataQualifierType_t

    0 = Not Applicable (default, if not specified)

    2 = Average

    8 = Maximum

    9 = Minimum

    12 = Normal

    29 = Standard Deviation of a Population (typically indicated by a lower case sigma)

    30 = Standard Deviation of a Sample Drawn from a Population (typically indicated by a lower case 's')

    All other values reserved.

*/

/** @} */

/** @defgroup CommodityType CommodityType
    @ingroup UInt8

    0 = Not Applicable (default, if not specified)

    1 = Electricity secondary metered value (a premises meter is typically on the low voltage, or secondary, side of a service transformer)

    2 = Electricity primary metered value (measured on the high voltage, or primary, side of the service transformer)

    4 = Air

    7 = NaturalGas

    8 = Propane

    9 = PotableWater

    10 = Steam

    11 = WasteWater

    12 = HeatingFluid

    13 = CoolingFluid

    All other values reserved.

    @{ */

/** @typedef SE_CommodityType_t

    0 = Not Applicable (default, if not specified)

    1 = Electricity secondary metered value (a premises meter is typically on the low voltage, or secondary, side of a service transformer)

    2 = Electricity primary metered value (measured on the high voltage, or primary, side of the service transformer)

    4 = Air

    7 = NaturalGas

    8 = Propane

    9 = PotableWater

    10 = Steam

    11 = WasteWater

    12 = HeatingFluid

    13 = CoolingFluid

    All other values reserved.

*/

/** @} */

/** @defgroup Int32 Int32
    @ingroup se_typedefs

    Signed integer, max inclusive 2147483647 (2^31), min inclusive -2147483647 (same as xs:int)

    @{ */

/** @typedef SE_Int32_t

    Signed integer, max inclusive 2147483647 (2^31), min inclusive -2147483647 (same as xs:int)

*/

/** @} */

/** @defgroup UomType UomType
    @ingroup UInt8

    The following values are recommended values sourced from the unit of measure enumeration in IEC 61968-9 [61968]. Other values from the unit of measure enumeration in IEC 61968-9 [61968] MAY be used.

    0 = Not Applicable (default, if not specified)

    5 = A (Current in Amperes (RMS))

    6 = Kelvin (Temperature)

    23 = Degrees Celsius (Relative temperature)

    29 = Voltage

    31 = J (Energy joule)

    33 = Hz (Frequency)

    38 =W (Real power in Watts)

    42 = m3 (Cubic Meter)

    61 = VA (Apparent power)

    63 = var (Reactive power)

    65 = CosTheta (Displacement Power Factor)

    67 = V² (Volts squared)

    69 = A² (Amp squared)

    71 = VAh (Apparent energy)

    72 = Wh (Real energy in Watt-hours)

    73 = varh (Reactive energy)

    106 = Ah (Ampere-hours / Available Charge)

    119 = ft3 (Cubic Feet)

    122 = ft3/h (Cubic Feet per Hour)

    125 = m3/h (Cubic Meter per Hour)

    128 = US gl (US Gallons)

    129 = US gl/h (US Gallons per Hour)

    130 = IMP gl (Imperial Gallons)

    131 = IMP gl/h (Imperial Gallons per Hour)

    132 = BTU

    133 = BTU/h

    134 = Liter

    137 = L/h (Liters per Hour)

    140 = PA(gauge)

    155 = PA(absolute)

    169 = Therm

    @{ */

/** @typedef SE_UomType_t

    The following values are recommended values sourced from the unit of measure enumeration in IEC 61968-9 [61968]. Other values from the unit of measure enumeration in IEC 61968-9 [61968] MAY be used.

    0 = Not Applicable (default, if not specified)

    5 = A (Current in Amperes (RMS))

    6 = Kelvin (Temperature)

    23 = Degrees Celsius (Relative temperature)

    29 = Voltage

    31 = J (Energy joule)

    33 = Hz (Frequency)

    38 =W (Real power in Watts)

    42 = m3 (Cubic Meter)

    61 = VA (Apparent power)

    63 = var (Reactive power)

    65 = CosTheta (Displacement Power Factor)

    67 = V² (Volts squared)

    69 = A² (Amp squared)

    71 = VAh (Apparent energy)

    72 = Wh (Real energy in Watt-hours)

    73 = varh (Reactive energy)

    106 = Ah (Ampere-hours / Available Charge)

    119 = ft3 (Cubic Feet)

    122 = ft3/h (Cubic Feet per Hour)

    125 = m3/h (Cubic Meter per Hour)

    128 = US gl (US Gallons)

    129 = US gl/h (US Gallons per Hour)

    130 = IMP gl (Imperial Gallons)

    131 = IMP gl/h (Imperial Gallons per Hour)

    132 = BTU

    133 = BTU/h

    134 = Liter

    137 = L/h (Liters per Hour)

    140 = PA(gauge)

    155 = PA(absolute)

    169 = Therm

*/

/** @} */

/** @defgroup Int8 Int8
    @ingroup se_typedefs

    Signed integer, min -128 max +127

    @{ */

/** @typedef SE_Int8_t

    Signed integer, min -128 max +127

*/

/** @} */

/** @defgroup PowerOfTenMultiplierType PowerOfTenMultiplierType
    @ingroup Int8

    -9 = nano=x10^-9

    -6 = micro=x10^-6

    -3 = milli=x10^-3

    0 = none=x1 (default, if not specified)

    1 = deca=x10

    2 = hecto=x100

    3 = kilo=x1000

    6 = Mega=x10^6

    9 = Giga=x10^9

    This is not a complete list. Any integer between -9 and 9 SHALL be supported, indicating the power of ten multiplier for the units.

    @{ */

/** @typedef SE_PowerOfTenMultiplierType_t

    -9 = nano=x10^-9

    -6 = micro=x10^-6

    -3 = milli=x10^-3

    0 = none=x1 (default, if not specified)

    1 = deca=x10

    2 = hecto=x100

    3 = kilo=x1000

    6 = Mega=x10^6

    9 = Giga=x10^9

    This is not a complete list. Any integer between -9 and 9 SHALL be supported, indicating the power of ten multiplier for the units.

*/

/** @} */

/** @defgroup UnitValueType UnitValueType
    @ingroup se_types

    Type for specification of a specific value, with units and power of ten multiplier.

    @{ */

/** @struct SE_UnitValueType_t se_types.h

    Type for specification of a specific value, with units and power of ten multiplier.

    @var SE_UnitValueType_t::multiplier
    Multiplier for 'unit'.

    @var SE_UnitValueType_t::unit
    Unit in symbol

    @var SE_UnitValueType_t::value
    Value in units specified

*/

/** @} */

/** @defgroup AccumulationBehaviourType AccumulationBehaviourType
    @ingroup UInt8

    0 = Not Applicable (default, if not specified)

    3 = Cumulative

    The sum of the previous billing period values. Note: “Cumulative” is commonly used in conjunction with “demand.” Each demand reset causes the maximum demand value for the present billing period (since the last demand reset) to accumulate as an accumulative total of all maximum demands. So instead of “zeroing” the demand register, a demand reset has the affect of adding the present maximum demand to this accumulating total.

    4 = DeltaData

    The difference between the value at the end of the prescribed interval and the beginning of the interval. This is used for incremental interval data. 

    Note: One common application would be for load profile data, another use might be to report the number of events within an interval (such as the number of equipment energizations within the specified period of time.)

    6 = Indicating

    As if a needle is swung out on the meter face to a value to indicate the current value. (Note: An “indicating” value is typically measured over hundreds of milliseconds or greater, or may imply a “pusher” mechanism to capture a value. Compare this to “instantaneous” which is measured over a shorter period of time.) 

    9 = Summation

    A form of accumulation which is selective with respect to time. 

    Note : “Summation” could be considered a specialization of “Bulk Quantity” according to the rules of inheritance where “Summation” selectively accumulates pulses over a timing pattern, and “BulkQuantity” accumulates pulses all of the time.

    12 = Instantaneous

    Typically measured over the fastest period of time allowed by the definition of the metric (usually milliseconds or tens of milliseconds.) (Note: “Instantaneous” was moved to attribute #3 in 61968-9Ed2 from attribute #1 in 61968-9Ed1.)

    All other values reserved.

    @{ */

/** @typedef SE_AccumulationBehaviourType_t

    0 = Not Applicable (default, if not specified)

    3 = Cumulative

    The sum of the previous billing period values. Note: “Cumulative” is commonly used in conjunction with “demand.” Each demand reset causes the maximum demand value for the present billing period (since the last demand reset) to accumulate as an accumulative total of all maximum demands. So instead of “zeroing” the demand register, a demand reset has the affect of adding the present maximum demand to this accumulating total.

    4 = DeltaData

    The difference between the value at the end of the prescribed interval and the beginning of the interval. This is used for incremental interval data. 

    Note: One common application would be for load profile data, another use might be to report the number of events within an interval (such as the number of equipment energizations within the specified period of time.)

    6 = Indicating

    As if a needle is swung out on the meter face to a value to indicate the current value. (Note: An “indicating” value is typically measured over hundreds of milliseconds or greater, or may imply a “pusher” mechanism to capture a value. Compare this to “instantaneous” which is measured over a shorter period of time.) 

    9 = Summation

    A form of accumulation which is selective with respect to time. 

    Note : “Summation” could be considered a specialization of “Bulk Quantity” according to the rules of inheritance where “Summation” selectively accumulates pulses over a timing pattern, and “BulkQuantity” accumulates pulses all of the time.

    12 = Instantaneous

    Typically measured over the fastest period of time allowed by the definition of the metric (usually milliseconds or tens of milliseconds.) (Note: “Instantaneous” was moved to attribute #3 in 61968-9Ed2 from attribute #1 in 61968-9Ed1.)

    All other values reserved.

*/

/** @} */

/** @defgroup Resource Resource
    @ingroup se_types

    A resource is an addressable unit of information, either a collection (List) or instance of an object (identifiedObject, or simply, Resource)

    @{ */

/** @struct SE_Resource_t se_types.h

    A resource is an addressable unit of information, either a collection (List) or instance of an object (identifiedObject, or simply, Resource)

    @var SE_Resource_t::_flags
    @var SE_Resource_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

*/

/** @} */

/** @defgroup ReadingType ReadingType
    @ingroup Resource

    Type of data conveyed by a specific Reading. See IEC 61968 Part 9 Annex C for full definitions of these values.

    @{ */

/** @struct SE_ReadingType_t se_types.h

    Type of data conveyed by a specific Reading. See IEC 61968 Part 9 Annex C for full definitions of these values.

    @var SE_ReadingType_t::_flags
#define SE_uom_exists (1 << 5)

#define SE_tieredConsumptionBlocks_true (1 << 6)
#define SE_tieredConsumptionBlocks_exists (1 << 7)

#define SE_supplyLimit_exists (1 << 8)

#define SE_subIntervalLength_exists (1 << 9)

#define SE_powerOfTenMultiplier_exists (1 << 10)

#define SE_phase_exists (1 << 11)

#define SE_numberOfTouTiers_exists (1 << 12)

#define SE_numberOfConsumptionBlocks_exists (1 << 13)

#define SE_maxNumberOfIntervals_exists (1 << 14)

#define SE_intervalLength_exists (1 << 15)

#define SE_flowDirection_exists (1 << 16)

#define SE_dataQualifier_exists (1 << 17)

#define SE_conversionFactor_exists (1 << 18)

#define SE_commodity_exists (1 << 19)

#define SE_calorificValue_exists (1 << 20)

#define SE_accumulationBehaviour_exists (1 << 21)

    @var SE_ReadingType_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_ReadingType_t::accumulationBehaviour
    The “accumulation behaviour” indicates how the value is represented to accumulate over time.

    @var SE_ReadingType_t::calorificValue
    The amount of heat generated when a given mass of fuel is completely burned. The CalorificValue is used to convert the measured volume or mass of gas into kWh. The CalorificValue attribute represents the current active value.

    @var SE_ReadingType_t::commodity
    Indicates the commodity applicable to this ReadingType.

    @var SE_ReadingType_t::conversionFactor
    Accounts for changes in the volume of gas based on temperature and pressure. The ConversionFactor attribute represents the current active value. The ConversionFactor is dimensionless. The default value for the ConversionFactor is 1, which means no conversion is applied. A price server can advertise a new/different value at any time.

    @var SE_ReadingType_t::dataQualifier
    The data type can be used to describe a salient attribute of the data. Possible values are average, absolute, and etc.

    @var SE_ReadingType_t::flowDirection
    Anything involving current might have a flow direction. Possible values include forward and reverse.

    @var SE_ReadingType_t::intervalLength
    Default interval length specified in seconds.

    @var SE_ReadingType_t::kind
    Compound class that contains kindCategory and kindIndex

    @var SE_ReadingType_t::maxNumberOfIntervals
    To be populated for mirrors of interval data to set the expected number of intervals per ReadingSet. Servers may discard intervals received that exceed this number.

    @var SE_ReadingType_t::numberOfConsumptionBlocks
    Number of consumption blocks. 0 means not applicable, and is the default if not specified. The value needs to be at least 1 if any actual prices are provided.

    @var SE_ReadingType_t::numberOfTouTiers
    The number of TOU tiers that can be used by any resource configured by this ReadingType. Servers SHALL populate this value with the largest touTier value that will <i>ever</i> be used while this ReadingType is in effect. Servers SHALL set numberOfTouTiers equal to the number of standard TOU tiers plus the number of CPP tiers that may be used while this ReadingType is in effect. Servers SHALL specify a value between 0 and 255 (inclusive) for numberOfTouTiers (servers providing flat rate pricing SHOULD set numberOfTouTiers to 0, as in practice there is no difference between having no tiers and having one tier).

    @var SE_ReadingType_t::phase
    Contains phase information associated with the type.

    @var SE_ReadingType_t::powerOfTenMultiplier
    Indicates the power of ten multiplier applicable to the unit of measure of this ReadingType.

    @var SE_ReadingType_t::subIntervalLength
    Default sub-interval length specified in seconds for Readings of ReadingType. Some demand calculations are done over a number of smaller intervals. For example, in a rolling demand calculation, the demand value is defined as the rolling sum of smaller intervals over the intervalLength. The subintervalLength is the length of the smaller interval in this calculation. It SHALL be an integral division of the intervalLength. The number of sub-intervals can be calculated by dividing the intervalLength by the subintervalLength.

    @var SE_ReadingType_t::supplyLimit
    Reflects the supply limit set in the meter. This value can be compared to the Reading value to understand if limits are being approached or exceeded. Units follow the same definition as in this ReadingType.

    @var SE_ReadingType_t::uom
    Indicates the measurement type for the units of measure for the readings of this type.

*/

/** @} */

/** @defgroup SubscribableType SubscribableType
    @ingroup UInt8

    The subscribable values.

    0 - Resource does not support subscriptions

    1 - Resource supports non-conditional subscriptions

    2 - Resource supports conditional subscriptions

    3 - Resource supports both conditional and non-conditional subscriptions

    All other values reserved.

    @{ */

/** @typedef SE_SubscribableType_t

    The subscribable values.

    0 - Resource does not support subscriptions

    1 - Resource supports non-conditional subscriptions

    2 - Resource supports conditional subscriptions

    3 - Resource supports both conditional and non-conditional subscriptions

    All other values reserved.

*/

/** @} */

/** @defgroup Int48 Int48
    @ingroup se_typedefs

    Signed integer, max inclusive 140737488355328 (2^47), min inclusive -140737488355328

    @{ */

/** @typedef SE_Int48_t

    Signed integer, max inclusive 140737488355328 (2^47), min inclusive -140737488355328

*/

/** @} */

/** @defgroup TOUType TOUType
    @ingroup UInt8

    0 = Not Applicable (default, if not specified)

    1 = TOU A

    2 = TOU B

    3 = TOU C

    4 = TOU D

    5 = TOU E

    6 = TOU F

    7 = TOU G

    8 = TOU H

    9 = TOU I

    10 = TOU J

    11 = TOU K

    12 = TOU L

    13 = TOU M

    14 = TOU N

    15 = TOU O

    All other values reserved.

    @{ */

/** @typedef SE_TOUType_t

    0 = Not Applicable (default, if not specified)

    1 = TOU A

    2 = TOU B

    3 = TOU C

    4 = TOU D

    5 = TOU E

    6 = TOU F

    7 = TOU G

    8 = TOU H

    9 = TOU I

    10 = TOU J

    11 = TOU K

    12 = TOU L

    13 = TOU M

    14 = TOU N

    15 = TOU O

    All other values reserved.

*/

/** @} */

/** @defgroup Int64 Int64
    @ingroup se_typedefs

    Signed integer, max inclusive 9223372036854775807 (2^63), min inclusive -9223372036854775808 (same as xs:long)

    @{ */

/** @typedef SE_Int64_t

    Signed integer, max inclusive 9223372036854775807 (2^63), min inclusive -9223372036854775808 (same as xs:long)

*/

/** @} */

/** @defgroup TimeType TimeType
    @ingroup Int64

    Time is a signed 64 bit value representing the number of seconds since 0 hours, 0 minutes, 0 seconds, on the 1st of January, 1970, in UTC, not counting leap seconds.

    @{ */

/** @typedef SE_TimeType_t

    Time is a signed 64 bit value representing the number of seconds since 0 hours, 0 minutes, 0 seconds, on the 1st of January, 1970, in UTC, not counting leap seconds.

*/

/** @} */

/** @defgroup DateTimeInterval DateTimeInterval
    @ingroup se_types

    Interval of date and time.

    @{ */

/** @struct SE_DateTimeInterval_t se_types.h

    Interval of date and time.

    @var SE_DateTimeInterval_t::duration
    Duration of the interval, in seconds.

    @var SE_DateTimeInterval_t::start
    Date and time of the start of the interval.

*/

/** @} */

/** @defgroup HexBinary16 HexBinary16
    @ingroup se_typedefs

    A 16-bit field encoded as a hex string (4 hex characters max). Where applicable, bit 0, or the least significant bit, goes on the right. Note that hexBinary requires pairs of hex characters, so an odd number of characters requires a leading "0".

    @{ */

/** @typedef SE_HexBinary16_t

    A 16-bit field encoded as a hex string (4 hex characters max). Where applicable, bit 0, or the least significant bit, goes on the right. Note that hexBinary requires pairs of hex characters, so an odd number of characters requires a leading "0".

*/

/** @} */

/** @defgroup ConsumptionBlockType ConsumptionBlockType
    @ingroup UInt8

    0 = Not Applicable (default, if not specified)

    1 = Block 1

    2 = Block 2

    3 = Block 3

    4 = Block 4

    5 = Block 5

    6 = Block 6

    7 = Block 7

    8 = Block 8

    9 = Block 9

    10 = Block 10

    11 = Block 11

    12 = Block 12

    13 = Block 13

    14 = Block 14

    15 = Block 15

    16 = Block 16

    All other values reserved.

    @{ */

/** @typedef SE_ConsumptionBlockType_t

    0 = Not Applicable (default, if not specified)

    1 = Block 1

    2 = Block 2

    3 = Block 3

    4 = Block 4

    5 = Block 5

    6 = Block 6

    7 = Block 7

    8 = Block 8

    9 = Block 9

    10 = Block 10

    11 = Block 11

    12 = Block 12

    13 = Block 13

    14 = Block 14

    15 = Block 15

    16 = Block 16

    All other values reserved.

*/

/** @} */

/** @defgroup ReadingBase ReadingBase
    @ingroup Resource

    Specific value measured by a meter or other asset. ReadingBase is abstract, used to define the elements common to Reading and IntervalReading.

    @{ */

/** @struct SE_ReadingBase_t se_types.h

    Specific value measured by a meter or other asset. ReadingBase is abstract, used to define the elements common to Reading and IntervalReading.

    @var SE_ReadingBase_t::_flags
#define SE_value_exists (1 << 5)

#define SE_touTier_exists (1 << 6)

#define SE_timePeriod_exists (1 << 7)

#define SE_qualityFlags_exists (1 << 8)

#define SE_consumptionBlock_exists (1 << 9)

    @var SE_ReadingBase_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_ReadingBase_t::consumptionBlock
    Indicates the consumption block related to the reading. REQUIRED if ReadingType numberOfConsumptionBlocks is non-zero. If not specified, is assumed to be "0 - N/A".

    @var SE_ReadingBase_t::qualityFlags
    List of codes indicating the quality of the reading, using specification:

    Bit 0 - valid: data that has gone through all required validation checks and either passed them all or has been verified 

    Bit 1 - manually edited: Replaced or approved by a human

    Bit 2 - estimated using reference day: data value was replaced by a machine computed value based on analysis of historical data using the same type of measurement.

    Bit 3 - estimated using linear interpolation: data value was computed using linear interpolation based on the readings before and after it

    Bit 4 - questionable: data that has failed one or more checks

    Bit 5 - derived: data that has been calculated (using logic or mathematical operations), not necessarily measured directly 

    Bit 6 - projected (forecast): data that has been calculated as a projection or forecast of future readings

    @var SE_ReadingBase_t::timePeriod
    The time interval associated with the reading. If not specified, then defaults to the intervalLength specified in the associated ReadingType.

    @var SE_ReadingBase_t::touTier
    Indicates the time of use tier related to the reading. REQUIRED if ReadingType numberOfTouTiers is non-zero. If not specified, is assumed to be "0 - N/A".

    @var SE_ReadingBase_t::value
    Value in units specified by ReadingType

*/

/** @} */

/** @defgroup Reading Reading
    @ingroup ReadingBase

    Specific value measured by a meter or other asset.

    @{ */

/** @struct SE_Reading_t se_types.h

    Specific value measured by a meter or other asset.

    @var SE_Reading_t::_flags
#define SE_value_exists (1 << 5)

#define SE_touTier_exists (1 << 6)

#define SE_timePeriod_exists (1 << 7)

#define SE_qualityFlags_exists (1 << 8)

#define SE_consumptionBlock_exists (1 << 9)

#define SE_localID_exists (1 << 10)

    @var SE_Reading_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_Reading_t::consumptionBlock
    Indicates the consumption block related to the reading. REQUIRED if ReadingType numberOfConsumptionBlocks is non-zero. If not specified, is assumed to be "0 - N/A".

    @var SE_Reading_t::qualityFlags
    List of codes indicating the quality of the reading, using specification:

    Bit 0 - valid: data that has gone through all required validation checks and either passed them all or has been verified 

    Bit 1 - manually edited: Replaced or approved by a human

    Bit 2 - estimated using reference day: data value was replaced by a machine computed value based on analysis of historical data using the same type of measurement.

    Bit 3 - estimated using linear interpolation: data value was computed using linear interpolation based on the readings before and after it

    Bit 4 - questionable: data that has failed one or more checks

    Bit 5 - derived: data that has been calculated (using logic or mathematical operations), not necessarily measured directly 

    Bit 6 - projected (forecast): data that has been calculated as a projection or forecast of future readings

    @var SE_Reading_t::timePeriod
    The time interval associated with the reading. If not specified, then defaults to the intervalLength specified in the associated ReadingType.

    @var SE_Reading_t::touTier
    Indicates the time of use tier related to the reading. REQUIRED if ReadingType numberOfTouTiers is non-zero. If not specified, is assumed to be "0 - N/A".

    @var SE_Reading_t::value
    Value in units specified by ReadingType

    @var SE_Reading_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_Reading_t::localID
    The local identifier for this reading within the reading set. localIDs are assigned in order of creation time. For interval data, this value SHALL increase with each interval time, and for block/tier readings, localID SHALL not be specified.

*/

/** @} */

/** @defgroup UInt16 UInt16
    @ingroup se_typedefs

    Unsigned integer, max inclusive 65535 (2^16-1)

    @{ */

/** @typedef SE_UInt16_t

    Unsigned integer, max inclusive 65535 (2^16-1)

*/

/** @} */

/** @defgroup VersionType VersionType
    @ingroup UInt16

    Version SHALL indicate a distinct identifier for each revision of an IdentifiedObject. If not specified, a default version of "0" (initial version) SHALL be assumed. Upon modification of any IdentifiedObject, the mRID SHALL remain the same, but the version SHALL be incremented. Servers MAY NOT modify objects that they did not create, unless they were notified of the change from the entity controlling the object's PEN.

    @{ */

/** @typedef SE_VersionType_t

    Version SHALL indicate a distinct identifier for each revision of an IdentifiedObject. If not specified, a default version of "0" (initial version) SHALL be assumed. Upon modification of any IdentifiedObject, the mRID SHALL remain the same, but the version SHALL be incremented. Servers MAY NOT modify objects that they did not create, unless they were notified of the change from the entity controlling the object's PEN.

*/

/** @} */

/** @defgroup String32 String32
    @ingroup se_typedefs

    Character string of max length 32. In order to limit internal storage, implementations SHALL reduce the length of strings using multi-byte characters so that the string may be stored using "maxLength" octets in the given encoding.

    @{ */

/** @typedef SE_String32_t

    Character string of max length 32. In order to limit internal storage, implementations SHALL reduce the length of strings using multi-byte characters so that the string may be stored using "maxLength" octets in the given encoding.

*/

/** @} */

/** @defgroup HexBinary128 HexBinary128
    @ingroup se_typedefs

    A 128-bit field encoded as a hex string (32 hex characters max). Where applicable, bit 0, or the least significant bit, goes on the right. Note that hexBinary requires pairs of hex characters, so an odd number of characters requires a leading "0".

    @{ */

/** @typedef SE_HexBinary128_t

    A 128-bit field encoded as a hex string (32 hex characters max). Where applicable, bit 0, or the least significant bit, goes on the right. Note that hexBinary requires pairs of hex characters, so an odd number of characters requires a leading "0".

*/

/** @} */

/** @defgroup mRIDType mRIDType
    @ingroup HexBinary128

    A master resource identifier. The IANA PEN [PEN] provider ID SHALL be specified in bits 0-31, the least-significant bits, and objects created by that provider SHALL be assigned unique IDs with the remaining 96 bits. 

    0xFFFFFFFFFFFFFFFFFFFFFFFF[XXXXXXXX], where [XXXXXXXX] is the PEN, is reserved for a object that is being created (e.g., a ReadingSet for the current time that is still accumulating). 

    Except for this special reserved identifier, each modification of an object (resource) representation MUST have a different "version".

    @{ */

/** @typedef SE_mRIDType_t

    A master resource identifier. The IANA PEN [PEN] provider ID SHALL be specified in bits 0-31, the least-significant bits, and objects created by that provider SHALL be assigned unique IDs with the remaining 96 bits. 

    0xFFFFFFFFFFFFFFFFFFFFFFFF[XXXXXXXX], where [XXXXXXXX] is the PEN, is reserved for a object that is being created (e.g., a ReadingSet for the current time that is still accumulating). 

    Except for this special reserved identifier, each modification of an object (resource) representation MUST have a different "version".

*/

/** @} */

/** @defgroup IdentifiedObject IdentifiedObject
    @ingroup Resource

    This is a root class to provide common naming attributes for all classes needing naming attributes

    @{ */

/** @struct SE_IdentifiedObject_t se_types.h

    This is a root class to provide common naming attributes for all classes needing naming attributes

    @var SE_IdentifiedObject_t::_flags
    @var SE_IdentifiedObject_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_IdentifiedObject_t::mRID
    The global identifier of the object.

    @var SE_IdentifiedObject_t::description
    The description is a human readable text describing or naming the object.

    @var SE_IdentifiedObject_t::version
    Contains the version number of the object. See the type definition for details.

*/

/** @} */

/** @defgroup ReadingSetBase ReadingSetBase
    @ingroup IdentifiedObject

    A set of Readings of the ReadingType indicated by the parent MeterReading. ReadingBase is abstract, used to define the elements common to ReadingSet and IntervalBlock.

    @{ */

/** @struct SE_ReadingSetBase_t se_types.h

    A set of Readings of the ReadingType indicated by the parent MeterReading. ReadingBase is abstract, used to define the elements common to ReadingSet and IntervalBlock.

    @var SE_ReadingSetBase_t::_flags
    @var SE_ReadingSetBase_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_ReadingSetBase_t::mRID
    The global identifier of the object.

    @var SE_ReadingSetBase_t::description
    The description is a human readable text describing or naming the object.

    @var SE_ReadingSetBase_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_ReadingSetBase_t::timePeriod
    Specifies the time range during which the contained readings were taken.

*/

/** @} */

/** @defgroup MirrorReadingSet MirrorReadingSet
    @ingroup ReadingSetBase

    A set of Readings of the ReadingType indicated by the parent MeterReading.

    @{ */

/** @struct SE_MirrorReadingSet_t se_types.h

    A set of Readings of the ReadingType indicated by the parent MeterReading.

    @var SE_MirrorReadingSet_t::_flags
    @var SE_MirrorReadingSet_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_MirrorReadingSet_t::mRID
    The global identifier of the object.

    @var SE_MirrorReadingSet_t::description
    The description is a human readable text describing or naming the object.

    @var SE_MirrorReadingSet_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_MirrorReadingSet_t::timePeriod
    Specifies the time range during which the contained readings were taken.

    @var SE_MirrorReadingSet_t::Reading
*/

/** @} */

/** @defgroup MeterReadingBase MeterReadingBase
    @ingroup IdentifiedObject

    A container for associating ReadingType, Readings and ReadingSets.

    @{ */

/** @struct SE_MeterReadingBase_t se_types.h

    A container for associating ReadingType, Readings and ReadingSets.

    @var SE_MeterReadingBase_t::_flags
    @var SE_MeterReadingBase_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_MeterReadingBase_t::mRID
    The global identifier of the object.

    @var SE_MeterReadingBase_t::description
    The description is a human readable text describing or naming the object.

    @var SE_MeterReadingBase_t::version
    Contains the version number of the object. See the type definition for details.

*/

/** @} */

/** @defgroup MirrorMeterReading MirrorMeterReading
    @ingroup MeterReadingBase

    Mimic of MeterReading used for managing mirrors.

    @{ */

/** @struct SE_MirrorMeterReading_t se_types.h

    Mimic of MeterReading used for managing mirrors.

    @var SE_MirrorMeterReading_t::_flags
#define SE_ReadingType_exists (1 << 5)

#define SE_Reading_exists (1 << 6)

#define SE_nextUpdateTime_exists (1 << 7)

#define SE_lastUpdateTime_exists (1 << 8)

    @var SE_MirrorMeterReading_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_MirrorMeterReading_t::mRID
    The global identifier of the object.

    @var SE_MirrorMeterReading_t::description
    The description is a human readable text describing or naming the object.

    @var SE_MirrorMeterReading_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_MirrorMeterReading_t::lastUpdateTime
    The date and time of the last update.

    @var SE_MirrorMeterReading_t::MirrorReadingSet
    @var SE_MirrorMeterReading_t::nextUpdateTime
    The date and time of the next planned update.

    @var SE_MirrorMeterReading_t::Reading
    @var SE_MirrorMeterReading_t::ReadingType
*/

/** @} */

/** @defgroup HexBinary160 HexBinary160
    @ingroup se_typedefs

    A 160-bit field encoded as a hex string (40 hex characters max). Where applicable, bit 0, or the least significant bit, goes on the right. Note that hexBinary requires pairs of hex characters, so an odd number of characters requires a leading "0".

    @{ */

/** @typedef SE_HexBinary160_t

    A 160-bit field encoded as a hex string (40 hex characters max). Where applicable, bit 0, or the least significant bit, goes on the right. Note that hexBinary requires pairs of hex characters, so an odd number of characters requires a leading "0".

*/

/** @} */

/** @defgroup ServiceKind ServiceKind
    @ingroup UInt8

    Service kind

    0 - electricity

    1 - gas

    2 - water

    3 - time

    4 - pressure

    5 - heat

    6 - cooling

    All other values reserved.

    @{ */

/** @typedef SE_ServiceKind_t

    Service kind

    0 - electricity

    1 - gas

    2 - water

    3 - time

    4 - pressure

    5 - heat

    6 - cooling

    All other values reserved.

*/

/** @} */

/** @defgroup RoleFlagsType RoleFlagsType
    @ingroup HexBinary16

    Specifies the roles that apply to a usage point.

    Bit 0 - isMirror - SHALL be set if the server is not the measurement device

    Bit 1 - isPremisesAggregationPoint - SHALL be set if the UsagePoint is the point of delivery for a premises

    Bit 2 - isPEV - SHALL be set if the usage applies to an electric vehicle

    Bit 3 - isDER - SHALL be set if the usage applies to a distributed energy resource, capable of delivering power to the grid.

    Bit 4 - isRevenueQuality - SHALL be set if usage was measured by a device certified as revenue quality

    Bit 5 - isDC - SHALL be set if the usage point measures direct current 

    Bit 6 - isSubmeter - SHALL be set if the usage point is not a premises aggregation point

    Bit 7-15 - Reserved

    @{ */

/** @typedef SE_RoleFlagsType_t

    Specifies the roles that apply to a usage point.

    Bit 0 - isMirror - SHALL be set if the server is not the measurement device

    Bit 1 - isPremisesAggregationPoint - SHALL be set if the UsagePoint is the point of delivery for a premises

    Bit 2 - isPEV - SHALL be set if the usage applies to an electric vehicle

    Bit 3 - isDER - SHALL be set if the usage applies to a distributed energy resource, capable of delivering power to the grid.

    Bit 4 - isRevenueQuality - SHALL be set if usage was measured by a device certified as revenue quality

    Bit 5 - isDC - SHALL be set if the usage point measures direct current 

    Bit 6 - isSubmeter - SHALL be set if the usage point is not a premises aggregation point

    Bit 7-15 - Reserved

*/

/** @} */

/** @defgroup UsagePointBase UsagePointBase
    @ingroup IdentifiedObject

    Logical point on a network at which consumption or production is either physically measured (e.g. metered) or estimated (e.g. unmetered street lights). A container for associating ReadingType, Readings and ReadingSets.

    @{ */

/** @struct SE_UsagePointBase_t se_types.h

    Logical point on a network at which consumption or production is either physically measured (e.g. metered) or estimated (e.g. unmetered street lights). A container for associating ReadingType, Readings and ReadingSets.

    @var SE_UsagePointBase_t::_flags
    @var SE_UsagePointBase_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_UsagePointBase_t::mRID
    The global identifier of the object.

    @var SE_UsagePointBase_t::description
    The description is a human readable text describing or naming the object.

    @var SE_UsagePointBase_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_UsagePointBase_t::roleFlags
    Specifies the roles that apply to the usage point.

    @var SE_UsagePointBase_t::serviceCategoryKind
    The kind of service provided by this usage point.

    @var SE_UsagePointBase_t::status
    Specifies the current status of the service at this usage point.

    0 = off

    1 = on

*/

/** @} */

/** @defgroup MirrorUsagePoint MirrorUsagePoint
    @ingroup UsagePointBase

    A parallel to UsagePoint to support mirroring

    @{ */

/** @struct SE_MirrorUsagePoint_t se_types.h

    A parallel to UsagePoint to support mirroring

    @var SE_MirrorUsagePoint_t::_flags
#define SE_postRate_exists (1 << 5)

    @var SE_MirrorUsagePoint_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_MirrorUsagePoint_t::mRID
    The global identifier of the object.

    @var SE_MirrorUsagePoint_t::description
    The description is a human readable text describing or naming the object.

    @var SE_MirrorUsagePoint_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_MirrorUsagePoint_t::roleFlags
    Specifies the roles that apply to the usage point.

    @var SE_MirrorUsagePoint_t::serviceCategoryKind
    The kind of service provided by this usage point.

    @var SE_MirrorUsagePoint_t::status
    Specifies the current status of the service at this usage point.

    0 = off

    1 = on

    @var SE_MirrorUsagePoint_t::deviceLFDI
    The LFDI of the device being mirrored.

    @var SE_MirrorUsagePoint_t::MirrorMeterReading
    @var SE_MirrorUsagePoint_t::postRate
    POST rate, or how often mirrored data should be POSTed, in seconds. A client MAY indicate a preferred postRate when POSTing MirrorUsagePoint. A server MAY add or modify postRate to indicate its preferred posting rate.

*/

/** @} */

/** @defgroup List List
    @ingroup Resource

    Container to hold a collection of object instances or references. See [ZB 11-0167] Design Patterns section for additional details.

    @{ */

/** @struct SE_List_t se_types.h

    Container to hold a collection of object instances or references. See [ZB 11-0167] Design Patterns section for additional details.

    @var SE_List_t::_flags
    @var SE_List_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_List_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_List_t::results
    Indicates the number of items in this page of results.

*/

/** @} */

/** @defgroup MirrorUsagePointList MirrorUsagePointList
    @ingroup List

    A List of MirrorUsagePoint instances.

    @{ */

/** @struct SE_MirrorUsagePointList_t se_types.h

    A List of MirrorUsagePoint instances.

    @var SE_MirrorUsagePointList_t::_flags
    @var SE_MirrorUsagePointList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_MirrorUsagePointList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_MirrorUsagePointList_t::results
    Indicates the number of items in this page of results.

    @var SE_MirrorUsagePointList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_MirrorUsagePointList_t::MirrorUsagePoint
*/

/** @} */

/** @defgroup MirrorMeterReadingList MirrorMeterReadingList
    @ingroup List

    A List of MirrorMeterReading instances.

    @{ */

/** @struct SE_MirrorMeterReadingList_t se_types.h

    A List of MirrorMeterReading instances.

    @var SE_MirrorMeterReadingList_t::_flags
    @var SE_MirrorMeterReadingList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_MirrorMeterReadingList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_MirrorMeterReadingList_t::results
    Indicates the number of items in this page of results.

    @var SE_MirrorMeterReadingList_t::MirrorMeterReading
*/

/** @} */

/** @defgroup UInt64 UInt64
    @ingroup se_typedefs

    Unsigned integer, max inclusive 18446744073709551615 (2^64-1)

    @{ */

/** @typedef SE_UInt64_t

    Unsigned integer, max inclusive 18446744073709551615 (2^64-1)

*/

/** @} */

/** @defgroup HexBinary48 HexBinary48
    @ingroup se_typedefs

    A 48-bit field encoded as a hex string (12 hex characters max). Where applicable, bit 0, or the least significant bit, goes on the right. Note that hexBinary requires pairs of hex characters, so an odd number of characters requires a leading "0".

    @{ */

/** @typedef SE_HexBinary48_t

    A 48-bit field encoded as a hex string (12 hex characters max). Where applicable, bit 0, or the least significant bit, goes on the right. Note that hexBinary requires pairs of hex characters, so an odd number of characters requires a leading "0".

*/

/** @} */

/** @defgroup Error Error
    @ingroup se_types

    Contains information about the nature of an error if a request could not be completed successfully.

    @{ */

/** @struct SE_Error_t se_types.h

    Contains information about the nature of an error if a request could not be completed successfully.

    @var SE_Error_t::_flags
#define SE_maxRetryDuration_exists (1 << 0)

    @var SE_Error_t::maxRetryDuration
    Contains the number of seconds the client SHOULD wait before retrying the request.

    @var SE_Error_t::reasonCode
    Code indicating the reason for failure.

    0 - Invalid request format

    1 - Invalid request values (e.g. invalid threshold values)

    2 - Resource limit reached

    3 - Conditional subscription field not supported

    4 - Maximum request frequency exceeded

    All other values reserved

*/

/** @} */

/** @defgroup HexBinary8 HexBinary8
    @ingroup se_typedefs

    An 8-bit field encoded as a hex string (2 hex characters). Where applicable, bit 0, or the least significant bit, goes on the right. Note that hexBinary requires pairs of hex characters, so an odd number of characters requires a leading "0".

    @{ */

/** @typedef SE_HexBinary8_t

    An 8-bit field encoded as a hex string (2 hex characters). Where applicable, bit 0, or the least significant bit, goes on the right. Note that hexBinary requires pairs of hex characters, so an odd number of characters requires a leading "0".

*/

/** @} */

/** @defgroup RespondableResource RespondableResource
    @ingroup Resource

    A Resource to which a Response can be requested.

    @{ */

/** @struct SE_RespondableResource_t se_types.h

    A Resource to which a Response can be requested.

    @var SE_RespondableResource_t::_flags
#define SE_responseRequired_exists (1 << 5)

    @var SE_RespondableResource_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_RespondableResource_t::replyTo
    A reference to the response resource address (URI). Required on a response to a GET if responseRequired is "true".

    @var SE_RespondableResource_t::responseRequired
    Indicates whether or not a response is required upon receipt, creation or update of this resource. Responses shall be posted to the collection specified in "replyTo". 

    If the resource has a deviceCategory field, devices that match one or more of the device types indicated in deviceCategory SHALL respond according to the rules listed below.  If the category does not match, the device SHALL NOT respond. If the resource does not have a deviceCategory field, a device receiving the resource SHALL respond according to the rules listed below.

    Value encoded as hex according to the following bit assignments, any combination is possible. 

    See Table 10-10 for the list of appropriate Response status codes to be sent for these purposes.

    0 - End device shall indicate that message was received 

    1 - End device shall indicate specific response. 

    2 - End user / customer response is required. 

    All other values reserved.

*/

/** @} */

/** @defgroup RespondableIdentifiedObject RespondableIdentifiedObject
    @ingroup RespondableResource

    An IdentifiedObject to which a Response can be requested.

    @{ */

/** @struct SE_RespondableIdentifiedObject_t se_types.h

    An IdentifiedObject to which a Response can be requested.

    @var SE_RespondableIdentifiedObject_t::_flags
#define SE_responseRequired_exists (1 << 5)

    @var SE_RespondableIdentifiedObject_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_RespondableIdentifiedObject_t::replyTo
    A reference to the response resource address (URI). Required on a response to a GET if responseRequired is "true".

    @var SE_RespondableIdentifiedObject_t::responseRequired
    Indicates whether or not a response is required upon receipt, creation or update of this resource. Responses shall be posted to the collection specified in "replyTo". 

    If the resource has a deviceCategory field, devices that match one or more of the device types indicated in deviceCategory SHALL respond according to the rules listed below.  If the category does not match, the device SHALL NOT respond. If the resource does not have a deviceCategory field, a device receiving the resource SHALL respond according to the rules listed below.

    Value encoded as hex according to the following bit assignments, any combination is possible. 

    See Table 10-10 for the list of appropriate Response status codes to be sent for these purposes.

    0 - End device shall indicate that message was received 

    1 - End device shall indicate specific response. 

    2 - End user / customer response is required. 

    All other values reserved.

    @var SE_RespondableIdentifiedObject_t::mRID
    The global identifier of the object.

    @var SE_RespondableIdentifiedObject_t::description
    The description is a human readable text describing or naming the object.

    @var SE_RespondableIdentifiedObject_t::version
    Contains the version number of the object. See the type definition for details.

*/

/** @} */

/** @defgroup Link Link
    @ingroup se_types

    Links provide a reference, via URI, to another resource.

    @{ */

/** @struct SE_Link_t se_types.h

    Links provide a reference, via URI, to another resource.

    @var SE_Link_t::_flags
    @var SE_Link_t::href
    A URI reference.

*/

/** @} */

/** @defgroup ListLink ListLink
    @ingroup Link

    ListLinks provide a reference, via URI, to a List.

    @{ */

/** @struct SE_ListLink_t se_types.h

    ListLinks provide a reference, via URI, to a List.

    @var SE_ListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_ListLink_t::href
    A URI reference.

    @var SE_ListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup NotificationListLink NotificationListLink
    @ingroup ListLink

    SHALL contain a Link to a List of Notification instances.

    @{ */

/** @struct SE_NotificationListLink_t se_types.h

    SHALL contain a Link to a List of Notification instances.

    @var SE_NotificationListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_NotificationListLink_t::href
    A URI reference.

    @var SE_NotificationListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup MeterReadingLink MeterReadingLink
    @ingroup Link

    SHALL contain a Link to an instance of MeterReading.

    @{ */

/** @struct SE_MeterReadingLink_t se_types.h

    SHALL contain a Link to an instance of MeterReading.

    @var SE_MeterReadingLink_t::_flags
    @var SE_MeterReadingLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup EndDeviceLink EndDeviceLink
    @ingroup Link

    SHALL contain a Link to an instance of EndDevice.

    @{ */

/** @struct SE_EndDeviceLink_t se_types.h

    SHALL contain a Link to an instance of EndDevice.

    @var SE_EndDeviceLink_t::_flags
    @var SE_EndDeviceLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup DeviceCapabilityLink DeviceCapabilityLink
    @ingroup Link

    SHALL contain a Link to an instance of DeviceCapability.

    @{ */

/** @struct SE_DeviceCapabilityLink_t se_types.h

    SHALL contain a Link to an instance of DeviceCapability.

    @var SE_DeviceCapabilityLink_t::_flags
    @var SE_DeviceCapabilityLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup DERProgramLink DERProgramLink
    @ingroup Link

    SHALL contain a Link to an instance of DERProgram.

    @{ */

/** @struct SE_DERProgramLink_t se_types.h

    SHALL contain a Link to an instance of DERProgram.

    @var SE_DERProgramLink_t::_flags
    @var SE_DERProgramLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup DERLink DERLink
    @ingroup Link

    SHALL contain a Link to an instance of DER.

    @{ */

/** @struct SE_DERLink_t se_types.h

    SHALL contain a Link to an instance of DER.

    @var SE_DERLink_t::_flags
    @var SE_DERLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup CustomerAccountLink CustomerAccountLink
    @ingroup Link

    SHALL contain a Link to an instance of CustomerAccount.

    @{ */

/** @struct SE_CustomerAccountLink_t se_types.h

    SHALL contain a Link to an instance of CustomerAccount.

    @var SE_CustomerAccountLink_t::_flags
    @var SE_CustomerAccountLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup ActiveFlowReservationListLink ActiveFlowReservationListLink
    @ingroup ListLink

    SHALL contain a Link to a List of active FlowReservation instances.

    @{ */

/** @struct SE_ActiveFlowReservationListLink_t se_types.h

    SHALL contain a Link to a List of active FlowReservation instances.

    @var SE_ActiveFlowReservationListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_ActiveFlowReservationListLink_t::href
    A URI reference.

    @var SE_ActiveFlowReservationListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup UnsignedFixedPointType UnsignedFixedPointType
    @ingroup se_types

    Abstract type for specifying an unsigned fixed-point value without a given unit of measure.

    @{ */

/** @struct SE_UnsignedFixedPointType_t se_types.h

    Abstract type for specifying an unsigned fixed-point value without a given unit of measure.

    @var SE_UnsignedFixedPointType_t::multiplier
    Specifies exponent of uom.

    @var SE_UnsignedFixedPointType_t::value
    Dimensionless value

*/

/** @} */

/** @defgroup Int16 Int16
    @ingroup se_typedefs

    Signed integer, min -32768 max +32767

    @{ */

/** @typedef SE_Int16_t

    Signed integer, min -32768 max +32767

*/

/** @} */

/** @defgroup FixedPointType FixedPointType
    @ingroup se_types

    Abstract type for specifying a fixed-point value without a given unit of measure.

    @{ */

/** @struct SE_FixedPointType_t se_types.h

    Abstract type for specifying a fixed-point value without a given unit of measure.

    @var SE_FixedPointType_t::multiplier
    Specifies exponent of uom.

    @var SE_FixedPointType_t::value
    Dimensionless value

*/

/** @} */

/** @defgroup StorageModeStatusType StorageModeStatusType
    @ingroup se_types

    DER StorageModeStatus value:

    0 – storage charging

    1 – storage discharging

    2 – storage holding

    All other values reserved.

    @{ */

/** @struct SE_StorageModeStatusType_t se_types.h

    DER StorageModeStatus value:

    0 – storage charging

    1 – storage discharging

    2 – storage holding

    All other values reserved.

    @var SE_StorageModeStatusType_t::dateTime
    The date and time at which the state applied.

    @var SE_StorageModeStatusType_t::value
    The value indicating the state.

*/

/** @} */

/** @defgroup PerCent PerCent
    @ingroup UInt16

    Used for percentages, specified in hundredths of a percent, 0 - 10000. (10000 = 100%)

    @{ */

/** @typedef SE_PerCent_t

    Used for percentages, specified in hundredths of a percent, 0 - 10000. (10000 = 100%)

*/

/** @} */

/** @defgroup StateOfChargeStatusType StateOfChargeStatusType
    @ingroup se_types

    DER StateOfChargeStatus value: Percent data type

    @{ */

/** @struct SE_StateOfChargeStatusType_t se_types.h

    DER StateOfChargeStatus value: Percent data type

    @var SE_StateOfChargeStatusType_t::dateTime
    The date and time at which the state applied.

    @var SE_StateOfChargeStatusType_t::value
    The value indicating the state.

*/

/** @} */

/** @defgroup OperationalModeStatusType OperationalModeStatusType
    @ingroup se_types

    DER OperationalModeStatus value:

    0 - Not applicable / Unknown

    1 - Off

    2 - Operational mode

    3 - Test mode

    All other values reserved.

    @{ */

/** @struct SE_OperationalModeStatusType_t se_types.h

    DER OperationalModeStatus value:

    0 - Not applicable / Unknown

    1 - Off

    2 - Operational mode

    3 - Test mode

    All other values reserved.

    @var SE_OperationalModeStatusType_t::dateTime
    The date and time at which the state applied.

    @var SE_OperationalModeStatusType_t::value
    The value indicating the state.

*/

/** @} */

/** @defgroup String6 String6
    @ingroup se_typedefs

    Character string of max length 6. In order to limit internal storage, implementations SHALL reduce the length of strings using multi-byte characters so that the string may be stored using "maxLength" octets in the given encoding.

    @{ */

/** @typedef SE_String6_t

    Character string of max length 6. In order to limit internal storage, implementations SHALL reduce the length of strings using multi-byte characters so that the string may be stored using "maxLength" octets in the given encoding.

*/

/** @} */

/** @defgroup ManufacturerStatusType ManufacturerStatusType
    @ingroup se_types

    DER ManufacturerStatus/value: String data type

    @{ */

/** @struct SE_ManufacturerStatusType_t se_types.h

    DER ManufacturerStatus/value: String data type

    @var SE_ManufacturerStatusType_t::dateTime
    The date and time at which the state applied.

    @var SE_ManufacturerStatusType_t::value
    The value indicating the state.

*/

/** @} */

/** @defgroup LocalControlModeStatusType LocalControlModeStatusType
    @ingroup se_types

    DER LocalControlModeStatus/value:

    0 – local control

    1 – remote control

    All other values reserved.

    @{ */

/** @struct SE_LocalControlModeStatusType_t se_types.h

    DER LocalControlModeStatus/value:

    0 – local control

    1 – remote control

    All other values reserved.

    @var SE_LocalControlModeStatusType_t::dateTime
    The date and time at which the state applied.

    @var SE_LocalControlModeStatusType_t::value
    The value indicating the state.

*/

/** @} */

/** @defgroup InverterStatusType InverterStatusType
    @ingroup se_types

    DER InverterStatus value:

    0 - N/A

    1 - off

    2 - sleeping (auto-shutdown) or DER is at low output power/voltage

    3 - starting up or ON but not producing power

    4 - tracking MPPT power point

    5 - forced power reduction/derating

    6 - shutting down

    7 - one or more faults exist

    8 - standby (service on unit) - DER may be at high output voltage/power

    9 - test mode

    10 - as defined in manufacturer status

    All other values reserved.

    @{ */

/** @struct SE_InverterStatusType_t se_types.h

    DER InverterStatus value:

    0 - N/A

    1 - off

    2 - sleeping (auto-shutdown) or DER is at low output power/voltage

    3 - starting up or ON but not producing power

    4 - tracking MPPT power point

    5 - forced power reduction/derating

    6 - shutting down

    7 - one or more faults exist

    8 - standby (service on unit) - DER may be at high output voltage/power

    9 - test mode

    10 - as defined in manufacturer status

    All other values reserved.

    @var SE_InverterStatusType_t::dateTime
    The date and time at which the state applied.

    @var SE_InverterStatusType_t::value
    The value indicating the state.

*/

/** @} */

/** @defgroup ConnectStatusType ConnectStatusType
    @ingroup se_types

    DER ConnectStatus value (bitmap):

    0 - Connected

    1 - Available

    2 - Operating

    3 - Test

    4 - Fault / Error

    All other values reserved.

    @{ */

/** @struct SE_ConnectStatusType_t se_types.h

    DER ConnectStatus value (bitmap):

    0 - Connected

    1 - Available

    2 - Operating

    3 - Test

    4 - Fault / Error

    All other values reserved.

    @var SE_ConnectStatusType_t::dateTime
    The date and time at which the state applied.

    @var SE_ConnectStatusType_t::value
    The value indicating the state.

*/

/** @} */

/** @defgroup SubscribableResource SubscribableResource
    @ingroup Resource

    A Resource to which a Subscription can be requested.

    @{ */

/** @struct SE_SubscribableResource_t se_types.h

    A Resource to which a Subscription can be requested.

    @var SE_SubscribableResource_t::_flags
    @var SE_SubscribableResource_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_SubscribableResource_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

*/

/** @} */

/** @defgroup DERStatus DERStatus
    @ingroup SubscribableResource

    DER status information.

    @{ */

/** @struct SE_DERStatus_t se_types.h

    DER status information.

    @var SE_DERStatus_t::_flags
#define SE_storConnectStatus_exists (1 << 5)

#define SE_storageModeStatus_exists (1 << 6)

#define SE_stateOfChargeStatus_exists (1 << 7)

#define SE_operationalModeStatus_exists (1 << 8)

#define SE_manufacturerStatus_exists (1 << 9)

#define SE_localControlModeStatus_exists (1 << 10)

#define SE_inverterStatus_exists (1 << 11)

#define SE_genConnectStatus_exists (1 << 12)

    @var SE_DERStatus_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DERStatus_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_DERStatus_t::genConnectStatus
    Connect/status value for generator DER. 

    See ConnectStatusType for values.

    @var SE_DERStatus_t::inverterStatus
    DER InverterStatus/value.

    See InverterStatusType for values.

    @var SE_DERStatus_t::localControlModeStatus
    The local control mode status.

    See LocalControlModeStatusType for values.

    @var SE_DERStatus_t::manufacturerStatus
    Manufacturer status code.

    @var SE_DERStatus_t::operationalModeStatus
    Operational mode currently in use.

    See OperationalModeStatusType for values.

    @var SE_DERStatus_t::readingTime
    The timestamp when the current status was last updated.

    @var SE_DERStatus_t::stateOfChargeStatus
    State of charge status.

    See StateOfChargeStatusType for values.

    @var SE_DERStatus_t::storageModeStatus
    Storage mode status.

    See StorageModeStatusType for values.

    @var SE_DERStatus_t::storConnectStatus
    Connect/status value for storage DER.

    See ConnectStatusType for values.

*/

/** @} */

/** @defgroup PrimacyType PrimacyType
    @ingroup UInt8

    Values possible for indication of "Primary" provider: 

    0: In home energy management system

    1: Contracted premises service provider

    2: Non-contractual service provider

    3 - 64: Reserved

    65 - 191: User-defined

    192 - 255: Reserved

    Lower numbers indicate higher priority.

    @{ */

/** @typedef SE_PrimacyType_t

    Values possible for indication of "Primary" provider: 

    0: In home energy management system

    1: Contracted premises service provider

    2: Non-contractual service provider

    3 - 64: Reserved

    65 - 191: User-defined

    192 - 255: Reserved

    Lower numbers indicate higher priority.

*/

/** @} */

/** @defgroup DERCurveListLink DERCurveListLink
    @ingroup ListLink

    SHALL contain a Link to a List of DERCurve instances.

    @{ */

/** @struct SE_DERCurveListLink_t se_types.h

    SHALL contain a Link to a List of DERCurve instances.

    @var SE_DERCurveListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_DERCurveListLink_t::href
    A URI reference.

    @var SE_DERCurveListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup DERControlListLink DERControlListLink
    @ingroup ListLink

    SHALL contain a Link to a List of DERControl instances.

    @{ */

/** @struct SE_DERControlListLink_t se_types.h

    SHALL contain a Link to a List of DERControl instances.

    @var SE_DERControlListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_DERControlListLink_t::href
    A URI reference.

    @var SE_DERControlListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup DefaultDERControlLink DefaultDERControlLink
    @ingroup Link

    SHALL contain a Link to an instance of DefaultDERControl. This is the default mode of the DER which MAY be overridden by DERControl events.

    @{ */

/** @struct SE_DefaultDERControlLink_t se_types.h

    SHALL contain a Link to an instance of DefaultDERControl. This is the default mode of the DER which MAY be overridden by DERControl events.

    @var SE_DefaultDERControlLink_t::_flags
    @var SE_DefaultDERControlLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup ActiveDERControlListLink ActiveDERControlListLink
    @ingroup ListLink

    SHALL contain a Link to a List of active DERControl instances.

    @{ */

/** @struct SE_ActiveDERControlListLink_t se_types.h

    SHALL contain a Link to a List of active DERControl instances.

    @var SE_ActiveDERControlListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_ActiveDERControlListLink_t::href
    A URI reference.

    @var SE_ActiveDERControlListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup SubscribableIdentifiedObject SubscribableIdentifiedObject
    @ingroup SubscribableResource

    An IdentifiedObject to which a Subscription can be requested.

    @{ */

/** @struct SE_SubscribableIdentifiedObject_t se_types.h

    An IdentifiedObject to which a Subscription can be requested.

    @var SE_SubscribableIdentifiedObject_t::_flags
    @var SE_SubscribableIdentifiedObject_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_SubscribableIdentifiedObject_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_SubscribableIdentifiedObject_t::mRID
    The global identifier of the object.

    @var SE_SubscribableIdentifiedObject_t::description
    The description is a human readable text describing or naming the object.

    @var SE_SubscribableIdentifiedObject_t::version
    Contains the version number of the object. See the type definition for details.

*/

/** @} */

/** @defgroup DERProgram DERProgram
    @ingroup SubscribableIdentifiedObject

    Distributed Energy Resource program.

    @{ */

/** @struct SE_DERProgram_t se_types.h

    Distributed Energy Resource program.

    @var SE_DERProgram_t::_flags
#define SE_DERCurveListLink_exists (1 << 5)

#define SE_DERControlListLink_exists (1 << 6)

#define SE_DefaultDERControlLink_exists (1 << 7)

#define SE_ActiveDERControlListLink_exists (1 << 8)

    @var SE_DERProgram_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DERProgram_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_DERProgram_t::mRID
    The global identifier of the object.

    @var SE_DERProgram_t::description
    The description is a human readable text describing or naming the object.

    @var SE_DERProgram_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_DERProgram_t::ActiveDERControlListLink
    @var SE_DERProgram_t::DefaultDERControlLink
    @var SE_DERProgram_t::DERControlListLink
    @var SE_DERProgram_t::DERCurveListLink
    @var SE_DERProgram_t::primacy
    Indicates the relative primacy of the provider of this Program.

*/

/** @} */

/** @defgroup SubscribableList SubscribableList
    @ingroup SubscribableResource

    A List to which a Subscription can be requested.

    @{ */

/** @struct SE_SubscribableList_t se_types.h

    A List to which a Subscription can be requested.

    @var SE_SubscribableList_t::_flags
    @var SE_SubscribableList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_SubscribableList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_SubscribableList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_SubscribableList_t::results
    Indicates the number of items in this page of results.

*/

/** @} */

/** @defgroup DERProgramList DERProgramList
    @ingroup SubscribableList

    A List element to hold DERProgram objects.

    @{ */

/** @struct SE_DERProgramList_t se_types.h

    A List element to hold DERProgram objects.

    @var SE_DERProgramList_t::_flags
    @var SE_DERProgramList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DERProgramList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_DERProgramList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_DERProgramList_t::results
    Indicates the number of items in this page of results.

    @var SE_DERProgramList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_DERProgramList_t::DERProgram
*/

/** @} */

/** @defgroup DERUnitRefType DERUnitRefType
    @ingroup UInt8

    Specifies context for interpreting percent values:

    0 - N/A

    1 - %setMaxW

    2 - %setMaxVar

    3 - %statVarAvail

    4 - %setEffectiveV

    5 - %setMaxChargeRateW

    6 - %setMaxDischargeRateW

    All other values reserved.

    @{ */

/** @typedef SE_DERUnitRefType_t

    Specifies context for interpreting percent values:

    0 - N/A

    1 - %setMaxW

    2 - %setMaxVar

    3 - %statVarAvail

    4 - %setEffectiveV

    5 - %setMaxChargeRateW

    6 - %setMaxDischargeRateW

    All other values reserved.

*/

/** @} */

/** @defgroup DERCurveType DERCurveType
    @ingroup UInt8

    0 - opModVoltVar (Volt-Var Mode)

    1 - opModFreqWatt (Frequency-Watt Curve Mode)

    2 - opModWattPF (Watt-PowerFactor Mode)

    3 - opModVoltWatt (Volt-Watt Mode)

    4 - opModLVRTMomentaryCessation (Low Voltage Ride Through, Momentary Cessation Mode)

    5 - opModLVRTMustTrip (Low Voltage Ride Through, Must Trip Mode)

    6 - opModHVRTMomentaryCessation (High Voltage Ride Through, Momentary Cessation Mode)

    7 - opModHVRTMustTrip (High Voltage Ride Through, Must Trip Mode)

    8 - opModLFRTMustTrip (Low Frequency Ride Through, Must Trip Mode)

    9 - opModHFRTMustTrip (High Frequency Ride Through, Must Trip Mode)

    10 - opModWattVar (Watt-Var Mode)

    All other values reserved.

    @{ */

/** @typedef SE_DERCurveType_t

    0 - opModVoltVar (Volt-Var Mode)

    1 - opModFreqWatt (Frequency-Watt Curve Mode)

    2 - opModWattPF (Watt-PowerFactor Mode)

    3 - opModVoltWatt (Volt-Watt Mode)

    4 - opModLVRTMomentaryCessation (Low Voltage Ride Through, Momentary Cessation Mode)

    5 - opModLVRTMustTrip (Low Voltage Ride Through, Must Trip Mode)

    6 - opModHVRTMomentaryCessation (High Voltage Ride Through, Momentary Cessation Mode)

    7 - opModHVRTMustTrip (High Voltage Ride Through, Must Trip Mode)

    8 - opModLFRTMustTrip (Low Frequency Ride Through, Must Trip Mode)

    9 - opModHFRTMustTrip (High Frequency Ride Through, Must Trip Mode)

    10 - opModWattVar (Watt-Var Mode)

    All other values reserved.

*/

/** @} */

/** @defgroup CurveData CurveData
    @ingroup se_types

    Data point values for defining a curve or schedule

    @{ */

/** @struct SE_CurveData_t se_types.h

    Data point values for defining a curve or schedule

    @var SE_CurveData_t::xvalue
    The data value of the X-axis (independent) variable, depending on the curve type. See definitions in DERControlBase for further information.

    @var SE_CurveData_t::yvalue
    The data value of the Y-axis (dependent) variable, depending on the curve type. See definitions in DERControlBase for further information.

*/

/** @} */

/** @defgroup DERCurve DERCurve
    @ingroup IdentifiedObject

    DER related curves such as Volt-Var mode curves. Relationship between an independent variable (X-axis) and a dependent variable (Y-axis).

    @{ */

/** @struct SE_DERCurve_t se_types.h

    DER related curves such as Volt-Var mode curves. Relationship between an independent variable (X-axis) and a dependent variable (Y-axis).

    @var SE_DERCurve_t::_flags
#define SE_rampPT1Tms_exists (1 << 5)

#define SE_rampIncTms_exists (1 << 6)

#define SE_rampDecTms_exists (1 << 7)

#define SE_openLoopTms_exists (1 << 8)

#define SE_CurveData_count(x) (((se_flags (x) >> 9) & 0xf) + 1)
#define SE_set_CurveData_count(x, n) (se_flags(x) |= (((n) - 1) & 0xf) << 9)

    @var SE_DERCurve_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DERCurve_t::mRID
    The global identifier of the object.

    @var SE_DERCurve_t::description
    The description is a human readable text describing or naming the object.

    @var SE_DERCurve_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_DERCurve_t::creationTime
    The time at which the object was created.

    @var SE_DERCurve_t::CurveData
    @var SE_DERCurve_t::curveType
    Specifies the associated curve-based control mode.

    @var SE_DERCurve_t::openLoopTms
    Open loop response time, the time to ramp up to 90% of the new reactive power target in response to the change in voltage, in hundredths of a second. Resolution is 1/100 sec. A value of 0 is used to mean no limit. When not present, the device SHOULD follow its default behavior.

    @var SE_DERCurve_t::rampDecTms
    Decreasing ramp rate, interpreted as a percentage change in output capability limit per second (e.g. %setMaxW / sec).  Resolution is in hundredths of a percent/second. A value of 0 means there is no limit. If absent, ramp rate defaults to setGradW.

    @var SE_DERCurve_t::rampIncTms
    Increasing ramp rate, interpreted as a percentage change in output capability limit per second (e.g. %setMaxW / sec).  Resolution is in hundredths of a percent/second. A value of 0 means there is no limit. If absent, ramp rate defaults to rampDecTms.

    @var SE_DERCurve_t::rampPT1Tms
    The configuration parameter for a low-pass filter, PT1 is a time, in hundredths of a second, in which the filter will settle to 95% of a step change in the input value. Resolution is 1/100 sec.

    @var SE_DERCurve_t::xMultiplier
    Exponent for X-axis value.

    @var SE_DERCurve_t::yMultiplier
    Exponent for Y-axis value.

    @var SE_DERCurve_t::yRefType
    The Y-axis units context.

*/

/** @} */

/** @defgroup DERCurveList DERCurveList
    @ingroup List

    A List element to hold DERCurve objects.

    @{ */

/** @struct SE_DERCurveList_t se_types.h

    A List element to hold DERCurve objects.

    @var SE_DERCurveList_t::_flags
    @var SE_DERCurveList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DERCurveList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_DERCurveList_t::results
    Indicates the number of items in this page of results.

    @var SE_DERCurveList_t::DERCurve
*/

/** @} */

/** @defgroup ActivePower ActivePower
    @ingroup se_types

    The active (real) power P (in W) is the product of root-mean-square (RMS) voltage, RMS current, and cos(theta) where theta is the phase angle of current relative to voltage.  It is the primary measure of the rate of flow of energy.

    @{ */

/** @struct SE_ActivePower_t se_types.h

    The active (real) power P (in W) is the product of root-mean-square (RMS) voltage, RMS current, and cos(theta) where theta is the phase angle of current relative to voltage.  It is the primary measure of the rate of flow of energy.

    @var SE_ActivePower_t::multiplier
    Specifies exponent for uom.

    @var SE_ActivePower_t::value
    Value in watts (uom 38)

*/

/** @} */

/** @defgroup ReactivePower ReactivePower
    @ingroup se_types

    The reactive power Q (in var) is the product of root mean square (RMS) voltage, RMS current, and sin(theta) where theta is the phase angle of current relative to voltage.

    @{ */

/** @struct SE_ReactivePower_t se_types.h

    The reactive power Q (in var) is the product of root mean square (RMS) voltage, RMS current, and sin(theta) where theta is the phase angle of current relative to voltage.

    @var SE_ReactivePower_t::multiplier
    Specifies exponent of uom.

    @var SE_ReactivePower_t::value
    Value in volt-amperes reactive (var) (uom 63)

*/

/** @} */

/** @defgroup DERCurveLink DERCurveLink
    @ingroup Link

    SHALL contain a Link to an instance of DERCurve.

    @{ */

/** @struct SE_DERCurveLink_t se_types.h

    SHALL contain a Link to an instance of DERCurve.

    @var SE_DERCurveLink_t::_flags
    @var SE_DERCurveLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup FreqDroopType FreqDroopType
    @ingroup se_types

    Type for Frequency-Droop (Frequency-Watt) operation.

    @{ */

/** @struct SE_FreqDroopType_t se_types.h

    Type for Frequency-Droop (Frequency-Watt) operation.

    @var SE_FreqDroopType_t::dBOF
    Frequency droop dead band for over-frequency conditions. In thousandths of Hz.

    @var SE_FreqDroopType_t::dBUF
    Frequency droop dead band for under-frequency conditions. In thousandths of Hz.

    @var SE_FreqDroopType_t::kOF
    Frequency droop per-unit frequency change for over-frequency conditions corresponding to 1 per-unit power output change. In thousandths, unitless.

    @var SE_FreqDroopType_t::kUF
    Frequency droop per-unit frequency change for under-frequency conditions corresponding to 1 per-unit power output change. In thousandths, unitless.

    @var SE_FreqDroopType_t::openLoopTms
    Open loop response time, the duration from a step change in control signal input until the output changes by 90% of its final change before any overshoot, in hundredths of a second. Resolution is 1/100 sec. A value of 0 is used to mean no limit.

*/

/** @} */

/** @defgroup SignedPerCent SignedPerCent
    @ingroup Int16

    Used for signed percentages, specified in hundredths of a percent, -10000 - 10000. (10000 = 100%)

    @{ */

/** @typedef SE_SignedPerCent_t

    Used for signed percentages, specified in hundredths of a percent, -10000 - 10000. (10000 = 100%)

*/

/** @} */

/** @defgroup FixedVar FixedVar
    @ingroup se_types

    Specifies a signed setpoint for reactive power.

    @{ */

/** @struct SE_FixedVar_t se_types.h

    Specifies a signed setpoint for reactive power.

    @var SE_FixedVar_t::refType
    Indicates whether to interpret 'value' as %setMaxVar or %statVarAvail.

    @var SE_FixedVar_t::value
    Specify a signed setpoint for reactive power in % (see 'refType' for context).

*/

/** @} */

/** @defgroup PowerFactor PowerFactor
    @ingroup se_types

    Specifies a setpoint for Displacement Power Factor, the ratio between apparent and active powers at the fundamental frequency (e.g. 60 Hz).

    @{ */

/** @struct SE_PowerFactor_t se_types.h

    Specifies a setpoint for Displacement Power Factor, the ratio between apparent and active powers at the fundamental frequency (e.g. 60 Hz).

    @var SE_PowerFactor_t::displacement
    Significand of a signed value of cos(theta) between -0.9999 and 1.0. E.g. a value of -0.95 may be specified as a displacement of -950 and a multiplier of -3. Sign SHALL be interpreted according to the EEI convention.

    @var SE_PowerFactor_t::multiplier
    Specifies exponent of 'displacement'.

*/

/** @} */

/** @defgroup DERControlBase DERControlBase
    @ingroup se_types

    Distributed Energy Resource (DER) control values.

    @{ */

/** @struct SE_DERControlBase_t se_types.h

    Distributed Energy Resource (DER) control values.

    @var SE_DERControlBase_t::_flags
#define SE_rampTms_exists (1 << 0)

#define SE_opModWattVar_exists (1 << 1)

#define SE_opModWattPF_exists (1 << 2)

#define SE_opModVoltWatt_exists (1 << 3)

#define SE_opModVoltVar_exists (1 << 4)

#define SE_opModTargetW_exists (1 << 5)

#define SE_opModTargetVar_exists (1 << 6)

#define SE_opModMaxLimW_exists (1 << 7)

#define SE_opModLVRTMustTrip_exists (1 << 8)

#define SE_opModLVRTMomentaryCessation_exists (1 << 9)

#define SE_opModLFRTMustTrip_exists (1 << 10)

#define SE_opModHVRTMustTrip_exists (1 << 11)

#define SE_opModHVRTMomentaryCessation_exists (1 << 12)

#define SE_opModHFRTMustTrip_exists (1 << 13)

#define SE_opModFreqWatt_exists (1 << 14)

#define SE_opModFreqDroop_exists (1 << 15)

#define SE_opModFixedW_exists (1 << 16)

#define SE_opModFixedVar_exists (1 << 17)

#define SE_opModFixedPF_exists (1 << 18)

#define SE_opModEnergize_true (1 << 19)
#define SE_opModEnergize_exists (1 << 20)

#define SE_opModConnect_true (1 << 21)
#define SE_opModConnect_exists (1 << 22)

    @var SE_DERControlBase_t::opModFixedPF
    The opModFixedPF function specifies a requested fixed Power Factor (PF) setting, consisting of a signed displacement value. The PF sign (which SHALL be interpreted according to the EEI convention, where unity PF is considered unsigned) indicates the direction of reactive power flow. The actual displacement SHALL be within the limits established by setMinPF and setMinPFNeg. If issued simultaneously with other reactive power controls (e.g. opModFixedVar) the control resulting in least var magnitude SHOULD take precedence.

    @var SE_DERControlBase_t::opModFixedVar
    The opModFixedVar function specifies the delivered or received reactive power setpoint.  The context for the setpoint value is determined by refType and SHALL be one of %setMaxW, %setMaxVar, or %statVarAvail.  If issued simultaneously with other reactive power controls (e.g. opModFixedPF) the control resulting in least var magnitude SHOULD take precedence.

    @var SE_DERControlBase_t::opModFixedW
    The opModFixedW function specifies a requested charge or discharge mode setpoint, in %setMaxChargeRateW if negative value or %setMaxW or %setMaxDischargeRateW if positive value (in hundredths).

    @var SE_DERControlBase_t::opModFreqDroop
    Specifies a frequency-watt operation. This operation limits active power generation or consumption when the line frequency deviates from nominal by a specified amount.

    @var SE_DERControlBase_t::opModFreqWatt
    Specify DERCurveLink for curve type == 1.  The Frequency-Watt function limits active power generation or consumption when the line frequency deviates from nominal by a specified amount. The Frequency-Watt curve is specified as an array of Frequency-Watt pairs that are interpolated into a piecewise linear function with hysteresis.  The x value of each pair specifies a frequency in Hz. The y value specifies a corresponding active power output in %setMaxW.

    @var SE_DERControlBase_t::opModHFRTMustTrip
    Specify curve type == 9.  The High Frequency Ride-Through (HFRT) function is specified by a duration-frequency curve that defines the operating region under high frequency conditions.  Each HFRT curve is specified by an array of duration-frequency pairs that will be interpolated into a piecewise linear function that defines an operating region.  The x value of each pair specifies a duration (time at a given frequency in seconds). The y value of each pair specifies a frequency, in Hz. This control specifies the "must trip" region.

    @var SE_DERControlBase_t::opModHVRTMomentaryCessation
    Specify curve type == 6.  The High Voltage Ride-Through (HVRT) function is specified by duration-volt curves that define the operating region under high voltage conditions.  Each HVRT curve is specified by an array of duration-volt pairs that will be interpolated into a piecewise linear function that defines an operating region.  The x value of each pair specifies a duration (time at a given voltage in seconds). The y value of each pair specifies an effective percent voltage, defined as ((locally measured voltage - setVRefOfs) / setVRef). This control specifies the "momentary cessation" region.

    @var SE_DERControlBase_t::opModHVRTMustTrip
    Specify curve type == 7.  The High Voltage Ride-Through (HVRT) function is specified by duration-volt curves that define the operating region under high voltage conditions.  Each HVRT curve is specified by an array of duration-volt pairs that will be interpolated into a piecewise linear function that defines an operating region.  The x value of each pair specifies a duration (time at a given voltage in seconds). The y value of each pair specifies an effective percent voltage, defined as ((locally measured voltage - setVRefOfs) / setVRef). This control specifies the "must trip" region.

    @var SE_DERControlBase_t::opModLFRTMustTrip
    Specify curve type == 8.  The Low Frequency Ride-Through (LFRT) function is specified by a duration-frequency curve that defines the operating region under low frequency conditions.  Each LFRT curve is specified by an array of duration-frequency pairs that will be interpolated into a piecewise linear function that defines an operating region.  The x value of each pair specifies a duration (time at a given frequency in seconds). The y value of each pair specifies a frequency, in Hz. This control specifies the "must trip" region.

    @var SE_DERControlBase_t::opModLVRTMomentaryCessation
    Specify curve type == 4.  The Low Voltage Ride-Through (LVRT) function is specified by duration-volt curves that define the operating region under low voltage conditions.  Each LVRT curve is specified by an array of duration-volt pairs that will be interpolated into a piecewise linear function that defines an operating region.  The x value of each pair specifies a duration (time at a given voltage in seconds). The y value of each pair specifies an effective percent voltage, defined as ((locally measured voltage - setVRefOfs) / setVRef). This control specifies the "momentary cessation" region.

    @var SE_DERControlBase_t::opModLVRTMustTrip
    Specify curve type == 5.  The Low Voltage Ride-Through (LVRT) function is specified by duration-volt curves that define the operating region under low voltage conditions.  Each LVRT curve is specified by an array of duration-volt pairs that will be interpolated into a piecewise linear function that defines an operating region.  The x value of each pair specifies a duration (time at a given voltage in seconds). The y value of each pair specifies an effective percent voltage, defined as ((locally measured voltage - setVRefOfs) / setVRef). This control specifies the "must trip" region.

    @var SE_DERControlBase_t::opModMaxLimW
    The opModMaxLimW function sets the maximum active power generation level at the electrical coupling point as a percentage of set capacity (%setMaxW, in hundredths). This limitation may be met e.g. by reducing PV output or by using excess PV output to charge associated storage.

    @var SE_DERControlBase_t::opModTargetVar
    Target reactive power, in var. This control is likely to be more useful for aggregators, as individual DERs may not be able to maintain a target setting.

    @var SE_DERControlBase_t::opModTargetW
    Target output power, in Watts. This control is likely to be more useful for aggregators, as individual DERs may not be able to maintain a target setting.

    @var SE_DERControlBase_t::opModVoltVar
    Specify DERCurveLink for curve type == 0.  The static volt-var function provides over- or under-excited var compensation as a function of measured voltage. The volt-var curve is specified as an array of volt-var pairs that are interpolated into a piecewise linear function with hysteresis. The x value of each pair specifies an effective percent voltage, defined as ((locally measured voltage - setVRefOfs) / setVRef) and SHOULD support a domain of at least 0 - 135.  The y value specifies a target var output interpreted as a signed percentage (-100 to 100). The meaning of the y value is determined by yRefType and must be one of %setMaxW, %setMaxVar, or %statVarAvail.

    @var SE_DERControlBase_t::opModVoltWatt
    Specify DERCurveLink for curve type == 3.  The Volt-Watt reduces active power output as a function of measured voltage. The Volt-Watt curve is specified as an array of Volt-Watt pairs that are interpolated into a piecewise linear function with hysteresis. The x value of each pair specifies an effective percent voltage, defined as ((locally measured voltage - setVRefOfs) / setVRef) and SHOULD support a domain of at least 0 - 135. The y value specifies an active power output in %setMaxW, (0 - 100).

    @var SE_DERControlBase_t::opModWattPF
    Specify DERCurveLink for curve type == 2.  The Watt-PF function varies Power Factor (PF) as a function of delivered active power. The Watt-PF curve is specified as an array of Watt-PF coordinates that are interpolated into a piecewise linear function with hysteresis.  The x value of each pair specifies a watt setting in %setMaxW, (0 - 100). The PF output setting is a signed displacement in y value (PF sign SHALL be interpreted according to the EEI convention, where unity PF is considered unsigned). These settings are not expected to be updated very often during the life of the installation, therefore only a single curve is required.  If issued simultaneously with other reactive power controls (e.g. opModFixedPF) the control resulting in least var magnitude SHOULD take precedence.

    @var SE_DERControlBase_t::opModWattVar
    Specify DERCurveLink for curve type == 10. The Watt-Var function varies vars as a function of delivered active power. The Watt-Var curve is specified as an array of Watt-Var pairs that are interpolated into a piecewise linear function with hysteresis. The x value of each pair specifies a watt setting in %setMaxW, (0-100). The y value specifies a target var output interpreted as a signed percentage (-100 to 100). The meaning of the y value is determined by yRefType and must be one of %setMaxW, %setMaxVar, or %statVarAvail.

    @var SE_DERControlBase_t::rampTms
    Requested ramp time, in hundredths of a second, for the device to transition from the current DERControl  mode setting(s) to the new mode setting(s). If absent, use default ramp rate (setGradW).  Resolution is 1/100 sec.

*/

/** @} */

/** @defgroup OneHourRangeType OneHourRangeType
    @ingroup Int16

    A signed time offset, typically applied to a Time value, expressed in seconds, with range -3600 to 3600.

    @{ */

/** @typedef SE_OneHourRangeType_t

    A signed time offset, typically applied to a Time value, expressed in seconds, with range -3600 to 3600.

*/

/** @} */

/** @defgroup String192 String192
    @ingroup se_typedefs

    Character string of max length 192. For all string types, in order to limit internal storage, implementations SHALL reduce the length of strings using multi-byte characters so that the string may be stored using "maxLength" octets in the given encoding.

    @{ */

/** @typedef SE_String192_t

    Character string of max length 192. For all string types, in order to limit internal storage, implementations SHALL reduce the length of strings using multi-byte characters so that the string may be stored using "maxLength" octets in the given encoding.

*/

/** @} */

/** @defgroup EventStatus EventStatus
    @ingroup se_types

    Current status information relevant to a specific object. The Status object is used to indicate the current status of an Event. Devices can read the containing resource (e.g. TextMessage) to get the most up to date status of the event.  Devices can also subscribe to a specific resource instance to get updates when any of its attributes change, including the Status object.

    @{ */

/** @struct SE_EventStatus_t se_types.h

    Current status information relevant to a specific object. The Status object is used to indicate the current status of an Event. Devices can read the containing resource (e.g. TextMessage) to get the most up to date status of the event.  Devices can also subscribe to a specific resource instance to get updates when any of its attributes change, including the Status object.

    @var SE_EventStatus_t::_flags
#define SE_reason_exists (1 << 0)

#define SE_potentiallySupersededTime_exists (1 << 1)

#define SE_potentiallySuperseded_true (1 << 2)

    @var SE_EventStatus_t::currentStatus
    Field representing the current status type. 

    0 = Scheduled

    This status indicates that the event has been scheduled and the event has not yet started.  The server SHALL set the event to this status when the event is first scheduled and persist until the event has become active or has been cancelled.  For events with a start time less than or equal to the current time, this status SHALL never be indicated, the event SHALL start with a status of “Active”. 

    1 = Active

    This status indicates that the event is currently active. The server SHALL set the event to this status when the event reaches its earliest Effective Start Time.

    2 = Cancelled 

    When events are cancelled, the Status.dateTime attribute SHALL be set to the time the cancellation occurred, which cannot be in the future.  The server is responsible for maintaining the cancelled event in its collection for the duration of the original event, or until the server has run out of space and needs to store a new event. Client devices SHALL be aware of Cancelled events, determine if the Cancelled event applies to them, and cancel the event immediately if applicable.

    3 = Cancelled with Randomization 

    The server is responsible for maintaining the cancelled event in its collection for the duration of the Effective Scheduled Period. Client devices SHALL be aware of Cancelled with Randomization events, determine if the Cancelled event applies to them, and cancel the event immediately, using the larger of (absolute value of randomizeStart) and (absolute value of randomizeDuration) as the end randomization, in seconds. This Status.type SHALL NOT be used with "regular" Events, only with specializations of RandomizableEvent.

    4 = Superseded

    Events marked as Superseded by servers are events that may have been replaced by new events from the same program that target the exact same set of deviceCategory's (if applicable) and overlap for a given period of time. Servers SHALL mark an event as Superseded at the earliest Effective Start Time of the overlapping event. Servers are responsible for maintaining the Superseded event in their collection for the duration of the Effective Scheduled Period. 

    Client devices encountering a Superseded event SHALL terminate execution of the event immediately and commence execution of the new event immediately, unless the current time is within the start randomization window of the superseded event, in which case the client SHALL obey the start randomization of the new event. This Status.type SHALL NOT be used with TextMessage, since multiple text messages can be active. 

    All other values reserved.

    @var SE_EventStatus_t::dateTime
    The dateTime attribute will provide a timestamp of when the current status was defined. dateTime MUST be set to the time at which the status change occurred, not a time in the future or past.

    @var SE_EventStatus_t::potentiallySupersededTime
    Indicates the time that the potentiallySuperseded flag was set.

    @var SE_EventStatus_t::reason
    The Reason attribute allows a Service provider to provide a textual explanation of the status.

*/

/** @} */

/** @defgroup RespondableSubscribableIdentifiedObject RespondableSubscribableIdentifiedObject
    @ingroup RespondableResource

    An IdentifiedObject to which a Response can be requested.

    @{ */

/** @struct SE_RespondableSubscribableIdentifiedObject_t se_types.h

    An IdentifiedObject to which a Response can be requested.

    @var SE_RespondableSubscribableIdentifiedObject_t::_flags
#define SE_responseRequired_exists (1 << 5)

    @var SE_RespondableSubscribableIdentifiedObject_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_RespondableSubscribableIdentifiedObject_t::replyTo
    A reference to the response resource address (URI). Required on a response to a GET if responseRequired is "true".

    @var SE_RespondableSubscribableIdentifiedObject_t::responseRequired
    Indicates whether or not a response is required upon receipt, creation or update of this resource. Responses shall be posted to the collection specified in "replyTo". 

    If the resource has a deviceCategory field, devices that match one or more of the device types indicated in deviceCategory SHALL respond according to the rules listed below.  If the category does not match, the device SHALL NOT respond. If the resource does not have a deviceCategory field, a device receiving the resource SHALL respond according to the rules listed below.

    Value encoded as hex according to the following bit assignments, any combination is possible. 

    See Table 10-10 for the list of appropriate Response status codes to be sent for these purposes.

    0 - End device shall indicate that message was received 

    1 - End device shall indicate specific response. 

    2 - End user / customer response is required. 

    All other values reserved.

    @var SE_RespondableSubscribableIdentifiedObject_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_RespondableSubscribableIdentifiedObject_t::mRID
    The global identifier of the object.

    @var SE_RespondableSubscribableIdentifiedObject_t::description
    The description is a human readable text describing or naming the object.

    @var SE_RespondableSubscribableIdentifiedObject_t::version
    Contains the version number of the object. See the type definition for details.

*/

/** @} */

/** @defgroup Event Event
    @ingroup RespondableSubscribableIdentifiedObject

    An Event indicates information that applies to a particular period of time. Events SHALL be executed relative to the time of the server, as described in the Time function set section 11.1.

    @{ */

/** @struct SE_Event_t se_types.h

    An Event indicates information that applies to a particular period of time. Events SHALL be executed relative to the time of the server, as described in the Time function set section 11.1.

    @var SE_Event_t::_flags
#define SE_responseRequired_exists (1 << 5)

    @var SE_Event_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_Event_t::replyTo
    A reference to the response resource address (URI). Required on a response to a GET if responseRequired is "true".

    @var SE_Event_t::responseRequired
    Indicates whether or not a response is required upon receipt, creation or update of this resource. Responses shall be posted to the collection specified in "replyTo". 

    If the resource has a deviceCategory field, devices that match one or more of the device types indicated in deviceCategory SHALL respond according to the rules listed below.  If the category does not match, the device SHALL NOT respond. If the resource does not have a deviceCategory field, a device receiving the resource SHALL respond according to the rules listed below.

    Value encoded as hex according to the following bit assignments, any combination is possible. 

    See Table 10-10 for the list of appropriate Response status codes to be sent for these purposes.

    0 - End device shall indicate that message was received 

    1 - End device shall indicate specific response. 

    2 - End user / customer response is required. 

    All other values reserved.

    @var SE_Event_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_Event_t::mRID
    The global identifier of the object.

    @var SE_Event_t::description
    The description is a human readable text describing or naming the object.

    @var SE_Event_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_Event_t::creationTime
    The time at which the Event was created.

    @var SE_Event_t::EventStatus
    @var SE_Event_t::interval
    The period during which the Event applies.

*/

/** @} */

/** @defgroup RandomizableEvent RandomizableEvent
    @ingroup Event

    An Event that can indicate time ranges over which the start time and duration SHALL be randomized.

    @{ */

/** @struct SE_RandomizableEvent_t se_types.h

    An Event that can indicate time ranges over which the start time and duration SHALL be randomized.

    @var SE_RandomizableEvent_t::_flags
#define SE_responseRequired_exists (1 << 5)

#define SE_randomizeStart_exists (1 << 6)

#define SE_randomizeDuration_exists (1 << 7)

    @var SE_RandomizableEvent_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_RandomizableEvent_t::replyTo
    A reference to the response resource address (URI). Required on a response to a GET if responseRequired is "true".

    @var SE_RandomizableEvent_t::responseRequired
    Indicates whether or not a response is required upon receipt, creation or update of this resource. Responses shall be posted to the collection specified in "replyTo". 

    If the resource has a deviceCategory field, devices that match one or more of the device types indicated in deviceCategory SHALL respond according to the rules listed below.  If the category does not match, the device SHALL NOT respond. If the resource does not have a deviceCategory field, a device receiving the resource SHALL respond according to the rules listed below.

    Value encoded as hex according to the following bit assignments, any combination is possible. 

    See Table 10-10 for the list of appropriate Response status codes to be sent for these purposes.

    0 - End device shall indicate that message was received 

    1 - End device shall indicate specific response. 

    2 - End user / customer response is required. 

    All other values reserved.

    @var SE_RandomizableEvent_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_RandomizableEvent_t::mRID
    The global identifier of the object.

    @var SE_RandomizableEvent_t::description
    The description is a human readable text describing or naming the object.

    @var SE_RandomizableEvent_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_RandomizableEvent_t::creationTime
    The time at which the Event was created.

    @var SE_RandomizableEvent_t::EventStatus
    @var SE_RandomizableEvent_t::interval
    The period during which the Event applies.

    @var SE_RandomizableEvent_t::randomizeDuration
    Number of seconds boundary inside which a random value must be selected to be applied to the associated interval duration, to avoid sudden synchronized demand changes. If related to price level changes, sign may be ignored. Valid range is -3600 to 3600. If not specified, 0 is the default.

    @var SE_RandomizableEvent_t::randomizeStart
    Number of seconds boundary inside which a random value must be selected to be applied to the associated interval start time, to avoid sudden synchronized demand changes. If related to price level changes, sign may be ignored. Valid range is -3600 to 3600. If not specified, 0 is the default.

*/

/** @} */

/** @defgroup DERControl DERControl
    @ingroup RandomizableEvent

    Distributed Energy Resource (DER) time/event-based control.

    @{ */

/** @struct SE_DERControl_t se_types.h

    Distributed Energy Resource (DER) time/event-based control.

    @var SE_DERControl_t::_flags
#define SE_responseRequired_exists (1 << 5)

#define SE_randomizeStart_exists (1 << 6)

#define SE_randomizeDuration_exists (1 << 7)

    @var SE_DERControl_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DERControl_t::replyTo
    A reference to the response resource address (URI). Required on a response to a GET if responseRequired is "true".

    @var SE_DERControl_t::responseRequired
    Indicates whether or not a response is required upon receipt, creation or update of this resource. Responses shall be posted to the collection specified in "replyTo". 

    If the resource has a deviceCategory field, devices that match one or more of the device types indicated in deviceCategory SHALL respond according to the rules listed below.  If the category does not match, the device SHALL NOT respond. If the resource does not have a deviceCategory field, a device receiving the resource SHALL respond according to the rules listed below.

    Value encoded as hex according to the following bit assignments, any combination is possible. 

    See Table 10-10 for the list of appropriate Response status codes to be sent for these purposes.

    0 - End device shall indicate that message was received 

    1 - End device shall indicate specific response. 

    2 - End user / customer response is required. 

    All other values reserved.

    @var SE_DERControl_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_DERControl_t::mRID
    The global identifier of the object.

    @var SE_DERControl_t::description
    The description is a human readable text describing or naming the object.

    @var SE_DERControl_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_DERControl_t::creationTime
    The time at which the Event was created.

    @var SE_DERControl_t::EventStatus
    @var SE_DERControl_t::interval
    The period during which the Event applies.

    @var SE_DERControl_t::randomizeDuration
    Number of seconds boundary inside which a random value must be selected to be applied to the associated interval duration, to avoid sudden synchronized demand changes. If related to price level changes, sign may be ignored. Valid range is -3600 to 3600. If not specified, 0 is the default.

    @var SE_DERControl_t::randomizeStart
    Number of seconds boundary inside which a random value must be selected to be applied to the associated interval start time, to avoid sudden synchronized demand changes. If related to price level changes, sign may be ignored. Valid range is -3600 to 3600. If not specified, 0 is the default.

    @var SE_DERControl_t::DERControlBase
*/

/** @} */

/** @defgroup DERControlList DERControlList
    @ingroup SubscribableList

    A List element to hold DERControl objects.

    @{ */

/** @struct SE_DERControlList_t se_types.h

    A List element to hold DERControl objects.

    @var SE_DERControlList_t::_flags
    @var SE_DERControlList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DERControlList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_DERControlList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_DERControlList_t::results
    Indicates the number of items in this page of results.

    @var SE_DERControlList_t::DERControl
*/

/** @} */

/** @defgroup DERType DERType
    @ingroup UInt8

    0 - Not applicable / Unknown 

    1 - Virtual or mixed DER 

    2 - Reciprocating engine 

    3 - Fuel cell 

    4 - Photovoltaic system 

    5 - Combined heat and power 

    80 - Storage (immobile)

    81 - Electric vehicle / EVSE

    82 - Combined PV and storage

    All other values reserved.

    @{ */

/** @typedef SE_DERType_t

    0 - Not applicable / Unknown 

    1 - Virtual or mixed DER 

    2 - Reciprocating engine 

    3 - Fuel cell 

    4 - Photovoltaic system 

    5 - Combined heat and power 

    80 - Storage (immobile)

    81 - Electric vehicle / EVSE

    82 - Combined PV and storage

    All other values reserved.

*/

/** @} */

/** @defgroup WattHour WattHour
    @ingroup se_types

    Active (real) energy

    @{ */

/** @struct SE_WattHour_t se_types.h

    Active (real) energy

    @var SE_WattHour_t::multiplier
    Specifies exponent of uom.

    @var SE_WattHour_t::value
    Value in watt-hours (uom 72)

*/

/** @} */

/** @defgroup ApparentPower ApparentPower
    @ingroup se_types

    The apparent power S (in VA) is the product of root mean square (RMS) voltage and RMS current.

    @{ */

/** @struct SE_ApparentPower_t se_types.h

    The apparent power S (in VA) is the product of root mean square (RMS) voltage and RMS current.

    @var SE_ApparentPower_t::multiplier
    Specifies exponent of uom.

    @var SE_ApparentPower_t::value
    Value in volt-amperes (uom 61)

*/

/** @} */

/** @defgroup AmpereHour AmpereHour
    @ingroup se_types

    Available electric charge

    @{ */

/** @struct SE_AmpereHour_t se_types.h

    Available electric charge

    @var SE_AmpereHour_t::multiplier
    Specifies exponent of uom.

    @var SE_AmpereHour_t::value
    Value in ampere-hours (uom 106)

*/

/** @} */

/** @defgroup CurrentRMS CurrentRMS
    @ingroup se_types

    Average flow of charge through a conductor.

    @{ */

/** @struct SE_CurrentRMS_t se_types.h

    Average flow of charge through a conductor.

    @var SE_CurrentRMS_t::multiplier
    Specifies exponent of value.

    @var SE_CurrentRMS_t::value
    Value in amperes RMS (uom 5)

*/

/** @} */

/** @defgroup HexBinary32 HexBinary32
    @ingroup se_typedefs

    A 32-bit field encoded as a hex string (8 hex characters max). Where applicable, bit 0, or the least significant bit, goes on the right. Note that hexBinary requires pairs of hex characters, so an odd number of characters requires a leading "0".

    @{ */

/** @typedef SE_HexBinary32_t

    A 32-bit field encoded as a hex string (8 hex characters max). Where applicable, bit 0, or the least significant bit, goes on the right. Note that hexBinary requires pairs of hex characters, so an odd number of characters requires a leading "0".

*/

/** @} */

/** @defgroup DERControlType DERControlType
    @ingroup HexBinary32

    Control modes supported by the DER.  Bit positions SHALL be defined as follows:

    0 - opModVoltVar (Volt-Var Mode)

    1 - opModFreqWatt (Frequency-Watt Curve Mode)

    2 - opModFreqDroop (Frequency-Watt Parameterized Mode)

    3 - opModWattPF (Watt-PowerFactor Mode)

    4 - opModVoltWatt (Volt-Watt Mode)

    5 - opModLVRTMomentaryCessation (Low Voltage Ride Through, Momentary Cessation Mode)

    6 - opModLVRTMustTrip (Low Voltage Ride Through, Must Trip Mode)

    7 - opModHVRTMomentaryCessation (High Voltage Ride Through, Momentary Cessation Mode)

    8 - opModHVRTMustTrip (High Voltage Ride Through, Must Trip Mode)

    9 - opModLFRTMustTrip (Low Frequency Ride Through, Must Trip Mode)

    10 - opModHFRTMustTrip (High Frequency Ride Through, Must Trip Mode)

    11 - opModConnect (Connect / Disconnect - implies galvanic isolation)

    12 - opModEnergize (Energize / De-Energize)

    13 - opModMaxLimW (Maximum Active Power)

    14 - opModFixedVar (Reactive Power Setpoint)

    15 - opModFixedPF (Fixed Power Factor Setpoint)

    16 - opModFixedW (Charge / Discharge Setpoint)

    17 - opModTargetW (Target Active Power)

    18 - opModTargetVar (Target Reactive Power)

    19 - Charge mode

    20 - Discharge mode

    21 - opModWattVar (Watt-Var Mode)

    All other values reserved.

    @{ */

/** @typedef SE_DERControlType_t

    Control modes supported by the DER.  Bit positions SHALL be defined as follows:

    0 - opModVoltVar (Volt-Var Mode)

    1 - opModFreqWatt (Frequency-Watt Curve Mode)

    2 - opModFreqDroop (Frequency-Watt Parameterized Mode)

    3 - opModWattPF (Watt-PowerFactor Mode)

    4 - opModVoltWatt (Volt-Watt Mode)

    5 - opModLVRTMomentaryCessation (Low Voltage Ride Through, Momentary Cessation Mode)

    6 - opModLVRTMustTrip (Low Voltage Ride Through, Must Trip Mode)

    7 - opModHVRTMomentaryCessation (High Voltage Ride Through, Momentary Cessation Mode)

    8 - opModHVRTMustTrip (High Voltage Ride Through, Must Trip Mode)

    9 - opModLFRTMustTrip (Low Frequency Ride Through, Must Trip Mode)

    10 - opModHFRTMustTrip (High Frequency Ride Through, Must Trip Mode)

    11 - opModConnect (Connect / Disconnect - implies galvanic isolation)

    12 - opModEnergize (Energize / De-Energize)

    13 - opModMaxLimW (Maximum Active Power)

    14 - opModFixedVar (Reactive Power Setpoint)

    15 - opModFixedPF (Fixed Power Factor Setpoint)

    16 - opModFixedW (Charge / Discharge Setpoint)

    17 - opModTargetW (Target Active Power)

    18 - opModTargetVar (Target Reactive Power)

    19 - Charge mode

    20 - Discharge mode

    21 - opModWattVar (Watt-Var Mode)

    All other values reserved.

*/

/** @} */

/** @defgroup DERCapability DERCapability
    @ingroup Resource

    Distributed energy resource type and nameplate ratings.

    @{ */

/** @struct SE_DERCapability_t se_types.h

    Distributed energy resource type and nameplate ratings.

    @var SE_DERCapability_t::_flags
#define SE_rtgWh_exists (1 << 5)

#define SE_rtgVarNeg_exists (1 << 6)

#define SE_rtgVar_exists (1 << 7)

#define SE_rtgVA_exists (1 << 8)

#define SE_rtgUnderExcitedW_exists (1 << 9)

#define SE_rtgUnderExcitedPF_exists (1 << 10)

#define SE_rtgOverExcitedW_exists (1 << 11)

#define SE_rtgOverExcitedPF_exists (1 << 12)

#define SE_rtgNormalCategory_exists (1 << 13)

#define SE_rtgMinPFNeg_exists (1 << 14)

#define SE_rtgMinPF_exists (1 << 15)

#define SE_rtgMaxDischargeRateW_exists (1 << 16)

#define SE_rtgMaxDischargeRateVA_exists (1 << 17)

#define SE_rtgMaxChargeRateW_exists (1 << 18)

#define SE_rtgMaxChargeRateVA_exists (1 << 19)

#define SE_rtgAh_exists (1 << 20)

#define SE_rtgAbnormalCategory_exists (1 << 21)

    @var SE_DERCapability_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DERCapability_t::modesSupported
    Bitmap indicating the DER Controls implemented by the device. See DERControlType for values.

    @var SE_DERCapability_t::rtgA
    Maximum continuous AC current capability of the DER, in Amperes (RMS).

    @var SE_DERCapability_t::rtgAbnormalCategory
    Abnormal operating performance category as defined by IEEE 1547-2017. One of:

    0 - not specified

    1 - Category I

    2 - Category II

    3 - Category III

    All other values reserved.

    @var SE_DERCapability_t::rtgAh
    Usable energy storage capacity of the DER, in AmpHours.

    @var SE_DERCapability_t::rtgMaxChargeRateVA
    Maximum apparent power charge rating in Volt-Amperes. May differ from the maximum apparent power rating.

    @var SE_DERCapability_t::rtgMaxChargeRateW
    Maximum rate of energy transfer received by the storage DER, in Watts.

    @var SE_DERCapability_t::rtgMaxDischargeRateVA
    Maximum apparent power discharge rating in Volt-Amperes. May differ from the maximum apparent power rating.

    @var SE_DERCapability_t::rtgMaxDischargeRateW
    Maximum rate of energy transfer delivered by the storage DER, in Watts. Required for combined generation/storage DERs (e.g. DERType == 82).

    @var SE_DERCapability_t::rtgMinPF
    Minimum Power Factor displacement capability of the DER; SHALL be a positive value between 0.0 (typically &gt; 0.7) and 1.0. If absent, defaults to unity. (Unity power factor is considered unsigned.)

    @var SE_DERCapability_t::rtgMinPFNeg
    Minimum Power Factor displacement capability of the DER; SHALL be a negative value between 0.0 (typically &lt; -0.7) and -0.9999.  If absent, defaults to negative rtgMinPF. (Unity power factor is considered unsigned.)

    @var SE_DERCapability_t::rtgNormalCategory
    Normal operating performance category as defined by IEEE 1547-2017. One of:

    0 - not specified

    1 - Category A

    2 - Category B

    All other values reserved.

    @var SE_DERCapability_t::rtgOverExcitedPF
    Specified over-excited power factor.

    @var SE_DERCapability_t::rtgOverExcitedW
    Active power rating in Watts at specified over-excited power factor (rtgOverExcitedPF). If present, rtgOverExcitedPF SHALL be present.

    @var SE_DERCapability_t::rtgUnderExcitedPF
    Specified under-excited power factor.

    @var SE_DERCapability_t::rtgUnderExcitedW
    Active power rating in Watts at specified under-excited power factor (rtgUnderExcitedPF). If present, rtgUnderExcitedPF SHALL be present.

    @var SE_DERCapability_t::rtgVA
    Maximum continuous apparent power output capability of the DER, in VA.

    @var SE_DERCapability_t::rtgVar
    Maximum continuous reactive power delivered by the DER, in var.

    @var SE_DERCapability_t::rtgVarNeg
    Maximum continuous reactive power received by the DER, in var.  If absent, defaults to negative rtgVar.

    @var SE_DERCapability_t::rtgW
    Maximum continuous active power output capability of the DER, in watts. Represents combined generation plus storage output if DERType == 82.

    @var SE_DERCapability_t::rtgWh
    Maximum energy storage capacity of the DER, in WattHours.

    @var SE_DERCapability_t::type
    Type of DER; see DERType object

*/

/** @} */

/** @defgroup DERAvailability DERAvailability
    @ingroup SubscribableResource

    Indicates current reserve generation status

    @{ */

/** @struct SE_DERAvailability_t se_types.h

    Indicates current reserve generation status

    @var SE_DERAvailability_t::_flags
#define SE_statWAvail_exists (1 << 5)

#define SE_statVarAvail_exists (1 << 6)

#define SE_reservePercent_exists (1 << 7)

#define SE_reserveChargePercent_exists (1 << 8)

#define SE_maxChargeDuration_exists (1 << 9)

    @var SE_DERAvailability_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DERAvailability_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_DERAvailability_t::availabilityDuration
    Indicates number of seconds the DER will be able to deliver active power at the reservePercent level.

    @var SE_DERAvailability_t::maxChargeDuration
    Indicates number of seconds the DER will be able to receive active power at the reserveChargePercent level.

    @var SE_DERAvailability_t::readingTime
    The timestamp when the DER availability was last updated.

    @var SE_DERAvailability_t::reserveChargePercent
    Percent of continuous received active power (%setMaxChargeRateW) that is estimated to be available in reserve.

    @var SE_DERAvailability_t::reservePercent
    Percent of continuous delivered active power (%setMaxW) that is estimated to be available in reserve.

    @var SE_DERAvailability_t::statVarAvail
    Estimated reserve reactive power, in var.  Represents the lesser of received or delivered reactive power.

    @var SE_DERAvailability_t::statWAvail
    Estimated reserve active power, in watts.

*/

/** @} */

/** @defgroup VoltageRMS VoltageRMS
    @ingroup se_types

    Average electric potential difference between two points.

    @{ */

/** @struct SE_VoltageRMS_t se_types.h

    Average electric potential difference between two points.

    @var SE_VoltageRMS_t::multiplier
    Specifies exponent of uom.

    @var SE_VoltageRMS_t::value
    Value in volts RMS (uom 29)

*/

/** @} */

/** @defgroup DERSettings DERSettings
    @ingroup SubscribableResource

    Distributed energy resource settings

    @{ */

/** @struct SE_DERSettings_t se_types.h

    Distributed energy resource settings

    @var SE_DERSettings_t::_flags
#define SE_setVRefOfs_exists (1 << 5)

#define SE_setVRef_exists (1 << 6)

#define SE_setMinPFNeg_exists (1 << 7)

#define SE_setMinPF_exists (1 << 8)

#define SE_setMaxWh_exists (1 << 9)

#define SE_setMaxVarNeg_exists (1 << 10)

#define SE_setMaxVar_exists (1 << 11)

#define SE_setMaxVA_exists (1 << 12)

#define SE_setMaxDischargeRateW_exists (1 << 13)

#define SE_setMaxDischargeRateVA_exists (1 << 14)

#define SE_setMaxChargeRateW_exists (1 << 15)

#define SE_setMaxChargeRateVA_exists (1 << 16)

#define SE_setMaxAh_exists (1 << 17)

#define SE_setMaxA_exists (1 << 18)

#define SE_modesEnabled_exists (1 << 19)

    @var SE_DERSettings_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DERSettings_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_DERSettings_t::modesEnabled
    Bitmap indicating the DER Controls enabled on the device. See DERControlType for values. If a control is supported (see DERCapability::modesSupported), but not enabled, the control will not be executed if encountered.

    @var SE_DERSettings_t::setESDelay
    Enter service delay, in hundredths of a second.

    @var SE_DERSettings_t::setESHighFreq
    Enter service frequency high. Specified in hundredths of Hz.

    @var SE_DERSettings_t::setESHighVolt
    Enter service voltage high. Specified as an effective percent voltage, defined as (100% * (locally measured voltage - setVRefOfs) / setVRef), in hundredths of a percent.

    @var SE_DERSettings_t::setESLowFreq
    Enter service frequency low. Specified in hundredths of Hz.

    @var SE_DERSettings_t::setESLowVolt
    Enter service voltage low. Specified as an effective percent voltage, defined as (100% * (locally measured voltage - setVRefOfs) / setVRef), in hundredths of a percent.

    @var SE_DERSettings_t::setESRandomDelay
    Enter service randomized delay, in hundredths of a second.

    @var SE_DERSettings_t::setGradW
    Set default rate of change (ramp rate) of active power output due to command or internal action, defined in %setWMax / second.  Resolution is in hundredths of a percent/second. A value of 0 means there is no limit. Interpreted as a percentage change in output capability limit per second when used as a default ramp rate.

    @var SE_DERSettings_t::setMaxA
    AC current maximum. Maximum AC current in RMS Amperes.

    @var SE_DERSettings_t::setMaxAh
    Maximum usable energy storage capacity of the DER, in AmpHours. Note: this may be different from physical capability.

    @var SE_DERSettings_t::setMaxChargeRateVA
    Apparent power charge maximum. Maximum apparent power the DER can absorb from the grid in Volt-Amperes. May differ from the apparent power maximum (setMaxVA).

    @var SE_DERSettings_t::setMaxChargeRateW
    Maximum rate of energy transfer received by the storage device, in Watts. Defaults to rtgMaxChargeRateW.

    @var SE_DERSettings_t::setMaxDischargeRateVA
    Apparent power discharge maximum. Maximum apparent power the DER can deliver to the grid in Volt-Amperes. May differ from the apparent power maximum (setMaxVA).

    @var SE_DERSettings_t::setMaxDischargeRateW
    Maximum rate of energy transfer delivered by the storage device, in Watts. Defaults to rtgMaxDischargeRateW.

    @var SE_DERSettings_t::setMaxVA
    Set limit for maximum apparent power capability of the DER (in VA). Defaults to rtgVA.

    @var SE_DERSettings_t::setMaxVar
    Set limit for maximum reactive power delivered by the DER (in var). SHALL be a positive value &lt;= rtgVar (default).

    @var SE_DERSettings_t::setMaxVarNeg
    Set limit for maximum reactive power received by the DER (in var). If present, SHALL be a negative value &gt;= rtgVarNeg (default). If absent, defaults to negative setMaxVar.

    @var SE_DERSettings_t::setMaxW
    Set limit for maximum active power capability of the DER (in W). Defaults to rtgW.

    @var SE_DERSettings_t::setMaxWh
    Maximum energy storage capacity of the DER, in WattHours. Note: this may be different from physical capability.

    @var SE_DERSettings_t::setMinPF
    Set minimum Power Factor displacement limit of the DER; positive value between 0.0 (typically &gt; 0.7) and 1.0.  SHALL be &gt;= rtgMinPF (default).

    @var SE_DERSettings_t::setMinPFNeg
    Set minimum Power Factor displacement limit of the DER; negative value between 0.0 (typically &lt; -0.7) and -0.9999.  If present, SHALL be &lt;= rtgMinPFNeg (default).  If absent, defaults to negative setMinPF.

    @var SE_DERSettings_t::setSoftGradW
    Set soft-start rate of change (soft-start ramp rate) of active power output due to command or internal action, defined in %setWMax / second.  Resolution is in hundredths of a percent/second. A value of 0 means there is no limit. Interpreted as a percentage change in output capability limit per second when used as a ramp rate.

    @var SE_DERSettings_t::setVRef
    The nominal AC voltage (RMS) at the utility's point of common coupling.

    @var SE_DERSettings_t::setVRefOfs
    The nominal AC voltage (RMS) offset between the DER's electrical connection point and the utility's point of common coupling.

    @var SE_DERSettings_t::updatedTime
    Specifies the time at which the DER information was last updated.

*/

/** @} */

/** @defgroup DERStatusLink DERStatusLink
    @ingroup Link

    SHALL contain a Link to an instance of DERStatus.

    @{ */

/** @struct SE_DERStatusLink_t se_types.h

    SHALL contain a Link to an instance of DERStatus.

    @var SE_DERStatusLink_t::_flags
    @var SE_DERStatusLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup DERSettingsLink DERSettingsLink
    @ingroup Link

    SHALL contain a Link to an instance of DERSettings.

    @{ */

/** @struct SE_DERSettingsLink_t se_types.h

    SHALL contain a Link to an instance of DERSettings.

    @var SE_DERSettingsLink_t::_flags
    @var SE_DERSettingsLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup DERCapabilityLink DERCapabilityLink
    @ingroup Link

    SHALL contain a Link to an instance of DERCapability.

    @{ */

/** @struct SE_DERCapabilityLink_t se_types.h

    SHALL contain a Link to an instance of DERCapability.

    @var SE_DERCapabilityLink_t::_flags
    @var SE_DERCapabilityLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup DERAvailabilityLink DERAvailabilityLink
    @ingroup Link

    SHALL contain a Link to an instance of DERAvailability.

    @{ */

/** @struct SE_DERAvailabilityLink_t se_types.h

    SHALL contain a Link to an instance of DERAvailability.

    @var SE_DERAvailabilityLink_t::_flags
    @var SE_DERAvailabilityLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup CurrentDERProgramLink CurrentDERProgramLink
    @ingroup Link

    SHALL contain a Link to an instance of DERProgram.  If present, this is the DERProgram containing the currently active DERControl.

    @{ */

/** @struct SE_CurrentDERProgramLink_t se_types.h

    SHALL contain a Link to an instance of DERProgram.  If present, this is the DERProgram containing the currently active DERControl.

    @var SE_CurrentDERProgramLink_t::_flags
    @var SE_CurrentDERProgramLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup AssociatedUsagePointLink AssociatedUsagePointLink
    @ingroup Link

    SHALL contain a Link to an instance of UsagePoint.  If present, this is the submeter that monitors the DER output.

    @{ */

/** @struct SE_AssociatedUsagePointLink_t se_types.h

    SHALL contain a Link to an instance of UsagePoint.  If present, this is the submeter that monitors the DER output.

    @var SE_AssociatedUsagePointLink_t::_flags
    @var SE_AssociatedUsagePointLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup AssociatedDERProgramListLink AssociatedDERProgramListLink
    @ingroup ListLink

    SHALL contain a Link to a List of DERPrograms having the DERControl(s) for this DER.

    @{ */

/** @struct SE_AssociatedDERProgramListLink_t se_types.h

    SHALL contain a Link to a List of DERPrograms having the DERControl(s) for this DER.

    @var SE_AssociatedDERProgramListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_AssociatedDERProgramListLink_t::href
    A URI reference.

    @var SE_AssociatedDERProgramListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup DER DER
    @ingroup SubscribableResource

    Contains links to DER resources.

    @{ */

/** @struct SE_DER_t se_types.h

    Contains links to DER resources.

    @var SE_DER_t::_flags
#define SE_DERStatusLink_exists (1 << 5)

#define SE_DERSettingsLink_exists (1 << 6)

#define SE_DERCapabilityLink_exists (1 << 7)

#define SE_DERAvailabilityLink_exists (1 << 8)

#define SE_CurrentDERProgramLink_exists (1 << 9)

#define SE_AssociatedUsagePointLink_exists (1 << 10)

#define SE_AssociatedDERProgramListLink_exists (1 << 11)

    @var SE_DER_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DER_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_DER_t::AssociatedDERProgramListLink
    @var SE_DER_t::AssociatedUsagePointLink
    @var SE_DER_t::CurrentDERProgramLink
    @var SE_DER_t::DERAvailabilityLink
    @var SE_DER_t::DERCapabilityLink
    @var SE_DER_t::DERSettingsLink
    @var SE_DER_t::DERStatusLink
*/

/** @} */

/** @defgroup DERList DERList
    @ingroup List

    A List element to hold DER objects.

    @{ */

/** @struct SE_DERList_t se_types.h

    A List element to hold DER objects.

    @var SE_DERList_t::_flags
    @var SE_DERList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DERList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_DERList_t::results
    Indicates the number of items in this page of results.

    @var SE_DERList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_DERList_t::DER
*/

/** @} */

/** @defgroup DefaultDERControl DefaultDERControl
    @ingroup SubscribableIdentifiedObject

    Contains control mode information to be used if no active DERControl is found.

    @{ */

/** @struct SE_DefaultDERControl_t se_types.h

    Contains control mode information to be used if no active DERControl is found.

    @var SE_DefaultDERControl_t::_flags
#define SE_setGradW_exists (1 << 5)

    @var SE_DefaultDERControl_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DefaultDERControl_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_DefaultDERControl_t::mRID
    The global identifier of the object.

    @var SE_DefaultDERControl_t::description
    The description is a human readable text describing or naming the object.

    @var SE_DefaultDERControl_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_DefaultDERControl_t::DERControlBase
    @var SE_DefaultDERControl_t::setESDelay
    Enter service delay, in hundredths of a second. When present, this value SHALL update the value of the corresponding setting (DERSettings::setESDelay).

    @var SE_DefaultDERControl_t::setESHighFreq
    Enter service frequency high. Specified in hundredths of Hz. When present, this value SHALL update the value of the corresponding setting (DERSettings::setESHighFreq).

    @var SE_DefaultDERControl_t::setESHighVolt
    Enter service voltage high. Specified as an effective percent voltage, defined as (100% * (locally measured voltage - setVRefOfs) / setVRef), in hundredths of a percent. When present, this value SHALL update the value of the corresponding setting (DERSettings::setESHighVolt).

    @var SE_DefaultDERControl_t::setESLowFreq
    Enter service frequency low. Specified in hundredths of Hz. When present, this value SHALL update the value of the corresponding setting (DERSettings::setESLowFreq).

    @var SE_DefaultDERControl_t::setESLowVolt
    Enter service voltage low. Specified as an effective percent voltage, defined as (100% * (locally measured voltage - setVRefOfs) / setVRef), in hundredths of a percent. When present, this value SHALL update the value of the corresponding setting (DERSettings::setESLowVolt).

    @var SE_DefaultDERControl_t::setESRandomDelay
    Enter service randomized delay, in hundredths of a second. When present, this value SHALL update the value of the corresponding setting (DERSettings::setESRandomDelay).

    @var SE_DefaultDERControl_t::setGradW
    Set default rate of change (ramp rate) of active power output due to command or internal action, defined in %setWMax / second.  Resolution is in hundredths of a percent/second. A value of 0 means there is no limit. Interpreted as a percentage change in output capability limit per second when used as a default ramp rate. When present, this value SHALL update the value of the corresponding setting (DERSettings::setGradW).

    @var SE_DefaultDERControl_t::setSoftGradW
    Set soft-start rate of change (soft-start ramp rate) of active power output due to command or internal action, defined in %setWMax / second.  Resolution is in hundredths of a percent/second. A value of 0 means there is no limit. Interpreted as a percentage change in output capability limit per second when used as a ramp rate. When present, this value SHALL update the value of the corresponding setting (DERSettings::setSoftGradW).

*/

/** @} */

/** @defgroup SignedRealEnergy SignedRealEnergy
    @ingroup se_types

    Real electrical energy, signed.

    @{ */

/** @struct SE_SignedRealEnergy_t se_types.h

    Real electrical energy, signed.

    @var SE_SignedRealEnergy_t::multiplier
    Multiplier for 'unit'.

    @var SE_SignedRealEnergy_t::value
    Value of the energy in Watt-hours. (uom 72)

*/

/** @} */

/** @defgroup FlowReservationResponse FlowReservationResponse
    @ingroup Event

    The server may modify the charging or discharging parameters and interval to provide a lower aggregated demand at the premises, or within a larger part of the distribution system.

    @{ */

/** @struct SE_FlowReservationResponse_t se_types.h

    The server may modify the charging or discharging parameters and interval to provide a lower aggregated demand at the premises, or within a larger part of the distribution system.

    @var SE_FlowReservationResponse_t::_flags
#define SE_responseRequired_exists (1 << 5)

    @var SE_FlowReservationResponse_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_FlowReservationResponse_t::replyTo
    A reference to the response resource address (URI). Required on a response to a GET if responseRequired is "true".

    @var SE_FlowReservationResponse_t::responseRequired
    Indicates whether or not a response is required upon receipt, creation or update of this resource. Responses shall be posted to the collection specified in "replyTo". 

    If the resource has a deviceCategory field, devices that match one or more of the device types indicated in deviceCategory SHALL respond according to the rules listed below.  If the category does not match, the device SHALL NOT respond. If the resource does not have a deviceCategory field, a device receiving the resource SHALL respond according to the rules listed below.

    Value encoded as hex according to the following bit assignments, any combination is possible. 

    See Table 10-10 for the list of appropriate Response status codes to be sent for these purposes.

    0 - End device shall indicate that message was received 

    1 - End device shall indicate specific response. 

    2 - End user / customer response is required. 

    All other values reserved.

    @var SE_FlowReservationResponse_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_FlowReservationResponse_t::mRID
    The global identifier of the object.

    @var SE_FlowReservationResponse_t::description
    The description is a human readable text describing or naming the object.

    @var SE_FlowReservationResponse_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_FlowReservationResponse_t::creationTime
    The time at which the Event was created.

    @var SE_FlowReservationResponse_t::EventStatus
    @var SE_FlowReservationResponse_t::interval
    The period during which the Event applies.

    @var SE_FlowReservationResponse_t::energyAvailable
    Indicates the amount of energy available.

    @var SE_FlowReservationResponse_t::powerAvailable
    Indicates the amount of power available.

    @var SE_FlowReservationResponse_t::subject
    The subject field provides a method to match the response with the originating event. It is populated with the mRID of the corresponding FlowReservationRequest object.

*/

/** @} */

/** @defgroup FlowReservationResponseList FlowReservationResponseList
    @ingroup SubscribableList

    A List element to hold FlowReservationResponse objects.

    @{ */

/** @struct SE_FlowReservationResponseList_t se_types.h

    A List element to hold FlowReservationResponse objects.

    @var SE_FlowReservationResponseList_t::_flags
    @var SE_FlowReservationResponseList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_FlowReservationResponseList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_FlowReservationResponseList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_FlowReservationResponseList_t::results
    Indicates the number of items in this page of results.

    @var SE_FlowReservationResponseList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_FlowReservationResponseList_t::FlowReservationResponse
*/

/** @} */

/** @defgroup RequestStatus RequestStatus
    @ingroup se_types

    The RequestStatus object is used to indicate the current status of a Flow Reservation Request.

    @{ */

/** @struct SE_RequestStatus_t se_types.h

    The RequestStatus object is used to indicate the current status of a Flow Reservation Request.

    @var SE_RequestStatus_t::dateTime
    The dateTime attribute will provide a timestamp of when the request status was set. dateTime MUST be set to the time at which the status change occurred, not a time in the future or past.

    @var SE_RequestStatus_t::requestStatus
    Field representing the request status type. 

    0 = Requested

    1 = Cancelled

    All other values reserved.

*/

/** @} */

/** @defgroup FlowReservationRequest FlowReservationRequest
    @ingroup IdentifiedObject

    Used to request flow transactions. Client EndDevices submit a request for charging or discharging from the server. The server creates an associated FlowReservationResponse containing the charging parameters and interval to provide a lower aggregated demand at the premises, or within a larger part of the distribution system.

    @{ */

/** @struct SE_FlowReservationRequest_t se_types.h

    Used to request flow transactions. Client EndDevices submit a request for charging or discharging from the server. The server creates an associated FlowReservationResponse containing the charging parameters and interval to provide a lower aggregated demand at the premises, or within a larger part of the distribution system.

    @var SE_FlowReservationRequest_t::_flags
#define SE_durationRequested_exists (1 << 5)

    @var SE_FlowReservationRequest_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_FlowReservationRequest_t::mRID
    The global identifier of the object.

    @var SE_FlowReservationRequest_t::description
    The description is a human readable text describing or naming the object.

    @var SE_FlowReservationRequest_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_FlowReservationRequest_t::creationTime
    The time at which the request was created.

    @var SE_FlowReservationRequest_t::durationRequested
    A value that is calculated by the storage device that defines the minimum duration, in seconds, that it will take to complete the actual flow transaction, including any ramp times and conditioning times, if applicable.

    @var SE_FlowReservationRequest_t::energyRequested
    Indicates the total amount of energy, in Watt-Hours, requested to be transferred between the storage device and the electric power system. Positive values indicate charging and negative values indicate discharging. This sign convention is different than for the DER function where discharging is positive.  Note that the energyRequestNow attribute in the PowerStatus Object must always represent a charging solution and it is not allowed to have a negative value.

    @var SE_FlowReservationRequest_t::intervalRequested
    The time window during which the flow reservation is needed. For example, if an electric vehicle is set with a 7:00 AM time charge is needed, and price drops to the lowest tier at 11:00 PM, then this window would likely be from 11:00 PM until 7:00 AM.

    @var SE_FlowReservationRequest_t::powerRequested
    Indicates the sustained level of power, in Watts, that is requested. For charging this is calculated by the storage device and it represents the charging system capability (which for an electric vehicle must also account for any power limitations due to the EVSE control pilot). For discharging, a lower value than the inverter capability can be used as a target.

    @var SE_FlowReservationRequest_t::RequestStatus
*/

/** @} */

/** @defgroup FlowReservationRequestList FlowReservationRequestList
    @ingroup List

    A List element to hold FlowReservationRequest objects.

    @{ */

/** @struct SE_FlowReservationRequestList_t se_types.h

    A List element to hold FlowReservationRequest objects.

    @var SE_FlowReservationRequestList_t::_flags
    @var SE_FlowReservationRequestList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_FlowReservationRequestList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_FlowReservationRequestList_t::results
    Indicates the number of items in this page of results.

    @var SE_FlowReservationRequestList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_FlowReservationRequestList_t::FlowReservationRequest
*/

/** @} */

/** @defgroup SupplyInterruptionOverride SupplyInterruptionOverride
    @ingroup Resource

    SupplyInterruptionOverride: There may be periods of time when social, regulatory or other concerns mean that service should not be interrupted, even when available credit has been exhausted. Each Prepayment instance links to a List of SupplyInterruptionOverride instances. Each SupplyInterruptionOverride defines a contiguous period of time during which supply SHALL NOT be interrupted.

    @{ */

/** @struct SE_SupplyInterruptionOverride_t se_types.h

    SupplyInterruptionOverride: There may be periods of time when social, regulatory or other concerns mean that service should not be interrupted, even when available credit has been exhausted. Each Prepayment instance links to a List of SupplyInterruptionOverride instances. Each SupplyInterruptionOverride defines a contiguous period of time during which supply SHALL NOT be interrupted.

    @var SE_SupplyInterruptionOverride_t::_flags
    @var SE_SupplyInterruptionOverride_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_SupplyInterruptionOverride_t::description
    The description is a human readable text describing or naming the object.

    @var SE_SupplyInterruptionOverride_t::interval
    Interval defines the period of time during which supply should not be interrupted.

*/

/** @} */

/** @defgroup SupplyInterruptionOverrideList SupplyInterruptionOverrideList
    @ingroup List

    A List element to hold SupplyInterruptionOverride objects.

    @{ */

/** @struct SE_SupplyInterruptionOverrideList_t se_types.h

    A List element to hold SupplyInterruptionOverride objects.

    @var SE_SupplyInterruptionOverrideList_t::_flags
    @var SE_SupplyInterruptionOverrideList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_SupplyInterruptionOverrideList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_SupplyInterruptionOverrideList_t::results
    Indicates the number of items in this page of results.

    @var SE_SupplyInterruptionOverrideList_t::SupplyInterruptionOverride
*/

/** @} */

/** @defgroup ServiceStatusType ServiceStatusType
    @ingroup UInt8

    0 - Connected

    1 - Disconnected

    2 - Armed for Connect

    3 - Armed for Disconnect

    4 - No Contactor

    5 - Load Limited

    All other values reserved.

    @{ */

/** @typedef SE_ServiceStatusType_t

    0 - Connected

    1 - Disconnected

    2 - Armed for Connect

    3 - Armed for Disconnect

    4 - No Contactor

    5 - Load Limited

    All other values reserved.

*/

/** @} */

/** @defgroup ServiceChange ServiceChange
    @ingroup se_types

    Specifies a change to the service status.

    @{ */

/** @struct SE_ServiceChange_t se_types.h

    Specifies a change to the service status.

    @var SE_ServiceChange_t::newStatus
    The new service status, to take effect at the time specified by startTime

    @var SE_ServiceChange_t::startTime
    The date/time when the change is to take effect.

*/

/** @} */

/** @defgroup CreditTypeType CreditTypeType
    @ingroup UInt8

    0 - Regular

    1 - Emergency

    2 - Regular, then Emergency

    3 - Emergency, then Regular

    All other values reserved.

    @{ */

/** @typedef SE_CreditTypeType_t

    0 - Regular

    1 - Emergency

    2 - Regular, then Emergency

    3 - Emergency, then Regular

    All other values reserved.

*/

/** @} */

/** @defgroup CreditTypeChange CreditTypeChange
    @ingroup se_types

    Specifies a change to the credit type.

    @{ */

/** @struct SE_CreditTypeChange_t se_types.h

    Specifies a change to the credit type.

    @var SE_CreditTypeChange_t::newType
    The new credit type, to take effect at the time specified by startTime

    @var SE_CreditTypeChange_t::startTime
    The date/time when the change is to take effect.

*/

/** @} */

/** @defgroup PrepayOperationStatus PrepayOperationStatus
    @ingroup Resource

    PrepayOperationStatus describes the status of the service or commodity being conditionally controlled by the Prepayment function set.

    @{ */

/** @struct SE_PrepayOperationStatus_t se_types.h

    PrepayOperationStatus describes the status of the service or commodity being conditionally controlled by the Prepayment function set.

    @var SE_PrepayOperationStatus_t::_flags
#define SE_serviceChange_exists (1 << 5)

#define SE_creditTypeInUse_exists (1 << 6)

#define SE_creditTypeChange_exists (1 << 7)

    @var SE_PrepayOperationStatus_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_PrepayOperationStatus_t::creditTypeChange
    CreditTypeChange is used to define a pending change of creditTypeInUse, which will activate at a specified time.

    @var SE_PrepayOperationStatus_t::creditTypeInUse
    CreditTypeInUse identifies whether the present mode of operation is consuming regular credit or emergency credit.

    @var SE_PrepayOperationStatus_t::serviceChange
    ServiceChange is used to define a pending change of serviceStatus, which will activate at a specified time.

    @var SE_PrepayOperationStatus_t::serviceStatus
    ServiceStatus identifies whether the service is connected or disconnected, or armed for connection or disconnection.

*/

/** @} */

/** @defgroup UsagePointLink UsagePointLink
    @ingroup Link

    SHALL contain a Link to an instance of UsagePoint.

    @{ */

/** @struct SE_UsagePointLink_t se_types.h

    SHALL contain a Link to an instance of UsagePoint.

    @var SE_UsagePointLink_t::_flags
    @var SE_UsagePointLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup MeterReadingListLink MeterReadingListLink
    @ingroup ListLink

    SHALL contain a Link to a List of MeterReading instances.

    @{ */

/** @struct SE_MeterReadingListLink_t se_types.h

    SHALL contain a Link to a List of MeterReading instances.

    @var SE_MeterReadingListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_MeterReadingListLink_t::href
    A URI reference.

    @var SE_MeterReadingListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup UsagePoint UsagePoint
    @ingroup UsagePointBase

    Logical point on a network at which consumption or production is either physically measured (e.g. metered) or estimated (e.g. unmetered street lights).

    @{ */

/** @struct SE_UsagePoint_t se_types.h

    Logical point on a network at which consumption or production is either physically measured (e.g. metered) or estimated (e.g. unmetered street lights).

    @var SE_UsagePoint_t::_flags
#define SE_MeterReadingListLink_exists (1 << 5)

#define SE_deviceLFDI_exists (1 << 6)

    @var SE_UsagePoint_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_UsagePoint_t::mRID
    The global identifier of the object.

    @var SE_UsagePoint_t::description
    The description is a human readable text describing or naming the object.

    @var SE_UsagePoint_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_UsagePoint_t::roleFlags
    Specifies the roles that apply to the usage point.

    @var SE_UsagePoint_t::serviceCategoryKind
    The kind of service provided by this usage point.

    @var SE_UsagePoint_t::status
    Specifies the current status of the service at this usage point.

    0 = off

    1 = on

    @var SE_UsagePoint_t::deviceLFDI
    The LFDI of the source device. This attribute SHALL be present when mirroring.

    @var SE_UsagePoint_t::MeterReadingListLink
*/

/** @} */

/** @defgroup SupplyInterruptionOverrideListLink SupplyInterruptionOverrideListLink
    @ingroup ListLink

    SHALL contain a Link to a List of SupplyInterruptionOverride instances.

    @{ */

/** @struct SE_SupplyInterruptionOverrideListLink_t se_types.h

    SHALL contain a Link to a List of SupplyInterruptionOverride instances.

    @var SE_SupplyInterruptionOverrideListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_SupplyInterruptionOverrideListLink_t::href
    A URI reference.

    @var SE_SupplyInterruptionOverrideListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup PrepayOperationStatusLink PrepayOperationStatusLink
    @ingroup Link

    SHALL contain a Link to an instance of PrepayOperationStatus.

    @{ */

/** @struct SE_PrepayOperationStatusLink_t se_types.h

    SHALL contain a Link to an instance of PrepayOperationStatus.

    @var SE_PrepayOperationStatusLink_t::_flags
    @var SE_PrepayOperationStatusLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup PrepayModeType PrepayModeType
    @ingroup UInt8

    0 - Central Wallet

    1 - ESI

    2 - Local

    3 - Credit

    All other values reserved.

    @{ */

/** @typedef SE_PrepayModeType_t

    0 - Central Wallet

    1 - ESI

    2 - Local

    3 - Credit

    All other values reserved.

*/

/** @} */

/** @defgroup CreditRegisterListLink CreditRegisterListLink
    @ingroup ListLink

    SHALL contain a Link to a List of CreditRegister instances.

    @{ */

/** @struct SE_CreditRegisterListLink_t se_types.h

    SHALL contain a Link to a List of CreditRegister instances.

    @var SE_CreditRegisterListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_CreditRegisterListLink_t::href
    A URI reference.

    @var SE_CreditRegisterListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup CurrencyCode CurrencyCode
    @ingroup UInt16

    Follows codes defined in [ISO 4217]. 

    0 - Not Applicable (default, if not specified)

    36 - Australian Dollar

    124 - Canadian Dollar

    840 - US Dollar

    978 - Euro

    This is not a complete list.

    @{ */

/** @typedef SE_CurrencyCode_t

    Follows codes defined in [ISO 4217]. 

    0 - Not Applicable (default, if not specified)

    36 - Australian Dollar

    124 - Canadian Dollar

    840 - US Dollar

    978 - Euro

    This is not a complete list.

*/

/** @} */

/** @defgroup RealEnergy RealEnergy
    @ingroup se_types

    Real electrical energy

    @{ */

/** @struct SE_RealEnergy_t se_types.h

    Real electrical energy

    @var SE_RealEnergy_t::multiplier
    Multiplier for 'unit'.

    @var SE_RealEnergy_t::value
    Value of the energy in Watt-hours. (uom 72)

*/

/** @} */

/** @defgroup AccountingUnit AccountingUnit
    @ingroup se_types

    Unit for accounting; use either 'energyUnit' or 'currencyUnit' to specify the unit for 'value'.

    @{ */

/** @struct SE_AccountingUnit_t se_types.h

    Unit for accounting; use either 'energyUnit' or 'currencyUnit' to specify the unit for 'value'.

    @var SE_AccountingUnit_t::_flags
#define SE_energyUnit_exists (1 << 0)

    @var SE_AccountingUnit_t::energyUnit
    Unit of service.

    @var SE_AccountingUnit_t::monetaryUnit
    Unit of currency.

    @var SE_AccountingUnit_t::multiplier
    Multiplier for the 'energyUnit' or 'monetaryUnit'.

    @var SE_AccountingUnit_t::value
    Value of the monetary aspect

*/

/** @} */

/** @defgroup ActiveSupplyInterruptionOverrideListLink ActiveSupplyInterruptionOverrideListLink
    @ingroup ListLink

    SHALL contain a Link to a List of active SupplyInterruptionOverride instances.

    @{ */

/** @struct SE_ActiveSupplyInterruptionOverrideListLink_t se_types.h

    SHALL contain a Link to a List of active SupplyInterruptionOverride instances.

    @var SE_ActiveSupplyInterruptionOverrideListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_ActiveSupplyInterruptionOverrideListLink_t::href
    A URI reference.

    @var SE_ActiveSupplyInterruptionOverrideListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup ActiveCreditRegisterListLink ActiveCreditRegisterListLink
    @ingroup ListLink

    SHALL contain a Link to a List of active CreditRegister instances.

    @{ */

/** @struct SE_ActiveCreditRegisterListLink_t se_types.h

    SHALL contain a Link to a List of active CreditRegister instances.

    @var SE_ActiveCreditRegisterListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_ActiveCreditRegisterListLink_t::href
    A URI reference.

    @var SE_ActiveCreditRegisterListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup AccountBalanceLink AccountBalanceLink
    @ingroup Link

    SHALL contain a Link to an instance of AccountBalance.

    @{ */

/** @struct SE_AccountBalanceLink_t se_types.h

    SHALL contain a Link to an instance of AccountBalance.

    @var SE_AccountBalanceLink_t::_flags
    @var SE_AccountBalanceLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup Prepayment Prepayment
    @ingroup IdentifiedObject

    Prepayment (inherited from CIM SDPAccountingFunction)

    @{ */

/** @struct SE_Prepayment_t se_types.h

    Prepayment (inherited from CIM SDPAccountingFunction)

    @var SE_Prepayment_t::_flags
#define SE_lowEmergencyCreditWarningLevel_exists (1 << 5)

#define SE_lowCreditWarningLevel_exists (1 << 6)

#define SE_creditExpiryLevel_exists (1 << 7)

#define SE_ActiveSupplyInterruptionOverrideListLink_exists (1 << 8)

#define SE_ActiveCreditRegisterListLink_exists (1 << 9)

    @var SE_Prepayment_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_Prepayment_t::mRID
    The global identifier of the object.

    @var SE_Prepayment_t::description
    The description is a human readable text describing or naming the object.

    @var SE_Prepayment_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_Prepayment_t::AccountBalanceLink
    @var SE_Prepayment_t::ActiveCreditRegisterListLink
    @var SE_Prepayment_t::ActiveSupplyInterruptionOverrideListLink
    @var SE_Prepayment_t::creditExpiryLevel
    CreditExpiryLevel is the set point for availableCredit at which the service level may be changed. The typical value for this attribute is 0, regardless of whether the account balance is measured in a monetary or commodity basis. The units for this attribute SHALL match the units used for availableCredit.

    @var SE_Prepayment_t::CreditRegisterListLink
    @var SE_Prepayment_t::lowCreditWarningLevel
    LowCreditWarningLevel is the set point for availableCredit at which the creditStatus attribute in the AccountBalance resource SHALL indicate that available credit is low. The units for this attribute SHALL match the units used for availableCredit. Typically, this value is set by the service provider.

    @var SE_Prepayment_t::lowEmergencyCreditWarningLevel
    LowEmergencyCreditWarningLevel is the set point for emergencyCredit at which the creditStatus attribute in the AccountBalance resource SHALL indicate that emergencycredit is low. The units for this attribute SHALL match the units used for availableCredit. Typically, this value is set by the service provider.

    @var SE_Prepayment_t::prepayMode
    PrepayMode specifies whether the given Prepayment instance is operating in Credit, Central Wallet, ESI, or Local prepayment mode. The Credit mode indicates that prepayment is not presently in effect. The other modes are described in the Overview Section above.

    @var SE_Prepayment_t::PrepayOperationStatusLink
    @var SE_Prepayment_t::SupplyInterruptionOverrideListLink
    @var SE_Prepayment_t::UsagePoint
    @var SE_Prepayment_t::UsagePointLink
*/

/** @} */

/** @defgroup PrepaymentList PrepaymentList
    @ingroup SubscribableList

    A List element to hold Prepayment objects.

    @{ */

/** @struct SE_PrepaymentList_t se_types.h

    A List element to hold Prepayment objects.

    @var SE_PrepaymentList_t::_flags
    @var SE_PrepaymentList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_PrepaymentList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_PrepaymentList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_PrepaymentList_t::results
    Indicates the number of items in this page of results.

    @var SE_PrepaymentList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_PrepaymentList_t::Prepayment
*/

/** @} */

/** @defgroup CreditRegister CreditRegister
    @ingroup IdentifiedObject

    CreditRegister instances define a credit-modifying transaction. Typically this would be a credit-adding transaction, but may be a subtracting transaction (perhaps in response to an out-of-band debt signal).

    @{ */

/** @struct SE_CreditRegister_t se_types.h

    CreditRegister instances define a credit-modifying transaction. Typically this would be a credit-adding transaction, but may be a subtracting transaction (perhaps in response to an out-of-band debt signal).

    @var SE_CreditRegister_t::_flags
#define SE_creditType_exists (1 << 5)

    @var SE_CreditRegister_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_CreditRegister_t::mRID
    The global identifier of the object.

    @var SE_CreditRegister_t::description
    The description is a human readable text describing or naming the object.

    @var SE_CreditRegister_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_CreditRegister_t::creditAmount
    CreditAmount is the amount of credit being added by a particular CreditRegister transaction. Negative values indicate that credit is being subtracted.

    @var SE_CreditRegister_t::creditType
    CreditType indicates whether the credit transaction applies to regular or emergency credit.

    @var SE_CreditRegister_t::effectiveTime
    EffectiveTime identifies the time at which the credit transaction goes into effect. For credit addition transactions, this is typically the moment at which the transaction takes place. For credit subtraction transactions, (e.g., non-fuel debt recovery transactions initiated from a back-haul or ESI) this may be a future time at which credit is deducted.

    @var SE_CreditRegister_t::token
    Token is security data that authenticates the legitimacy of the transaction. The details of this token are not defined by IEEE 2030.5. How a Prepayment server handles this field is left as vendor specific implementation or will be defined by one or more other standards.

*/

/** @} */

/** @defgroup CreditRegisterList CreditRegisterList
    @ingroup List

    A List element to hold CreditRegister objects.

    @{ */

/** @struct SE_CreditRegisterList_t se_types.h

    A List element to hold CreditRegister objects.

    @var SE_CreditRegisterList_t::_flags
    @var SE_CreditRegisterList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_CreditRegisterList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_CreditRegisterList_t::results
    Indicates the number of items in this page of results.

    @var SE_CreditRegisterList_t::CreditRegister
*/

/** @} */

/** @defgroup CreditStatusType CreditStatusType
    @ingroup UInt8

    0 - Credit Ok

    1 - Credit Low

    2 - Credit Exhausted

    3 - Credit Negative

    All other values reserved.

    @{ */

/** @typedef SE_CreditStatusType_t

    0 - Credit Ok

    1 - Credit Low

    2 - Credit Exhausted

    3 - Credit Negative

    All other values reserved.

*/

/** @} */

/** @defgroup AccountBalance AccountBalance
    @ingroup Resource

    AccountBalance contains the regular credit and emergency credit balance for this given service or commodity prepay instance. It may also contain status information concerning the balance data.

    @{ */

/** @struct SE_AccountBalance_t se_types.h

    AccountBalance contains the regular credit and emergency credit balance for this given service or commodity prepay instance. It may also contain status information concerning the balance data.

    @var SE_AccountBalance_t::_flags
#define SE_emergencyCreditStatus_exists (1 << 5)

#define SE_emergencyCredit_exists (1 << 6)

#define SE_creditStatus_exists (1 << 7)

    @var SE_AccountBalance_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_AccountBalance_t::availableCredit
    AvailableCredit shows the balance of the sum of credits minus the sum of charges. In a Central Wallet mode this value may be passed down to the Prepayment server via an out-of-band mechanism. In Local or ESI modes, this value may be calculated based upon summation of CreditRegister transactions minus consumption charges calculated using Metering (and possibly Pricing) function set data. This value may be negative; for instance, if disconnection is prevented due to a Supply Interruption Override.

    @var SE_AccountBalance_t::creditStatus
    CreditStatus identifies whether the present value of availableCredit is considered OK, low, exhausted, or negative.

    @var SE_AccountBalance_t::emergencyCredit
    EmergencyCredit is the amount of credit still available for the given service or commodity prepayment instance. If both availableCredit and emergyCredit are exhausted, then service will typically be disconnected.

    @var SE_AccountBalance_t::emergencyCreditStatus
    EmergencyCreditStatus identifies whether the present value of emergencyCredit is considered OK, low, exhausted, or negative.

*/

/** @} */

/** @defgroup String42 String42
    @ingroup se_typedefs

    Character string of max length 42. In order to limit internal storage, implementations SHALL reduce the length of strings using multi-byte characters so that the string may be stored using "maxLength" octets in the given encoding.

    @{ */

/** @typedef SE_String42_t

    Character string of max length 42. In order to limit internal storage, implementations SHALL reduce the length of strings using multi-byte characters so that the string may be stored using "maxLength" octets in the given encoding.

*/

/** @} */

/** @defgroup String20 String20
    @ingroup se_typedefs

    Character string of max length 20. In order to limit internal storage, implementations SHALL reduce the length of strings using multi-byte characters so that the string may be stored using "maxLength" octets in the given encoding.

    @{ */

/** @typedef SE_String20_t

    Character string of max length 20. In order to limit internal storage, implementations SHALL reduce the length of strings using multi-byte characters so that the string may be stored using "maxLength" octets in the given encoding.

*/

/** @} */

/** @defgroup ServiceSupplier ServiceSupplier
    @ingroup IdentifiedObject

    Organisation that provides services to Customers.

    @{ */

/** @struct SE_ServiceSupplier_t se_types.h

    Organisation that provides services to Customers.

    @var SE_ServiceSupplier_t::_flags
#define SE_web_exists (1 << 5)

#define SE_providerID_exists (1 << 6)

#define SE_phone_exists (1 << 7)

#define SE_email_exists (1 << 8)

    @var SE_ServiceSupplier_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_ServiceSupplier_t::mRID
    The global identifier of the object.

    @var SE_ServiceSupplier_t::description
    The description is a human readable text describing or naming the object.

    @var SE_ServiceSupplier_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_ServiceSupplier_t::email
    E-mail address for this service supplier.

    @var SE_ServiceSupplier_t::phone
    Human-readable phone number for this service supplier.

    @var SE_ServiceSupplier_t::providerID
    Contains the IANA PEN for the commodity provider.

    @var SE_ServiceSupplier_t::web
    Website URI address for this service supplier.

*/

/** @} */

/** @defgroup ServiceSupplierList ServiceSupplierList
    @ingroup List

    A List element to hold ServiceSupplier objects.

    @{ */

/** @struct SE_ServiceSupplierList_t se_types.h

    A List element to hold ServiceSupplier objects.

    @var SE_ServiceSupplierList_t::_flags
    @var SE_ServiceSupplierList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_ServiceSupplierList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_ServiceSupplierList_t::results
    Indicates the number of items in this page of results.

    @var SE_ServiceSupplierList_t::ServiceSupplier
*/

/** @} */

/** @defgroup ReadingTypeLink ReadingTypeLink
    @ingroup Link

    SHALL contain a Link to an instance of ReadingType.

    @{ */

/** @struct SE_ReadingTypeLink_t se_types.h

    SHALL contain a Link to an instance of ReadingType.

    @var SE_ReadingTypeLink_t::_flags
    @var SE_ReadingTypeLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup BillingReadingSetListLink BillingReadingSetListLink
    @ingroup ListLink

    SHALL contain a Link to a List of BillingReadingSet instances.

    @{ */

/** @struct SE_BillingReadingSetListLink_t se_types.h

    SHALL contain a Link to a List of BillingReadingSet instances.

    @var SE_BillingReadingSetListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_BillingReadingSetListLink_t::href
    A URI reference.

    @var SE_BillingReadingSetListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup BillingMeterReadingBase BillingMeterReadingBase
    @ingroup MeterReadingBase

    Contains historical, target, and projection readings of various types, possibly associated with charges.

    @{ */

/** @struct SE_BillingMeterReadingBase_t se_types.h

    Contains historical, target, and projection readings of various types, possibly associated with charges.

    @var SE_BillingMeterReadingBase_t::_flags
#define SE_ReadingTypeLink_exists (1 << 5)

#define SE_BillingReadingSetListLink_exists (1 << 6)

    @var SE_BillingMeterReadingBase_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_BillingMeterReadingBase_t::mRID
    The global identifier of the object.

    @var SE_BillingMeterReadingBase_t::description
    The description is a human readable text describing or naming the object.

    @var SE_BillingMeterReadingBase_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_BillingMeterReadingBase_t::BillingReadingSetListLink
    @var SE_BillingMeterReadingBase_t::ReadingTypeLink
*/

/** @} */

/** @defgroup TargetReading TargetReading
    @ingroup BillingMeterReadingBase

    Contains readings that specify a target or goal, such as a consumption target, to which billing incentives or other contractual ramifications may be associated.

    @{ */

/** @struct SE_TargetReading_t se_types.h

    Contains readings that specify a target or goal, such as a consumption target, to which billing incentives or other contractual ramifications may be associated.

    @var SE_TargetReading_t::_flags
#define SE_ReadingTypeLink_exists (1 << 5)

#define SE_BillingReadingSetListLink_exists (1 << 6)

    @var SE_TargetReading_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_TargetReading_t::mRID
    The global identifier of the object.

    @var SE_TargetReading_t::description
    The description is a human readable text describing or naming the object.

    @var SE_TargetReading_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_TargetReading_t::BillingReadingSetListLink
    @var SE_TargetReading_t::ReadingTypeLink
*/

/** @} */

/** @defgroup TargetReadingList TargetReadingList
    @ingroup List

    A List element to hold TargetReading objects.

    @{ */

/** @struct SE_TargetReadingList_t se_types.h

    A List element to hold TargetReading objects.

    @var SE_TargetReadingList_t::_flags
    @var SE_TargetReadingList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_TargetReadingList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_TargetReadingList_t::results
    Indicates the number of items in this page of results.

    @var SE_TargetReadingList_t::TargetReading
*/

/** @} */

/** @defgroup ProjectionReading ProjectionReading
    @ingroup BillingMeterReadingBase

    Contains values that forecast a future reading for the time or interval specified.

    @{ */

/** @struct SE_ProjectionReading_t se_types.h

    Contains values that forecast a future reading for the time or interval specified.

    @var SE_ProjectionReading_t::_flags
#define SE_ReadingTypeLink_exists (1 << 5)

#define SE_BillingReadingSetListLink_exists (1 << 6)

    @var SE_ProjectionReading_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_ProjectionReading_t::mRID
    The global identifier of the object.

    @var SE_ProjectionReading_t::description
    The description is a human readable text describing or naming the object.

    @var SE_ProjectionReading_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_ProjectionReading_t::BillingReadingSetListLink
    @var SE_ProjectionReading_t::ReadingTypeLink
*/

/** @} */

/** @defgroup ProjectionReadingList ProjectionReadingList
    @ingroup List

    A List element to hold ProjectionReading objects.

    @{ */

/** @struct SE_ProjectionReadingList_t se_types.h

    A List element to hold ProjectionReading objects.

    @var SE_ProjectionReadingList_t::_flags
    @var SE_ProjectionReadingList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_ProjectionReadingList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_ProjectionReadingList_t::results
    Indicates the number of items in this page of results.

    @var SE_ProjectionReadingList_t::ProjectionReading
*/

/** @} */

/** @defgroup HistoricalReading HistoricalReading
    @ingroup BillingMeterReadingBase

    To be used to present readings that have been processed and possibly corrected (as allowed, due to missing or incorrect data) by backend systems. This includes quality codes valid, verified, estimated, and derived / corrected.

    @{ */

/** @struct SE_HistoricalReading_t se_types.h

    To be used to present readings that have been processed and possibly corrected (as allowed, due to missing or incorrect data) by backend systems. This includes quality codes valid, verified, estimated, and derived / corrected.

    @var SE_HistoricalReading_t::_flags
#define SE_ReadingTypeLink_exists (1 << 5)

#define SE_BillingReadingSetListLink_exists (1 << 6)

    @var SE_HistoricalReading_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_HistoricalReading_t::mRID
    The global identifier of the object.

    @var SE_HistoricalReading_t::description
    The description is a human readable text describing or naming the object.

    @var SE_HistoricalReading_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_HistoricalReading_t::BillingReadingSetListLink
    @var SE_HistoricalReading_t::ReadingTypeLink
*/

/** @} */

/** @defgroup HistoricalReadingList HistoricalReadingList
    @ingroup List

    A List element to hold HistoricalReading objects.

    @{ */

/** @struct SE_HistoricalReadingList_t se_types.h

    A List element to hold HistoricalReading objects.

    @var SE_HistoricalReadingList_t::_flags
    @var SE_HistoricalReadingList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_HistoricalReadingList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_HistoricalReadingList_t::results
    Indicates the number of items in this page of results.

    @var SE_HistoricalReadingList_t::HistoricalReading
*/

/** @} */

/** @defgroup TariffProfileLink TariffProfileLink
    @ingroup Link

    SHALL contain a Link to an instance of TariffProfile.

    @{ */

/** @struct SE_TariffProfileLink_t se_types.h

    SHALL contain a Link to an instance of TariffProfile.

    @var SE_TariffProfileLink_t::_flags
    @var SE_TariffProfileLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup TargetReadingListLink TargetReadingListLink
    @ingroup ListLink

    SHALL contain a Link to a List of TargetReading instances.

    @{ */

/** @struct SE_TargetReadingListLink_t se_types.h

    SHALL contain a Link to a List of TargetReading instances.

    @var SE_TargetReadingListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_TargetReadingListLink_t::href
    A URI reference.

    @var SE_TargetReadingListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup ProjectionReadingListLink ProjectionReadingListLink
    @ingroup ListLink

    SHALL contain a Link to a List of ProjectionReading instances.

    @{ */

/** @struct SE_ProjectionReadingListLink_t se_types.h

    SHALL contain a Link to a List of ProjectionReading instances.

    @var SE_ProjectionReadingListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_ProjectionReadingListLink_t::href
    A URI reference.

    @var SE_ProjectionReadingListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup PrepaymentLink PrepaymentLink
    @ingroup Link

    SHALL contain a Link to an instance of Prepayment.

    @{ */

/** @struct SE_PrepaymentLink_t se_types.h

    SHALL contain a Link to an instance of Prepayment.

    @var SE_PrepaymentLink_t::_flags
    @var SE_PrepaymentLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup HistoricalReadingListLink HistoricalReadingListLink
    @ingroup ListLink

    SHALL contain a Link to a List of HistoricalReading instances.

    @{ */

/** @struct SE_HistoricalReadingListLink_t se_types.h

    SHALL contain a Link to a List of HistoricalReading instances.

    @var SE_HistoricalReadingListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_HistoricalReadingListLink_t::href
    A URI reference.

    @var SE_HistoricalReadingListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup BillingPeriodListLink BillingPeriodListLink
    @ingroup ListLink

    SHALL contain a Link to a List of BillingPeriod instances.

    @{ */

/** @struct SE_BillingPeriodListLink_t se_types.h

    SHALL contain a Link to a List of BillingPeriod instances.

    @var SE_BillingPeriodListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_BillingPeriodListLink_t::href
    A URI reference.

    @var SE_BillingPeriodListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup ActiveTargetReadingListLink ActiveTargetReadingListLink
    @ingroup ListLink

    SHALL contain a Link to a List of active TargetReading instances.

    @{ */

/** @struct SE_ActiveTargetReadingListLink_t se_types.h

    SHALL contain a Link to a List of active TargetReading instances.

    @var SE_ActiveTargetReadingListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_ActiveTargetReadingListLink_t::href
    A URI reference.

    @var SE_ActiveTargetReadingListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup ActiveProjectionReadingListLink ActiveProjectionReadingListLink
    @ingroup ListLink

    SHALL contain a Link to a List of active ProjectionReading instances.

    @{ */

/** @struct SE_ActiveProjectionReadingListLink_t se_types.h

    SHALL contain a Link to a List of active ProjectionReading instances.

    @var SE_ActiveProjectionReadingListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_ActiveProjectionReadingListLink_t::href
    A URI reference.

    @var SE_ActiveProjectionReadingListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup ActiveBillingPeriodListLink ActiveBillingPeriodListLink
    @ingroup ListLink

    SHALL contain a Link to a List of active BillingPeriod instances.

    @{ */

/** @struct SE_ActiveBillingPeriodListLink_t se_types.h

    SHALL contain a Link to a List of active BillingPeriod instances.

    @var SE_ActiveBillingPeriodListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_ActiveBillingPeriodListLink_t::href
    A URI reference.

    @var SE_ActiveBillingPeriodListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup CustomerAgreement CustomerAgreement
    @ingroup IdentifiedObject

    Agreement between the customer and the service supplier to pay for service at a specific service location. It records certain billing information about the type of service provided at the service location and is used during charge creation to determine the type of service.

    @{ */

/** @struct SE_CustomerAgreement_t se_types.h

    Agreement between the customer and the service supplier to pay for service at a specific service location. It records certain billing information about the type of service provided at the service location and is used during charge creation to determine the type of service.

    @var SE_CustomerAgreement_t::_flags
#define SE_TariffProfileLink_exists (1 << 5)

#define SE_TargetReadingListLink_exists (1 << 6)

#define SE_serviceLocation_exists (1 << 7)

#define SE_serviceAccount_exists (1 << 8)

#define SE_ProjectionReadingListLink_exists (1 << 9)

#define SE_PrepaymentLink_exists (1 << 10)

#define SE_HistoricalReadingListLink_exists (1 << 11)

#define SE_BillingPeriodListLink_exists (1 << 12)

#define SE_ActiveTargetReadingListLink_exists (1 << 13)

#define SE_ActiveProjectionReadingListLink_exists (1 << 14)

#define SE_ActiveBillingPeriodListLink_exists (1 << 15)

    @var SE_CustomerAgreement_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_CustomerAgreement_t::mRID
    The global identifier of the object.

    @var SE_CustomerAgreement_t::description
    The description is a human readable text describing or naming the object.

    @var SE_CustomerAgreement_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_CustomerAgreement_t::ActiveBillingPeriodListLink
    @var SE_CustomerAgreement_t::ActiveProjectionReadingListLink
    @var SE_CustomerAgreement_t::ActiveTargetReadingListLink
    @var SE_CustomerAgreement_t::BillingPeriodListLink
    @var SE_CustomerAgreement_t::HistoricalReadingListLink
    @var SE_CustomerAgreement_t::PrepaymentLink
    @var SE_CustomerAgreement_t::ProjectionReadingListLink
    @var SE_CustomerAgreement_t::serviceAccount
    The account number of the service account (if applicable).

    @var SE_CustomerAgreement_t::serviceLocation
    The address or textual description of the service location.

    @var SE_CustomerAgreement_t::TargetReadingListLink
    @var SE_CustomerAgreement_t::TariffProfileLink
    @var SE_CustomerAgreement_t::UsagePointLink
*/

/** @} */

/** @defgroup CustomerAgreementList CustomerAgreementList
    @ingroup SubscribableList

    A List element to hold CustomerAgreement objects.

    @{ */

/** @struct SE_CustomerAgreementList_t se_types.h

    A List element to hold CustomerAgreement objects.

    @var SE_CustomerAgreementList_t::_flags
    @var SE_CustomerAgreementList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_CustomerAgreementList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_CustomerAgreementList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_CustomerAgreementList_t::results
    Indicates the number of items in this page of results.

    @var SE_CustomerAgreementList_t::CustomerAgreement
*/

/** @} */

/** @defgroup ServiceSupplierLink ServiceSupplierLink
    @ingroup Link

    SHALL contain a Link to an instance of ServiceSupplier.

    @{ */

/** @struct SE_ServiceSupplierLink_t se_types.h

    SHALL contain a Link to an instance of ServiceSupplier.

    @var SE_ServiceSupplierLink_t::_flags
    @var SE_ServiceSupplierLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup CustomerAgreementListLink CustomerAgreementListLink
    @ingroup ListLink

    SHALL contain a Link to a List of CustomerAgreement instances.

    @{ */

/** @struct SE_CustomerAgreementListLink_t se_types.h

    SHALL contain a Link to a List of CustomerAgreement instances.

    @var SE_CustomerAgreementListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_CustomerAgreementListLink_t::href
    A URI reference.

    @var SE_CustomerAgreementListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup CustomerAccount CustomerAccount
    @ingroup IdentifiedObject

    Assignment of a group of products and services purchased by the Customer through a CustomerAgreement, used as a mechanism for customer billing and payment. It contains common information from the various types of CustomerAgreements to create billings (invoices) for a Customer and receive payment.

    @{ */

/** @struct SE_CustomerAccount_t se_types.h

    Assignment of a group of products and services purchased by the Customer through a CustomerAgreement, used as a mechanism for customer billing and payment. It contains common information from the various types of CustomerAgreements to create billings (invoices) for a Customer and receive payment.

    @var SE_CustomerAccount_t::_flags
#define SE_ServiceSupplierLink_exists (1 << 5)

#define SE_customerName_exists (1 << 6)

#define SE_CustomerAgreementListLink_exists (1 << 7)

#define SE_customerAccount_exists (1 << 8)

    @var SE_CustomerAccount_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_CustomerAccount_t::mRID
    The global identifier of the object.

    @var SE_CustomerAccount_t::description
    The description is a human readable text describing or naming the object.

    @var SE_CustomerAccount_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_CustomerAccount_t::currency
    The ISO 4217 code indicating the currency applicable to the bill amounts in the summary. See list at http://www.unece.org/cefact/recommendations/rec09/rec09_ecetrd203.pdf

    @var SE_CustomerAccount_t::customerAccount
    The account number for the customer (if applicable).

    @var SE_CustomerAccount_t::CustomerAgreementListLink
    @var SE_CustomerAccount_t::customerName
    The name of the customer.

    @var SE_CustomerAccount_t::pricePowerOfTenMultiplier
    Indicates the power of ten multiplier for the prices in this function set.

    @var SE_CustomerAccount_t::ServiceSupplierLink
*/

/** @} */

/** @defgroup CustomerAccountList CustomerAccountList
    @ingroup SubscribableList

    A List element to hold CustomerAccount objects.

    @{ */

/** @struct SE_CustomerAccountList_t se_types.h

    A List element to hold CustomerAccount objects.

    @var SE_CustomerAccountList_t::_flags
    @var SE_CustomerAccountList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_CustomerAccountList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_CustomerAccountList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_CustomerAccountList_t::results
    Indicates the number of items in this page of results.

    @var SE_CustomerAccountList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_CustomerAccountList_t::CustomerAccount
*/

/** @} */

/** @defgroup BillingReadingListLink BillingReadingListLink
    @ingroup ListLink

    SHALL contain a Link to a List of BillingReading instances.

    @{ */

/** @struct SE_BillingReadingListLink_t se_types.h

    SHALL contain a Link to a List of BillingReading instances.

    @var SE_BillingReadingListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_BillingReadingListLink_t::href
    A URI reference.

    @var SE_BillingReadingListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup BillingReadingSet BillingReadingSet
    @ingroup ReadingSetBase

    Time sequence of readings of the same reading type.

    @{ */

/** @struct SE_BillingReadingSet_t se_types.h

    Time sequence of readings of the same reading type.

    @var SE_BillingReadingSet_t::_flags
#define SE_BillingReadingListLink_exists (1 << 5)

    @var SE_BillingReadingSet_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_BillingReadingSet_t::mRID
    The global identifier of the object.

    @var SE_BillingReadingSet_t::description
    The description is a human readable text describing or naming the object.

    @var SE_BillingReadingSet_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_BillingReadingSet_t::timePeriod
    Specifies the time range during which the contained readings were taken.

    @var SE_BillingReadingSet_t::BillingReadingListLink
*/

/** @} */

/** @defgroup BillingReadingSetList BillingReadingSetList
    @ingroup SubscribableList

    A List element to hold BillingReadingSet objects.

    @{ */

/** @struct SE_BillingReadingSetList_t se_types.h

    A List element to hold BillingReadingSet objects.

    @var SE_BillingReadingSetList_t::_flags
    @var SE_BillingReadingSetList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_BillingReadingSetList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_BillingReadingSetList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_BillingReadingSetList_t::results
    Indicates the number of items in this page of results.

    @var SE_BillingReadingSetList_t::BillingReadingSet
*/

/** @} */

/** @defgroup ChargeKind ChargeKind
    @ingroup UInt8

    Kind of charge.

    0 - Consumption Charge

    1 - Rebate 

    2 - Auxiliary Charge

    3 - Demand Charge

    4 - Tax Charge

    @{ */

/** @typedef SE_ChargeKind_t

    Kind of charge.

    0 - Consumption Charge

    1 - Rebate 

    2 - Auxiliary Charge

    3 - Demand Charge

    4 - Tax Charge

*/

/** @} */

/** @defgroup Charge Charge
    @ingroup se_types

    Charges contain charges on a customer bill.  These could be items like taxes, levies, surcharges, rebates, or others.  This is meant to allow the HAN device to retrieve enough information to be able to reconstruct an estimate of what the total bill would look like. 

    Providers can provide line item billing, including multiple charge kinds (e.g. taxes, surcharges) at whatever granularity desired, using as many Charges as desired during a billing period. There can also be any number of Charges associated with different ReadingTypes to distinguish between TOU tiers, consumption blocks, or demand charges.

    @{ */

/** @struct SE_Charge_t se_types.h

    Charges contain charges on a customer bill.  These could be items like taxes, levies, surcharges, rebates, or others.  This is meant to allow the HAN device to retrieve enough information to be able to reconstruct an estimate of what the total bill would look like. 

    Providers can provide line item billing, including multiple charge kinds (e.g. taxes, surcharges) at whatever granularity desired, using as many Charges as desired during a billing period. There can also be any number of Charges associated with different ReadingTypes to distinguish between TOU tiers, consumption blocks, or demand charges.

    @var SE_Charge_t::_flags
    @var SE_Charge_t::description
    A description of the charge.

    @var SE_Charge_t::kind
    The type (kind) of charge.

    @var SE_Charge_t::value
    A monetary charge.

*/

/** @} */

/** @defgroup BillingReading BillingReading
    @ingroup ReadingBase

    Data captured at regular intervals of time. Interval data could be captured as incremental data, absolute data, or relative data. The source for the data is usually a tariff quantity or an engineering quantity. Data is typically captured in time-tagged, uniform, fixed-length intervals of 5 min, 10 min, 15 min, 30 min, or 60 min. However, consumption aggregations can also be represented with this class.

    @{ */

/** @struct SE_BillingReading_t se_types.h

    Data captured at regular intervals of time. Interval data could be captured as incremental data, absolute data, or relative data. The source for the data is usually a tariff quantity or an engineering quantity. Data is typically captured in time-tagged, uniform, fixed-length intervals of 5 min, 10 min, 15 min, 30 min, or 60 min. However, consumption aggregations can also be represented with this class.

    @var SE_BillingReading_t::_flags
#define SE_value_exists (1 << 5)

#define SE_touTier_exists (1 << 6)

#define SE_timePeriod_exists (1 << 7)

#define SE_qualityFlags_exists (1 << 8)

#define SE_consumptionBlock_exists (1 << 9)

    @var SE_BillingReading_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_BillingReading_t::consumptionBlock
    Indicates the consumption block related to the reading. REQUIRED if ReadingType numberOfConsumptionBlocks is non-zero. If not specified, is assumed to be "0 - N/A".

    @var SE_BillingReading_t::qualityFlags
    List of codes indicating the quality of the reading, using specification:

    Bit 0 - valid: data that has gone through all required validation checks and either passed them all or has been verified 

    Bit 1 - manually edited: Replaced or approved by a human

    Bit 2 - estimated using reference day: data value was replaced by a machine computed value based on analysis of historical data using the same type of measurement.

    Bit 3 - estimated using linear interpolation: data value was computed using linear interpolation based on the readings before and after it

    Bit 4 - questionable: data that has failed one or more checks

    Bit 5 - derived: data that has been calculated (using logic or mathematical operations), not necessarily measured directly 

    Bit 6 - projected (forecast): data that has been calculated as a projection or forecast of future readings

    @var SE_BillingReading_t::timePeriod
    The time interval associated with the reading. If not specified, then defaults to the intervalLength specified in the associated ReadingType.

    @var SE_BillingReading_t::touTier
    Indicates the time of use tier related to the reading. REQUIRED if ReadingType numberOfTouTiers is non-zero. If not specified, is assumed to be "0 - N/A".

    @var SE_BillingReading_t::value
    Value in units specified by ReadingType

    @var SE_BillingReading_t::Charge
*/

/** @} */

/** @defgroup BillingReadingList BillingReadingList
    @ingroup List

    A List element to hold BillingReading objects.

    @{ */

/** @struct SE_BillingReadingList_t se_types.h

    A List element to hold BillingReading objects.

    @var SE_BillingReadingList_t::_flags
    @var SE_BillingReadingList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_BillingReadingList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_BillingReadingList_t::results
    Indicates the number of items in this page of results.

    @var SE_BillingReadingList_t::BillingReading
*/

/** @} */

/** @defgroup BillingPeriod BillingPeriod
    @ingroup Resource

    A Billing Period relates to the period of time on which a customer is billed.  As an example the billing period interval for a particular customer might be 31 days starting on July 1, 2011. The start date and interval can change on each billing period. There may also be multiple billing periods related to a customer agreement to support different tariff structures.

    @{ */

/** @struct SE_BillingPeriod_t se_types.h

    A Billing Period relates to the period of time on which a customer is billed.  As an example the billing period interval for a particular customer might be 31 days starting on July 1, 2011. The start date and interval can change on each billing period. There may also be multiple billing periods related to a customer agreement to support different tariff structures.

    @var SE_BillingPeriod_t::_flags
#define SE_statusTimeStamp_exists (1 << 5)

#define SE_billToDate_exists (1 << 6)

#define SE_billLastPeriod_exists (1 << 7)

    @var SE_BillingPeriod_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_BillingPeriod_t::billLastPeriod
    The amount of the bill for the previous billing period.

    @var SE_BillingPeriod_t::billToDate
    The bill amount related to the billing period as of the statusTimeStamp.

    @var SE_BillingPeriod_t::interval
    The time interval for this billing period.

    @var SE_BillingPeriod_t::statusTimeStamp
    The date / time of the last update of this resource.

*/

/** @} */

/** @defgroup BillingPeriodList BillingPeriodList
    @ingroup SubscribableList

    A List element to hold BillingPeriod objects.

    @{ */

/** @struct SE_BillingPeriodList_t se_types.h

    A List element to hold BillingPeriod objects.

    @var SE_BillingPeriodList_t::_flags
    @var SE_BillingPeriodList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_BillingPeriodList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_BillingPeriodList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_BillingPeriodList_t::results
    Indicates the number of items in this page of results.

    @var SE_BillingPeriodList_t::BillingPeriod
*/

/** @} */

/** @defgroup PriorityType PriorityType
    @ingroup UInt8

    Indicates the priority of a message:

    0 - Low

    1 - Normal

    2 - High

    3 - Critical

    All other values reserved.

    @{ */

/** @typedef SE_PriorityType_t

    Indicates the priority of a message:

    0 - Low

    1 - Normal

    2 - High

    3 - Critical

    All other values reserved.

*/

/** @} */

/** @defgroup TextMessage TextMessage
    @ingroup Event

    Text message such as a notification.

    @{ */

/** @struct SE_TextMessage_t se_types.h

    Text message such as a notification.

    @var SE_TextMessage_t::_flags
#define SE_responseRequired_exists (1 << 5)

#define SE_originator_exists (1 << 6)

    @var SE_TextMessage_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_TextMessage_t::replyTo
    A reference to the response resource address (URI). Required on a response to a GET if responseRequired is "true".

    @var SE_TextMessage_t::responseRequired
    Indicates whether or not a response is required upon receipt, creation or update of this resource. Responses shall be posted to the collection specified in "replyTo". 

    If the resource has a deviceCategory field, devices that match one or more of the device types indicated in deviceCategory SHALL respond according to the rules listed below.  If the category does not match, the device SHALL NOT respond. If the resource does not have a deviceCategory field, a device receiving the resource SHALL respond according to the rules listed below.

    Value encoded as hex according to the following bit assignments, any combination is possible. 

    See Table 10-10 for the list of appropriate Response status codes to be sent for these purposes.

    0 - End device shall indicate that message was received 

    1 - End device shall indicate specific response. 

    2 - End user / customer response is required. 

    All other values reserved.

    @var SE_TextMessage_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_TextMessage_t::mRID
    The global identifier of the object.

    @var SE_TextMessage_t::description
    The description is a human readable text describing or naming the object.

    @var SE_TextMessage_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_TextMessage_t::creationTime
    The time at which the Event was created.

    @var SE_TextMessage_t::EventStatus
    @var SE_TextMessage_t::interval
    The period during which the Event applies.

    @var SE_TextMessage_t::originator
    Indicates the human-readable name of the publisher of the message

    @var SE_TextMessage_t::priority
    The priority is used to inform the client of the priority of the particular message.  Devices with constrained or limited resources for displaying Messages should use this attribute to determine how to handle displaying currently active Messages (e.g. if a device uses a scrolling method with a single Message viewable at a time it MAY want to push a low priority Message to the background and bring a newly received higher priority Message to the foreground).

    @var SE_TextMessage_t::textMessage
    The textMessage attribute contains the actual UTF-8 encoded text to be displayed in conjunction with the messageLength attribute which contains the overall length of the textMessage attribute.  Clients and servers SHALL support a reception of a Message of 100 bytes in length.  Messages that exceed the clients display size will be left to the client to choose what method to handle the message (truncation, scrolling, etc.).

*/

/** @} */

/** @defgroup TextMessageList TextMessageList
    @ingroup SubscribableList

    A List element to hold TextMessage objects.

    @{ */

/** @struct SE_TextMessageList_t se_types.h

    A List element to hold TextMessage objects.

    @var SE_TextMessageList_t::_flags
    @var SE_TextMessageList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_TextMessageList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_TextMessageList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_TextMessageList_t::results
    Indicates the number of items in this page of results.

    @var SE_TextMessageList_t::TextMessage
*/

/** @} */

/** @defgroup TextMessageListLink TextMessageListLink
    @ingroup ListLink

    SHALL contain a Link to a List of TextMessage instances.

    @{ */

/** @struct SE_TextMessageListLink_t se_types.h

    SHALL contain a Link to a List of TextMessage instances.

    @var SE_TextMessageListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_TextMessageListLink_t::href
    A URI reference.

    @var SE_TextMessageListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup LocaleType LocaleType
    @ingroup String42

    [RFC 4646] identifier of a language-region

    @{ */

/** @typedef SE_LocaleType_t

    [RFC 4646] identifier of a language-region

*/

/** @} */

/** @defgroup ActiveTextMessageListLink ActiveTextMessageListLink
    @ingroup ListLink

    SHALL contain a Link to a List of active TextMessage instances.

    @{ */

/** @struct SE_ActiveTextMessageListLink_t se_types.h

    SHALL contain a Link to a List of active TextMessage instances.

    @var SE_ActiveTextMessageListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_ActiveTextMessageListLink_t::href
    A URI reference.

    @var SE_ActiveTextMessageListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup MessagingProgram MessagingProgram
    @ingroup SubscribableIdentifiedObject

    Provides a container for collections of text messages.

    @{ */

/** @struct SE_MessagingProgram_t se_types.h

    Provides a container for collections of text messages.

    @var SE_MessagingProgram_t::_flags
#define SE_TextMessageListLink_exists (1 << 5)

#define SE_ActiveTextMessageListLink_exists (1 << 6)

    @var SE_MessagingProgram_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_MessagingProgram_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_MessagingProgram_t::mRID
    The global identifier of the object.

    @var SE_MessagingProgram_t::description
    The description is a human readable text describing or naming the object.

    @var SE_MessagingProgram_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_MessagingProgram_t::ActiveTextMessageListLink
    @var SE_MessagingProgram_t::locale
    Indicates the language and region of the messages in this collection.

    @var SE_MessagingProgram_t::primacy
    Indicates the relative primacy of the provider of this program.

    @var SE_MessagingProgram_t::TextMessageListLink
*/

/** @} */

/** @defgroup MessagingProgramList MessagingProgramList
    @ingroup SubscribableList

    A List element to hold MessagingProgram objects.

    @{ */

/** @struct SE_MessagingProgramList_t se_types.h

    A List element to hold MessagingProgram objects.

    @var SE_MessagingProgramList_t::_flags
    @var SE_MessagingProgramList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_MessagingProgramList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_MessagingProgramList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_MessagingProgramList_t::results
    Indicates the number of items in this page of results.

    @var SE_MessagingProgramList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_MessagingProgramList_t::MessagingProgram
*/

/** @} */

/** @defgroup ConsumptionTariffIntervalListLink ConsumptionTariffIntervalListLink
    @ingroup ListLink

    SHALL contain a Link to a List of ConsumptionTariffInterval instances.

    @{ */

/** @struct SE_ConsumptionTariffIntervalListLink_t se_types.h

    SHALL contain a Link to a List of ConsumptionTariffInterval instances.

    @var SE_ConsumptionTariffIntervalListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_ConsumptionTariffIntervalListLink_t::href
    A URI reference.

    @var SE_ConsumptionTariffIntervalListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup TimeTariffInterval TimeTariffInterval
    @ingroup RandomizableEvent

    Describes the time-differentiated portion of the RateComponent, if applicable, and provides the ability to specify multiple time intervals, each with its own consumption-based components and other attributes.

    @{ */

/** @struct SE_TimeTariffInterval_t se_types.h

    Describes the time-differentiated portion of the RateComponent, if applicable, and provides the ability to specify multiple time intervals, each with its own consumption-based components and other attributes.

    @var SE_TimeTariffInterval_t::_flags
#define SE_responseRequired_exists (1 << 5)

#define SE_randomizeStart_exists (1 << 6)

#define SE_randomizeDuration_exists (1 << 7)

#define SE_ConsumptionTariffIntervalListLink_exists (1 << 8)

    @var SE_TimeTariffInterval_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_TimeTariffInterval_t::replyTo
    A reference to the response resource address (URI). Required on a response to a GET if responseRequired is "true".

    @var SE_TimeTariffInterval_t::responseRequired
    Indicates whether or not a response is required upon receipt, creation or update of this resource. Responses shall be posted to the collection specified in "replyTo". 

    If the resource has a deviceCategory field, devices that match one or more of the device types indicated in deviceCategory SHALL respond according to the rules listed below.  If the category does not match, the device SHALL NOT respond. If the resource does not have a deviceCategory field, a device receiving the resource SHALL respond according to the rules listed below.

    Value encoded as hex according to the following bit assignments, any combination is possible. 

    See Table 10-10 for the list of appropriate Response status codes to be sent for these purposes.

    0 - End device shall indicate that message was received 

    1 - End device shall indicate specific response. 

    2 - End user / customer response is required. 

    All other values reserved.

    @var SE_TimeTariffInterval_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_TimeTariffInterval_t::mRID
    The global identifier of the object.

    @var SE_TimeTariffInterval_t::description
    The description is a human readable text describing or naming the object.

    @var SE_TimeTariffInterval_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_TimeTariffInterval_t::creationTime
    The time at which the Event was created.

    @var SE_TimeTariffInterval_t::EventStatus
    @var SE_TimeTariffInterval_t::interval
    The period during which the Event applies.

    @var SE_TimeTariffInterval_t::randomizeDuration
    Number of seconds boundary inside which a random value must be selected to be applied to the associated interval duration, to avoid sudden synchronized demand changes. If related to price level changes, sign may be ignored. Valid range is -3600 to 3600. If not specified, 0 is the default.

    @var SE_TimeTariffInterval_t::randomizeStart
    Number of seconds boundary inside which a random value must be selected to be applied to the associated interval start time, to avoid sudden synchronized demand changes. If related to price level changes, sign may be ignored. Valid range is -3600 to 3600. If not specified, 0 is the default.

    @var SE_TimeTariffInterval_t::ConsumptionTariffIntervalListLink
    @var SE_TimeTariffInterval_t::touTier
    Indicates the time of use tier related to the reading. If not specified, is assumed to be "0 - N/A".

*/

/** @} */

/** @defgroup TimeTariffIntervalList TimeTariffIntervalList
    @ingroup SubscribableList

    A List element to hold TimeTariffInterval objects.

    @{ */

/** @struct SE_TimeTariffIntervalList_t se_types.h

    A List element to hold TimeTariffInterval objects.

    @var SE_TimeTariffIntervalList_t::_flags
    @var SE_TimeTariffIntervalList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_TimeTariffIntervalList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_TimeTariffIntervalList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_TimeTariffIntervalList_t::results
    Indicates the number of items in this page of results.

    @var SE_TimeTariffIntervalList_t::TimeTariffInterval
*/

/** @} */

/** @defgroup RateComponentListLink RateComponentListLink
    @ingroup ListLink

    SHALL contain a Link to a List of RateComponent instances.

    @{ */

/** @struct SE_RateComponentListLink_t se_types.h

    SHALL contain a Link to a List of RateComponent instances.

    @var SE_RateComponentListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_RateComponentListLink_t::href
    A URI reference.

    @var SE_RateComponentListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup TariffProfile TariffProfile
    @ingroup IdentifiedObject

    A schedule of charges; structure that allows the definition of tariff structures such as step (block) and time of use (tier) when used in conjunction with TimeTariffInterval and ConsumptionTariffInterval.

    @{ */

/** @struct SE_TariffProfile_t se_types.h

    A schedule of charges; structure that allows the definition of tariff structures such as step (block) and time of use (tier) when used in conjunction with TimeTariffInterval and ConsumptionTariffInterval.

    @var SE_TariffProfile_t::_flags
#define SE_rateCode_exists (1 << 5)

#define SE_pricePowerOfTenMultiplier_exists (1 << 6)

#define SE_currency_exists (1 << 7)

    @var SE_TariffProfile_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_TariffProfile_t::mRID
    The global identifier of the object.

    @var SE_TariffProfile_t::description
    The description is a human readable text describing or naming the object.

    @var SE_TariffProfile_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_TariffProfile_t::currency
    The currency code indicating the currency for this TariffProfile.

    @var SE_TariffProfile_t::pricePowerOfTenMultiplier
    Indicates the power of ten multiplier for the price attribute.

    @var SE_TariffProfile_t::primacy
    Indicates the relative primacy of the provider of this program.

    @var SE_TariffProfile_t::rateCode
    The rate code for this tariff profile.  Provided by the Pricing service provider per its internal business needs and practices and provides a method to identify the specific rate code for the TariffProfile instance.  This would typically not be communicated to the user except to facilitate troubleshooting due to its service provider-specific technical nature.

    @var SE_TariffProfile_t::RateComponentListLink
    @var SE_TariffProfile_t::serviceCategoryKind
    The kind of service provided by this usage point.

*/

/** @} */

/** @defgroup TariffProfileList TariffProfileList
    @ingroup SubscribableList

    A List element to hold TariffProfile objects.

    @{ */

/** @struct SE_TariffProfileList_t se_types.h

    A List element to hold TariffProfile objects.

    @var SE_TariffProfileList_t::_flags
    @var SE_TariffProfileList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_TariffProfileList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_TariffProfileList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_TariffProfileList_t::results
    Indicates the number of items in this page of results.

    @var SE_TariffProfileList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_TariffProfileList_t::TariffProfile
*/

/** @} */

/** @defgroup TimeTariffIntervalListLink TimeTariffIntervalListLink
    @ingroup ListLink

    SHALL contain a Link to a List of TimeTariffInterval instances.

    @{ */

/** @struct SE_TimeTariffIntervalListLink_t se_types.h

    SHALL contain a Link to a List of TimeTariffInterval instances.

    @var SE_TimeTariffIntervalListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_TimeTariffIntervalListLink_t::href
    A URI reference.

    @var SE_TimeTariffIntervalListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup ActiveTimeTariffIntervalListLink ActiveTimeTariffIntervalListLink
    @ingroup ListLink

    SHALL contain a Link to a List of active TimeTariffInterval instances.

    @{ */

/** @struct SE_ActiveTimeTariffIntervalListLink_t se_types.h

    SHALL contain a Link to a List of active TimeTariffInterval instances.

    @var SE_ActiveTimeTariffIntervalListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_ActiveTimeTariffIntervalListLink_t::href
    A URI reference.

    @var SE_ActiveTimeTariffIntervalListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup RateComponent RateComponent
    @ingroup IdentifiedObject

    Specifies the applicable charges for a single component of the rate, which could be generation price or consumption price, for example.

    @{ */

/** @struct SE_RateComponent_t se_types.h

    Specifies the applicable charges for a single component of the rate, which could be generation price or consumption price, for example.

    @var SE_RateComponent_t::_flags
#define SE_flowRateStartLimit_exists (1 << 5)

#define SE_flowRateEndLimit_exists (1 << 6)

#define SE_ActiveTimeTariffIntervalListLink_exists (1 << 7)

    @var SE_RateComponent_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_RateComponent_t::mRID
    The global identifier of the object.

    @var SE_RateComponent_t::description
    The description is a human readable text describing or naming the object.

    @var SE_RateComponent_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_RateComponent_t::ActiveTimeTariffIntervalListLink
    @var SE_RateComponent_t::flowRateEndLimit
    Specifies the maximum flow rate (e.g. kW for electricity) for which this RateComponent applies, for the usage point and given rate / tariff. 

    In combination with flowRateStartLimit, allows a service provider to define the demand or output characteristics for the particular tariff design.  If a server includes the flowRateEndLimit attribute, then it SHALL also include flowRateStartLimit attribute.

    For example, a service provider’s tariff limits customers to 20 kWs of demand for the given rate structure.  Above this threshold (from 20-50 kWs), there are different demand charges per unit of consumption.  The service provider can use flowRateStartLimit and flowRateEndLimit to describe the demand characteristics of the different rates.  Similarly, these attributes can be used to describe limits on premises DERs that might be producing a commodity and sending it back into the distribution network. 

    Note: At the time of writing, service provider tariffs with demand-based components were not originally identified as being in scope, and service provider tariffs vary widely in their use of demand components and the method for computing charges.  It is expected that industry groups (e.g., OpenSG) will document requirements in the future that the IEEE 2030.5 community can then use as source material for the next version of IEEE 2030.5.

    @var SE_RateComponent_t::flowRateStartLimit
    Specifies the minimum flow rate (e.g., kW for electricity) for which this RateComponent applies, for the usage point and given rate / tariff.

    In combination with flowRateEndLimit, allows a service provider to define the demand or output characteristics for the particular tariff design.  If a server includes the flowRateStartLimit attribute, then it SHALL also include flowRateEndLimit attribute.

    @var SE_RateComponent_t::ReadingTypeLink
    Provides indication of the ReadingType with which this price is associated.

    @var SE_RateComponent_t::roleFlags
    Specifies the roles that this usage point has been assigned.

    @var SE_RateComponent_t::TimeTariffIntervalListLink
*/

/** @} */

/** @defgroup RateComponentList RateComponentList
    @ingroup List

    A List element to hold RateComponent objects.

    @{ */

/** @struct SE_RateComponentList_t se_types.h

    A List element to hold RateComponent objects.

    @var SE_RateComponentList_t::_flags
    @var SE_RateComponentList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_RateComponentList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_RateComponentList_t::results
    Indicates the number of items in this page of results.

    @var SE_RateComponentList_t::RateComponent
*/

/** @} */

/** @defgroup CostKindType CostKindType
    @ingroup UInt8

    0 - Carbon Dioxide emissions, in grams per unit

    1 - Sulfur Dioxide emissions, in grams per unit

    2 - Nitrogen Oxides emissions, in grams per unit

    3 - Renewable generation, as a percentage of overall generation

    All other values reserved.

    @{ */

/** @typedef SE_CostKindType_t

    0 - Carbon Dioxide emissions, in grams per unit

    1 - Sulfur Dioxide emissions, in grams per unit

    2 - Nitrogen Oxides emissions, in grams per unit

    3 - Renewable generation, as a percentage of overall generation

    All other values reserved.

*/

/** @} */

/** @defgroup EnvironmentalCost EnvironmentalCost
    @ingroup se_types

    Provides alternative or secondary price information for the relevant RateComponent. Supports jurisdictions that seek to convey the environmental price per unit of the specified commodity not expressed in currency.

    Implementers and consumers can use this attribute to prioritize operations of their HAN devices (e.g., PEV charging during times of high availability of renewable electricity resources).

    @{ */

/** @struct SE_EnvironmentalCost_t se_types.h

    Provides alternative or secondary price information for the relevant RateComponent. Supports jurisdictions that seek to convey the environmental price per unit of the specified commodity not expressed in currency.

    Implementers and consumers can use this attribute to prioritize operations of their HAN devices (e.g., PEV charging during times of high availability of renewable electricity resources).

    @var SE_EnvironmentalCost_t::amount
    The estimated or actual environmental or other cost, per commodity unit defined by the ReadingType, for this RateComponent (e.g., grams of carbon dioxide emissions each per kWh).

    @var SE_EnvironmentalCost_t::costKind
    The kind of cost referred to in the amount.

    @var SE_EnvironmentalCost_t::costLevel
    The relative level of the amount attribute.  In conjunction with numCostLevels, this attribute informs a device of the relative scarcity of the amount attribute (e.g., a high or low availability of renewable generation).

    numCostLevels and costLevel values SHALL ascend in order of scarcity, where "0" signals the lowest relative cost and higher values signal increasing cost.  For example, if numCostLevels is equal to “3,” then if the lowest relative costLevel were equal to “0,” devices would assume this is the lowest relative period to operate.  Likewise, if the costLevel in the next TimeTariffInterval instance is equal to “1,” then the device would assume it is relatively more expensive, in environmental terms, to operate during this TimeTariffInterval instance than the previous one.

    There is no limit to the number of relative price levels other than that indicated in the attribute type, but for practicality, service providers should strive for simplicity and recognize the diminishing returns derived from increasing the numCostLevel value greater than four.

    @var SE_EnvironmentalCost_t::numCostLevels
    The number of all relative cost levels. 

    In conjunction with costLevel, numCostLevels signals the relative scarcity of the commodity for the duration of the TimeTariffInterval instance (e.g., a relative indication of cost). This is useful in providing context for nominal cost signals to consumers or devices that might see a range of amount values from different service providres or from the same service provider.

*/

/** @} */

/** @defgroup ConsumptionTariffInterval ConsumptionTariffInterval
    @ingroup Resource

    One of a sequence of thresholds defined in terms of consumption quantity of a service such as electricity, water, gas, etc. It defines the steps or blocks in a step tariff structure, where startValue simultaneously defines the entry value of this step and the closing value of the previous step. Where consumption is greater than startValue, it falls within this block and where consumption is less than or equal to startValue, it falls within one of the previous blocks.

    @{ */

/** @struct SE_ConsumptionTariffInterval_t se_types.h

    One of a sequence of thresholds defined in terms of consumption quantity of a service such as electricity, water, gas, etc. It defines the steps or blocks in a step tariff structure, where startValue simultaneously defines the entry value of this step and the closing value of the previous step. Where consumption is greater than startValue, it falls within this block and where consumption is less than or equal to startValue, it falls within one of the previous blocks.

    @var SE_ConsumptionTariffInterval_t::_flags
#define SE_price_exists (1 << 5)

    @var SE_ConsumptionTariffInterval_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_ConsumptionTariffInterval_t::consumptionBlock
    Indicates the consumption block related to the reading. If not specified, is assumed to be "0 - N/A".

    @var SE_ConsumptionTariffInterval_t::EnvironmentalCost
    @var SE_ConsumptionTariffInterval_t::price
    The charge for this rate component, per unit of measure defined by the associated ReadingType, in currency specified in TariffProfile. 

    The Pricing service provider determines the appropriate price attribute value based on its applicable regulatory rules. For example, price could be net or inclusive of applicable taxes, fees, or levies.

    The Billing function set provides the ability to represent billing information in a more detailed manner.

    @var SE_ConsumptionTariffInterval_t::startValue
    The lowest level of consumption that defines the starting point of this consumption step or block. Thresholds start at zero for each billing period.    

    If specified, the first ConsumptionTariffInterval.startValue for a TimeTariffInteral instance SHALL begin at "0." Subsequent ConsumptionTariffInterval.startValue elements SHALL be greater than the previous one.

*/

/** @} */

/** @defgroup ConsumptionTariffIntervalList ConsumptionTariffIntervalList
    @ingroup List

    A List element to hold ConsumptionTariffInterval objects.

    @{ */

/** @struct SE_ConsumptionTariffIntervalList_t se_types.h

    A List element to hold ConsumptionTariffInterval objects.

    @var SE_ConsumptionTariffIntervalList_t::_flags
    @var SE_ConsumptionTariffIntervalList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_ConsumptionTariffIntervalList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_ConsumptionTariffIntervalList_t::results
    Indicates the number of items in this page of results.

    @var SE_ConsumptionTariffIntervalList_t::ConsumptionTariffInterval
*/

/** @} */

/** @defgroup UsagePointList UsagePointList
    @ingroup SubscribableList

    A List element to hold UsagePoint objects.

    @{ */

/** @struct SE_UsagePointList_t se_types.h

    A List element to hold UsagePoint objects.

    @var SE_UsagePointList_t::_flags
    @var SE_UsagePointList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_UsagePointList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_UsagePointList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_UsagePointList_t::results
    Indicates the number of items in this page of results.

    @var SE_UsagePointList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_UsagePointList_t::UsagePoint
*/

/** @} */

/** @defgroup ReadingListLink ReadingListLink
    @ingroup ListLink

    SHALL contain a Link to a List of Reading instances.

    @{ */

/** @struct SE_ReadingListLink_t se_types.h

    SHALL contain a Link to a List of Reading instances.

    @var SE_ReadingListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_ReadingListLink_t::href
    A URI reference.

    @var SE_ReadingListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup ReadingSet ReadingSet
    @ingroup ReadingSetBase

    A set of Readings of the ReadingType indicated by the parent MeterReading.

    @{ */

/** @struct SE_ReadingSet_t se_types.h

    A set of Readings of the ReadingType indicated by the parent MeterReading.

    @var SE_ReadingSet_t::_flags
#define SE_ReadingListLink_exists (1 << 5)

    @var SE_ReadingSet_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_ReadingSet_t::mRID
    The global identifier of the object.

    @var SE_ReadingSet_t::description
    The description is a human readable text describing or naming the object.

    @var SE_ReadingSet_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_ReadingSet_t::timePeriod
    Specifies the time range during which the contained readings were taken.

    @var SE_ReadingSet_t::ReadingListLink
*/

/** @} */

/** @defgroup ReadingSetList ReadingSetList
    @ingroup SubscribableList

    A List element to hold ReadingSet objects.

    @{ */

/** @struct SE_ReadingSetList_t se_types.h

    A List element to hold ReadingSet objects.

    @var SE_ReadingSetList_t::_flags
    @var SE_ReadingSetList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_ReadingSetList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_ReadingSetList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_ReadingSetList_t::results
    Indicates the number of items in this page of results.

    @var SE_ReadingSetList_t::ReadingSet
*/

/** @} */

/** @defgroup ReadingList ReadingList
    @ingroup SubscribableList

    A List element to hold Reading objects.

    @{ */

/** @struct SE_ReadingList_t se_types.h

    A List element to hold Reading objects.

    @var SE_ReadingList_t::_flags
    @var SE_ReadingList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_ReadingList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_ReadingList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_ReadingList_t::results
    Indicates the number of items in this page of results.

    @var SE_ReadingList_t::Reading
*/

/** @} */

/** @defgroup ReadingSetListLink ReadingSetListLink
    @ingroup ListLink

    SHALL contain a Link to a List of ReadingSet instances.

    @{ */

/** @struct SE_ReadingSetListLink_t se_types.h

    SHALL contain a Link to a List of ReadingSet instances.

    @var SE_ReadingSetListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_ReadingSetListLink_t::href
    A URI reference.

    @var SE_ReadingSetListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup ReadingLink ReadingLink
    @ingroup Link

    A Link to a Reading.

    @{ */

/** @struct SE_ReadingLink_t se_types.h

    A Link to a Reading.

    @var SE_ReadingLink_t::_flags
    @var SE_ReadingLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup MeterReading MeterReading
    @ingroup MeterReadingBase

    Set of values obtained from the meter.

    @{ */

/** @struct SE_MeterReading_t se_types.h

    Set of values obtained from the meter.

    @var SE_MeterReading_t::_flags
#define SE_ReadingSetListLink_exists (1 << 5)

#define SE_ReadingLink_exists (1 << 6)

    @var SE_MeterReading_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_MeterReading_t::mRID
    The global identifier of the object.

    @var SE_MeterReading_t::description
    The description is a human readable text describing or naming the object.

    @var SE_MeterReading_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_MeterReading_t::RateComponentListLink
    @var SE_MeterReading_t::ReadingLink
    @var SE_MeterReading_t::ReadingSetListLink
    @var SE_MeterReading_t::ReadingTypeLink
*/

/** @} */

/** @defgroup MeterReadingList MeterReadingList
    @ingroup SubscribableList

    A List element to hold MeterReading objects.

    @{ */

/** @struct SE_MeterReadingList_t se_types.h

    A List element to hold MeterReading objects.

    @var SE_MeterReadingList_t::_flags
    @var SE_MeterReadingList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_MeterReadingList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_MeterReadingList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_MeterReadingList_t::results
    Indicates the number of items in this page of results.

    @var SE_MeterReadingList_t::MeterReading
*/

/** @} */

/** @defgroup UnitType UnitType
    @ingroup UInt8

    The unit types defined for end device control target reductions.

    0 - kWh 

    1 - kW  

    2 - Watts 

    3 - Cubic Meters 

    4 - Cubic Feet 

    5 - US Gallons 

    6 - Imperial Gallons 

    7 - BTUs 

    8 - Liters 

    9 - kPA (gauge) 

    10 - kPA (absolute)  

    11 - Mega Joule 

    12 - Unitless

    All other values reserved.

    @{ */

/** @typedef SE_UnitType_t

    The unit types defined for end device control target reductions.

    0 - kWh 

    1 - kW  

    2 - Watts 

    3 - Cubic Meters 

    4 - Cubic Feet 

    5 - US Gallons 

    6 - Imperial Gallons 

    7 - BTUs 

    8 - Liters 

    9 - kPA (gauge) 

    10 - kPA (absolute)  

    11 - Mega Joule 

    12 - Unitless

    All other values reserved.

*/

/** @} */

/** @defgroup TargetReduction TargetReduction
    @ingroup se_types

    The TargetReduction object is used by a Demand Response service provider to provide a RECOMMENDED threshold that a device/premises should maintain its consumption below. For example, a service provider can provide a RECOMMENDED threshold of some kWh for a 3-hour event. This means that the device/premises would maintain its consumption below the specified limit for the specified period.

    @{ */

/** @struct SE_TargetReduction_t se_types.h

    The TargetReduction object is used by a Demand Response service provider to provide a RECOMMENDED threshold that a device/premises should maintain its consumption below. For example, a service provider can provide a RECOMMENDED threshold of some kWh for a 3-hour event. This means that the device/premises would maintain its consumption below the specified limit for the specified period.

    @var SE_TargetReduction_t::type
    Indicates the type of reduction requested.

    @var SE_TargetReduction_t::value
    Indicates the requested amount of the relevant commodity to be reduced.

*/

/** @} */

/** @defgroup SetPoint SetPoint
    @ingroup se_types

    The SetPoint object is used to apply specific temperature set points to a temperature control device. The values of the heatingSetpoint and coolingSetpoint attributes SHALL be calculated as follows:

    Cooling/Heating Temperature Set Point / 100 = temperature in degrees Celsius where -273.15°C &lt;= temperature &lt;= 327.67°C, corresponding to a Cooling and/or Heating Temperature Set Point. The maximum resolution this format allows is 0.01°C. 

    The field not present in a Response indicates that this field has not been used by the end device. 

    If a temperature is sent that exceeds the temperature limit boundaries that are programmed into the device, the device SHALL respond by setting the temperature at the limit.

    @{ */

/** @struct SE_SetPoint_t se_types.h

    The SetPoint object is used to apply specific temperature set points to a temperature control device. The values of the heatingSetpoint and coolingSetpoint attributes SHALL be calculated as follows:

    Cooling/Heating Temperature Set Point / 100 = temperature in degrees Celsius where -273.15°C &lt;= temperature &lt;= 327.67°C, corresponding to a Cooling and/or Heating Temperature Set Point. The maximum resolution this format allows is 0.01°C. 

    The field not present in a Response indicates that this field has not been used by the end device. 

    If a temperature is sent that exceeds the temperature limit boundaries that are programmed into the device, the device SHALL respond by setting the temperature at the limit.

    @var SE_SetPoint_t::_flags
#define SE_heatingSetpoint_exists (1 << 0)

#define SE_coolingSetpoint_exists (1 << 1)

    @var SE_SetPoint_t::coolingSetpoint
    This attribute represents the cooling temperature set point in degrees Celsius / 100. (Hundredths of a degree C)

    @var SE_SetPoint_t::heatingSetpoint
    This attribute represents the heating temperature set point in degrees Celsius / 100. (Hundredths of a degree C)

*/

/** @} */

/** @defgroup Offset Offset
    @ingroup se_types

    If a temperature offset is sent that causes the heating or cooling temperature set point to exceed the limit boundaries that are programmed into the device, the device SHALL respond by setting the temperature at the limit.

    If an EDC is being targeted at multiple devices or to a device that controls multiple devices (e.g., EMS), it can provide multiple Offset types within one EDC. For events with multiple Offset types, a client SHALL select the Offset that best fits their operating function. 

    Alternatively, an event with a single Offset type can be targeted at an EMS in order to request a percentage load reduction on the average energy usage of the entire premise. An EMS SHOULD use the Metering function set to determine the initial load in the premise, reduce energy consumption by controlling devices at its disposal, and at the conclusion of the event, once again use the Metering function set to determine if the desired load reduction was achieved.

    @{ */

/** @struct SE_Offset_t se_types.h

    If a temperature offset is sent that causes the heating or cooling temperature set point to exceed the limit boundaries that are programmed into the device, the device SHALL respond by setting the temperature at the limit.

    If an EDC is being targeted at multiple devices or to a device that controls multiple devices (e.g., EMS), it can provide multiple Offset types within one EDC. For events with multiple Offset types, a client SHALL select the Offset that best fits their operating function. 

    Alternatively, an event with a single Offset type can be targeted at an EMS in order to request a percentage load reduction on the average energy usage of the entire premise. An EMS SHOULD use the Metering function set to determine the initial load in the premise, reduce energy consumption by controlling devices at its disposal, and at the conclusion of the event, once again use the Metering function set to determine if the desired load reduction was achieved.

    @var SE_Offset_t::_flags
#define SE_loadAdjustmentPercentageOffset_exists (1 << 0)

#define SE_heatingOffset_exists (1 << 1)

#define SE_coolingOffset_exists (1 << 2)

    @var SE_Offset_t::coolingOffset
    The value change requested for the cooling offset, in degree C / 10. The value should be added to the normal set point for cooling, or if loadShiftForward is true, then the value should be subtracted from the normal set point.

    @var SE_Offset_t::heatingOffset
    The value change requested for the heating offset, in degree C / 10. The value should be subtracted for heating, or if loadShiftForward is true, then the value should be added to the normal set point.

    @var SE_Offset_t::loadAdjustmentPercentageOffset
    The value change requested for the load adjustment percentage. The value should be subtracted from the normal setting, or if loadShiftForward is true, then the value should be added to the normal setting.

*/

/** @} */

/** @defgroup DutyCycle DutyCycle
    @ingroup se_types

    Duty cycle control is a device specific issue and is managed by the device.  The duty cycle of the device under control should span the shortest practical time period in accordance with the nature of the device under control and the intent of the request for demand reduction.  The default factory setting SHOULD be three minutes for each 10% of duty cycle.  This indicates that the default time period over which a duty cycle is applied is 30 minutes, meaning a 10% duty cycle would cause a device to be ON for 3 minutes.   The “off state” SHALL precede the “on state”.

    @{ */

/** @struct SE_DutyCycle_t se_types.h

    Duty cycle control is a device specific issue and is managed by the device.  The duty cycle of the device under control should span the shortest practical time period in accordance with the nature of the device under control and the intent of the request for demand reduction.  The default factory setting SHOULD be three minutes for each 10% of duty cycle.  This indicates that the default time period over which a duty cycle is applied is 30 minutes, meaning a 10% duty cycle would cause a device to be ON for 3 minutes.   The “off state” SHALL precede the “on state”.

    @var SE_DutyCycle_t::normalValue
    Contains the maximum On state duty cycle applied by the end device, as a percentage of time.  The field not present indicates that this field has not been used by the end device.

*/

/** @} */

/** @defgroup DeviceCategoryType DeviceCategoryType
    @ingroup HexBinary32

    The Device category types defined.

    Bit positions SHALL be defined as follows:

    0 - Programmable Communicating Thermostat 

    1 - Strip Heaters 

    2 - Baseboard Heaters 

    3 - Water Heater 

    4 - Pool Pump 

    5 - Sauna 

    6 - Hot tub

    7 - Smart Appliance 

    8 - Irrigation Pump 

    9 - Managed Commercial and Industrial (C&amp;I) Loads 

    10 - Simple misc. (Residential On/Off) loads 

    11 - Exterior Lighting 

    12 - Interior Lighting 

    13 - Electric Vehicle 

    14 - Generation Systems 

    15 - Load Control Switch 

    16 - Smart Inverter 

    17 - EVSE 

    18 - RESU (Residential Energy Storage Unit)

    19 - Energy Management System 

    20 - Smart Energy Module

    All other values reserved.

    @{ */

/** @typedef SE_DeviceCategoryType_t

    The Device category types defined.

    Bit positions SHALL be defined as follows:

    0 - Programmable Communicating Thermostat 

    1 - Strip Heaters 

    2 - Baseboard Heaters 

    3 - Water Heater 

    4 - Pool Pump 

    5 - Sauna 

    6 - Hot tub

    7 - Smart Appliance 

    8 - Irrigation Pump 

    9 - Managed Commercial and Industrial (C&amp;I) Loads 

    10 - Simple misc. (Residential On/Off) loads 

    11 - Exterior Lighting 

    12 - Interior Lighting 

    13 - Electric Vehicle 

    14 - Generation Systems 

    15 - Load Control Switch 

    16 - Smart Inverter 

    17 - EVSE 

    18 - RESU (Residential Energy Storage Unit)

    19 - Energy Management System 

    20 - Smart Energy Module

    All other values reserved.

*/

/** @} */

/** @defgroup ApplianceLoadReductionType ApplianceLoadReductionType
    @ingroup UInt8

    0 - Delay Appliance Load

    Parameter requesting the appliance to respond by providing a moderate load reduction for the duration of a delay period.  Typically referring to a “non-emergency” event in which appliances can continue operating if already in a load consuming period.

    1 - Temporary Appliance Load Reduction

    Parameter requesting the appliance to respond by providing an aggressive load reduction for a short time period.  Typically referring to an “emergency/spinning reserve” event in which an appliance should start shedding load if currently in a load consuming period. 

    * Full definition of how appliances react when receiving each parameter is document in the EPA document - ENERGY STAR® Program Requirements, Product Specification for Residential Refrigerators and Freezers, Eligibility Criteria 5, Draft 2 Version 5.0.

    All other values reserved.

    @{ */

/** @typedef SE_ApplianceLoadReductionType_t

    0 - Delay Appliance Load

    Parameter requesting the appliance to respond by providing a moderate load reduction for the duration of a delay period.  Typically referring to a “non-emergency” event in which appliances can continue operating if already in a load consuming period.

    1 - Temporary Appliance Load Reduction

    Parameter requesting the appliance to respond by providing an aggressive load reduction for a short time period.  Typically referring to an “emergency/spinning reserve” event in which an appliance should start shedding load if currently in a load consuming period. 

    * Full definition of how appliances react when receiving each parameter is document in the EPA document - ENERGY STAR® Program Requirements, Product Specification for Residential Refrigerators and Freezers, Eligibility Criteria 5, Draft 2 Version 5.0.

    All other values reserved.

*/

/** @} */

/** @defgroup ApplianceLoadReduction ApplianceLoadReduction
    @ingroup se_types

    The ApplianceLoadReduction object is used by a Demand Response service provider to provide signals for ENERGY STAR compliant appliances. See the definition of ApplianceLoadReductionType for more information.

    @{ */

/** @struct SE_ApplianceLoadReduction_t se_types.h

    The ApplianceLoadReduction object is used by a Demand Response service provider to provide signals for ENERGY STAR compliant appliances. See the definition of ApplianceLoadReductionType for more information.

    @var SE_ApplianceLoadReduction_t::type
    Indicates the type of appliance load reduction requested.

*/

/** @} */

/** @defgroup EndDeviceControl EndDeviceControl
    @ingroup RandomizableEvent

    Instructs an EndDevice to perform a specified action.

    @{ */

/** @struct SE_EndDeviceControl_t se_types.h

    Instructs an EndDevice to perform a specified action.

    @var SE_EndDeviceControl_t::_flags
#define SE_responseRequired_exists (1 << 5)

#define SE_randomizeStart_exists (1 << 6)

#define SE_randomizeDuration_exists (1 << 7)

#define SE_TargetReduction_exists (1 << 8)

#define SE_loadShiftForward_true (1 << 9)

#define SE_drProgramMandatory_true (1 << 10)

    @var SE_EndDeviceControl_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_EndDeviceControl_t::replyTo
    A reference to the response resource address (URI). Required on a response to a GET if responseRequired is "true".

    @var SE_EndDeviceControl_t::responseRequired
    Indicates whether or not a response is required upon receipt, creation or update of this resource. Responses shall be posted to the collection specified in "replyTo". 

    If the resource has a deviceCategory field, devices that match one or more of the device types indicated in deviceCategory SHALL respond according to the rules listed below.  If the category does not match, the device SHALL NOT respond. If the resource does not have a deviceCategory field, a device receiving the resource SHALL respond according to the rules listed below.

    Value encoded as hex according to the following bit assignments, any combination is possible. 

    See Table 10-10 for the list of appropriate Response status codes to be sent for these purposes.

    0 - End device shall indicate that message was received 

    1 - End device shall indicate specific response. 

    2 - End user / customer response is required. 

    All other values reserved.

    @var SE_EndDeviceControl_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_EndDeviceControl_t::mRID
    The global identifier of the object.

    @var SE_EndDeviceControl_t::description
    The description is a human readable text describing or naming the object.

    @var SE_EndDeviceControl_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_EndDeviceControl_t::creationTime
    The time at which the Event was created.

    @var SE_EndDeviceControl_t::EventStatus
    @var SE_EndDeviceControl_t::interval
    The period during which the Event applies.

    @var SE_EndDeviceControl_t::randomizeDuration
    Number of seconds boundary inside which a random value must be selected to be applied to the associated interval duration, to avoid sudden synchronized demand changes. If related to price level changes, sign may be ignored. Valid range is -3600 to 3600. If not specified, 0 is the default.

    @var SE_EndDeviceControl_t::randomizeStart
    Number of seconds boundary inside which a random value must be selected to be applied to the associated interval start time, to avoid sudden synchronized demand changes. If related to price level changes, sign may be ignored. Valid range is -3600 to 3600. If not specified, 0 is the default.

    @var SE_EndDeviceControl_t::ApplianceLoadReduction
    @var SE_EndDeviceControl_t::deviceCategory
    Specifies the bitmap indicating  the categories of devices that SHOULD respond. Devices SHOULD ignore events that do not indicate their device category.

    @var SE_EndDeviceControl_t::DutyCycle
    @var SE_EndDeviceControl_t::Offset
    @var SE_EndDeviceControl_t::overrideDuration
    The overrideDuration attribute provides a duration, in seconds, for which a client device is allowed to override this EndDeviceControl and still meet the contractual agreement with a service provider without opting out. If overrideDuration is not specified, then it SHALL default to 0.

    @var SE_EndDeviceControl_t::SetPoint
    @var SE_EndDeviceControl_t::TargetReduction
*/

/** @} */

/** @defgroup EndDeviceControlList EndDeviceControlList
    @ingroup SubscribableList

    A List element to hold EndDeviceControl objects.

    @{ */

/** @struct SE_EndDeviceControlList_t se_types.h

    A List element to hold EndDeviceControl objects.

    @var SE_EndDeviceControlList_t::_flags
    @var SE_EndDeviceControlList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_EndDeviceControlList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_EndDeviceControlList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_EndDeviceControlList_t::results
    Indicates the number of items in this page of results.

    @var SE_EndDeviceControlList_t::EndDeviceControl
*/

/** @} */

/** @defgroup EndDeviceControlListLink EndDeviceControlListLink
    @ingroup ListLink

    SHALL contain a Link to a List of EndDeviceControl instances.

    @{ */

/** @struct SE_EndDeviceControlListLink_t se_types.h

    SHALL contain a Link to a List of EndDeviceControl instances.

    @var SE_EndDeviceControlListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_EndDeviceControlListLink_t::href
    A URI reference.

    @var SE_EndDeviceControlListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup ActiveEndDeviceControlListLink ActiveEndDeviceControlListLink
    @ingroup ListLink

    SHALL contain a Link to a List of active EndDeviceControl instances.

    @{ */

/** @struct SE_ActiveEndDeviceControlListLink_t se_types.h

    SHALL contain a Link to a List of active EndDeviceControl instances.

    @var SE_ActiveEndDeviceControlListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_ActiveEndDeviceControlListLink_t::href
    A URI reference.

    @var SE_ActiveEndDeviceControlListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup DemandResponseProgram DemandResponseProgram
    @ingroup IdentifiedObject

    Demand response program.

    @{ */

/** @struct SE_DemandResponseProgram_t se_types.h

    Demand response program.

    @var SE_DemandResponseProgram_t::_flags
#define SE_EndDeviceControlListLink_exists (1 << 5)

#define SE_availabilityUpdatePowerChangeThreshold_exists (1 << 6)

#define SE_availabilityUpdatePercentChangeThreshold_exists (1 << 7)

#define SE_ActiveEndDeviceControlListLink_exists (1 << 8)

    @var SE_DemandResponseProgram_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DemandResponseProgram_t::mRID
    The global identifier of the object.

    @var SE_DemandResponseProgram_t::description
    The description is a human readable text describing or naming the object.

    @var SE_DemandResponseProgram_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_DemandResponseProgram_t::ActiveEndDeviceControlListLink
    @var SE_DemandResponseProgram_t::availabilityUpdatePercentChangeThreshold
    This attribute allows program providers to specify the requested granularity of updates to LoadShedAvailability sheddablePercent. If not present, or set to 0, then updates to LoadShedAvailability SHALL NOT be provided. If present and greater than zero, then clients SHALL provide their LoadShedAvailability if it has not previously been provided, and thereafter if the difference between the previously provided value and the current value of LoadShedAvailability sheddablePercent is greater than availabilityUpdatePercentChangeThreshold.

    @var SE_DemandResponseProgram_t::availabilityUpdatePowerChangeThreshold
    This attribute allows program providers to specify the requested granularity of updates to LoadShedAvailability sheddablePower. If not present, or set to 0, then updates to LoadShedAvailability SHALL NOT be provided. If present and greater than zero, then clients SHALL provide their LoadShedAvailability if it has not previously been provided, and thereafter if the difference between the previously provided value and the current value of LoadShedAvailability sheddablePower is greater than availabilityUpdatePowerChangeThreshold.

    @var SE_DemandResponseProgram_t::EndDeviceControlListLink
    @var SE_DemandResponseProgram_t::primacy
    Indicates the relative primacy of the provider of this program.

*/

/** @} */

/** @defgroup DemandResponseProgramList DemandResponseProgramList
    @ingroup SubscribableList

    A List element to hold DemandResponseProgram objects.

    @{ */

/** @struct SE_DemandResponseProgramList_t se_types.h

    A List element to hold DemandResponseProgram objects.

    @var SE_DemandResponseProgramList_t::_flags
    @var SE_DemandResponseProgramList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DemandResponseProgramList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_DemandResponseProgramList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_DemandResponseProgramList_t::results
    Indicates the number of items in this page of results.

    @var SE_DemandResponseProgramList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_DemandResponseProgramList_t::DemandResponseProgram
*/

/** @} */

/** @defgroup DemandResponseProgramLink DemandResponseProgramLink
    @ingroup Link

    SHALL contain a Link to an instance of DemandResponseProgram.

    @{ */

/** @struct SE_DemandResponseProgramLink_t se_types.h

    SHALL contain a Link to an instance of DemandResponseProgram.

    @var SE_DemandResponseProgramLink_t::_flags
    @var SE_DemandResponseProgramLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup LoadShedAvailability LoadShedAvailability
    @ingroup Resource

    Indicates current consumption status and ability to shed load.

    @{ */

/** @struct SE_LoadShedAvailability_t se_types.h

    Indicates current consumption status and ability to shed load.

    @var SE_LoadShedAvailability_t::_flags
#define SE_sheddablePower_exists (1 << 5)

#define SE_sheddablePercent_exists (1 << 6)

#define SE_DemandResponseProgramLink_exists (1 << 7)

    @var SE_LoadShedAvailability_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_LoadShedAvailability_t::availabilityDuration
    Indicates for how many seconds the consuming device will be able to reduce consumption at the maximum response level.

    @var SE_LoadShedAvailability_t::DemandResponseProgramLink
    @var SE_LoadShedAvailability_t::sheddablePercent
    Maximum percent of current operating load that is estimated to be sheddable.

    @var SE_LoadShedAvailability_t::sheddablePower
    Maximum amount of current operating load that is estimated to be sheddable, in Watts.

*/

/** @} */

/** @defgroup LoadShedAvailabilityList LoadShedAvailabilityList
    @ingroup List

    A List element to hold LoadShedAvailability objects.

    @{ */

/** @struct SE_LoadShedAvailabilityList_t se_types.h

    A List element to hold LoadShedAvailability objects.

    @var SE_LoadShedAvailabilityList_t::_flags
    @var SE_LoadShedAvailabilityList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_LoadShedAvailabilityList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_LoadShedAvailabilityList_t::results
    Indicates the number of items in this page of results.

    @var SE_LoadShedAvailabilityList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_LoadShedAvailabilityList_t::LoadShedAvailability
*/

/** @} */

/** @defgroup FileLink FileLink
    @ingroup Link

    This element MUST be set to the URI of the most recent File being loaded/activated by the LD. In the case of file status 0, this element MUST be omitted.

    @{ */

/** @struct SE_FileLink_t se_types.h

    This element MUST be set to the URI of the most recent File being loaded/activated by the LD. In the case of file status 0, this element MUST be omitted.

    @var SE_FileLink_t::_flags
    @var SE_FileLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup FileStatus FileStatus
    @ingroup Resource

    This object provides status of device file load and activation operations.

    @{ */

/** @struct SE_FileStatus_t se_types.h

    This object provides status of device file load and activation operations.

    @var SE_FileStatus_t::_flags
#define SE_FileLink_exists (1 << 5)

    @var SE_FileStatus_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_FileStatus_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_FileStatus_t::activateTime
    Date/time at which this File, referred to by FileLink, will be activated. Omission of or presence and value of this element MUST exactly match omission or presence and value of the activateTime element from the File resource.

    @var SE_FileStatus_t::FileLink
    @var SE_FileStatus_t::loadPercent
    This element MUST be set to the percentage of the file, indicated by FileLink, that was loaded during the latest load attempt. This value MUST be reset to 0 each time a load attempt is started for the File indicated by FileLink. This value MUST be increased when an LD receives HTTP response containing file content. This value MUST be set to 100 when the full content of the file has been received by the LD

    @var SE_FileStatus_t::nextRequestAttempt
    This element MUST be set to the time at which the LD will issue its next GET request for file content from the File indicated by FileLink

    @var SE_FileStatus_t::request503Count
    This value MUST be reset to 0 when FileLink is first pointed at a new File. This value MUST be incremented each time an 

    LD receives a 503 error from the FS.

    @var SE_FileStatus_t::requestFailCount
    This value MUST be reset to 0 when FileLink is first pointed at a new File. This value MUST be incremented each time a GET request for file content failed. 503 errors MUST be excluded from this counter.

    @var SE_FileStatus_t::status
    Current loading status of the file indicated by FileLink. This element MUST be set to one of the following values:

    0 - No load operation in progress

    1 - File load in progress (first request for file content has been issued by LD)

    2 - File load failed

    3 - File loaded successfully (full content of file has been received by the LD), signature verification in progress

    4 - File signature verification failed

    5 - File signature verified, waiting to activate file.

    6 - File activation failed

    7 - File activation in progress  

    8 - File activated successfully (this state may not be reached/persisted through an image activation)

    9-255 - Reserved for future use.

    @var SE_FileStatus_t::statusTime
    This element MUST be set to the time at which file status transitioned to the value indicated in the status element.

*/

/** @} */

/** @defgroup String16 String16
    @ingroup se_typedefs

    Character string of max length 16. In order to limit internal storage, implementations SHALL reduce the length of strings using multi-byte characters so that the string may be stored using "maxLength" octets in the given encoding.

    @{ */

/** @typedef SE_String16_t

    Character string of max length 16. In order to limit internal storage, implementations SHALL reduce the length of strings using multi-byte characters so that the string may be stored using "maxLength" octets in the given encoding.

*/

/** @} */

/** @defgroup PENType PENType
    @ingroup UInt32

    IANA Private Enterprise Number [PEN].

    @{ */

/** @typedef SE_PENType_t

    IANA Private Enterprise Number [PEN].

*/

/** @} */

/** @defgroup File File
    @ingroup Resource

    This resource contains various meta-data describing a file's characteristics.  The meta-data provides general file information and also is used to support filtered queries of file lists

    @{ */

/** @struct SE_File_t se_types.h

    This resource contains various meta-data describing a file's characteristics.  The meta-data provides general file information and also is used to support filtered queries of file lists

    @var SE_File_t::_flags
#define SE_mfSerNum_exists (1 << 5)

#define SE_mfHwVer_exists (1 << 6)

    @var SE_File_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_File_t::activateTime
    This element MUST be set to the date/time at which this file is activated. If the activation time is less than or equal to current time, the LD MUST immediately place the file into the activated state (in the case of a firmware file, the file is now the running image).  If the activation time is greater than the current time, the LD MUST wait until the specified activation time is reached, then MUST place the file into the activated state. Omission of this element means that the LD MUST NOT take any action to activate the file until a subsequent GET to this File resource provides an activateTime.

    @var SE_File_t::fileURI
    This element MUST be set to the URI location of the file binary artifact.  This is the BLOB (binary large object) that is actually loaded by the LD

    @var SE_File_t::lFDI
    This element MUST be set to the LFDI of the device for which this file in targeted.

    @var SE_File_t::mfHwVer
    This element MUST be set to the hardware version for which this file is targeted.

    @var SE_File_t::mfID
    This element MUST be set to the manufacturer's Private Enterprise Number (assigned by IANA).

    @var SE_File_t::mfModel
    This element MUST be set to the manufacturer model number for which this file is targeted. The syntax and semantics are left to the manufacturer.

    @var SE_File_t::mfSerNum
    This element MUST be set to the manufacturer serial number for which this file is targeted. The syntax and semantics are left to the manufacturer.

    @var SE_File_t::mfVer
    This element MUST be set to the software version information for this file. The syntax and semantics are left to the manufacturer.

    @var SE_File_t::size
    This element MUST be set to the total size (in bytes) of the file referenced by fileURI.

    @var SE_File_t::type
    A value indicating the type of the file.  MUST be one of the following values:

    00 = Software Image

    01 = Security Credential

    02 = Configuration

    03 = Log

    04–7FFF = reserved

    8000-FFFF = Manufacturer defined

*/

/** @} */

/** @defgroup FileList FileList
    @ingroup List

    A List element to hold File objects.

    @{ */

/** @struct SE_FileList_t se_types.h

    A List element to hold File objects.

    @var SE_FileList_t::_flags
    @var SE_FileList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_FileList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_FileList_t::results
    Indicates the number of items in this page of results.

    @var SE_FileList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_FileList_t::File
*/

/** @} */

/** @defgroup RateComponentLink RateComponentLink
    @ingroup Link

    SHALL contain a Link to an instance of RateComponent.

    @{ */

/** @struct SE_RateComponentLink_t se_types.h

    SHALL contain a Link to an instance of RateComponent.

    @var SE_RateComponentLink_t::_flags
    @var SE_RateComponentLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup PriceResponseCfg PriceResponseCfg
    @ingroup Resource

    Configuration data that specifies how price responsive devices SHOULD respond to price changes while acting upon a given RateComponent.

    @{ */

/** @struct SE_PriceResponseCfg_t se_types.h

    Configuration data that specifies how price responsive devices SHOULD respond to price changes while acting upon a given RateComponent.

    @var SE_PriceResponseCfg_t::_flags
    @var SE_PriceResponseCfg_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_PriceResponseCfg_t::consumeThreshold
    Price responsive clients acting upon the associated RateComponent SHOULD consume the associated commodity while the price is less than this threshold.

    @var SE_PriceResponseCfg_t::maxReductionThreshold
    Price responsive clients acting upon the associated RateComponent SHOULD reduce consumption to the maximum extent possible while the price is greater than this threshold.

    @var SE_PriceResponseCfg_t::RateComponentLink
*/

/** @} */

/** @defgroup PriceResponseCfgList PriceResponseCfgList
    @ingroup List

    A List element to hold PriceResponseCfg objects.

    @{ */

/** @struct SE_PriceResponseCfgList_t se_types.h

    A List element to hold PriceResponseCfg objects.

    @var SE_PriceResponseCfgList_t::_flags
    @var SE_PriceResponseCfgList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_PriceResponseCfgList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_PriceResponseCfgList_t::results
    Indicates the number of items in this page of results.

    @var SE_PriceResponseCfgList_t::PriceResponseCfg
*/

/** @} */

/** @defgroup TimeOffsetType TimeOffsetType
    @ingroup Int32

    A signed time offset, typically applied to a Time value, expressed in seconds.

    @{ */

/** @typedef SE_TimeOffsetType_t

    A signed time offset, typically applied to a Time value, expressed in seconds.

*/

/** @} */

/** @defgroup DstRuleType DstRuleType
    @ingroup HexBinary32

    Bit map encoded rule from which is calculated the start or end time, within the current year, to which daylight savings time offset must be applied. 

    The rule encoding:

    Bits  0 - 11: seconds 0 - 3599

    Bits 12 - 16: hours 0 - 23

    Bits 17 - 19: day of the week 0 = not applicable, 1 - 7 (Monday = 1)

    Bits 20 - 24: day of the month 0 = not applicable, 1 - 31

    Bits 25 - 27: operator  (detailed below)

    Bits 28 - 31: month 1 - 12

    Rule value of 0xFFFFFFFF means rule processing/DST correction is disabled.

    The operators:

    0: DST starts/ends on the Day of the Month

    1: DST starts/ends on the Day of the Week that is on or after the Day of the Month

    2: DST starts/ends on the first occurrence of the Day of the Week in a month

    3: DST starts/ends on the second occurrence of the Day of the Week in a month

    4: DST starts/ends on the third occurrence of the Day of the Week in a month

    5: DST starts/ends on the forth occurrence of the Day of the Week in a month

    6: DST starts/ends on the fifth occurrence of the Day of the Week in a month

    7: DST starts/ends on the last occurrence of the Day of the Week in a month

    An example: DST starts on third Friday in March at 1:45 AM.  The rule...

    Seconds: 2700

    Hours: 1

    Day of Week: 5

    Day of Month: 0

    Operator: 4

    Month: 3

    @{ */

/** @typedef SE_DstRuleType_t

    Bit map encoded rule from which is calculated the start or end time, within the current year, to which daylight savings time offset must be applied. 

    The rule encoding:

    Bits  0 - 11: seconds 0 - 3599

    Bits 12 - 16: hours 0 - 23

    Bits 17 - 19: day of the week 0 = not applicable, 1 - 7 (Monday = 1)

    Bits 20 - 24: day of the month 0 = not applicable, 1 - 31

    Bits 25 - 27: operator  (detailed below)

    Bits 28 - 31: month 1 - 12

    Rule value of 0xFFFFFFFF means rule processing/DST correction is disabled.

    The operators:

    0: DST starts/ends on the Day of the Month

    1: DST starts/ends on the Day of the Week that is on or after the Day of the Month

    2: DST starts/ends on the first occurrence of the Day of the Week in a month

    3: DST starts/ends on the second occurrence of the Day of the Week in a month

    4: DST starts/ends on the third occurrence of the Day of the Week in a month

    5: DST starts/ends on the forth occurrence of the Day of the Week in a month

    6: DST starts/ends on the fifth occurrence of the Day of the Week in a month

    7: DST starts/ends on the last occurrence of the Day of the Week in a month

    An example: DST starts on third Friday in March at 1:45 AM.  The rule...

    Seconds: 2700

    Hours: 1

    Day of Week: 5

    Day of Month: 0

    Operator: 4

    Month: 3

*/

/** @} */

/** @defgroup TimeConfiguration TimeConfiguration
    @ingroup se_types

    Contains attributes related to the configuration of the time service.

    @{ */

/** @struct SE_TimeConfiguration_t se_types.h

    Contains attributes related to the configuration of the time service.

    @var SE_TimeConfiguration_t::dstEndRule
    Rule to calculate end of daylight savings time in the current year.  Result of dstEndRule must be greater than result of dstStartRule.

    @var SE_TimeConfiguration_t::dstOffset
    Daylight savings time offset from local standard time.

    @var SE_TimeConfiguration_t::dstStartRule
    Rule to calculate start of daylight savings time in the current year. Result of dstEndRule must be greater than result of dstStartRule.

    @var SE_TimeConfiguration_t::tzOffset
    Local time zone offset from UTCTime. Does not include any daylight savings time offsets.

*/

/** @} */

/** @defgroup PriceResponseCfgListLink PriceResponseCfgListLink
    @ingroup ListLink

    SHALL contain a Link to a List of PriceResponseCfg instances.

    @{ */

/** @struct SE_PriceResponseCfgListLink_t se_types.h

    SHALL contain a Link to a List of PriceResponseCfg instances.

    @var SE_PriceResponseCfgListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_PriceResponseCfgListLink_t::href
    A URI reference.

    @var SE_PriceResponseCfgListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup PowerConfiguration PowerConfiguration
    @ingroup se_types

    Contains configuration related to the device's power sources

    @{ */

/** @struct SE_PowerConfiguration_t se_types.h

    Contains configuration related to the device's power sources

    @var SE_PowerConfiguration_t::_flags
#define SE_lowChargeThreshold_exists (1 << 0)

#define SE_batteryInstallTime_exists (1 << 1)

    @var SE_PowerConfiguration_t::batteryInstallTime
    Time/Date at which battery was installed,

    @var SE_PowerConfiguration_t::lowChargeThreshold
    In context of the PowerStatus resource, this is the value of EstimatedTimeRemaining below which BatteryStatus "low" is indicated and the LE_LOW_BATTERY is raised.

*/

/** @} */

/** @defgroup Configuration Configuration
    @ingroup SubscribableResource

    This resource contains various settings to control the operation of the device

    @{ */

/** @struct SE_Configuration_t se_types.h

    This resource contains various settings to control the operation of the device

    @var SE_Configuration_t::_flags
#define SE_TimeConfiguration_exists (1 << 5)

#define SE_PriceResponseCfgListLink_exists (1 << 6)

#define SE_PowerConfiguration_exists (1 << 7)

    @var SE_Configuration_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_Configuration_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_Configuration_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_Configuration_t::currentLocale
    [RFC 4646] identifier of the language-region currently in use.

    @var SE_Configuration_t::PowerConfiguration
    @var SE_Configuration_t::PriceResponseCfgListLink
    @var SE_Configuration_t::TimeConfiguration
    @var SE_Configuration_t::userDeviceName
    User assigned, convenience name used for network browsing displays, etc.  Example "My Thermostat"

*/

/** @} */

/** @defgroup LogEvent LogEvent
    @ingroup Resource

    A time stamped instance of a significant event detected by the device.

    @{ */

/** @struct SE_LogEvent_t se_types.h

    A time stamped instance of a significant event detected by the device.

    @var SE_LogEvent_t::_flags
#define SE_extendedData_exists (1 << 5)

#define SE_details_exists (1 << 6)

    @var SE_LogEvent_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_LogEvent_t::createdDateTime
    The date and time that the event occurred.

    @var SE_LogEvent_t::details
    Human readable text that MAY be used to transmit additional details about the event. A host MAY remove this field when received.

    @var SE_LogEvent_t::extendedData
    May be used to transmit additional details about the event.

    @var SE_LogEvent_t::functionSet
    If the profileID indicates this is IEEE 2030.5, the functionSet is defined by IEEE 2030.5 and SHALL be one of the values from the table below (IEEE 2030.5 function set identifiers). If the profileID is anything else, the functionSet is defined by the identified profile. 

    0	General (not specific to a function set)

    1	Publish and Subscribe

    2	End Device

    3	Function Set Assignment

    4	Response

    5	Demand Response and Load Control

    6	Metering

    7	Pricing

    8	Messaging

    9	Billing

    10	Prepayment

    11	Distributed Energy Resources

    12	Time

    13	Software  Download

    14	Device Information

    15	Power Status

    16	Network Status

    17	Log Event List

    18	Configuration

    19	Security

    All other values are reserved.

    @var SE_LogEvent_t::logEventCode
    An 8 bit unsigned integer. logEventCodes are scoped to a profile and a function set. If the profile is IEEE 2030.5, the logEventCode is defined by IEEE 2030.5 within one of the function sets of IEEE 2030.5. If the profile is anything else, the logEventCode is defined by the specified profile.

    @var SE_LogEvent_t::logEventID
    This 16-bit value, combined with createdDateTime, profileID, and logEventPEN, should provide a reasonable level of uniqueness.

    @var SE_LogEvent_t::logEventPEN
    The Private Enterprise Number(PEN) of the entity that defined the profileID, functionSet, and logEventCode of the logEvent. ZigBee-assigned logEventCodes SHALL use the ZigBee Alliance PEN.  Combinations of profileID, functionSet, and logEventCode SHALL have unique meaning within a logEventPEN and are defined by the owner of the PEN.

    @var SE_LogEvent_t::profileID
    The profileID identifies which profile (HA, BA, SE, etc) defines the following event information.

    0	Not profile specific.

    1	Vendor Defined

    2	IEEE 2030.5

    3	Home Automation

    4	Building Automation

    All other values are reserved.

*/

/** @} */

/** @defgroup LogEventList LogEventList
    @ingroup SubscribableList

    A List element to hold LogEvent objects.

    @{ */

/** @struct SE_LogEventList_t se_types.h

    A List element to hold LogEvent objects.

    @var SE_LogEventList_t::_flags
    @var SE_LogEventList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_LogEventList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_LogEventList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_LogEventList_t::results
    Indicates the number of items in this page of results.

    @var SE_LogEventList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_LogEventList_t::LogEvent
*/

/** @} */

/** @defgroup RPLSourceRoutes RPLSourceRoutes
    @ingroup Resource

    A RPL source routes object.

    @{ */

/** @struct SE_RPLSourceRoutes_t se_types.h

    A RPL source routes object.

    @var SE_RPLSourceRoutes_t::_flags
    @var SE_RPLSourceRoutes_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_RPLSourceRoutes_t::DestAddress
    See [RFC 6554].

    @var SE_RPLSourceRoutes_t::SourceRoute
    See [RFC 6554].

*/

/** @} */

/** @defgroup RPLSourceRoutesList RPLSourceRoutesList
    @ingroup List

    List or RPL source routes if the hosting device is the DODAGroot

    @{ */

/** @struct SE_RPLSourceRoutesList_t se_types.h

    List or RPL source routes if the hosting device is the DODAGroot

    @var SE_RPLSourceRoutesList_t::_flags
    @var SE_RPLSourceRoutesList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_RPLSourceRoutesList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_RPLSourceRoutesList_t::results
    Indicates the number of items in this page of results.

    @var SE_RPLSourceRoutesList_t::RPLSourceRoutes
*/

/** @} */

/** @defgroup RPLSourceRoutesListLink RPLSourceRoutesListLink
    @ingroup ListLink

    SHALL contain a Link to a List of RPLSourceRoutes instances.

    @{ */

/** @struct SE_RPLSourceRoutesListLink_t se_types.h

    SHALL contain a Link to a List of RPLSourceRoutes instances.

    @var SE_RPLSourceRoutesListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_RPLSourceRoutesListLink_t::href
    A URI reference.

    @var SE_RPLSourceRoutesListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup RPLInstance RPLInstance
    @ingroup Resource

    Specific RPLInstance resource.  This resource may be thought of as network status information for a specific RPL instance associated with IPInterface.

    @{ */

/** @struct SE_RPLInstance_t se_types.h

    Specific RPLInstance resource.  This resource may be thought of as network status information for a specific RPL instance associated with IPInterface.

    @var SE_RPLInstance_t::_flags
#define SE_RPLSourceRoutesListLink_exists (1 << 5)

#define SE_groundedFlag_true (1 << 6)

#define SE_DODAGroot_true (1 << 7)

    @var SE_RPLInstance_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_RPLInstance_t::DODAGid
    See [RFC 6550].

    @var SE_RPLInstance_t::flags
    See [RFC 6550].

    @var SE_RPLInstance_t::MOP
    See [RFC 6550].

    @var SE_RPLInstance_t::PRF
    See [RFC 6550].

    @var SE_RPLInstance_t::rank
    See [RFC 6550].

    @var SE_RPLInstance_t::RPLInstanceID
    See [RFC 6550].

    @var SE_RPLInstance_t::RPLSourceRoutesListLink
    @var SE_RPLInstance_t::versionNumber
    See [RFC 6550].

*/

/** @} */

/** @defgroup RPLInstanceList RPLInstanceList
    @ingroup List

    List of RPLInstances associated with the IPinterface.

    @{ */

/** @struct SE_RPLInstanceList_t se_types.h

    List of RPLInstances associated with the IPinterface.

    @var SE_RPLInstanceList_t::_flags
    @var SE_RPLInstanceList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_RPLInstanceList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_RPLInstanceList_t::results
    Indicates the number of items in this page of results.

    @var SE_RPLInstanceList_t::RPLInstance
*/

/** @} */

/** @defgroup Neighbor Neighbor
    @ingroup Resource

    Contains 802.15.4 link layer specific attributes.

    @{ */

/** @struct SE_Neighbor_t se_types.h

    Contains 802.15.4 link layer specific attributes.

    @var SE_Neighbor_t::_flags
#define SE_isChild_true (1 << 5)

    @var SE_Neighbor_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_Neighbor_t::linkQuality
    The quality of the link, as defined by 802.15.4

    @var SE_Neighbor_t::shortAddress
    As defined by IEEE 802.15.4

*/

/** @} */

/** @defgroup NeighborList NeighborList
    @ingroup List

    List of 15.4 neighbors.

    @{ */

/** @struct SE_NeighborList_t se_types.h

    List of 15.4 neighbors.

    @var SE_NeighborList_t::_flags
    @var SE_NeighborList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_NeighborList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_NeighborList_t::results
    Indicates the number of items in this page of results.

    @var SE_NeighborList_t::Neighbor
*/

/** @} */

/** @defgroup loWPAN loWPAN
    @ingroup se_types

    Contains information specific to 6LoWPAN.

    @{ */

/** @struct SE_loWPAN_t se_types.h

    Contains information specific to 6LoWPAN.

    @var SE_loWPAN_t::_flags
#define SE_octetsTx_exists (1 << 0)

#define SE_octetsRx_exists (1 << 1)

    @var SE_loWPAN_t::octetsRx
    Number of Bytes received

    @var SE_loWPAN_t::octetsTx
    Number of Bytes transmitted

    @var SE_loWPAN_t::packetsRx
    Number of packets received

    @var SE_loWPAN_t::packetsTx
    Number of packets transmitted

    @var SE_loWPAN_t::rxFragError
    Number of errors receiving fragments

*/

/** @} */

/** @defgroup NeighborListLink NeighborListLink
    @ingroup ListLink

    SHALL contain a Link to a List of Neighbor instances.

    @{ */

/** @struct SE_NeighborListLink_t se_types.h

    SHALL contain a Link to a List of Neighbor instances.

    @var SE_NeighborListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_NeighborListLink_t::href
    A URI reference.

    @var SE_NeighborListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup IEEE_802_15_4 IEEE_802_15_4
    @ingroup se_types

    Contains 802.15.4 link layer specific attributes.

    @{ */

/** @struct SE_IEEE_802_15_4_t se_types.h

    Contains 802.15.4 link layer specific attributes.

    @var SE_IEEE_802_15_4_t::_flags
#define SE_NeighborListLink_exists (1 << 0)

    @var SE_IEEE_802_15_4_t::capabilityInfo
    As defined by IEEE 802.15.4

    @var SE_IEEE_802_15_4_t::NeighborListLink
    @var SE_IEEE_802_15_4_t::shortAddress
    As defined by IEEE 802.15.4

*/

/** @} */

/** @defgroup HexBinary64 HexBinary64
    @ingroup se_typedefs

    A 64-bit field encoded as a hex string (16 hex characters max). Where applicable, bit 0, or the least significant bit, goes on the right. Note that hexBinary requires pairs of hex characters, so an odd number of characters requires a leading "0".

    @{ */

/** @typedef SE_HexBinary64_t

    A 64-bit field encoded as a hex string (16 hex characters max). Where applicable, bit 0, or the least significant bit, goes on the right. Note that hexBinary requires pairs of hex characters, so an odd number of characters requires a leading "0".

*/

/** @} */

/** @defgroup LLInterface LLInterface
    @ingroup Resource

    A link-layer interface object.

    @{ */

/** @struct SE_LLInterface_t se_types.h

    A link-layer interface object.

    @var SE_LLInterface_t::_flags
#define SE_loWPAN_exists (1 << 5)

#define SE_LLSecurityErrorRx_exists (1 << 6)

#define SE_LLRetryCount_exists (1 << 7)

#define SE_LLOctetsTx_exists (1 << 8)

#define SE_LLOctetsRx_exists (1 << 9)

#define SE_LLMediaAccessFail_exists (1 << 10)

#define SE_LLFramesTx_exists (1 << 11)

#define SE_LLFramesRx_exists (1 << 12)

#define SE_LLFramesDropTx_exists (1 << 13)

#define SE_LLFramesDropRx_exists (1 << 14)

#define SE_LLCSMAFail_exists (1 << 15)

#define SE_LLAckNotRx_exists (1 << 16)

#define SE_IEEE_802_15_4_exists (1 << 17)

    @var SE_LLInterface_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_LLInterface_t::CRCerrors
    Contains the number of CRC errors since reset.

    @var SE_LLInterface_t::EUI64
    Contains the EUI-64 of the link layer interface. 48 bit MAC addresses SHALL be changed into an EUI-64 using the method defined in [RFC 4291], Appendix A. (The method is to insert "0xFFFE" as described in the reference.)

    @var SE_LLInterface_t::IEEE_802_15_4
    @var SE_LLInterface_t::linkLayerType
    Specifies the type of link layer interface associated with the IPInterface. Values are below.

    0 = Unspecified 

    1 = IEEE 802.3 (Ethernet)

    2 = IEEE 802.11 (WLAN)

    3 = IEEE 802.15 (PAN)

    4 = IEEE 1901 (PLC)

    All other values reserved.

    @var SE_LLInterface_t::LLAckNotRx
    Number of times an ACK was not received for a frame transmitted (when ACK was requested).

    @var SE_LLInterface_t::LLCSMAFail
    Number of times CSMA failed.

    @var SE_LLInterface_t::LLFramesDropRx
    Number of dropped receive frames.

    @var SE_LLInterface_t::LLFramesDropTx
    Number of dropped transmit frames.

    @var SE_LLInterface_t::LLFramesRx
    Number of link layer frames received.

    @var SE_LLInterface_t::LLFramesTx
    Number of link layer frames transmitted.

    @var SE_LLInterface_t::LLMediaAccessFail
    Number of times access to media failed.

    @var SE_LLInterface_t::LLOctetsRx
    Number of Bytes received.

    @var SE_LLInterface_t::LLOctetsTx
    Number of Bytes transmitted.

    @var SE_LLInterface_t::LLRetryCount
    Number of MAC transmit retries.

    @var SE_LLInterface_t::LLSecurityErrorRx
    Number of receive security errors.

    @var SE_LLInterface_t::loWPAN
*/

/** @} */

/** @defgroup LLInterfaceList LLInterfaceList
    @ingroup List

    List of LLInterface instances.

    @{ */

/** @struct SE_LLInterfaceList_t se_types.h

    List of LLInterface instances.

    @var SE_LLInterfaceList_t::_flags
    @var SE_LLInterfaceList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_LLInterfaceList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_LLInterfaceList_t::results
    Indicates the number of items in this page of results.

    @var SE_LLInterfaceList_t::LLInterface
*/

/** @} */

/** @defgroup LLInterfaceListLink LLInterfaceListLink
    @ingroup ListLink

    SHALL contain a Link to a List of LLInterface instances.

    @{ */

/** @struct SE_LLInterfaceListLink_t se_types.h

    SHALL contain a Link to a List of LLInterface instances.

    @var SE_LLInterfaceListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_LLInterfaceListLink_t::href
    A URI reference.

    @var SE_LLInterfaceListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup IPAddrListLink IPAddrListLink
    @ingroup ListLink

    SHALL contain a Link to a List of IPAddr instances.

    @{ */

/** @struct SE_IPAddrListLink_t se_types.h

    SHALL contain a Link to a List of IPAddr instances.

    @var SE_IPAddrListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_IPAddrListLink_t::href
    A URI reference.

    @var SE_IPAddrListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup IPInterface IPInterface
    @ingroup Resource

    Specific IPInterface resource.  This resource may be thought of as network status information for a specific network (IP) layer interface.

    @{ */

/** @struct SE_IPInterface_t se_types.h

    Specific IPInterface resource.  This resource may be thought of as network status information for a specific network (IP) layer interface.

    @var SE_IPInterface_t::_flags
#define SE_LLInterfaceListLink_exists (1 << 5)

#define SE_lastUpdatedTime_exists (1 << 6)

#define SE_lastResetTime_exists (1 << 7)

#define SE_IPAddrListLink_exists (1 << 8)

#define SE_ifType_exists (1 << 9)

#define SE_ifSpeed_exists (1 << 10)

#define SE_ifPromiscuousMode_true (1 << 11)
#define SE_ifPromiscuousMode_exists (1 << 12)

#define SE_ifOutUcastPkts_exists (1 << 13)

#define SE_ifOutOctets_exists (1 << 14)

#define SE_ifOutMulticastPkts_exists (1 << 15)

#define SE_ifOutErrors_exists (1 << 16)

#define SE_ifOutDiscards_exists (1 << 17)

#define SE_ifOutBroadcastPkts_exists (1 << 18)

#define SE_ifOperStatus_exists (1 << 19)

#define SE_ifName_exists (1 << 20)

#define SE_ifMtu_exists (1 << 21)

#define SE_ifInUnknownProtos_exists (1 << 22)

#define SE_ifInUcastPkts_exists (1 << 23)

#define SE_ifInOctets_exists (1 << 24)

#define SE_ifInMulticastPkts_exists (1 << 25)

#define SE_ifInErrors_exists (1 << 26)

#define SE_ifInDiscards_exists (1 << 27)

#define SE_ifIndex_exists (1 << 28)

#define SE_ifInBroadcastPkts_exists (1 << 29)

#define SE_ifHighSpeed_exists (1 << 30)

#define SE_ifDescr_exists (1 << 31)

    @var SE_IPInterface_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_IPInterface_t::ifDescr
    Use rules from [RFC 2863].

    @var SE_IPInterface_t::ifHighSpeed
    Use rules from [RFC 2863].

    @var SE_IPInterface_t::ifInBroadcastPkts
    Use rules from [RFC 2863].

    @var SE_IPInterface_t::ifIndex
    Use rules from [RFC 2863].

    @var SE_IPInterface_t::ifInDiscards
    Use rules from [RFC 2863]. Can be thought of as Input Datagrams Discarded.

    @var SE_IPInterface_t::ifInErrors
    Use rules from [RFC 2863].

    @var SE_IPInterface_t::ifInMulticastPkts
    Use rules from [RFC 2863]. Can be thought of as Multicast Datagrams Received.

    @var SE_IPInterface_t::ifInOctets
    Use rules from [RFC 2863]. Can be thought of as Bytes Received.

    @var SE_IPInterface_t::ifInUcastPkts
    Use rules from [RFC 2863]. Can be thought of as Datagrams Received.

    @var SE_IPInterface_t::ifInUnknownProtos
    Use rules from [RFC 2863]. Can be thought of as Datagrams with Unknown Protocol Received.

    @var SE_IPInterface_t::ifMtu
    Use rules from [RFC 2863].

    @var SE_IPInterface_t::ifName
    Use rules from [RFC 2863].

    @var SE_IPInterface_t::ifOperStatus
    Use rules and assignments from [RFC 2863].

    @var SE_IPInterface_t::ifOutBroadcastPkts
    Use rules from [RFC 2863]. Can be thought of as Broadcast Datagrams Sent.

    @var SE_IPInterface_t::ifOutDiscards
    Use rules from [RFC 2863]. Can be thought of as Output Datagrams Discarded.

    @var SE_IPInterface_t::ifOutErrors
    Use rules from [RFC 2863].

    @var SE_IPInterface_t::ifOutMulticastPkts
    Use rules from [RFC 2863]. Can be thought of as Multicast Datagrams Sent.

    @var SE_IPInterface_t::ifOutOctets
    Use rules from [RFC 2863]. Can be thought of as Bytes Sent.

    @var SE_IPInterface_t::ifOutUcastPkts
    Use rules from [RFC 2863]. Can be thought of as Datagrams Sent.

    @var SE_IPInterface_t::ifSpeed
    Use rules from [RFC 2863].

    @var SE_IPInterface_t::ifType
    Use rules and assignments from [RFC 2863].

    @var SE_IPInterface_t::IPAddrListLink
    @var SE_IPInterface_t::lastResetTime
    Similar to ifLastChange in [RFC 2863].

    @var SE_IPInterface_t::lastUpdatedTime
    The date/time of the reported status.

    @var SE_IPInterface_t::LLInterfaceListLink
*/

/** @} */

/** @defgroup IPInterfaceList IPInterfaceList
    @ingroup List

    List of IPInterface instances.

    @{ */

/** @struct SE_IPInterfaceList_t se_types.h

    List of IPInterface instances.

    @var SE_IPInterfaceList_t::_flags
    @var SE_IPInterfaceList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_IPInterfaceList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_IPInterfaceList_t::results
    Indicates the number of items in this page of results.

    @var SE_IPInterfaceList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_IPInterfaceList_t::IPInterface
*/

/** @} */

/** @defgroup RPLInstanceListLink RPLInstanceListLink
    @ingroup ListLink

    SHALL contain a Link to a List of RPLInterface instances.

    @{ */

/** @struct SE_RPLInstanceListLink_t se_types.h

    SHALL contain a Link to a List of RPLInterface instances.

    @var SE_RPLInstanceListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_RPLInstanceListLink_t::href
    A URI reference.

    @var SE_RPLInstanceListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup IPAddr IPAddr
    @ingroup Resource

    An Internet Protocol address object.

    @{ */

/** @struct SE_IPAddr_t se_types.h

    An Internet Protocol address object.

    @var SE_IPAddr_t::_flags
#define SE_RPLInstanceListLink_exists (1 << 5)

    @var SE_IPAddr_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_IPAddr_t::address
    An IP address value.

    @var SE_IPAddr_t::RPLInstanceListLink
*/

/** @} */

/** @defgroup IPAddrList IPAddrList
    @ingroup List

    List of IPAddr instances.

    @{ */

/** @struct SE_IPAddrList_t se_types.h

    List of IPAddr instances.

    @var SE_IPAddrList_t::_flags
    @var SE_IPAddrList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_IPAddrList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_IPAddrList_t::results
    Indicates the number of items in this page of results.

    @var SE_IPAddrList_t::IPAddr
*/

/** @} */

/** @defgroup PEVInfo PEVInfo
    @ingroup se_types

    Contains attributes that can be exposed by PEVs and other devices that have charging requirements.

    @{ */

/** @struct SE_PEVInfo_t se_types.h

    Contains attributes that can be exposed by PEVs and other devices that have charging requirements.

    @var SE_PEVInfo_t::chargingPowerNow
    This is the actual power flow in or out of the charger or inverter. This is calculated by the vehicle based on actual measurements. This number is positive for charging.

    @var SE_PEVInfo_t::energyRequestNow
    This is the amount of energy that must be transferred from the grid to EVSE and PEV to achieve the target state of charge allowing for charger efficiency and any vehicle and EVSE parasitic loads. This is calculated by the vehicle and changes throughout the connection as forward or reverse power flow change the battery state of charge.  This number is positive for charging.

    @var SE_PEVInfo_t::maxForwardPower
    This is maximum power transfer capability that could be used for charging the PEV to perform the requested energy transfer.  It is the lower of the vehicle or EVSE physical power limitations. It is not based on economic considerations. The vehicle may draw less power than this value based on its charging cycle. The vehicle defines this parameter. This number is positive for charging power flow.

    @var SE_PEVInfo_t::minimumChargingDuration
    This is computed by the PEV based on the charging profile to complete the energy transfer if the maximum power is authorized.  The value will never be smaller than the ratio of the energy request to the power request because the charging profile may not allow the maximum power to be used throughout the transfer.   This is a critical parameter for determining whether any slack time exists in the charging cycle between the current time and the TCIN.

    @var SE_PEVInfo_t::targetStateOfCharge
    This is the target state of charge that is to be achieved during charging before the time of departure (TCIN).  The default value is 100%. The value cannot be set to a value less than the actual state of charge.

    @var SE_PEVInfo_t::timeChargeIsNeeded
    Time Charge is Needed (TCIN) is the time that the PEV is expected to depart. The value is manually entered using controls and displays in the vehicle or on the EVSE or using a mobile device.  It is authenticated and saved by the PEV.  This value may be updated during a charging session.

    @var SE_PEVInfo_t::timeChargingStatusPEV
    This is the time that the parameters are updated, except for changes to TCIN.

*/

/** @} */

/** @defgroup PowerSourceType PowerSourceType
    @ingroup UInt8

    0 - none

    1 - mains 

    2 - battery

    3 - local generation

    4 - emergency 

    5 - unknown

    All other values reserved.

    @{ */

/** @typedef SE_PowerSourceType_t

    0 - none

    1 - mains 

    2 - battery

    3 - local generation

    4 - emergency 

    5 - unknown

    All other values reserved.

*/

/** @} */

/** @defgroup PowerStatus PowerStatus
    @ingroup Resource

    Contains the status of the device's power sources

    @{ */

/** @struct SE_PowerStatus_t se_types.h

    Contains the status of the device's power sources

    @var SE_PowerStatus_t::_flags
#define SE_totalTimeOnBattery_exists (1 << 5)

#define SE_sessionTimeOnBattery_exists (1 << 6)

#define SE_PEVInfo_exists (1 << 7)

#define SE_estimatedTimeRemaining_exists (1 << 8)

#define SE_estimatedChargeRemaining_exists (1 << 9)

    @var SE_PowerStatus_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_PowerStatus_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_PowerStatus_t::batteryStatus
    Battery system status

    0 = unknown

    1 = normal (more than LowChargeThreshold remaining)

    2 = low (less than LowChargeThreshold remaining) 

    3 = depleted (0% charge remaining)

    4 = not applicable (mains powered only)

    @var SE_PowerStatus_t::changedTime
    The time at which the reported values were recorded.

    @var SE_PowerStatus_t::currentPowerSource
    This value will be fixed for devices powered by a single source.  This value may change for devices able to transition between multiple power sources (mains to battery backup, etc.).

    @var SE_PowerStatus_t::estimatedChargeRemaining
    Estimate of remaining battery charge as a percent of full charge.

    @var SE_PowerStatus_t::estimatedTimeRemaining
    Estimated time (in seconds) to total battery charge depletion (under current load)

    @var SE_PowerStatus_t::PEVInfo
    @var SE_PowerStatus_t::sessionTimeOnBattery
    If the device has a battery, this is the time since the device last switched to battery power, or the time since the device was restarted, whichever is less, in seconds.

    @var SE_PowerStatus_t::totalTimeOnBattery
    If the device has a battery, this is the total time the device has been on battery power, in seconds. It may be reset when the battery is replaced.

*/

/** @} */

/** @defgroup SupportedLocale SupportedLocale
    @ingroup Resource

    Specifies a locale that is supported

    @{ */

/** @struct SE_SupportedLocale_t se_types.h

    Specifies a locale that is supported

    @var SE_SupportedLocale_t::_flags
    @var SE_SupportedLocale_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_SupportedLocale_t::locale
    The code for a locale that is supported

*/

/** @} */

/** @defgroup SupportedLocaleList SupportedLocaleList
    @ingroup List

    A List element to hold SupportedLocale objects.

    @{ */

/** @struct SE_SupportedLocaleList_t se_types.h

    A List element to hold SupportedLocale objects.

    @var SE_SupportedLocaleList_t::_flags
    @var SE_SupportedLocaleList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_SupportedLocaleList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_SupportedLocaleList_t::results
    Indicates the number of items in this page of results.

    @var SE_SupportedLocaleList_t::SupportedLocale
*/

/** @} */

/** @defgroup SupportedLocaleListLink SupportedLocaleListLink
    @ingroup ListLink

    SHALL contain a Link to a List of SupportedLocale instances.

    @{ */

/** @struct SE_SupportedLocaleListLink_t se_types.h

    SHALL contain a Link to a List of SupportedLocale instances.

    @var SE_SupportedLocaleListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_SupportedLocaleListLink_t::href
    A URI reference.

    @var SE_SupportedLocaleListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup DRLCCapabilities DRLCCapabilities
    @ingroup se_types

    Contains information about the static capabilities of the device, to allow service providers to know what types of functions are supported, what the normal operating ranges and limits are, and other similar information, in order to provide better suggestions of applicable programs to receive the maximum benefit.

    @{ */

/** @struct SE_DRLCCapabilities_t se_types.h

    Contains information about the static capabilities of the device, to allow service providers to know what types of functions are supported, what the normal operating ranges and limits are, and other similar information, in order to provide better suggestions of applicable programs to receive the maximum benefit.

    @var SE_DRLCCapabilities_t::averageEnergy
    The average hourly energy usage when in normal operating mode.

    @var SE_DRLCCapabilities_t::maxDemand
    The maximum demand rating of this end device.

    @var SE_DRLCCapabilities_t::optionsImplemented
    Bitmap indicating the DRLC options implemented by the device.

    0 - Target reduction (kWh)

    1 - Target reduction (kW)

    2 - Target reduction (Watts)

    3 - Target reduction (Cubic Meters)

    4 - Target reduction (Cubic Feet)

    5 - Target reduction (US Gallons)

    6 - Target reduction (Imperial Gallons)

    7 - Target reduction (BTUs)

    8 - Target reduction (Liters)

    9 - Target reduction (kPA (gauge))

    10 - Target reduction (kPA (absolute))

    11 - Target reduction (Mega Joule)

    12 - Target reduction (Unitless)

    13-15 - Reserved

    16 - Temperature set point

    17 - Temperature offset

    18 - Duty cycle

    19 - Load adjustment percentage

    20 - Appliance load reduction

    21-32 - Reserved

*/

/** @} */

/** @defgroup DeviceInformation DeviceInformation
    @ingroup Resource

    Contains identification and other information about the device that changes very infrequently, typically only when updates are applied, if ever.

    @{ */

/** @struct SE_DeviceInformation_t se_types.h

    Contains identification and other information about the device that changes very infrequently, typically only when updates are applied, if ever.

    @var SE_DeviceInformation_t::_flags
#define SE_SupportedLocaleListLink_exists (1 << 5)

#define SE_mfInfo_exists (1 << 6)

#define SE_functionsImplemented_exists (1 << 7)

#define SE_DRLCCapabilities_exists (1 << 8)

    @var SE_DeviceInformation_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DeviceInformation_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_DeviceInformation_t::DRLCCapabilities
    @var SE_DeviceInformation_t::functionsImplemented
    Bitmap indicating the function sets used by the device as a client.

    0 - Device Capability

    1 - Self Device Resource

    2 - End Device Resource

    3 - Function Set Assignments

    4 - Subscription/Notification Mechanism

    5 - Response

    6 - Time

    7 - Device Information

    8 - Power Status

    9 - Network Status

    10 - Log Event

    11 - Configuration Resource

    12 - Software Download

    13 - DRLC

    14 - Metering

    15 - Pricing

    16 - Messaging

    17 - Billing

    18 - Prepayment

    19 - Flow Reservation

    20 - DER Control

    @var SE_DeviceInformation_t::lFDI
    Long form device identifier. See the Security section for full details.

    @var SE_DeviceInformation_t::mfDate
    Date/time of manufacture

    @var SE_DeviceInformation_t::mfHwVer
    Manufacturer hardware version

    @var SE_DeviceInformation_t::mfID
    The manufacturer's IANA Enterprise Number.

    @var SE_DeviceInformation_t::mfInfo
    Manufacturer dependent information related to the manufacture of this device

    @var SE_DeviceInformation_t::mfModel
    Manufacturer's model number

    @var SE_DeviceInformation_t::mfSerNum
    Manufacturer assigned serial number

    @var SE_DeviceInformation_t::primaryPower
    Primary source of power.

    @var SE_DeviceInformation_t::secondaryPower
    Secondary source of power

    @var SE_DeviceInformation_t::SupportedLocaleListLink
    @var SE_DeviceInformation_t::swActTime
    Activation date/time of currently running software

    @var SE_DeviceInformation_t::swVer
    Currently running software version

*/

/** @} */

/** @defgroup Time Time
    @ingroup Resource

    Contains the representation of time, constantly updated.

    @{ */

/** @struct SE_Time_t se_types.h

    Contains the representation of time, constantly updated.

    @var SE_Time_t::_flags
#define SE_localTime_exists (1 << 5)

    @var SE_Time_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_Time_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_Time_t::currentTime
    The current time, in the format defined by TimeType.

    @var SE_Time_t::dstEndTime
    Time at which daylight savings ends (dstOffset no longer applied).  Result of dstEndRule calculation.

    @var SE_Time_t::dstOffset
    Daylight savings time offset from local standard time. A typical practice is advancing clocks one hour when daylight savings time is in effect, which would result in a positive dstOffset.

    @var SE_Time_t::dstStartTime
    Time at which daylight savings begins (apply dstOffset).  Result of dstStartRule calculation.

    @var SE_Time_t::localTime
    Local time: localTime = currentTime + tzOffset (+ dstOffset when in effect).

    @var SE_Time_t::quality
    Metric indicating the quality of the time source from which the service acquired time. Lower (smaller) quality enumeration values are assumed to be more accurate.

    3 - time obtained from external authoritative source such as NTP

    4 - time obtained from level 3 source

    5 - time manually set or obtained from level 4 source

    6 - time obtained from level 5 source

    7 - time intentionally uncoordinated

    All other values are reserved for future use.

    @var SE_Time_t::tzOffset
    Local time zone offset from currentTime. Does not include any daylight savings time offsets. For American time zones, a negative tzOffset SHALL be used (eg, EST = GMT-5 which is -18000).

*/

/** @} */

/** @defgroup Response Response
    @ingroup Resource

    The Response object is the generic response data repository which is extended for specific function sets.

    @{ */

/** @struct SE_Response_t se_types.h

    The Response object is the generic response data repository which is extended for specific function sets.

    @var SE_Response_t::_flags
#define SE_status_exists (1 << 5)

#define SE_createdDateTime_exists (1 << 6)

    @var SE_Response_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_Response_t::createdDateTime
    The createdDateTime field contains the date and time when the acknowledgement/status occurred in the client. The client will provide the timestamp to ensure the proper time is captured in case the response is delayed in reaching the server (server receipt time would not be the same as the actual confirmation time). The time reported from the client should be relative to the time server indicated by the FunctionSetAssignment that also indicated the event resource; if no FunctionSetAssignment exists, the time of the server where the event resource was hosted.

    @var SE_Response_t::endDeviceLFDI
    Contains the LFDI of the device providing the response.

    @var SE_Response_t::status
    The status field contains the acknowledgement or status. Each event type (DRLC, DER, Price, or Text) can return different status information (e.g. an Acknowledge will be returned for a Price event where a DRLC event can return Event Received, Event Started, and Event Completed). The Status field value definitions are defined in Table 27: Response Types by Function Set.

    @var SE_Response_t::subject
    The subject field provides a method to match the response with the originating event. It is populated with the mRID of the original object.

*/

/** @} */

/** @defgroup TextResponse TextResponse
    @ingroup Response

    A response to a text message

    @{ */

/** @struct SE_TextResponse_t se_types.h

    A response to a text message

    @var SE_TextResponse_t::_flags
#define SE_status_exists (1 << 5)

#define SE_createdDateTime_exists (1 << 6)

    @var SE_TextResponse_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_TextResponse_t::createdDateTime
    The createdDateTime field contains the date and time when the acknowledgement/status occurred in the client. The client will provide the timestamp to ensure the proper time is captured in case the response is delayed in reaching the server (server receipt time would not be the same as the actual confirmation time). The time reported from the client should be relative to the time server indicated by the FunctionSetAssignment that also indicated the event resource; if no FunctionSetAssignment exists, the time of the server where the event resource was hosted.

    @var SE_TextResponse_t::endDeviceLFDI
    Contains the LFDI of the device providing the response.

    @var SE_TextResponse_t::status
    The status field contains the acknowledgement or status. Each event type (DRLC, DER, Price, or Text) can return different status information (e.g. an Acknowledge will be returned for a Price event where a DRLC event can return Event Received, Event Started, and Event Completed). The Status field value definitions are defined in Table 27: Response Types by Function Set.

    @var SE_TextResponse_t::subject
    The subject field provides a method to match the response with the originating event. It is populated with the mRID of the original object.

*/

/** @} */

/** @defgroup ResponseListLink ResponseListLink
    @ingroup ListLink

    SHALL contain a Link to a List of Response instances.

    @{ */

/** @struct SE_ResponseListLink_t se_types.h

    SHALL contain a Link to a List of Response instances.

    @var SE_ResponseListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_ResponseListLink_t::href
    A URI reference.

    @var SE_ResponseListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup ResponseSet ResponseSet
    @ingroup IdentifiedObject

    A container for a ResponseList.

    @{ */

/** @struct SE_ResponseSet_t se_types.h

    A container for a ResponseList.

    @var SE_ResponseSet_t::_flags
#define SE_ResponseListLink_exists (1 << 5)

    @var SE_ResponseSet_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_ResponseSet_t::mRID
    The global identifier of the object.

    @var SE_ResponseSet_t::description
    The description is a human readable text describing or naming the object.

    @var SE_ResponseSet_t::version
    Contains the version number of the object. See the type definition for details.

    @var SE_ResponseSet_t::ResponseListLink
*/

/** @} */

/** @defgroup ResponseSetList ResponseSetList
    @ingroup List

    A List element to hold ResponseSet objects.

    @{ */

/** @struct SE_ResponseSetList_t se_types.h

    A List element to hold ResponseSet objects.

    @var SE_ResponseSetList_t::_flags
    @var SE_ResponseSetList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_ResponseSetList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_ResponseSetList_t::results
    Indicates the number of items in this page of results.

    @var SE_ResponseSetList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_ResponseSetList_t::ResponseSet
*/

/** @} */

/** @defgroup ResponseList ResponseList
    @ingroup List

    A List element to hold Response objects.

    @{ */

/** @struct SE_ResponseList_t se_types.h

    A List element to hold Response objects.

    @var SE_ResponseList_t::_flags
    @var SE_ResponseList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_ResponseList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_ResponseList_t::results
    Indicates the number of items in this page of results.

    @var SE_ResponseList_t::Response
*/

/** @} */

/** @defgroup PriceResponse PriceResponse
    @ingroup Response

    A response related to a price message.

    @{ */

/** @struct SE_PriceResponse_t se_types.h

    A response related to a price message.

    @var SE_PriceResponse_t::_flags
#define SE_status_exists (1 << 5)

#define SE_createdDateTime_exists (1 << 6)

    @var SE_PriceResponse_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_PriceResponse_t::createdDateTime
    The createdDateTime field contains the date and time when the acknowledgement/status occurred in the client. The client will provide the timestamp to ensure the proper time is captured in case the response is delayed in reaching the server (server receipt time would not be the same as the actual confirmation time). The time reported from the client should be relative to the time server indicated by the FunctionSetAssignment that also indicated the event resource; if no FunctionSetAssignment exists, the time of the server where the event resource was hosted.

    @var SE_PriceResponse_t::endDeviceLFDI
    Contains the LFDI of the device providing the response.

    @var SE_PriceResponse_t::status
    The status field contains the acknowledgement or status. Each event type (DRLC, DER, Price, or Text) can return different status information (e.g. an Acknowledge will be returned for a Price event where a DRLC event can return Event Received, Event Started, and Event Completed). The Status field value definitions are defined in Table 27: Response Types by Function Set.

    @var SE_PriceResponse_t::subject
    The subject field provides a method to match the response with the originating event. It is populated with the mRID of the original object.

*/

/** @} */

/** @defgroup AppliedTargetReduction AppliedTargetReduction
    @ingroup se_types

    Specifies the value of the TargetReduction applied by the device.

    @{ */

/** @struct SE_AppliedTargetReduction_t se_types.h

    Specifies the value of the TargetReduction applied by the device.

    @var SE_AppliedTargetReduction_t::type
    Enumerated field representing the type of reduction requested.

    @var SE_AppliedTargetReduction_t::value
    Indicates the requested amount of the relevant commodity to be reduced.

*/

/** @} */

/** @defgroup DrResponse DrResponse
    @ingroup Response

    A response to a Demand Response Load Control (EndDeviceControl) message.

    @{ */

/** @struct SE_DrResponse_t se_types.h

    A response to a Demand Response Load Control (EndDeviceControl) message.

    @var SE_DrResponse_t::_flags
#define SE_status_exists (1 << 5)

#define SE_createdDateTime_exists (1 << 6)

#define SE_AppliedTargetReduction_exists (1 << 7)

    @var SE_DrResponse_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DrResponse_t::createdDateTime
    The createdDateTime field contains the date and time when the acknowledgement/status occurred in the client. The client will provide the timestamp to ensure the proper time is captured in case the response is delayed in reaching the server (server receipt time would not be the same as the actual confirmation time). The time reported from the client should be relative to the time server indicated by the FunctionSetAssignment that also indicated the event resource; if no FunctionSetAssignment exists, the time of the server where the event resource was hosted.

    @var SE_DrResponse_t::endDeviceLFDI
    Contains the LFDI of the device providing the response.

    @var SE_DrResponse_t::status
    The status field contains the acknowledgement or status. Each event type (DRLC, DER, Price, or Text) can return different status information (e.g. an Acknowledge will be returned for a Price event where a DRLC event can return Event Received, Event Started, and Event Completed). The Status field value definitions are defined in Table 27: Response Types by Function Set.

    @var SE_DrResponse_t::subject
    The subject field provides a method to match the response with the originating event. It is populated with the mRID of the original object.

    @var SE_DrResponse_t::ApplianceLoadReduction
    @var SE_DrResponse_t::AppliedTargetReduction
    @var SE_DrResponse_t::DutyCycle
    @var SE_DrResponse_t::Offset
    @var SE_DrResponse_t::overrideDuration
    Indicates the amount of time, in seconds, that the client partially opts-out during the demand response event. When overriding within the allowed override duration, the client SHALL send a partial opt-out (Response status code 8) for partial opt-out upon completion, with the total time the event was overridden (this attribute) populated. The client SHALL send a no participation status response (status type 10) if the user partially opts-out for longer than EndDeviceControl.overrideDuration.

    @var SE_DrResponse_t::SetPoint
*/

/** @} */

/** @defgroup FlowReservationResponseResponse FlowReservationResponseResponse
    @ingroup Response

    A response to a FlowReservationResponse

    @{ */

/** @struct SE_FlowReservationResponseResponse_t se_types.h

    A response to a FlowReservationResponse

    @var SE_FlowReservationResponseResponse_t::_flags
#define SE_status_exists (1 << 5)

#define SE_createdDateTime_exists (1 << 6)

    @var SE_FlowReservationResponseResponse_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_FlowReservationResponseResponse_t::createdDateTime
    The createdDateTime field contains the date and time when the acknowledgement/status occurred in the client. The client will provide the timestamp to ensure the proper time is captured in case the response is delayed in reaching the server (server receipt time would not be the same as the actual confirmation time). The time reported from the client should be relative to the time server indicated by the FunctionSetAssignment that also indicated the event resource; if no FunctionSetAssignment exists, the time of the server where the event resource was hosted.

    @var SE_FlowReservationResponseResponse_t::endDeviceLFDI
    Contains the LFDI of the device providing the response.

    @var SE_FlowReservationResponseResponse_t::status
    The status field contains the acknowledgement or status. Each event type (DRLC, DER, Price, or Text) can return different status information (e.g. an Acknowledge will be returned for a Price event where a DRLC event can return Event Received, Event Started, and Event Completed). The Status field value definitions are defined in Table 27: Response Types by Function Set.

    @var SE_FlowReservationResponseResponse_t::subject
    The subject field provides a method to match the response with the originating event. It is populated with the mRID of the original object.

*/

/** @} */

/** @defgroup DERControlResponse DERControlResponse
    @ingroup Response

    A response to a DERControl

    @{ */

/** @struct SE_DERControlResponse_t se_types.h

    A response to a DERControl

    @var SE_DERControlResponse_t::_flags
#define SE_status_exists (1 << 5)

#define SE_createdDateTime_exists (1 << 6)

    @var SE_DERControlResponse_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DERControlResponse_t::createdDateTime
    The createdDateTime field contains the date and time when the acknowledgement/status occurred in the client. The client will provide the timestamp to ensure the proper time is captured in case the response is delayed in reaching the server (server receipt time would not be the same as the actual confirmation time). The time reported from the client should be relative to the time server indicated by the FunctionSetAssignment that also indicated the event resource; if no FunctionSetAssignment exists, the time of the server where the event resource was hosted.

    @var SE_DERControlResponse_t::endDeviceLFDI
    Contains the LFDI of the device providing the response.

    @var SE_DERControlResponse_t::status
    The status field contains the acknowledgement or status. Each event type (DRLC, DER, Price, or Text) can return different status information (e.g. an Acknowledge will be returned for a Price event where a DRLC event can return Event Received, Event Started, and Event Completed). The Status field value definitions are defined in Table 27: Response Types by Function Set.

    @var SE_DERControlResponse_t::subject
    The subject field provides a method to match the response with the originating event. It is populated with the mRID of the original object.

*/

/** @} */

/** @defgroup SubscriptionBase SubscriptionBase
    @ingroup Resource

    Holds the information related to a client subscription to receive updates to a resource automatically. The actual resources may be passed in the Notification by specifying a specific xsi:type for the Resource and passing the full representation.

    @{ */

/** @struct SE_SubscriptionBase_t se_types.h

    Holds the information related to a client subscription to receive updates to a resource automatically. The actual resources may be passed in the Notification by specifying a specific xsi:type for the Resource and passing the full representation.

    @var SE_SubscriptionBase_t::_flags
    @var SE_SubscriptionBase_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_SubscriptionBase_t::subscribedResource
    The resource for which the subscription applies. Query string parameters SHALL NOT be specified when subscribing to list resources.  Should a query string parameter be specified, servers SHALL ignore them.

*/

/** @} */

/** @defgroup Notification Notification
    @ingroup SubscriptionBase

    Holds the information related to a client subscription to receive updates to a resource automatically. The actual resources may be passed in the Notification by specifying a specific xsi:type for the Resource and passing the full representation.

    @{ */

/** @struct SE_Notification_t se_types.h

    Holds the information related to a client subscription to receive updates to a resource automatically. The actual resources may be passed in the Notification by specifying a specific xsi:type for the Resource and passing the full representation.

    @var SE_Notification_t::_flags
#define SE_Resource_exists (1 << 5)

    @var SE_Notification_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_Notification_t::subscribedResource
    The resource for which the subscription applies. Query string parameters SHALL NOT be specified when subscribing to list resources.  Should a query string parameter be specified, servers SHALL ignore them.

    @var SE_Notification_t::newResourceURI
    The new location of the resource, if moved. This attribute SHALL be a fully-qualified absolute URI, not a relative reference.

    @var SE_Notification_t::Resource
    @var SE_Notification_t::status
    0 = Default Status

    1 = Subscription canceled, no additional information

    2 = Subscription canceled, resource moved

    3 = Subscription canceled, resource definition changed (e.g., a new version of IEEE 2030.5)

    4 = Subscription canceled, resource deleted

    All other values reserved.

    @var SE_Notification_t::subscriptionURI
    The subscription from which this notification was triggered. This attribute SHALL be a fully-qualified absolute URI, not a relative reference.

*/

/** @} */

/** @defgroup NotificationList NotificationList
    @ingroup List

    A List element to hold Notification objects.

    @{ */

/** @struct SE_NotificationList_t se_types.h

    A List element to hold Notification objects.

    @var SE_NotificationList_t::_flags
    @var SE_NotificationList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_NotificationList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_NotificationList_t::results
    Indicates the number of items in this page of results.

    @var SE_NotificationList_t::Notification
*/

/** @} */

/** @defgroup Condition Condition
    @ingroup se_types

    Indicates a condition that must be satisfied for the Notification to be triggered.

    @{ */

/** @struct SE_Condition_t se_types.h

    Indicates a condition that must be satisfied for the Notification to be triggered.

    @var SE_Condition_t::attributeIdentifier
    0 = Reading value

    1-255 = Reserved

    @var SE_Condition_t::lowerThreshold
    The value of the lower threshold

    @var SE_Condition_t::upperThreshold
    The value of the upper threshold

*/

/** @} */

/** @defgroup Subscription Subscription
    @ingroup SubscriptionBase

    Holds the information related to a client subscription to receive updates to a resource automatically.

    @{ */

/** @struct SE_Subscription_t se_types.h

    Holds the information related to a client subscription to receive updates to a resource automatically.

    @var SE_Subscription_t::_flags
#define SE_Condition_exists (1 << 5)

    @var SE_Subscription_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_Subscription_t::subscribedResource
    The resource for which the subscription applies. Query string parameters SHALL NOT be specified when subscribing to list resources.  Should a query string parameter be specified, servers SHALL ignore them.

    @var SE_Subscription_t::Condition
    @var SE_Subscription_t::encoding
    0 - application/sep+xml

    1 - application/sep-exi

    2-255 - reserved

    @var SE_Subscription_t::level
    Contains the preferred schema and extensibility level indication such as "+S1"

    @var SE_Subscription_t::limit
    This element is used to indicate the maximum number of list items that should be included in a notification when the subscribed resource changes. This limit is meant to be functionally equivalent to the ‘limit’ query string parameter, but applies to both list resources as well as other resources.  For list resources, if a limit of ‘0’ is specified, then notifications SHALL contain a list resource with results=’0’ (equivalent to a simple change notification).  For list resources, if a limit greater than ‘0’ is specified, then notifications SHALL contain a list resource with results equal to the limit specified (or less, should the list contain fewer items than the limit specified or should the server be unable to provide the requested number of items for any reason) and follow the same rules for list resources (e.g., ordering).  For non-list resources, if a limit of ‘0’ is specified, then notifications SHALL NOT contain a resource representation (equivalent to a simple change notification).  For non-list resources, if a limit greater than ‘0’ is specified, then notifications SHALL contain the representation of the changed resource.

    @var SE_Subscription_t::notificationURI
    The resource to which to post the notifications about the requested subscribed resource. Because this URI will exist on a server other than the one being POSTed to, this attribute SHALL be a fully-qualified absolute URI, not a relative reference.

*/

/** @} */

/** @defgroup SubscriptionList SubscriptionList
    @ingroup List

    A List element to hold Subscription objects.

    @{ */

/** @struct SE_SubscriptionList_t se_types.h

    A List element to hold Subscription objects.

    @var SE_SubscriptionList_t::_flags
    @var SE_SubscriptionList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_SubscriptionList_t::all
    The number specifying "all" of the items in the list. Required on a response to a GET, ignored otherwise.

    @var SE_SubscriptionList_t::results
    Indicates the number of items in this page of results.

    @var SE_SubscriptionList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_SubscriptionList_t::Subscription
*/

/** @} */

/** @defgroup UsagePointListLink UsagePointListLink
    @ingroup ListLink

    SHALL contain a Link to a List of UsagePoint instances.

    @{ */

/** @struct SE_UsagePointListLink_t se_types.h

    SHALL contain a Link to a List of UsagePoint instances.

    @var SE_UsagePointListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_UsagePointListLink_t::href
    A URI reference.

    @var SE_UsagePointListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup TimeLink TimeLink
    @ingroup Link

    SHALL contain a Link to an instance of Time.

    @{ */

/** @struct SE_TimeLink_t se_types.h

    SHALL contain a Link to an instance of Time.

    @var SE_TimeLink_t::_flags
    @var SE_TimeLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup TariffProfileListLink TariffProfileListLink
    @ingroup ListLink

    SHALL contain a Link to a List of TariffProfile instances.

    @{ */

/** @struct SE_TariffProfileListLink_t se_types.h

    SHALL contain a Link to a List of TariffProfile instances.

    @var SE_TariffProfileListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_TariffProfileListLink_t::href
    A URI reference.

    @var SE_TariffProfileListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup ResponseSetListLink ResponseSetListLink
    @ingroup ListLink

    SHALL contain a Link to a List of ResponseSet instances.

    @{ */

/** @struct SE_ResponseSetListLink_t se_types.h

    SHALL contain a Link to a List of ResponseSet instances.

    @var SE_ResponseSetListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_ResponseSetListLink_t::href
    A URI reference.

    @var SE_ResponseSetListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup PrepaymentListLink PrepaymentListLink
    @ingroup ListLink

    SHALL contain a Link to a List of Prepayment instances.

    @{ */

/** @struct SE_PrepaymentListLink_t se_types.h

    SHALL contain a Link to a List of Prepayment instances.

    @var SE_PrepaymentListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_PrepaymentListLink_t::href
    A URI reference.

    @var SE_PrepaymentListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup MessagingProgramListLink MessagingProgramListLink
    @ingroup ListLink

    SHALL contain a Link to a List of MessagingProgram instances.

    @{ */

/** @struct SE_MessagingProgramListLink_t se_types.h

    SHALL contain a Link to a List of MessagingProgram instances.

    @var SE_MessagingProgramListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_MessagingProgramListLink_t::href
    A URI reference.

    @var SE_MessagingProgramListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup FileListLink FileListLink
    @ingroup ListLink

    SHALL contain a Link to a List of File instances.

    @{ */

/** @struct SE_FileListLink_t se_types.h

    SHALL contain a Link to a List of File instances.

    @var SE_FileListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_FileListLink_t::href
    A URI reference.

    @var SE_FileListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup DERProgramListLink DERProgramListLink
    @ingroup ListLink

    SHALL contain a Link to a List of DERProgram instances.

    @{ */

/** @struct SE_DERProgramListLink_t se_types.h

    SHALL contain a Link to a List of DERProgram instances.

    @var SE_DERProgramListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_DERProgramListLink_t::href
    A URI reference.

    @var SE_DERProgramListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup DemandResponseProgramListLink DemandResponseProgramListLink
    @ingroup ListLink

    SHALL contain a Link to a List of DemandResponseProgram instances.

    @{ */

/** @struct SE_DemandResponseProgramListLink_t se_types.h

    SHALL contain a Link to a List of DemandResponseProgram instances.

    @var SE_DemandResponseProgramListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_DemandResponseProgramListLink_t::href
    A URI reference.

    @var SE_DemandResponseProgramListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup CustomerAccountListLink CustomerAccountListLink
    @ingroup ListLink

    SHALL contain a Link to a List of CustomerAccount instances.

    @{ */

/** @struct SE_CustomerAccountListLink_t se_types.h

    SHALL contain a Link to a List of CustomerAccount instances.

    @var SE_CustomerAccountListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_CustomerAccountListLink_t::href
    A URI reference.

    @var SE_CustomerAccountListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup FunctionSetAssignmentsBase FunctionSetAssignmentsBase
    @ingroup Resource

    Defines a collection of function set instances that are to be used by one or more devices as indicated by the EndDevice object(s) of the server.

    @{ */

/** @struct SE_FunctionSetAssignmentsBase_t se_types.h

    Defines a collection of function set instances that are to be used by one or more devices as indicated by the EndDevice object(s) of the server.

    @var SE_FunctionSetAssignmentsBase_t::_flags
#define SE_UsagePointListLink_exists (1 << 5)

#define SE_TariffProfileListLink_exists (1 << 6)

#define SE_ResponseSetListLink_exists (1 << 7)

#define SE_PrepaymentListLink_exists (1 << 8)

#define SE_MessagingProgramListLink_exists (1 << 9)

#define SE_FileListLink_exists (1 << 10)

#define SE_DERProgramListLink_exists (1 << 11)

#define SE_DemandResponseProgramListLink_exists (1 << 12)

#define SE_CustomerAccountListLink_exists (1 << 13)

    @var SE_FunctionSetAssignmentsBase_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_FunctionSetAssignmentsBase_t::CustomerAccountListLink
    @var SE_FunctionSetAssignmentsBase_t::DemandResponseProgramListLink
    @var SE_FunctionSetAssignmentsBase_t::DERProgramListLink
    @var SE_FunctionSetAssignmentsBase_t::FileListLink
    @var SE_FunctionSetAssignmentsBase_t::MessagingProgramListLink
    @var SE_FunctionSetAssignmentsBase_t::PrepaymentListLink
    @var SE_FunctionSetAssignmentsBase_t::ResponseSetListLink
    @var SE_FunctionSetAssignmentsBase_t::TariffProfileListLink
    @var SE_FunctionSetAssignmentsBase_t::TimeLink
    @var SE_FunctionSetAssignmentsBase_t::UsagePointListLink
*/

/** @} */

/** @defgroup FunctionSetAssignments FunctionSetAssignments
    @ingroup FunctionSetAssignmentsBase

    Provides an identifiable, subscribable collection of resources for a particular device to consume.

    @{ */

/** @struct SE_FunctionSetAssignments_t se_types.h

    Provides an identifiable, subscribable collection of resources for a particular device to consume.

    @var SE_FunctionSetAssignments_t::_flags
#define SE_UsagePointListLink_exists (1 << 5)

#define SE_TariffProfileListLink_exists (1 << 6)

#define SE_ResponseSetListLink_exists (1 << 7)

#define SE_PrepaymentListLink_exists (1 << 8)

#define SE_MessagingProgramListLink_exists (1 << 9)

#define SE_FileListLink_exists (1 << 10)

#define SE_DERProgramListLink_exists (1 << 11)

#define SE_DemandResponseProgramListLink_exists (1 << 12)

#define SE_CustomerAccountListLink_exists (1 << 13)

    @var SE_FunctionSetAssignments_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_FunctionSetAssignments_t::CustomerAccountListLink
    @var SE_FunctionSetAssignments_t::DemandResponseProgramListLink
    @var SE_FunctionSetAssignments_t::DERProgramListLink
    @var SE_FunctionSetAssignments_t::FileListLink
    @var SE_FunctionSetAssignments_t::MessagingProgramListLink
    @var SE_FunctionSetAssignments_t::PrepaymentListLink
    @var SE_FunctionSetAssignments_t::ResponseSetListLink
    @var SE_FunctionSetAssignments_t::TariffProfileListLink
    @var SE_FunctionSetAssignments_t::TimeLink
    @var SE_FunctionSetAssignments_t::UsagePointListLink
    @var SE_FunctionSetAssignments_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_FunctionSetAssignments_t::mRID
    The global identifier of the object.

    @var SE_FunctionSetAssignments_t::description
    The description is a human readable text describing or naming the object.

    @var SE_FunctionSetAssignments_t::version
    Contains the version number of the object. See the type definition for details.

*/

/** @} */

/** @defgroup FunctionSetAssignmentsList FunctionSetAssignmentsList
    @ingroup SubscribableList

    A List element to hold FunctionSetAssignments objects.

    @{ */

/** @struct SE_FunctionSetAssignmentsList_t se_types.h

    A List element to hold FunctionSetAssignments objects.

    @var SE_FunctionSetAssignmentsList_t::_flags
    @var SE_FunctionSetAssignmentsList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_FunctionSetAssignmentsList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_FunctionSetAssignmentsList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_FunctionSetAssignmentsList_t::results
    Indicates the number of items in this page of results.

    @var SE_FunctionSetAssignmentsList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_FunctionSetAssignmentsList_t::FunctionSetAssignments
*/

/** @} */

/** @defgroup UInt40 UInt40
    @ingroup se_typedefs

    Unsigned integer, max inclusive 1099511627775 (2^40-1)

    @{ */

/** @typedef SE_UInt40_t

    Unsigned integer, max inclusive 1099511627775 (2^40-1)

*/

/** @} */

/** @defgroup SFDIType SFDIType
    @ingroup UInt40

    Unsigned integer, max inclusive 687194767359, which is 2^36-1 (68719476735), with added check digit. See Section 8.3.2 for check digit calculation.

    @{ */

/** @typedef SE_SFDIType_t

    Unsigned integer, max inclusive 687194767359, which is 2^36-1 (68719476735), with added check digit. See Section 8.3.2 for check digit calculation.

*/

/** @} */

/** @defgroup PowerStatusLink PowerStatusLink
    @ingroup Link

    SHALL contain a Link to an instance of PowerStatus.

    @{ */

/** @struct SE_PowerStatusLink_t se_types.h

    SHALL contain a Link to an instance of PowerStatus.

    @var SE_PowerStatusLink_t::_flags
    @var SE_PowerStatusLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup LogEventListLink LogEventListLink
    @ingroup ListLink

    SHALL contain a Link to a List of LogEvent instances.

    @{ */

/** @struct SE_LogEventListLink_t se_types.h

    SHALL contain a Link to a List of LogEvent instances.

    @var SE_LogEventListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_LogEventListLink_t::href
    A URI reference.

    @var SE_LogEventListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup LoadShedAvailabilityListLink LoadShedAvailabilityListLink
    @ingroup ListLink

    SHALL contain a Link to a List of LoadShedAvailability instances.

    @{ */

/** @struct SE_LoadShedAvailabilityListLink_t se_types.h

    SHALL contain a Link to a List of LoadShedAvailability instances.

    @var SE_LoadShedAvailabilityListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_LoadShedAvailabilityListLink_t::href
    A URI reference.

    @var SE_LoadShedAvailabilityListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup IPInterfaceListLink IPInterfaceListLink
    @ingroup ListLink

    SHALL contain a Link to a List of IPInterface instances.

    @{ */

/** @struct SE_IPInterfaceListLink_t se_types.h

    SHALL contain a Link to a List of IPInterface instances.

    @var SE_IPInterfaceListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_IPInterfaceListLink_t::href
    A URI reference.

    @var SE_IPInterfaceListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup FileStatusLink FileStatusLink
    @ingroup Link

    SHALL contain a Link to an instance of FileStatus.

    @{ */

/** @struct SE_FileStatusLink_t se_types.h

    SHALL contain a Link to an instance of FileStatus.

    @var SE_FileStatusLink_t::_flags
    @var SE_FileStatusLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup DeviceStatusLink DeviceStatusLink
    @ingroup Link

    SHALL contain a Link to an instance of DeviceStatus.

    @{ */

/** @struct SE_DeviceStatusLink_t se_types.h

    SHALL contain a Link to an instance of DeviceStatus.

    @var SE_DeviceStatusLink_t::_flags
    @var SE_DeviceStatusLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup DeviceInformationLink DeviceInformationLink
    @ingroup Link

    SHALL contain a Link to an instance of DeviceInformation.

    @{ */

/** @struct SE_DeviceInformationLink_t se_types.h

    SHALL contain a Link to an instance of DeviceInformation.

    @var SE_DeviceInformationLink_t::_flags
    @var SE_DeviceInformationLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup DERListLink DERListLink
    @ingroup ListLink

    SHALL contain a Link to a List of DER instances.

    @{ */

/** @struct SE_DERListLink_t se_types.h

    SHALL contain a Link to a List of DER instances.

    @var SE_DERListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_DERListLink_t::href
    A URI reference.

    @var SE_DERListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup ConfigurationLink ConfigurationLink
    @ingroup Link

    SHALL contain a Link to an instance of Configuration.

    @{ */

/** @struct SE_ConfigurationLink_t se_types.h

    SHALL contain a Link to an instance of Configuration.

    @var SE_ConfigurationLink_t::_flags
    @var SE_ConfigurationLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup AbstractDevice AbstractDevice
    @ingroup SubscribableResource

    The EndDevice providing the resources available within the DeviceCapabilities.

    @{ */

/** @struct SE_AbstractDevice_t se_types.h

    The EndDevice providing the resources available within the DeviceCapabilities.

    @var SE_AbstractDevice_t::_flags
#define SE_PowerStatusLink_exists (1 << 5)

#define SE_LogEventListLink_exists (1 << 6)

#define SE_loadShedDeviceCategory_exists (1 << 7)

#define SE_LoadShedAvailabilityListLink_exists (1 << 8)

#define SE_IPInterfaceListLink_exists (1 << 9)

#define SE_FileStatusLink_exists (1 << 10)

#define SE_DeviceStatusLink_exists (1 << 11)

#define SE_DeviceInformationLink_exists (1 << 12)

#define SE_DERListLink_exists (1 << 13)

#define SE_ConfigurationLink_exists (1 << 14)

    @var SE_AbstractDevice_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_AbstractDevice_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_AbstractDevice_t::ConfigurationLink
    @var SE_AbstractDevice_t::DERListLink
    @var SE_AbstractDevice_t::DeviceInformationLink
    @var SE_AbstractDevice_t::DeviceStatusLink
    @var SE_AbstractDevice_t::FileStatusLink
    @var SE_AbstractDevice_t::IPInterfaceListLink
    @var SE_AbstractDevice_t::lFDI
    Long form of device identifier. See the Security section for additional details.

    @var SE_AbstractDevice_t::LoadShedAvailabilityListLink
    @var SE_AbstractDevice_t::loadShedDeviceCategory
    This field is for use in devices that can shed load.  If you are a device that does not respond to EndDeviceControls (for instance, an ESI), this field should not have any bits set.

    @var SE_AbstractDevice_t::LogEventListLink
    @var SE_AbstractDevice_t::PowerStatusLink
    @var SE_AbstractDevice_t::sFDI
    Short form of device identifier, WITH the checksum digit. See the Security section for additional details.

*/

/** @} */

/** @defgroup SelfDevice SelfDevice
    @ingroup AbstractDevice

    The EndDevice providing the resources available within the DeviceCapabilities.

    @{ */

/** @struct SE_SelfDevice_t se_types.h

    The EndDevice providing the resources available within the DeviceCapabilities.

    @var SE_SelfDevice_t::_flags
#define SE_PowerStatusLink_exists (1 << 5)

#define SE_LogEventListLink_exists (1 << 6)

#define SE_loadShedDeviceCategory_exists (1 << 7)

#define SE_LoadShedAvailabilityListLink_exists (1 << 8)

#define SE_IPInterfaceListLink_exists (1 << 9)

#define SE_FileStatusLink_exists (1 << 10)

#define SE_DeviceStatusLink_exists (1 << 11)

#define SE_DeviceInformationLink_exists (1 << 12)

#define SE_DERListLink_exists (1 << 13)

#define SE_ConfigurationLink_exists (1 << 14)

    @var SE_SelfDevice_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_SelfDevice_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_SelfDevice_t::ConfigurationLink
    @var SE_SelfDevice_t::DERListLink
    @var SE_SelfDevice_t::DeviceInformationLink
    @var SE_SelfDevice_t::DeviceStatusLink
    @var SE_SelfDevice_t::FileStatusLink
    @var SE_SelfDevice_t::IPInterfaceListLink
    @var SE_SelfDevice_t::lFDI
    Long form of device identifier. See the Security section for additional details.

    @var SE_SelfDevice_t::LoadShedAvailabilityListLink
    @var SE_SelfDevice_t::loadShedDeviceCategory
    This field is for use in devices that can shed load.  If you are a device that does not respond to EndDeviceControls (for instance, an ESI), this field should not have any bits set.

    @var SE_SelfDevice_t::LogEventListLink
    @var SE_SelfDevice_t::PowerStatusLink
    @var SE_SelfDevice_t::sFDI
    Short form of device identifier, WITH the checksum digit. See the Security section for additional details.

    @var SE_SelfDevice_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

*/

/** @} */

/** @defgroup PINType PINType
    @ingroup UInt32

    6 digit unsigned decimal integer (0 - 999999). 

    (Note that this only requires 20 bits, if it can be allocated.)

    @{ */

/** @typedef SE_PINType_t

    6 digit unsigned decimal integer (0 - 999999). 

    (Note that this only requires 20 bits, if it can be allocated.)

*/

/** @} */

/** @defgroup Registration Registration
    @ingroup Resource

    Registration represents an authorization to access the resources on a host.

    @{ */

/** @struct SE_Registration_t se_types.h

    Registration represents an authorization to access the resources on a host.

    @var SE_Registration_t::_flags
    @var SE_Registration_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_Registration_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_Registration_t::dateTimeRegistered
    Contains the time at which this registration was created, by which clients MAY prioritize information providers with the most recent registrations, when no additional direction from the consumer is available.

    @var SE_Registration_t::pIN
    Contains the registration PIN number associated with the device, including the checksum digit.

*/

/** @} */

/** @defgroup SubscriptionListLink SubscriptionListLink
    @ingroup ListLink

    SHALL contain a Link to a List of Subscription instances.

    @{ */

/** @struct SE_SubscriptionListLink_t se_types.h

    SHALL contain a Link to a List of Subscription instances.

    @var SE_SubscriptionListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_SubscriptionListLink_t::href
    A URI reference.

    @var SE_SubscriptionListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup RegistrationLink RegistrationLink
    @ingroup Link

    SHALL contain a Link to an instance of Registration.

    @{ */

/** @struct SE_RegistrationLink_t se_types.h

    SHALL contain a Link to an instance of Registration.

    @var SE_RegistrationLink_t::_flags
    @var SE_RegistrationLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup FunctionSetAssignmentsListLink FunctionSetAssignmentsListLink
    @ingroup ListLink

    SHALL contain a Link to a List of FunctionSetAssignments instances.

    @{ */

/** @struct SE_FunctionSetAssignmentsListLink_t se_types.h

    SHALL contain a Link to a List of FunctionSetAssignments instances.

    @var SE_FunctionSetAssignmentsListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_FunctionSetAssignmentsListLink_t::href
    A URI reference.

    @var SE_FunctionSetAssignmentsListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup FlowReservationResponseListLink FlowReservationResponseListLink
    @ingroup ListLink

    SHALL contain a Link to a List of FlowReservationResponse instances.

    @{ */

/** @struct SE_FlowReservationResponseListLink_t se_types.h

    SHALL contain a Link to a List of FlowReservationResponse instances.

    @var SE_FlowReservationResponseListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_FlowReservationResponseListLink_t::href
    A URI reference.

    @var SE_FlowReservationResponseListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup FlowReservationRequestListLink FlowReservationRequestListLink
    @ingroup ListLink

    SHALL contain a Link to a List of FlowReservationRequest instances.

    @{ */

/** @struct SE_FlowReservationRequestListLink_t se_types.h

    SHALL contain a Link to a List of FlowReservationRequest instances.

    @var SE_FlowReservationRequestListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_FlowReservationRequestListLink_t::href
    A URI reference.

    @var SE_FlowReservationRequestListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup EndDevice EndDevice
    @ingroup AbstractDevice

    Asset container that performs one or more end device functions. Contains information about individual devices in the network.

    @{ */

/** @struct SE_EndDevice_t se_types.h

    Asset container that performs one or more end device functions. Contains information about individual devices in the network.

    @var SE_EndDevice_t::_flags
#define SE_PowerStatusLink_exists (1 << 5)

#define SE_LogEventListLink_exists (1 << 6)

#define SE_loadShedDeviceCategory_exists (1 << 7)

#define SE_LoadShedAvailabilityListLink_exists (1 << 8)

#define SE_IPInterfaceListLink_exists (1 << 9)

#define SE_FileStatusLink_exists (1 << 10)

#define SE_DeviceStatusLink_exists (1 << 11)

#define SE_DeviceInformationLink_exists (1 << 12)

#define SE_DERListLink_exists (1 << 13)

#define SE_ConfigurationLink_exists (1 << 14)

#define SE_SubscriptionListLink_exists (1 << 15)

#define SE_RegistrationLink_exists (1 << 16)

#define SE_FunctionSetAssignmentsListLink_exists (1 << 17)

#define SE_FlowReservationResponseListLink_exists (1 << 18)

#define SE_FlowReservationRequestListLink_exists (1 << 19)

#define SE_enabled_true (1 << 20)
#define SE_enabled_exists (1 << 21)

    @var SE_EndDevice_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_EndDevice_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_EndDevice_t::ConfigurationLink
    @var SE_EndDevice_t::DERListLink
    @var SE_EndDevice_t::DeviceInformationLink
    @var SE_EndDevice_t::DeviceStatusLink
    @var SE_EndDevice_t::FileStatusLink
    @var SE_EndDevice_t::IPInterfaceListLink
    @var SE_EndDevice_t::lFDI
    Long form of device identifier. See the Security section for additional details.

    @var SE_EndDevice_t::LoadShedAvailabilityListLink
    @var SE_EndDevice_t::loadShedDeviceCategory
    This field is for use in devices that can shed load.  If you are a device that does not respond to EndDeviceControls (for instance, an ESI), this field should not have any bits set.

    @var SE_EndDevice_t::LogEventListLink
    @var SE_EndDevice_t::PowerStatusLink
    @var SE_EndDevice_t::sFDI
    Short form of device identifier, WITH the checksum digit. See the Security section for additional details.

    @var SE_EndDevice_t::changedTime
    The time at which this resource was last modified or created.

    @var SE_EndDevice_t::FlowReservationRequestListLink
    @var SE_EndDevice_t::FlowReservationResponseListLink
    @var SE_EndDevice_t::FunctionSetAssignmentsListLink
    @var SE_EndDevice_t::RegistrationLink
    @var SE_EndDevice_t::SubscriptionListLink
*/

/** @} */

/** @defgroup EndDeviceList EndDeviceList
    @ingroup SubscribableList

    A List element to hold EndDevice objects.

    @{ */

/** @struct SE_EndDeviceList_t se_types.h

    A List element to hold EndDevice objects.

    @var SE_EndDeviceList_t::_flags
    @var SE_EndDeviceList_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_EndDeviceList_t::subscribable
    Indicates whether or not subscriptions are supported for this resource, and whether or not conditional (thresholds) are supported. If not specified, is "not subscribable" (0).

    @var SE_EndDeviceList_t::all
    The number specifying "all" of the items in the list. Required on GET, ignored otherwise.

    @var SE_EndDeviceList_t::results
    Indicates the number of items in this page of results.

    @var SE_EndDeviceList_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_EndDeviceList_t::EndDevice
*/

/** @} */

/** @defgroup Temperature Temperature
    @ingroup se_types

    Specification of a temperature.

    @{ */

/** @struct SE_Temperature_t se_types.h

    Specification of a temperature.

    @var SE_Temperature_t::multiplier
    Multiplier for 'unit'.

    @var SE_Temperature_t::subject
    The subject of the temperature measurement

    0 - Enclosure

    1 - Transformer

    2 - HeatSink

    @var SE_Temperature_t::value
    Value in Degrees Celsius (uom 23).

*/

/** @} */

/** @defgroup DeviceStatus DeviceStatus
    @ingroup Resource

    Status of device

    @{ */

/** @struct SE_DeviceStatus_t se_types.h

    Status of device

    @var SE_DeviceStatus_t::_flags
#define SE_opTime_exists (1 << 5)

#define SE_opState_exists (1 << 6)

#define SE_onCount_exists (1 << 7)

    @var SE_DeviceStatus_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DeviceStatus_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_DeviceStatus_t::changedTime
    The time at which the reported values were recorded.

    @var SE_DeviceStatus_t::onCount
    The number of times that the device has been turned on: Count of "device on" times, since the last time the counter was reset

    @var SE_DeviceStatus_t::opState
    Device operational state: 

    0 - Not applicable / Unknown

    1 - Not operating

    2 - Operating

    3 - Starting up

    4 - Shutting down

    5 - At disconnect level

    6 - kW ramping

    7 - kVar ramping

    @var SE_DeviceStatus_t::opTime
    Total time device has operated: re-settable: Accumulated time in seconds since the last time the counter was reset.

    @var SE_DeviceStatus_t::Temperature
    @var SE_DeviceStatus_t::TimeLink
*/

/** @} */

/** @defgroup SelfDeviceLink SelfDeviceLink
    @ingroup Link

    SHALL contain a Link to an instance of SelfDevice.

    @{ */

/** @struct SE_SelfDeviceLink_t se_types.h

    SHALL contain a Link to an instance of SelfDevice.

    @var SE_SelfDeviceLink_t::_flags
    @var SE_SelfDeviceLink_t::href
    A URI reference.

*/

/** @} */

/** @defgroup MirrorUsagePointListLink MirrorUsagePointListLink
    @ingroup ListLink

    SHALL contain a Link to a List of MirrorUsagePoint instances.

    @{ */

/** @struct SE_MirrorUsagePointListLink_t se_types.h

    SHALL contain a Link to a List of MirrorUsagePoint instances.

    @var SE_MirrorUsagePointListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_MirrorUsagePointListLink_t::href
    A URI reference.

    @var SE_MirrorUsagePointListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup EndDeviceListLink EndDeviceListLink
    @ingroup ListLink

    SHALL contain a Link to a List of EndDevice instances.

    @{ */

/** @struct SE_EndDeviceListLink_t se_types.h

    SHALL contain a Link to a List of EndDevice instances.

    @var SE_EndDeviceListLink_t::_flags
#define SE_all_exists (1 << 0)

    @var SE_EndDeviceListLink_t::href
    A URI reference.

    @var SE_EndDeviceListLink_t::all
    Indicates the total number of items in the referenced list. This attribute SHALL be present if the href is a local or relative URI. This attribute SHOULD NOT be present if the href is a remote or absolute URI, as the server may be unaware of changes to the value.

*/

/** @} */

/** @defgroup DeviceCapability DeviceCapability
    @ingroup FunctionSetAssignmentsBase

    Returned by the URI provided by DNS-SD, to allow clients to find the URIs to the resources in which they are interested.

    @{ */

/** @struct SE_DeviceCapability_t se_types.h

    Returned by the URI provided by DNS-SD, to allow clients to find the URIs to the resources in which they are interested.

    @var SE_DeviceCapability_t::_flags
#define SE_UsagePointListLink_exists (1 << 5)

#define SE_TariffProfileListLink_exists (1 << 6)

#define SE_ResponseSetListLink_exists (1 << 7)

#define SE_PrepaymentListLink_exists (1 << 8)

#define SE_MessagingProgramListLink_exists (1 << 9)

#define SE_FileListLink_exists (1 << 10)

#define SE_DERProgramListLink_exists (1 << 11)

#define SE_DemandResponseProgramListLink_exists (1 << 12)

#define SE_CustomerAccountListLink_exists (1 << 13)

#define SE_SelfDeviceLink_exists (1 << 14)

#define SE_MirrorUsagePointListLink_exists (1 << 15)

#define SE_EndDeviceListLink_exists (1 << 16)

    @var SE_DeviceCapability_t::href
    A reference to the resource address (URI). Required in a response to a GET, ignored otherwise.

    @var SE_DeviceCapability_t::CustomerAccountListLink
    @var SE_DeviceCapability_t::DemandResponseProgramListLink
    @var SE_DeviceCapability_t::DERProgramListLink
    @var SE_DeviceCapability_t::FileListLink
    @var SE_DeviceCapability_t::MessagingProgramListLink
    @var SE_DeviceCapability_t::PrepaymentListLink
    @var SE_DeviceCapability_t::ResponseSetListLink
    @var SE_DeviceCapability_t::TariffProfileListLink
    @var SE_DeviceCapability_t::TimeLink
    @var SE_DeviceCapability_t::UsagePointListLink
    @var SE_DeviceCapability_t::pollRate
    The default polling rate for this function set (this resource and all resources below), in seconds. If not specified, a default of 900 seconds (15 minutes) is used. It is RECOMMENDED a client poll the resources of this function set every pollRate seconds.

    @var SE_DeviceCapability_t::EndDeviceListLink
    @var SE_DeviceCapability_t::MirrorUsagePointListLink
    @var SE_DeviceCapability_t::SelfDeviceLink
*/

/** @} */

