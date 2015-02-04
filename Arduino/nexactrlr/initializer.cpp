
#include "initializer.h"


void Initializer::init() {
    //configure();
    _nexaConfigStore->readFromEeprom();
}

void Initializer::configure() {
    // Stue ovn 1: 32211232.0
    _nexaConfigStore->add(32211232, 0, 1, 14, 0, 127); // man - søn
    _nexaConfigStore->add(32211232, 0, 0, 22, 0, 127); // man - søn

    // Stue ovn 2: 32211232.1
    _nexaConfigStore->add(32211232, 1, 1, 14, 0);
    _nexaConfigStore->add(32211232, 1, 0, 22, 0);

    // Kjøkken ovn 32211244.0
    _nexaConfigStore->add(32211244, 0, 1, 6, 0, 31); // man - fre
    _nexaConfigStore->add(32211244, 0, 0, 8, 0, 31); // man - fre
    _nexaConfigStore->add(32211244, 0, 1, 14, 0, 31); // man - fre
    _nexaConfigStore->add(32211244, 0, 1, 7, 0, 96); // lør - søn
    _nexaConfigStore->add(32211244, 0, 0, 22, 0);

    // Loftstua: 32211240.0
    _nexaConfigStore->add(32211240, 0, 1, 6, 0);
    _nexaConfigStore->add(32211240, 0, 0, 8, 0);
    _nexaConfigStore->add(32211240, 0, 1, 14, 0);
    _nexaConfigStore->add(32211240, 0, 0, 22, 0);

    // Soverom 1: 32211236.0
    _nexaConfigStore->add(32211236, 0, 1, 5, 30);
    _nexaConfigStore->add(32211236, 0, 0, 7, 0);
    _nexaConfigStore->add(32211236, 0, 1, 15, 0);
    _nexaConfigStore->add(32211236, 0, 0, 22, 0);

    // Kontor: 32211234.0
    _nexaConfigStore->add(32211234, 0, 1, 15, 0);
    _nexaConfigStore->add(32211234, 0, 0, 22, 0);

    // Remote for stua, kjøkken, loftstue
    _nexaConfigStore->addRc(12134882, 0, 32211232, 0);
    _nexaConfigStore->addRc(12134882, 0, 32211232, 1);
    _nexaConfigStore->addRc(12134882, 0, 32211244, 0);
    _nexaConfigStore->addRc(12134882, 0, 32211240, 0);

    // Remote for soverom
    _nexaConfigStore->addRc(12134882, 1, 32211236, 0);

    // Remote for kontoret
    _nexaConfigStore->addRc(12134882, 2, 32211234, 0);

    _nexaConfigStore->writeToEeprom();
}
