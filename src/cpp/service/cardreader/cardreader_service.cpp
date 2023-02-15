#include "cardreader_service.h"
#include "qeventloop.h"

CardreaderService::CardreaderService(const QString &strFileConfig) : AbstractService{ strFileConfig } { }
