#include "rtcclock.h"
#include <Cosa/Trace.hh>

void RtcClock::begin() {
    //setTime();
    setAlarms();
    initConfig();
    DEBUG("RtcClock::begin", 0);
}

void RtcClock::viewClock() {
    time_t now;
    get_time(now);
    trace << now << ' ';
    int16_t temp = get_temperature();
    trace << (temp >> 2) << '.' << (25 * (temp & 0x3)) << PSTR(" C") << endl;
}

void RtcClock::on_clock_interrupt() {
    DEBUG("RtcClock::on_clock_interrupt", 0);
    clearInterruptFlags();
    if (IS_LOG_PRIO(LOG_INFO)) {
        viewClock();
    }
}

void RtcClock::setTime() {
    time_t now;
    get_time(now);
    now.day = 0x07;
    now.year = 0x13;
    now.month = 0x01;
    now.date = 0x05;
    now.hours = 0x20;
    now.minutes = 0x01;
    now.seconds = 0x00;
    set_time(now);
}

void RtcClock::setAlarms() {
    DS3231::alarm1_t alrm1;
    uint8_t mask;
    get_alarm1(alrm1, mask);
    alrm1.date = 1;
    alrm1.day = 1;
    alrm1.hours = 13;
    alrm1.minutes = 1;
    alrm1.seconds = 35;
    alrm1.to_bcd();
    if (IS_LOG_PRIO(LOG_DEBUG)) {
        trace << PSTR("alarm1: ") << alrm1 << PSTR(" mask: ") << mask << endl;
    }
    set_alarm1(alrm1, DS3231::alarm1_t::WHEN_TIME_MATCH);
    DS3231::alarm2_t alrm2;
    get_alarm2(alrm2, mask);
    alrm2.date = 1;
    alrm2.day = 2;
    alrm2.hours = 13;
    alrm2.minutes = 1;
    alrm2.to_bcd();
    if (IS_LOG_PRIO(LOG_DEBUG)) {
        trace << PSTR("alarm2: ") << alrm2 << PSTR(" mask: ") << mask << endl;
    }
    set_alarm2(alrm2, DS3231::alarm2_t::ONCE_PER_MIN);
    //set_alarm2(alrm2, DS3231::alarm2_t::WHEN_TIME_MATCH);
}

void RtcClock::initConfig() {
    DS3231::timekeeper_t keeper;
    DS3231::read(&keeper, sizeof (keeper));

    keeper.control.a1ie = 0;
    keeper.control.a2ie = 1;
    keeper.control.intcn = 1;
    keeper.control.rs = 0;
    keeper.control.conv = 0;
    keeper.control.bbsqw = 0;
    keeper.control.eosc = 0;

    keeper.status.a1f = 0;
    keeper.status.a2f = 0;
    keeper.status.en32khz = 0;
    keeper.aging = 0;

    DS3231::write(&keeper, sizeof (keeper));
    if (IS_LOG_PRIO(LOG_DEBUG)) {
        DS3231::read(&keeper, sizeof (keeper));
        trace << keeper << endl;
    }
}

void RtcClock::clearInterruptFlags() {
    DS3231::timekeeper_t keeper;
    DS3231::read(&keeper, sizeof (keeper));
    if (IS_LOG_PRIO(LOG_DEBUG)) {
        trace << keeper << endl;
    }
    keeper.status.a1f = 0;
    keeper.status.a2f = 0;
    DS3231::write(&keeper, sizeof (keeper));
    if (IS_LOG_PRIO(LOG_DEBUG)) {
        trace << keeper << endl;
    }
}

void RtcClock::on_event(uint8_t type, uint16_t value) {
    on_clock_interrupt();
}

