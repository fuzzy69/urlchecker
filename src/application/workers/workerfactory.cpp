﻿#include "workerfactory.h"

#include "../tools/tools.h"
#include "checkalexarank.h"
#include "checkurlstatusworker.h"
#include "dummyworker.h"
#include "scrapeproxies.h"
#include "testworker.h"

Worker *workerFactory(int id, int workerId, QQueue<QVariantMap> *inputDataQueue, QMutex *mutex, const QVariantMap &settings)
{
    Worker* worker(nullptr);

    switch (id)
    {
        case Tools::CHECK_URL_STATUS:
            worker = new CheckUrlStatusWorker(workerId, inputDataQueue, mutex, settings);
            break;
        case Tools::CHECK_ALEXA_RANK:
            worker = new CheckAlexaRankWorker(workerId, inputDataQueue, mutex, settings);
            break;
        case Tools::SCRAPE_PROXIES:
            worker = new ScrapeProxiesWorker(workerId, inputDataQueue, mutex, settings);
            break;
        case Tools::TEST:
            worker = new TestWorker(workerId, inputDataQueue, mutex, settings);
            break;
        default:
            worker = new DummyWorker(workerId, inputDataQueue, mutex, settings);
    }

    return worker;
}
