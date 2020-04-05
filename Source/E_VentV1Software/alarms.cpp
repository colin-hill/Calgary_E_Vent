#include "alarms.h"
#include "pressure.h"


uint16_t check_high_pressure(const float pressure) {
    if (pressure > MAX_PRESSURE) {
        return HIGH_PRESSURE_ALARM;
    } else {
        return 0;
    }
}


uint16_t check_low_pressure(const float pressure) {
    if (pressure < MIN_PRESSURE) {
        return LOW_PRESSURE_ALARM;
    } else {
        return 0;
    }
}


uint16_t check_pressure(const float pressure) {
    return check_high_pressure(pressure) | check_low_pressure(pressure);
}


uint16_t check_high_peep(const float pressure) {
    if (pressure > MAX_PEEP_PRESSURE) {
        return HIGH_PEEP_ALARM;
    } else {
        return 0;
    }
}


uint16_t check_low_peep(const float pressure) {
    if (pressure < MIN_PEEP_PRESSURE) {
        return LOW_PEEP_ALARM;
    } else {
        return 0;
    }
}


uint16_t check_peep(const float pressure) {
    return check_high_peep(pressure) | check_low_peep(pressure);
}
